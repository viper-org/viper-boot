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

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += descSize)
    {
        EFI_MEMORY_DESCRIPTOR* entry = (EFI_MEMORY_DESCRIPTOR*)offset;

        for(uint8_t i = 0; i < moduleCount; i++)
        {
            if(entry->PhysicalStart >= (uint64_t)modules[i].address && entry->PhysicalStart + entry->NumberOfPages * PAGE_SIZE <= (uint64_t)modules[i].address + modules[i].size)
            {
                entry->Type = ViperMemmapKernelModules;
                goto loop_end;
            }
        }

        if(entry->PhysicalStart >= kernelStart && entry->PhysicalStart + entry->NumberOfPages * PAGE_SIZE <= kernelEnd)
        {
            entry->Type = ViperMemmapKernelModules;
            goto loop_end;
        }

        switch(entry->Type)
        {
            case EfiLoaderData:
            case EfiLoaderCode:
                entry->Type = ViperMemmapBootloaderReclaimable;
                break;

            case EfiBootServicesData:
            case EfiBootServicesCode:
            case EfiConventionalMemory:
                entry->Type = ViperMemmapUsable;
                break;
            
            case EfiACPIReclaimMemory:
                entry->Type = ViperMemmapAcpiReclaimable;
                break;
            case EfiACPIMemoryNVS:
                entry->Type = ViperMemmapAcpiNvs;
                break;
            default:
                entry->Type = ViperMemmapReserved;
                break;
        }
loop_end:

        entry->NumberOfPages *= PAGE_SIZE;
    }

    MemMap->count = mapSize / descSize;
    MemMap->entries = (struct ViperMemmapEntry**)&memoryMap;
}