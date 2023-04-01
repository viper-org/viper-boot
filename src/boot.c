#include <efi/efi.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello world");
    for(;;);
}