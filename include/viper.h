#ifndef VIPER_H
#define VIPER_H
#include <stdint.h>

#define VIPER_MAGIC 0x7638c0c9cf567885, 0x80430eef20c20f7a


// Boot information

#define VIPER_BOOT_INFO_MAGIC 0x45e5fb847b93fb1b
#define VIPER_BOOT_INFO { VIPER_MAGIC, VIPER_BOOT_INFO_MAGIC }

struct ViperBootInfoResponse
{
    uint64_t version;
};

struct ViperBootInfoRequest
{
    uint64_t id[3];
    struct ViperBootInfoResponse* response;
};


// Modules

#define VIPER_MODULE_MAGIC 0xf432391f30da9fa6
#define VIPER_MODULE { VIPER_MAGIC, VIPER_MODULE_MAGIC }

struct ViperModule
{
    void* address;
    uint64_t size;
    char* name;
};

struct ViperModuleResponse
{
    uint8_t count;
    struct ViperModule* modules;
};

struct ViperModuleRequest
{
    uint64_t id[3];
    struct ViperModuleResponse* response;
};


// Framebuffer

#define VIPER_FRAMEBUFFER_MAGIC 0x3f9f2267faf12a57
#define VIPER_FRAMEBUFFER { VIPER_MAGIC, VIPER_FRAMEBUFFER_MAGIC }

struct ViperFramebufferResponse
{
    void* base;
    uint64_t size;
    uint64_t width;
    uint64_t height;
    uint16_t pitch;
};

struct ViperFramebufferRequest
{
    uint64_t id[3];
    struct ViperFramebufferResponse* response;
};


// Memory map

#define VIPER_MEMMAP_MAGIC 0x3804de92f9e0c569
#define VIPER_MEMMAP { VIPER_MAGIC, VIPER_MEMMAP_MAGIC }

enum ViperMemmapType
{
    ViperMemmapUsable,
    ViperMemmapReserved,
    ViperMemmapBootloaderReclaimable,
    ViperMemmapAcpiNvs,
    ViperMemmapAcpiReclaimable,
    ViperMemmapKernelModules
};

struct ViperMemmapEntry
{
    enum ViperMemmapType type;
    uint64_t base;
    uint64_t size;
};

struct ViperMemmapResponse
{
    uint64_t count;
    struct ViperMemmapEntry* entries;
};

struct ViperMemmapRequest
{
    uint64_t id[3];
    struct ViperMemmapResponse* response;
};


// Kernel location

#define VIPER_KERNEL_LOCATION_MAGIC 0xc8ce8f3e3d18804e
#define VIPER_KERNEL_LOCATION { VIPER_MAGIC, VIPER_KERNEL_LOCATION_MAGIC }

struct ViperKernelLocationResponse
{
    void* physicalBase;
    void* virtualBase;
    uint64_t size;
};

struct ViperKernelLocationRequest
{
    uint64_t id[3];
    struct ViperKernelLocationResponse* response;
};


// RSDP

#define VIPER_RSDP_MAGIC 0x7dc5e2feb8b64329
#define VIPER_RSDP { VIPER_MAGIC, VIPER_RSDP_MAGIC }

struct ViperRSDPResponse
{
    void* rsdp;
};

struct ViperRSDPRequest
{
    uint64_t id[3];
    struct ViperRSDPResponse* response;
};

#endif