#include <request/request.h>
#include <request/framebuffer.h>
#include <lib.h>
#include <file.h>
#include <viper.h>

struct ViperModule* modules;
uint8_t moduleCount = 0;

struct ViperMemmapResponse* MemMap = NULL;

struct ViperModuleResponse ModuleResponse;

void ParseRequest(void* requestAddr, KernelInfo info)
{
    UINT64* id = (UINT64*)requestAddr;
    switch(*(id + 2))
    {
        case VIPER_BOOT_INFO_MAGIC:
        {
            struct ViperBootInfoRequest* req = (struct ViperBootInfoRequest*)requestAddr;

            void* addr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperBootInfoResponse), &addr);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing boot information");
            req->response = addr;

            req->response->version = 100;

            addr += 0xFFFF800000000000;
            req->response = addr;
            break;
        }
        case VIPER_MODULE_MAGIC:
        {
            struct ViperModuleRequest* req = (struct ViperModuleRequest*)requestAddr;

            void* addr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperModuleResponse), &addr);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing modules");
            req->response = addr;

            req->response->modules = (void*)modules + 0xFFFF800000000000;
            req->response->count = moduleCount;

            addr += 0xFFFF800000000000;
            req->response = addr;
            break;
        }
        case VIPER_FRAMEBUFFER_MAGIC:
        {
            struct ViperFramebufferRequest* req = (struct ViperFramebufferRequest*)requestAddr;

            void* addr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperFramebufferResponse), &addr);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing framebuffer");
            req->response = addr;

            struct ViperFramebufferResponse fb = GetFramebuffer();
            memcpy(req->response, &fb, sizeof(struct ViperFramebufferResponse));
            req->response->base += 0xFFFF800000000000;

            addr += 0xFFFF800000000000;
            req->response = addr;
            break;
        }
        case VIPER_MEMMAP_MAGIC:
        {
            struct ViperMemmapRequest* req = (struct ViperMemmapRequest*)requestAddr;
            
            void* addr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperMemmapResponse), &addr);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initialzing memory map");
            req->response = addr;
            
            MemMap = req->response;

            addr += 0xFFFF800000000000;
            req->response = addr;
            break;
        }
        case VIPER_KERNEL_LOCATION_MAGIC:
        {
            struct ViperKernelLocationRequest* req = (struct ViperKernelLocationRequest*)requestAddr;
            
            void* addr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperKernelLocationResponse), &addr);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initialzing memory map");
            req->response = addr;

            req->response->physicalBase = (void*)info.Start;
            req->response->virtualBase  = (void*)0xFFFFFFFF80000000;
            req->response->size         = info.End - info.Start;

            addr += 0xFFFF800000000000;
            req->response = addr;
            break;
        }
        default:
            break;
    }
}

void AddModule(CHAR16* fileName)
{
    if(!modules)
        BS->AllocatePool(EfiLoaderData, sizeof(struct ViperModule) * 16, (void**)&modules);

    FILE file = ReadFile(IH, fileName);

    char* buf;
    BS->AllocatePool(EfiLoaderData, strlenw(fileName) * sizeof(char) + 1, (void**)&buf);
    for(int i = 0; i < strlenw(fileName); i++)
        buf[i] = fileName[i];

    modules[moduleCount++] = (struct ViperModule){file.Buffer + 0xFFFF800000000000, file.Size, buf + 0xFFFF800000000000};
}