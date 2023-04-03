#ifndef REQUEST_H
#define REQUEST_H
#include <efi/efi.h>
#include <elf.h>

void ParseRequest(void* requestAddr, KernelInfo info);

void AddModule(CHAR16* fileName);

#endif