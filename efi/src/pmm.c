#include <pmm.h>
#include <stddef.h>
#include <stdint.h>

uint8_t* Bitmap;
uint64_t PhysLimit = 0;
uint64_t TotalMemory = 0;
uint64_t FreeMemory = 0;

void BmpMarkUsed(void* addr, uint32_t npages)
{
    for(uint64_t i = (uint64_t)addr; i < (uint64_t)addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
        Bitmap[i / (PAGE_SIZE * 8)] &= ~(1 << ((i / PAGE_SIZE) % 8));
}

bool BmpTest(void* addr, uint32_t npages)
{
    bool result = true;

    for(uint64_t i = (uint64_t)addr; i < (uint64_t)addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
    {
        result = Bitmap[i / (PAGE_SIZE * 8)] & (1 << ((i / PAGE_SIZE) % 8));
        if (!result)
            break;
    }

    return result;
}

void PMMInit(VBMemoryMapEntry* memoryMap)
{
    uint8_t* mmap_start = (uint8_t*)memoryMap;
    uint8_t* mmap_end = (uint8_t*)mmap_start + memoryMap->MapSize;

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
    {
        VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

        if(entry->Base + entry->Size < 0x100000)
            continue;

        uint64_t NewPhysLimit = entry->Base + entry->Size;
        if(NewPhysLimit > PhysLimit)
            PhysLimit = NewPhysLimit;

        if(entry->Type == VBMemoryUsable)
            TotalMemory += entry->Size;
    }

    uint32_t BitmapSize = PhysLimit / (PAGE_SIZE * 8);
    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
    {
        VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

        if(entry->Base + entry->Size < 0x100000)
            continue;
        
        if(entry->Size >= BitmapSize && (entry->Type == VBMemoryUsable))
        {
            Bitmap = (uint8_t*)entry->Base;
            break;
        }
    }

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
    {
        VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

        if(entry->Base + entry->Size < 0x100000)
            continue;

        if(entry->Type == VBMemoryUsable)
        {
            PMMFreePages((void*)entry->Base, NPAGES(entry->Size));
        }
    }

    PMMAlloc(Bitmap, NPAGES(BitmapSize));
}

bool PMMAlloc(void* address, uint32_t npages)
{
    if(!BmpTest(address, npages))
        return false;

    BmpMarkUsed(address, npages);
    FreeMemory -= npages * PAGE_SIZE;
    return true;
}

void* PMMGetPage()
{
    return PMMGetPages(1);
}

void PMMFreePage(void* address)
{
    PMMFreePages(address, 1);
}

void* PMMGetPages(uint32_t npages)
{
    static uint64_t LastUsedPage;

    for(uint64_t i = LastUsedPage; i < PhysLimit; i += PAGE_SIZE)
    {
        if(PMMAlloc((void*)i, npages))
            return (void*)i;
    }

    for(uint64_t i = 0; i < LastUsedPage; i += PAGE_SIZE)
    {
        if(PMMAlloc((void*)i, npages))
            return (void*)i;
    }

    for(;;);
}

void PMMFreePages(void* address, uint32_t npages)
{
    for(uint64_t i = (uint64_t)address; i < (uint64_t)address + (npages * PAGE_SIZE); i += PAGE_SIZE)
    {
        if(!BmpTest((void*)i, 1))
            FreeMemory += PAGE_SIZE;
        
        Bitmap[i / (PAGE_SIZE * 8)] |= 1 << ((i / PAGE_SIZE) % 8);
    }
}
