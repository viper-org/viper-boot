#include <mm/vmm.h>
#include <mm/pmm.h>
#include <request/framebuffer.h>
#include <lib.h>

uint64_t* pml4;

#define PRESENT_WRITABLE 0x3

void VMMInit(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize, EFI_PHYSICAL_ADDRESS kernelStart, EFI_PHYSICAL_ADDRESS kernelEnd)
{
    pml4 = (uint64_t*)PMMGetPage();
    memset((uint8_t*)pml4, 0, PAGE_SIZE);
    uint64_t tmp = (uint64_t)pml4;
    tmp &= ~0xFFF;
    pml4 = (uint64_t*)tmp;

    // Map lower 4GiB of memory
    VMMMapPages(0, 0, PRESENT_WRITABLE, NPAGES(0x400000));
    VMMMapPages(0, 0xFFFF800000000000, PRESENT_WRITABLE, NPAGES(0x400000));

    uint8_t* mmap_start = (uint8_t*)memoryMap;
    uint8_t* mmap_end = (uint8_t*)mmap_start + mapSize;

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += descSize)
    {
        EFI_MEMORY_DESCRIPTOR* entry = (EFI_MEMORY_DESCRIPTOR*)offset;

        if(entry->PhysicalStart + entry->NumberOfPages * PAGE_SIZE < 0x400000)
            continue;

        // Map other memory regions above 4GiB
        VMMMapPages(entry->PhysicalStart, entry->PhysicalStart + 0xFFFF800000000000, 3, entry->NumberOfPages);
        VMMMapPages(entry->PhysicalStart, entry->PhysicalStart, PRESENT_WRITABLE, entry->NumberOfPages);
    }

    // Map the framebuffer
    VMMMapPages((uint64_t)fb.base, (uint64_t)fb.base, PRESENT_WRITABLE, NPAGES(fb.size));
    VMMMapPages((uint64_t)fb.base, (uint64_t)fb.base + 0xFFFF800000000000, PRESENT_WRITABLE, NPAGES(fb.size));

    // Map the kernel to 0xFFFFFFFF80000000
    VMMMapPages(kernelStart, 0xFFFFFFFF80000000, PRESENT_WRITABLE, NPAGES(kernelEnd - kernelStart));
    asm volatile("mov %0, %%cr3" : : "r"(pml4));
}

static inline uint64_t* Entry(uint64_t pt)
{
    return (uint64_t*)(pt & ~0xFFF);
}

void VMMMapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags)
{
    PhysAddr &= ~0xFFF;
    VirtAddr &= ~0xFFF;
    Flags    &=  0xFFF;

    if(VirtAddr & ((uint64_t)1 << 47))
        VirtAddr |= 0xFFFF000000000000;

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
            memset((uint8_t*)Entry(pt[idx]), 0, PAGE_SIZE);
            pt[idx] |= 1 | Flags;
        }
        pt = Entry(pt[idx]);
    }
    if(!(pt[idx] & 0x1))
        pt[idx] = PhysAddr | Flags | 1;
}

void VMMMapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
{
    for(uint64_t i = 0; i < Npages * PAGE_SIZE; i += PAGE_SIZE)
        VMMMapPage(PhysAddr + i, VirtAddr + i, Flags);
}