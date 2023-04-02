#ifndef REQUEST_H
#define REQUEST_H
#include <efi/efi.h>

void ParseRequest(void* requestAddr);

void AddModule(CHAR16* fileName);

#endif