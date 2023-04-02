#ifndef MEM_VMM_H
#define MEM_VMM_H
#include <efi/efi.h>
#include <viper.h>
#include <stdint.h>

void VMMInit(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize, EFI_PHYSICAL_ADDRESS kernelStart, EFI_PHYSICAL_ADDRESS kernelEnd);

void VMMMapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
void VMMMapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

#endif