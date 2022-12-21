#pragma once

#include <stdbool.h>
#include <kernel/memory.h>

#define PACKED __attribute__((__packed__))

#define SECTOR_SIZE 512

typedef struct BiosParamBlock
{
    char jump[3];
    char oem[8];
    unsigned short bytesPerSector;
    char sectorsPerCluster;
    unsigned short reservedSectorCount;
    char fatCount;
    unsigned short rootEntryCount;
    unsigned short sectorCount;
    char mediaType;
    unsigned short sectorsPerFat;
    unsigned short sectorsPerTrack;
    unsigned short headCount;
    unsigned int hiddenSectorCount;
    unsigned int largeSectorCount;

    // Extended block
    char driveNumber;
    char flags;
    char signature;
    unsigned int volumeId;
    char volumeLabel[11];
    char fileSystem[8];
} PACKED BiosParamBlock;

typedef struct DirEntry
{
    // Following conventions of DOS 7.0
    char name[8];
    char ext[3];
    char attribs;
    char reserved;
    char createTimeMs;
    unsigned short createTime;
    unsigned short createDate;
    unsigned short accessDate;
    unsigned short extendedAttribsIndex;
    unsigned short mTime;
    unsigned short mDate;
    unsigned short clusterIndex;
    unsigned int fileSize;
} PACKED DirEntry;

#define ENTRY_AVAILABLE 0x00
#define ENTRY_ERASED 0xe5

unsigned int FatGetTotalSectorCount(char* image);
unsigned int FatGetMetaSectorCount(char* image);
unsigned int FatGetClusterCount(char* image);
unsigned int FatGetImageSize(char* image);

unsigned short* FatGetTable(char* image, unsigned int fatIndex);
unsigned short FatGetClusterValue(char* image, unsigned int fatIndex, unsigned int clusterIndex);
void FatSetClusterValue(char* image, unsigned int fatIndex, unsigned int clusterIndex, unsigned short value);
unsigned int FatGetClusterOffset(char* image, unsigned int clusterIndex);
DirEntry* FatGetRootDirectory(char* image);

char* FatAllocImage(unsigned int imageSize);
bool FatInitImage(char* image, char* bootSector);

void FatSplitPath(char dstName[8], char dstExt[3], const char* path);
unsigned short FatFindFreeCluster(char* image);
void FatUpdateCluster(char* image, unsigned int clusterIndex, unsigned short value);
DirEntry* FatFindFreeRootEntry(char* image);
void FatUpdateDirEntry(DirEntry* entry, unsigned short clusterIndex, const char name[8], const char ext[3], unsigned int fileSize);
void FatRemoveDirEntry(DirEntry* entry);
unsigned short FatAddData(char* image, const void* data, unsigned int len);
void FatRemoveData(char* image, unsigned int rootClusterIndex);
DirEntry* FatAddFile(char* image, const char* path, const void* data, unsigned int len);
void FatRemoveFile(char* image, DirEntry* entry);
void FatPrintFile(char* image, char* filename);
