#include <request/request.h>
#include <request/framebuffer.h>
#include <lib.h>
#include <file.h>
#include <viper.h>

struct ViperModule* modules = NULL;
uint8_t moduleCount = 0;

struct ViperMemmapResponse* MemMap = NULL;

void ParseRequest(void* requestAddr)
{
    UINT64* id = (UINT64*)requestAddr;
    switch(*(id + 2))
    {
        case VIPER_BOOT_INFO_MAGIC:
        {
            struct ViperBootInfoRequest* req = (struct ViperBootInfoRequest*)requestAddr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperBootInfoResponse), (void**)req->response);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing boot information");
            req->response->version = 100;
            break;
        }
        case VIPER_MODULE_MAGIC:
        {
            struct ViperModuleRequest* req = (struct ViperModuleRequest*)requestAddr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperModuleResponse), (void**)req->response);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing modules");
            req->response->modules = modules;
            req->response->count = moduleCount;
            break;
        }
        case VIPER_FRAMEBUFFER_MAGIC:
        {
            struct ViperFramebufferRequest* req = (struct ViperFramebufferRequest*)requestAddr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperFramebufferResponse), (void**)req->response);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initializing framebuffer");
            struct ViperFramebufferResponse fb = GetFramebuffer();
            memcpy(req->response, &fb, sizeof(struct ViperFramebufferResponse));
            req->response->base += 0xFFFF800000000000;
            break;
        }
        case VIPER_MEMMAP_MAGIC:
        {
            struct ViperMemmapRequest* req = (struct ViperMemmapRequest*)requestAddr;
            EFI_STATUS status = BS->AllocatePool(EfiLoaderData, sizeof(struct ViperMemmapResponse), (void**)req->response);
            if(EFI_ERROR(status))
                ST->ConOut->OutputString(ST->ConOut, L"Error initialzing memory map");
            MemMap = req->response;
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

    char* buf = NULL;
    BS->AllocatePool(EfiLoaderData, strlenw(fileName) * sizeof(char), (void**)&buf);
    for(int i = 0; i < strlenw(fileName); i++)
        buf[i] = fileName[i];

    modules[moduleCount++] = (struct ViperModule){file.Buffer, file.Size, buf};
}