#ifndef ELF_H
#define ELF_H
#include <efi.h>
#include <efilib.h>

EFI_PHYSICAL_ADDRESS ParseKernel(UINT8* Buffer);

#endif