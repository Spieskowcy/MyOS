#include <stdio.h>

#include <kernel/com.h>
#include <kernel/asm.h>
#include <test.h>

#include <kernel/fat16.h>

#include <string.h>


TEST(strlenTest){
	char* str = "abc";
	size_t size = strlen(str);
	ASSERT(size, 3);
}
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
    .sectorsPerFat = 250,
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
TEST(diskTest){
    uint imageSize = bpb.sectorCount * bpb.bytesPerSector;
    u8 *image = FatAllocImage(imageSize);
    u8 bootSector[0x200];
    memset(bootSector, 0, sizeof(bootSector));
    memcpy(bootSector, &bpb, sizeof(bpb));
    bootSector[0x1fe] = 0x55;
    bootSector[0x1ff] = 0xaa;

    uint metaSectorCount = 86;
    uint clusterCount = 512000;
				ASSERT_TRUE(FatInitImage(image, bootSector));
    ASSERT_TRUE(FatGetImageSize(image) == imageSize);
    ASSERT_TRUE(FatGetMetaSectorCount(image) == metaSectorCount);
    ASSERT_TRUE(FatGetClusterCount(image) == clusterCount);
}

void kernel_main(void) {
	serial_initialize();

	RUN(strlenTest);
	RUN(diskTest);

	outb(0xf4, 0x00); // shutdown
}
