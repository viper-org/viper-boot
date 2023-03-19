#ifndef ELF_H
#define ELF_H
#include <efi.h>
#include <efilib.h>

typedef struct
{
    EFI_PHYSICAL_ADDRESS EntryPoint;
    EFI_PHYSICAL_ADDRESS Start;
    EFI_PHYSICAL_ADDRESS End;
} KernelInfo;

KernelInfo ParseKernel(UINT8* Buffer);

#endif