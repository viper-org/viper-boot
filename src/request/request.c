#include <request/request.h>
#include <lib.h>
#include <viper.h>

void ParseRequest(void* requestAddr)
{
    UINT64* id = (UINT64*)requestAddr;
    switch(*(id + 2))
    {
        case BOOT_INFO_MAGIC:
        {
            struct BootInfoRequest* req = (struct BootInfoRequest*)requestAddr;
            BS->AllocatePool(EfiLoaderData, sizeof(struct BootInfo), (void**)req->response);
            req->response->Version = 100;
            break;
        }
        default:
            break;
    }
}