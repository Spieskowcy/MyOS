#ifndef _DMA_H_INCLUDED
#define _DMA_H_INCLUDED

#define DMA_MAX_CHANNELS 16
#define DMA_CHANNELS_PER_DMAC 8

void DMASetMode(uint8_t channel, uint8_t mode);
void DMASetRead(uint8_t channel);
void DMASetWrite(uint8_t channel);
void DMASetAddress(uint8_t channel, uint8_t low, uint8_t high);
void DMASetCount(uint8_t channel, uint8_t low, uint8_t high);
void DMAMaskChannel(uint8_t channel);
void DMAUnmaskChannel(uint8_t channel);
void DMAResetFlipflop(int dma);
void DMAReset();
void DMASetExternalPageRegister(uint8_t reg, uint8_t val);
void DMAUnmaskAll();

#endif