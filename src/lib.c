#include <lib.h>

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RS;

void memset(void* Buffer, UINT8 n, UINTN count)
{
    for(UINTN i = 0; i < count; i++)
        ((UINT8*)Buffer)[i] = n;
}