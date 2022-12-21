#include "kernel/fat16.h"
#include <stdlib.h>
#include <string.h>
#define assert(ignore) ((void)0)
unsigned int FatGetTotalSectorCount(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    if (bpb->sectorCount)
    {
        return bpb->sectorCount;
    }
    else
    {
        return bpb->largeSectorCount;
    }
}

unsigned int FatGetMetaSectorCount(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    return
        bpb->reservedSectorCount +
        bpb->fatCount * bpb->sectorsPerFat +
        (bpb->rootEntryCount * sizeof(DirEntry)) / bpb->bytesPerSector;
}

unsigned int FatGetClusterCount(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    unsigned int totalSectorCount = FatGetTotalSectorCount(image);
    unsigned int metaSectorCount = FatGetMetaSectorCount(image);
    unsigned int dataSectorCount = totalSectorCount - metaSectorCount;

    return dataSectorCount / bpb->sectorsPerCluster;
}

unsigned int FatGetImageSize(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    return FatGetTotalSectorCount(image) * bpb->bytesPerSector;
}

unsigned short* FatGetTable(char* image, unsigned int fatIndex)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    assert(fatIndex < bpb->fatCount);

    unsigned int offset = (bpb->reservedSectorCount + fatIndex * bpb->sectorsPerFat) * bpb->bytesPerSector;

    return (unsigned short*)(image + offset);
}

unsigned short FatGetClusterValue(char* image, unsigned int fatIndex, unsigned int clusterIndex)
{
    unsigned short* fat = FatGetTable(image, fatIndex);

    assert(clusterIndex < FatGetClusterCount(image));

    return fat[clusterIndex];
}

unsigned int FatGetClusterOffset(char* image, unsigned int clusterIndex)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    return
        (bpb->reservedSectorCount + bpb->fatCount * bpb->sectorsPerFat) * bpb->bytesPerSector +
        bpb->rootEntryCount * sizeof(DirEntry) +
        (clusterIndex - 2) * (bpb->sectorsPerCluster * bpb->bytesPerSector);
}

void FatSetClusterValue(char* image, unsigned int fatIndex, unsigned int clusterIndex, unsigned short value)
{
    unsigned short* fat = FatGetTable(image, fatIndex);

    assert(clusterIndex < FatGetClusterCount(image));

    fat[clusterIndex] = value;
}

DirEntry* FatGetRootDirectory(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    unsigned int offset = (bpb->reservedSectorCount + bpb->fatCount * bpb->sectorsPerFat) * bpb->bytesPerSector;
    unsigned int dataSize = bpb->rootEntryCount * sizeof(DirEntry);

    assert(offset + dataSize <= FatGetImageSize(image));

    return (DirEntry*)(image + offset);
}

char storage[512*1000];
char* FatAllocImage(unsigned int imageSize)
{
    char* image = (char*)(&storage);
    memset(image, ENTRY_ERASED, imageSize);
    return image;
}

bool FatInitImage(char* image, char* bootSector)
{
    BiosParamBlock* bpb = (BiosParamBlock*)bootSector;

    // Validate signature
    if (bootSector[0x1fe] != 0x55 || bootSector[0x1ff] != 0xaa)
    {
        return false;
    }

    // Copy to sector 0
    memcpy(image, bootSector, bpb->bytesPerSector);

    // Initialize clusters
    unsigned int clusterCount = FatGetClusterCount(image);
    assert(clusterCount >= 2);

    FatUpdateCluster(image, 0, 0xff00 | bpb->mediaType);    // media type
    FatUpdateCluster(image, 1, 0xffff);                     // end of chain cluster marker

    for (unsigned int clusterIndex = 2; clusterIndex < clusterCount; ++clusterIndex)
    {
        FatUpdateCluster(image, clusterIndex, 0x0000);
    }

    return true;
}

unsigned short FatFindFreeCluster(char* image)
{
    unsigned int clusterCount = FatGetClusterCount(image);

    unsigned short* fat = FatGetTable(image, 0);

    for (unsigned int clusterIndex = 2; clusterIndex < clusterCount; ++clusterIndex)
    {
        unsigned short data = fat[clusterIndex];
        if (data == 0)
        {
            return clusterIndex;
        }
    }

    return 0;
}

void FatUpdateCluster(char* image, unsigned int clusterIndex, unsigned short value)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    for (unsigned int fatIndex = 0; fatIndex < bpb->fatCount; ++fatIndex)
    {
        FatSetClusterValue(image, fatIndex, clusterIndex, value);
    }
}

DirEntry* FatFindFreeRootEntry(char* image)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;

    DirEntry* start = FatGetRootDirectory(image);
    DirEntry* end = start + bpb->rootEntryCount;

    for (DirEntry* entry = start; entry != end; ++entry)
    {
        char marker = entry->name[0];
        if (marker == ENTRY_AVAILABLE || marker == ENTRY_ERASED)
        {
            return entry;
        }
    }

    return 0;
}

static void SetPaddedString(char* dst, unsigned int dstLen, const char* src, unsigned int srcLen)
{
    if (src)
    {
        if (srcLen > dstLen)
        {
            memcpy(dst, src, dstLen);
        }
        else
        {
            memcpy(dst, src, srcLen);
            memset(dst + srcLen, ' ', dstLen - srcLen);
        }

        for (unsigned int i = 0; i < dstLen; ++i)
        {
            dst[i] = strupr(dst[i]);
        }
    }
    else
    {
        memset(dst, ' ', dstLen);
    }
}

