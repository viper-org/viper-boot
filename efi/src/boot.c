#include <efi.h>
#include <efilib.h>
#include <file.h>
#include <elf.h>
#include <bootInfo.h>

extern VBInfo GetBootInfo();
typedef void(*KMain)();

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    UINT8* Buffer = ReadFile(ImageHandle, L"kernel.elf").Buffer;
    EFI_PHYSICAL_ADDRESS kernel = ParseKernel(Buffer);
    VBInfo BootInfo = GetBootInfo();

    Print(L"%x", kernel);

    KMain KernelEntry = (KMain)kernel;

    UINTN MapSize = 0, MapKey, DescSize;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
    UINT32 DescVers;
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, NULL, &DescSize, NULL);

    MapSize += DescSize * 2;

    SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&MemoryMap);
    SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, &MapKey, &DescSize, &DescVers);
    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

    VBFramebuffer fb = BootInfo.Framebuffer;
    *((unsigned int*)(fb.Base + fb.Pitch * 50 + 4 * 50)) = 0xFF00FF;

    KernelEntry();

    for(;;); // In case kernel returns
}