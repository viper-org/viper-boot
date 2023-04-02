#include <request/memmap.h>
#include <mm/pmm.h>
#include <viper.h>

extern struct ViperMemmapResponse* MemMap;

void InitMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize)
{
    if(!MemMap)
        return;
    uint8_t* mmap_start = (uint8_t*)memoryMap;
    uint8_t* mmap_end = (uint8_t*)mmap_start + mapSize;

    for(uint8_t* offset = mmap_start; offset < mmap_end; offset += descSize)
    {
        EFI_MEMORY_DESCRIPTOR* entry = (EFI_MEMORY_DESCRIPTOR*)offset;

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

        entry->NumberOfPages *= PAGE_SIZE;
    }

    MemMap->count = mapSize / descSize;
    MemMap->entries = (struct ViperMemmapEntry**)&memoryMap;
}