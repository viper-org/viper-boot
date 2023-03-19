#ifndef VMM_H
#define VMM_H
#include <efi.h>

void VMMInit();
void VMMMapPage(UINT64 PhysAddr, UINT64 VirtAddr, UINT64 Flags);

#endif