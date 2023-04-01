#include <efi/efi.h>
#include <lib.h>
#include <file.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    ST = SystemTable;
    BS = SystemTable->BootServices;
    RS = SystemTable->RuntimeServices;
    
    FILE f = ReadFile(ImageHandle, L"test.txt");
    CHAR16 buffer[f.Size];
    for(UINTN i = 0; i < f.Size; i++)
        buffer[i] = f.Buffer[i];
    
    ST->ConOut->OutputString(ST->ConOut, buffer);
    for(;;);
}