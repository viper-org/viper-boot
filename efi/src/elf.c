#include <elf.h>
#include <linux/elf.h>

KernelInfo ParseKernel(UINT8* Buffer, UINT64 Size)
{
    Elf64_Ehdr* EHeader = (Elf64_Ehdr*)Buffer;
    Elf64_Phdr* PHeader = (Elf64_Phdr*)(Buffer + EHeader->e_phoff);
    void* KernelLocation;
    BS->AllocatePool(EfiConventionalMemory, Size, KernelLocation);
    EFI_PHYSICAL_ADDRESS KernelStart = KernelLocation;
    EFI_PHYSICAL_ADDRESS KernelEnd = KernelLocation + Size;
    for(UINT32 i = 0; i < EHeader->e_phnum; i++, PHeader++)
    {
        if(PHeader->p_type != PT_LOAD)
            continue;
        
        UINT8* VirtualAddr = KernelLocation;
        UINT8* HeaderOffset = (UINT8*)EHeader + PHeader->p_offset;
        for(UINT64 j = 0; j < PHeader->p_filesz; j++)
            *(VirtualAddr + j) = *(HeaderOffset + j);
        
        VirtualAddr += PHeader->p_filesz;
        for(UINT64 j = 0; j < PHeader->p_memsz - PHeader->p_filesz; j++)
            *(VirtualAddr + j) = 0;
    }
    return (KernelInfo){EHeader->e_entry, KernelStart, KernelEnd};
}