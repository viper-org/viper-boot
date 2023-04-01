#ifndef LIB_H
#define LIB_H
#include <efi/efi.h>

extern EFI_SYSTEM_TABLE* ST;
extern EFI_BOOT_SERVICES* BS;
extern EFI_RUNTIME_SERVICES* RS;

void memset(void* Buffer, UINT8 n, UINTN count);

#endif