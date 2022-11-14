/*	SOURCES
	- https://wiki.osdev.org/Floppy_Disk_Controller
	- http://www.brokenthorn.com/Resources/OSDev20.html
	- https://forum.osdev.org/viewtopic.php?f=1&t=13538&fbclid=IwAR2ExXLzlHS8Fj_TIe88254fX36S17kInCGz7cT6ur0PU5AFVn_OJ9rozfQ
*/

#include "fdcConstants.h"
#include <stdbool.h>
#include <stdint.h>
#include <kernel/asm.h>
#include <kernel/dma.h>

extern void sleep(int);


void FloppyDiskIRQ() {
	_asm {
		add esp, 12
		pushad
		cli
	}

	FloppyDiskIRQWorking = 1;

	// TODO
	// interruptdone(FLOPPY_IRQ);

	_asm {
		sti
		popad
		iretd
	}
}


void WaitForIRQ() {
	while (!FloppyDiskIRQDone);
	FloppyDiskIRQDone = 0;
}


bool InitializeDMA(uint8_t* buffer, unsigned length) {
	union {
		uint8_t byte[4];//Lo[0], Mid[1], Hi[2]
		unsigned long l;
	}a, c;

	a.l = (unsigned)buffer;
	c.l = (unsigned)length - 1;

	if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff) + c.l) >> 16)) return false;

	DMAReset();
	DMAMaskChannel(DMA_CHANNEL);
	DMAResetFlipflop(1);
	DMASetAddress(DMA_CHANNEL, a.byte[0], a.byte[1]);
	DMAResetFlipflop(1);
	DMASetCount(DMA_CHANNEL, c.byte[0], c.byte[1]);
	DMASetRead(DMA_CHANNEL);
	DMAUnmaskAll();
}


void WriteDOR(uint8_t value) {
	outb(Digital_Output_Register, value);
}


uint8_t ReadStatus() {
	return inb(Main_Status_Register);
}


void SendCommand(uint8_t command) {
	for (int i = 0; i < 600; i++) {
		if (ReadStatus() & MSRMasks_RQM) {
			return outb(Data_FIFO, command);
		}
	}
}


uint8_t ReadData() {
	for (int i = 0; i < 600; i++) {
		if (ReadStatus() & MSRMasks_RQM) {
			return inb(Data_FIFO, command);
		}
	}
	return 0;
}


void WriteCCR(uint8_t value) {
	outb(Configuration_Control_Register, value)
}


void ControlMotor(bool MotorOn) {
	if (MotorOn) {
		WriteDOR(uint8_t(DORMasks_Drive_0 | DORMasks_Drive_0_Motor_Control | DORMasks_Reset | DORMasks_Mode))
	}
	else {
		WriteDOR(DORMasks_Reset);
	}
	sleep(20);
}


void Specify(uint32_t steprate, uint32_t loadTime, uint32_t unloadTime, bool dma) {
	SendCommand(FloppyDiskCommands_Specify);
	uint32_t tmp = ((steprate & 0xF) << 4) | (unloadTime & 0xF);
	SendCommand(tmp);
	tmp = (loadTime << 1) | (dma == true ? 1 : 0);
	SendCommand(tmp);
}


void CheckInterrupt(uint32_t* st0, uint32_t* cyl) {
	SendCommand(FloppyDiskCommands_Check_Interupt);
	*st0 = ReadData();
	*cyl = ReadData();
}


void ReadSectorCmd(uint8_t head, uint8_t track, uint8_t sector) {
	uint32_t st0, cyl;
	PrepareDMAToRead();
	SendCommand(FloppyDiskCommands_Read_Sector | FloppyDiskCommandsExtend_Multitrack | FloppyDiskCommandsExtend_Skip | FloppyDiskCommandsExtend_Density);
	SendCommand(head << 2 | DORMasks_Drive_0);
	SendCommand(track);
	SendCommand(head);
	SendCommand(sector);
	SendCommand(FloppyDiskSectorDTL_512);
	SendCommand(((sector + 1) >= FLOPPY_DISK_SECTORS_PER_TRACK) ? FLOPPY_DISK_SECTORS_PER_TRACK : sector + 1);
	SendCommand(FloppyDiskGAP3_3_5);
	SendCommand(0xFF);
	WaitForIRQ();

	for (int j = 0; j < 7; j++)
		ReadData();
	CheckInterrupt();
}

int Calibrate() {
	uint32_t st0, cyl = -1;

	ControlMotor(true);

	for (int i = 0; i < 10; i++) {
		SendCommand(FloppyDiskCommands_Calibrate);
		SendCommand(DORMasks_Drive_0);
		WaitForIRQ();
		CheckInterrupt(&st0, &cyl);

		if (!cyl) {
			ControlMotor(false);
			return 0;
		}
	}

	ControlMotor(false);
	return -1;
}


void Reset() {
	WriteDOR(0);								// Disable controller
	WriteDOR(DORMasks_Reset | DORMasks_Mode);	// Enable controller
	WaitForIRQ();

	uint32_t st0, cyl;
	CheckInterrupt(&st0, &cyl);

	WriteCCR(0);						// set speed of transfer to 500kb/s
	Specify(3, 16, 240, true);
	if (Calibrate()) return -1;			// check if reset failed
}


int Seek(uint8_t cylinder, uint8_t head) {
	uint32_t st0, cyl;

	for (int i = 0; i < 10; i++) {
		SendCommand(FloppyDiskCommands_Seek);
		SendCommand(head << 2);
		SendCommand(cylinder);
		WaitForIRQ();
		CheckInterrupt(&st0, &cyl);
		if (cyl == cylinder) return 0;		// We found cylinder that we wanted
	}

	return -1;		// fail
}


void InstallFloppyDisk(int irq) {

	// TODO
	// setvect(irq, FloppyDiskIRQ);

	Reset();
	Specify(13, 1, 0xf, true);
}


uint8_t* ReadSector(int sectorLBA) {
	int head = (lba % (FLOPPY_DISK_SECTORS_PER_TRACK * 2)) / (FLOPPY_DISK_SECTORS_PER_TRACK); 
	int  track = lba / (FLOPPY_DISK_SECTORS_PER_TRACK * 2); 
	int sector = lba % FLOPPY_DISK_SECTORS_PER_TRACK + 1;;
	
	ControlMotor(true);
	if (Seek((uint8_t)track, (uint8_t)head) != 0) return 0;	// We didn't find what we wanted :(
	ReadSectorCmd((uint8_t)head, (uint8_t)track, (uint8_t)sector);
	ControlMotor(false);
	return (uint8_t*)DMA_BUFFER;
}