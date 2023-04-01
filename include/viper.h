#ifndef VIPER_H
#define VIPER_H
#include <stdint.h>

#define VIPER_MAGIC 0x7638c0c9cf567885, 0x80430eef20c20f7a


// Bootloader information

#define BOOT_INFO_MAGIC 0x45e5fb847b93fb1b
#define BOOT_INFO { VIPER_MAGIC, BOOT_INFO_MAGIC }

struct BootInfo
{
    char* Name;
    uint64_t Version;
};

struct BootInfoRequest
{
    uint64_t id[3];
    struct BootInfo* response;
};

#endif