void FatSplitPath(char dstName[8], char dstExt[3], const char* path)
{
    const char* name;// = strrchr(path, '/');
    if (name)
    {
        name = name + 1;
    }
    else
    {
        name = path;
    }

    unsigned int nameLen = strlen(name);

    char* ext = 0;
    unsigned int extLen = 0;
    char* p;// = strchr(name, '.');
    if (p)
    {
        nameLen = p - name;
        ext = p + 1;
        extLen = strlen(ext);
    }

    SetPaddedString(dstName, 8, name, nameLen);
    SetPaddedString(dstExt, 3, ext, extLen);
}

void FatUpdateDirEntry(DirEntry* entry, unsigned short clusterIndex, const char name[8], const char ext[3], unsigned int fileSize)
{
    entry->clusterIndex = clusterIndex;
    memcpy(entry->name, name, sizeof(entry->name));
    memcpy(entry->ext, ext, sizeof(entry->ext));
    entry->fileSize = fileSize;
}

void FatRemoveDirEntry(DirEntry* entry)
{
    entry->name[0] = ENTRY_AVAILABLE;
}

unsigned short FatAddData(char* image, const void* data, unsigned int len)
{
    BiosParamBlock* bpb = (BiosParamBlock*)image;
    unsigned int bytesPerCluster = bpb->sectorsPerCluster * bpb->bytesPerSector;

    // Skip empty files
    if (len == 0)
        return 0;

    unsigned short endOfChainValue = FatGetClusterValue(image, 0, 1);

    unsigned short prevClusterIndex = 0;
    unsigned short rootClusterIndex = 0;

    // Copy data one cluster at a time.
    const char* p = (const char*)data;
    const char* end = p + len;
    while (p < end)
    {
        // Find a free cluster
        unsigned short clusterIndex = FatFindFreeCluster(image);
        if (clusterIndex == 0)
        {
            // Ran out of disk space, free allocated clusters
            if (rootClusterIndex != 0)
                FatRemoveData(image, rootClusterIndex);

            return 0;
        }

        // Determine amount of data to copy
        unsigned int count = end - p;
        if (count > bytesPerCluster)
            count = bytesPerCluster;

        // Transfer bytes into image at cluster location
        unsigned int offset = FatGetClusterOffset(image, clusterIndex);
        memcpy(image + offset, p, count);
        p += count;

        // Update FAT clusters
        FatUpdateCluster(image, clusterIndex, endOfChainValue);
        if (prevClusterIndex)
            FatUpdateCluster(image, prevClusterIndex, clusterIndex);
        else
            rootClusterIndex = clusterIndex;

        prevClusterIndex = clusterIndex;
    }

    return rootClusterIndex;
}

void FatRemoveData(char* image, unsigned int clusterIndex)
{
    assert(clusterIndex != 0);

    unsigned short endOfChainValue = FatGetClusterValue(image, 0, 1);

    while (clusterIndex != endOfChainValue)
    {
        unsigned short nextClusterIndex = FatGetClusterValue(image, 0, clusterIndex);
        FatUpdateCluster(image, clusterIndex, 0);
        clusterIndex = nextClusterIndex;
    }
}

DirEntry* FatAddFile(char* image, const char* path, const void* data, unsigned int len)
{
    // Find Directory Entry
    DirEntry* entry = FatFindFreeRootEntry(image);
    if (!entry)
        return 0;

    // Add File
    unsigned short rootClusterIndex = FatAddData(image, data, len);
    if (!rootClusterIndex)
        return 0;

    // Update Directory Entry
    char name[8];
    char ext[3];
    FatSplitPath(name, ext, path);

    FatUpdateDirEntry(entry, rootClusterIndex, name, ext, len);
    return entry;
}

void FatRemoveFile(char* image, DirEntry* entry)
{
    FatRemoveData(image, entry->clusterIndex);
    FatRemoveDirEntry(entry);
}

DirEntry* FatFindFile(char* image, char* filename){
	DirEntry* rootDir = FatGetRootDirectory(image);
	BiosParamBlock* bpb = (BiosParamBlock*)image;

	for(int i = 0; i < bpb->rootEntryCount; i++){
		if(memcmp(rootDir[i].name, filename, strlen(rootDir[i].name))){
			return &rootDir[i];
		}
	}
	return 0;

}

void FatPrintFile(char* image, char* filename){
	char* writtenData;
	DirEntry* file = FatFindFile(image, filename);
	if(file == 0){
		printf(" :( %s", "File not found");
		return;
	}
	unsigned short endOfChainValue = FatGetClusterValue(image, 0, 1);

	printf("endOfChain: %i", endOfChainValue);
	unsigned short clusterIdx = file->clusterIndex;
	while(clusterIdx != endOfChainValue){
		char *writtenData = (char *)(image + FatGetClusterOffset(image, clusterIdx));
		printf(" : %s", writtenData);
		clusterIdx = FatGetClusterValue(image, 0, clusterIdx);

	}

}
