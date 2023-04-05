#include <request/memmap.h>
#include <mm/pmm.h>
#include <viper.h>

extern struct ViperMemmapResponse* MemMap;
extern struct ViperModule* modules;
extern uint8_t moduleCount;

void InitMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize, UINTN kernelStart, UINTN kernelEnd)
{
    if(!MemMap)
        return;
    uint8_t* mmap_start = (uint8_t*)memoryMap;
    uint8_t* mmap_end = (uint8_t*)mmap_start + mapSize;
    uint32_t mapCount = mapSize / descSize;

    struct ViperMemmapEntry* map = PMMGetPages(NPAGES(mapCount * sizeof(struct ViperMemmapEntry)));
    void* mapEnd = map + (mapCount * sizeof(struct ViperMemmapEntry));


    for(uint8_t* offset = mmap_start, i = 0; offset < mmap_end; offset += descSize, i++)
    {
        EFI_MEMORY_DESCRIPTOR* efiEntry = (EFI_MEMORY_DESCRIPTOR*)offset;
        struct ViperMemmapEntry* entry = map + i;

        for(uint8_t i = 0; i < moduleCount; i++)
        {
            if(efiEntry->PhysicalStart >= (uint64_t)modules[i].address && efiEntry->PhysicalStart + efiEntry->NumberOfPages * PAGE_SIZE <= (uint64_t)modules[i].address + modules[i].size)
            {
                entry->type = ViperMemmapKernelModules;
                goto loop_end;
            }
        }

        if(efiEntry->PhysicalStart >= kernelStart && efiEntry->PhysicalStart + efiEntry->NumberOfPages * PAGE_SIZE <= kernelEnd)
        {
            entry->type = ViperMemmapKernelModules;
            goto loop_end;
        }

        if(efiEntry->PhysicalStart >= (uint64_t)map && efiEntry->PhysicalStart + efiEntry->NumberOfPages * PAGE_SIZE <= (uint64_t)mapEnd)
        {
            entry->type = ViperMemmapBootloaderReclaimable;
            goto loop_end;
        }

        switch(efiEntry->Type)
        {
            case EfiLoaderData:
            case EfiLoaderCode:
                entry->type = ViperMemmapBootloaderReclaimable;
                break;

            case EfiBootServicesData:
            case EfiBootServicesCode:
            case EfiConventionalMemory:
                entry->type = ViperMemmapUsable;
                break;
            
            case EfiACPIReclaimMemory:
                entry->type = ViperMemmapAcpiReclaimable;
                break;
            case EfiACPIMemoryNVS:
                entry->type = ViperMemmapAcpiNvs;
                break;
            default:
                entry->type = ViperMemmapReserved;
                break;
        }
loop_end:

        entry->size = efiEntry->NumberOfPages * PAGE_SIZE;
        entry->base = efiEntry->PhysicalStart;
    }

    MemMap->count = mapCount;
    MemMap->entries = (struct ViperMemmapEntry*)map;
}