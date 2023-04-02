#ifndef REQUEST_MEMMAP_H
#define REQUEST_MEMMAP_H
#include <efi/efi.h>

void InitMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize);

#endif