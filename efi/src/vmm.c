#include <vmm.h>
#include <pmm.h>
#include <efi.h>
#include <efilib.h>

uint64_t* pml4;

#define PRESENT_WRITABLE 0x3

void VMMInit(VBInfo* BootInfo)
{
    pml4 = (uint64_t*)PMMGetPage();
    SetMem((uint8_t*)pml4, PAGE_SIZE, 0);
    uint64_t tmp = (uint64_t)pml4;
    tmp &= ~0xFFFF000000000FFF;
    pml4 = (uint64_t*)tmp;
    VMMMapPages(0, 0, 3, NPAGES(0x400000));
    uint8_t* mmap_start = (uint8_t*)BootInfo->MemoryMap;
    uint8_t* mmap_end = (uint8_t*)mmap_start + BootInfo->MemoryMap->MapSize;

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += BootInfo->MemoryMap->EntrySize)
    {
        VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

        if(entry->Base + entry->Size < 0x400000)
            continue;

        VMMMapPages(entry->Base, entry->Base, 3, entry->Size / 4096);
    }
    VMMMapPages((uint64_t)BootInfo->Framebuffer.Base, (uint64_t)BootInfo->Framebuffer.Base, 3, NPAGES(BootInfo->Framebuffer.Size));
    asm volatile("mov %0, %%cr3" : : "r"(pml4));
}

static inline uint64_t* Entry(uint64_t pt)
{
    return (uint64_t*)(pt & ~0xFFFF000000000FFF);
}

void VMMMapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags)
{
    PhysAddr &= ~0xFFF;
    VirtAddr &= ~0xFFF;
    Flags    &=  0xFFF;

    uint64_t shift = 48;
    uint64_t* pt = Entry((uint64_t)pml4);
    uint64_t idx;
    for(char i = 0; i < 4; i++)
    {
        shift -= 9;
        idx = (VirtAddr >> shift) & 0x1FF;
        if(i == 3)
            break;
        
        if(!(pt[idx] & 0x1))
        {
            pt[idx] = (uint64_t)PMMGetPage();
            SetMem((uint8_t*)Entry(pt[idx]), PAGE_SIZE, 0);
            pt[idx] |= 3;
            pt[idx] &= ~0xFFFF000000000000;
        }
        pt = Entry(pt[idx]);
    }
    pt[idx] = PhysAddr | Flags | 1;
    pt[idx] &= ~0xFFFF000000000000;
}

void VMMMapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
{
    for(uint64_t i = 0; i < Npages * PAGE_SIZE; i += PAGE_SIZE)
        VMMMapPage(PhysAddr + i, VirtAddr + i, Flags);
}