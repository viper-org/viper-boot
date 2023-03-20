#ifndef MEM_VMM_H
#define MEM_VMM_H
#include <stdint.h>
#include <bootInfo.h>

void VMMInit(VBInfo* BootInfo);

void VMMMapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
void VMMMapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

#endif