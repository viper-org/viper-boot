#include <elf.h>

EFI_PHYSICAL_ADDRESS ParseKernel(UINT8* Buffer)
{
    UINT8* Header = Buffer + Buffer[0x20];
    UINT8  HeaderSize = Buffer[0x36];
    for(UINT8 i = 0; i < Buffer[0x38]; i++, Header += HeaderSize)
    {
        if(Header[0x0] != 1)
            continue;
        
        UINT8* VirtualAddr  = (UINT8*)*(UINT64*)(Header + 0x10);
        UINT8* HeaderOffset = Buffer + Header[0x8];
        for(UINT64 j = 0; j < Header[0x20]; j++)
            *(VirtualAddr + j) = *(HeaderOffset + j);

        VirtualAddr += Header[0x20];
        for(UINT64 j = 0; j < Header[0x28] - Header[0x20]; j++)
            *(VirtualAddr + j) = 0;
    }
    return *(UINT64*)(Buffer + 0x18);
}