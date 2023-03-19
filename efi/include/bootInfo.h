#ifndef VB_INFO_H
#define VB_INFO_H

typedef struct
{
    char Signature[8];
    unsigned char Checksum;
    char OEMID[6];
    unsigned char Revision;
    unsigned int RsdtAddress;
} __attribute__((packed)) RSDPDescriptor;

typedef struct
{
    RSDPDescriptor Desc;

    unsigned int Length;
    unsigned long XsdtAddress;
    unsigned char ExtendedChecksum;
    unsigned char Reserved[3];
} __attribute__((packed)) RSDPDescriptor20;

typedef struct
{
    unsigned int* Base;
    unsigned long Size;

    unsigned int Horiz;
    unsigned int Vert;

    unsigned int Pitch;
} VBFramebuffer;

typedef enum
{
    VBMemoryReserved=0,
    VBMemoryUsable,
    VBMemoryUnusable,
    VBMemoryACPIReclaimable,
    VBMemoryACPINVS,
    VBMemoryBadMemory,
    VBMemoryBootloaderReclaimable,
    VBMemoryKernel,
    VBMemoryFramebuffer,
} VBMemoryMapType; 

typedef struct
{
    VBMemoryMapType Type;
    unsigned int EntrySize;
    unsigned long long Base;
    unsigned long long MapSize;
    unsigned long long Size;
    unsigned long long Pad;
} VBMemoryMapEntry;

typedef struct
{
    unsigned long Magic;
    VBFramebuffer Framebuffer;
    VBMemoryMapEntry* MemoryMap;
} VBInfo;

#endif