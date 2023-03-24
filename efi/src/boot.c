#include <efi.h>
#include <efilib.h>
#include <bootInfo.h>
#include <file.h>
#include <elf.h>
#include <vmm.h>
#include <pmm.h>

extern VBInfo GetBootInfo();
typedef void(*KMain)(VBInfo*);

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    File f = ReadFile(ImageHandle, L"kernel.elf");
    VBInfo BootInfo = GetBootInfo();
    KernelInfo Kernel = ParseKernel(f.Buffer, f.Size);

    KMain KernelEntry = (KMain)Kernel.EntryPoint + 0xffffffff80000000;

    //VMMInit();
    //for(UINT64 i = Kernel.Start; i < Kernel.End; i += 4096)
    //    VMMMapPage(i, i, 0);

    UINTN MapSize = 0, MapKey, DescSize;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINT32 DescVers;
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, NULL, &DescSize, NULL);

    MapSize += DescSize * 2;

    SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&MemoryMap);
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, &MapKey, &DescSize, &DescVers);

    BootInfo.MemoryMap = (VBMemoryMapEntry*)MemoryMap;

    UINT8* mmap_start = (UINT8*)MemoryMap;
    UINT8* mmap_end  = (UINT8*)mmap_start + MapSize;
    UINT8* offset = mmap_start;

    MemoryMap->VirtualStart = MapSize;
    MemoryMap->Pad = DescSize;

    while(offset < mmap_end)
    {
        EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR* )offset;
        Desc->NumberOfPages *= 4096;

        if(Desc->Type == EfiConventionalMemory)
        {
            if(Desc->PhysicalStart >= Kernel.Start && Desc->PhysicalStart <= Kernel.End)
                Desc->Type = VBMemoryKernel;
            else
                Desc->Type = VBMemoryUsable;
        }
        else if(Desc->Type == EfiReservedMemoryType || Desc->Type == EfiMemoryMappedIO || Desc->Type == EfiMemoryMappedIOPortSpace)
            Desc->Type = VBMemoryReserved;
        else if(Desc->Type == EfiLoaderData || Desc->Type == EfiLoaderCode || Desc->Type == EfiBootServicesData || Desc->Type == EfiBootServicesCode)
            Desc->Type = VBMemoryBootloaderReclaimable;
        else if(Desc->Type == EfiACPIReclaimMemory)
            Desc->Type = VBMemoryACPIReclaimable;
        else if(Desc->Type == EfiACPIMemoryNVS)
            Desc->Type = VBMemoryACPINVS;
        else if(Desc->Type == EfiRuntimeServicesCode || Desc->Type == EfiRuntimeServicesData || Desc->Type == EfiUnusableMemory)
            Desc->Type = VBMemoryUnusable;
        offset += DescSize;
    }

    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
    VMMInit(&BootInfo);
    VMMMapPages(Kernel.Start, Kernel.Start + 0xffffffff80000000, 3, NPAGES(Kernel.End - Kernel.Start));

    KernelEntry(&BootInfo);

    for(;;); // In case kernel returns
}