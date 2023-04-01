#ifndef FILE_H
#define FILE_H
#include <efi/efi.h>

typedef struct
{
    UINT8* Buffer;
    UINT64 Size;
} FILE;

FILE ReadFile(EFI_HANDLE ImageHandle, CHAR16* FileName);

#endif