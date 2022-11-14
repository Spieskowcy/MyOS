// Floppy Disk Controller Driver header file

#ifndef _FDC_DRIVER_H
#define _FDC_DRIVER_H

#include <stdint.h>

void InstallFloppyDisk(int irq);
uint8_t* flpydsk_read_sector(int sectorLBA);

#endif
