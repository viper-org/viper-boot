#ifndef FILE_H
#define FILE_H
#include <efi.h>
#include <efilib.h>

typedef struct
{
    UINT8* Buffer;
    UINT64 Size;
} File;

File ReadFile(EFI_HANDLE ImageHandle, CHAR16* FileName);

#endif