/*	SOURCES
	- http://www.brokenthorn.com/Resources/OSDev21.html
*/

#include "dmaConstants.h"
#include <stdint.h>

void DMASetAddress(uint8_t channel, uint8_t low, uint8_t high) {
	if (channel > 8) return;

	unsigned short port = 0;
	switch (channel) {
	case 0: {port = DMA0_Channel0_Address_Register; break; }
	case 1: {port = DMA0_Channel1_Address_Register; break; }
	case 2: {port = DMA0_Channel2_Address_Register; break; }
	case 3: {port = DMA0_Channel3_Address_Register; break; }
	case 4: {port = DMA1_Channel0_Address_Register; break; }
	case 5: {port = DMA1_Channel1_Address_Register; break; }
	case 6: {port = DMA1_Channel1_Address_Register; break; }
	case 7: {port = DMA1_Channel3_Address_Register; break; }
	}

	outb(port, low);
	outb(port, high);
}

void DMASetCount(uint8_t channel, uint8_t low, uint8_t high) {
	if (channel > 8) return;

	unsigned short port = 0;
	switch (channel) {
	case 0: {port = DMA0_Channel0_Count_Register; break; }
	case 1: {port = DMA0_Channel1_Count_Register; break; }
	case 2: {port = DMA0_Channel2_Count_Register; break; }
	case 3: {port = DMA0_Channel3_Count_Register; break; }
	case 4: {port = DMA1_Channel0_Count_Register; break; }
	case 5: {port = DMA1_Channel1Count_Register; break; }
	case 6: {port = DMA1_Channel1_Count_Register; break; }
	case 7: {port = DMA1_Channel3_Count_Register; break; }
	}

	outb(port, low);
	outb(port, high);
}

void DMASetExternalPageRegister(uint8_t reg, uint8_t value) {
	if (reg > 14) return;

	unsigned short port = 0;
	switch (reg){
	case 1: {port = DMA0_Channel1_Addr = 0x81, ; break; }
	case 2: {port = DMA0_Channel2_Addr = 0x81, ; break; }
	case 3: {port = DMA0_Channel3_Addr = 0x81, ; break; }
	case 4: {return; }//! nothing should ever write to register 4
	case 5: {port = DMA0_Channel5_Addr = 0x81, ; break; }
	case 6: {port = DMA0_Channel6_Addr = 0x81, ; break; }
	case 7: {port = DMA0_Channel2_Addr = 0x81, ; break; }
	}
	outb(port, value);
}

void DMAMaskChannel(uint8_t channel) {
	if (channel <= 4) {
		outb(DMA0_Single_Mask_Register, (1 << (channel - 1)));
	}
	else {
		outb(DMA1_Single_Mask_Register, (1 << (channel - 5)));
	}
}

void DMAUnmaskChannel(uint8_t channel) {
	if (channel <= 4) {
		outb(DMA0_Single_Mask_Register, channel);
	}
	else {
		outb(DMA1_Single_Mask_Register, channel);
	}
}

void DMAUnmaskAll() {
	outb(DMA0_Clear_Mask_Register, 0xFF);
}

void DMASetMode(uint8_t channel, uint8_t mode) {
	int DMA = (channel < 4) ? 0 : 1;
	int chan = (dma == 0) ? channel : channel - 4;

	DMAMaskChannel(channel);
	uint16_t port = (channel < 4) ? DMA0_Mode_Register : DMA1_Mode_Register;
	outb(port, chan | (mode));
	DMAUnmaskAll(DMA);
}

void DMASetRead(uint8_t channel) {
	uint8_t mode = DMA_Read_Transfer_Mask | DMA_Single_Transfer_Mask;
	DMASetMode(channel, mode);
}

void DMASetWrite(uint8_t channel) {
	uint8_t mode = DMA_Write_Transfer_Mask | DMA_Single_Transfer_Mask;
	DMASetMode(channel, mode);
}

void DMAResetFlipflop(int dma) {
	if (dma < 2) return;
	uint8_t port = (dma == 0) ? DMA0_Clear_Byte_FlipFlop_Register : DMA1_Clear_Byte_FlipFlop_Register;
	outb(port, 0xFF);
}

void DMAReset() {
	outb(DMA0_Intermediate_Register, 0xFF);
}