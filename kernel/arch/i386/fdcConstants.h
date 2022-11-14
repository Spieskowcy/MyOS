
enum FloppyDiskRegisters {
	Digital_Output_Register = 0x3F2,		// DOR, write only
	Main_Status_Register = 0x3F4,			// MSR
	Data_FIFO = 0x3F5,
	Configuration_Control_Register = 0x3F7
};

enum DORMasks {		// Only one drive will be used, rest will be skipped
	DORMasks_Drive_0 = 0,					// 00000000
	DORMasks_Reset = 4,						// 00000100
	DORMasks_Mode = 8,						// 00001000
	DORMasks_Drive_0_Motor_Control = 16,	// 00010000
};

enum MSRMasks {		// FDD - Floppy Disk Drives
	MSRMasks_FDD_0_Mode = 1,				// 00000001
	MSRMasks_FDC_Busy = 16,					// 00010000
	MSRMasks_FDC_Non_DMA_Mode = 32,			// 00100000
	MSRMasks_DIO = 64,						// 01000000
	MSRMasks_RQM = 128						// 10000000
};

enum FloppyDiskCommands {
	FloppyDiskCommands_Read_Track = 2,
	FloppyDiskCommands_Specify = 3,
	FloppyDiskCommands_Check_Status = 4,
	FloppyDiskCommands_Write_Sector = 5,
	FloppyDiskCommands_Read_Sector = 6,
	FloppyDiskCommands_Calibrate = 7,
	FloppyDiskCommands_Check_Interupt = 8,
	FloppyDiskCommands_Format_Track = 0xD,
	FloppyDiskCommands_Seek = 0xF
};

enum FloppyDiskCommandsExtend {
	FloppyDiskCommandsExtend_Skip = 32,
	FloppyDiskCommandsExtend_Density = 64,
	FloppyDiskCommandsExtend_Multitrack = 128
};

enum FloppyDiskGAP3 {
	FloppyDiskGAP3_STD = 42,
	FloppyDiskGAP3_5_14 = 32,
	FloppyDiskGAP3_3_5 = 27
};

enum FloppyDiskSectorDTL {

	FloppyDiskSectorDTL_128 = 0,
	FloppyDiskSectorDTL_256 = 1,
	FloppyDiskSectorDTL_512 = 2,
	FloppyDiskSectorDTL_1024 = 4
};

const int FLOPPY_DISK_IRQ = 6, FLOPPY_DISK_SECTORS_PER_TRACK = 18;
static volatile bool FloppyDiskIRQDone = false;
const int DMA_BUFFER = 0x1000;
const int DMA_CHANNEL = 2;

