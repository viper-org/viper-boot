#include <stdint.h>
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
#define VIPER_MAGIC 0x7638c0c9cf567885, 0x80430eef20c20f7a


// Bootloader information

#define BOOT_INFO_MAGIC 0x45e5fb847b93fb1b
#define BOOT_INFO { VIPER_MAGIC, BOOT_INFO_MAGIC }
static volatile struct BootInfoRequest bootInfo = {
    .id = BOOT_INFO
};

int kmain()
{
    return bootInfo.response->Version;
}