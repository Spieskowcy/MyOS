#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/fat16.h>
#include <kernel/keyboard.h>
BiosParamBlock bpb =
{
    .jump = { 0xeb, 0x3c, 0x90 }, // short jmp followed by nop
    .oem = { 'F', 'A', 'T', ' ', 'T', 'E', 'S', 'T' },
    .bytesPerSector = 512,
    .sectorsPerCluster = 1,
    .reservedSectorCount = 4,
    .fatCount = 2,
    .rootEntryCount = 512,
    .sectorCount = 1000,
    .mediaType = 0xf8,
    .sectorsPerFat = 25,
    .sectorsPerTrack = 63,
    .headCount = 255,
    .hiddenSectorCount = 0,
    .largeSectorCount = 0,
    .driveNumber = 0x80,
    .flags = 0x00,
    .signature = 0x29,
    .volumeId = 0xa0a1a2a3,
    .volumeLabel = { 'V', 'o', 'l', 'u', 'm', 'e', ' ', 'N', 'a', 'm', 'e' },
    .fileSystem = { 'F', 'A', 'T', '1', '6', ' ', ' ', ' ' },
};


void kernel_main(void) {
	terminal_initialize();
	int v = 12;
	int a = 0xF, b = 0xF0000000;
	int* wsk = &v; 
	printf("Are you working? %p wsk: %p  hex: %X  hex2: %x dec: %d dec2: %i\n", &v, wsk, b, a, b, a);
    uint imageSize = bpb.sectorCount * bpb.bytesPerSector;
    u8 *image = FatAllocImage(imageSize);
    printf("-> %p", image);

    u8 bootSector[0x200];
    memset(bootSector, 0, sizeof(bootSector));
    memcpy(bootSector, &bpb, sizeof(bpb));
    bootSector[0x1fe] = 0x55;
    bootSector[0x1ff] = 0xaa;
    char *data = "Hello World!";
    u16 rootClusterIndex = FatAddData(image, data, strlen(data) + 1);
    char *writtenData = (char *)(image + FatGetClusterOffset(image, rootClusterIndex));

    printf("; %s", writtenData);
	//printf("Are you working? %p wsk: %p  hex: %X  hex2: %x dec: %d dec2: %i\n", &v, wsk, b, a, b, a);

	keyboard_handler(image);

}
