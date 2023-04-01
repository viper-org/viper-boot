#ifndef ELF_H
#define ELF_H
#include <efi.h>
#include <file.h>

typedef struct
{
    EFI_PHYSICAL_ADDRESS EntryPoint;
    EFI_PHYSICAL_ADDRESS Start;
    EFI_PHYSICAL_ADDRESS End;
} KernelInfo;

KernelInfo ParseKernel(FILE f);

#endif