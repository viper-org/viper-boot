#ifndef CONFIG_H
#define CONFIG_H
#include <efi/efi.h>
#include <file.h>

typedef struct
{
    CHAR16* Name;
    CHAR16* KernelPath;
} ConfigInfo;

ConfigInfo ParseCfg(FILE ConfigFile);

#endif