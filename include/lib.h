#ifndef LIB_H
#define LIB_H
#include <efi/efi.h>

extern EFI_SYSTEM_TABLE* ST;
extern EFI_HANDLE IH;
extern EFI_BOOT_SERVICES* BS;
extern EFI_RUNTIME_SERVICES* RS;

void memcpy(void* dest, void* src, UINTN count);
void memset(void* Buffer, UINT8 n, UINTN count);
int memcmp(void* B1, void* B2, UINTN count);
int strcmpw(CHAR16* S1, CHAR16* S2);
void strcpyw(CHAR16* Dest, CHAR16* Src);
int strlenw(CHAR16* str);

#endif