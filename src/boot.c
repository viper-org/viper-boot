#include <efi/efi.h>
#include <lib.h>
#include <file.h>
#include <elf.h>
#include <config.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <request/memmap.h>

typedef void(*EntryPoint)();

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    ST = SystemTable;
    IH = ImageHandle;
    BS = SystemTable->BootServices;
    RS = SystemTable->RuntimeServices;

    FILE ConfigFile = ReadFile(ImageHandle, L"viper.cfg");
    ConfigInfo cfg = ParseCfg(ConfigFile);

    FILE KernelFile = ReadFile(ImageHandle, cfg.KernelPath);
    KernelInfo Kernel = ParseKernel(KernelFile);
    EntryPoint KernelEntry = (EntryPoint)Kernel.EntryPoint;

    UINTN MapSize = 0, MapKey, DescSize;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINT32 DescVers;
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, NULL, &DescSize, NULL);

    MapSize += DescSize * 2;

    SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&MemoryMap);
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, &MapKey, &DescSize, &DescVers);

    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

    PMMInit(MemoryMap, MapSize, DescSize);
    VMMInit(MemoryMap, MapSize, DescSize, Kernel.Start, Kernel.End);

    void* KernelStack = PMMGetPage();
    asm volatile("mov %0, %%rsp" : : "m"(KernelStack));

    InitMemoryMap(MemoryMap, MapSize, DescSize, Kernel.Start, Kernel.End);

    KernelEntry();
    for(;;);
}