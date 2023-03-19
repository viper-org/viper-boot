#include <elf.h>
#include <linux/elf.h>

KernelInfo ParseKernel(UINT8* Buffer)
{
    Elf64_Ehdr* EHeader = (Elf64_Ehdr*)Buffer;
    Elf64_Phdr* PHeader = (Elf64_Phdr*)(Buffer + EHeader->e_phoff);
    EFI_PHYSICAL_ADDRESS KernelStart = PHeader->p_vaddr;
    EFI_PHYSICAL_ADDRESS KernelEnd = KernelStart;
    for(UINT32 i = 0; i < EHeader->e_phnum; i++, PHeader++)
    {
        if(PHeader->p_type != PT_LOAD)
            continue;
        
        UINT8* VirtualAddr = (UINT8*)PHeader->p_vaddr;
        UINT8* HeaderOffset = (UINT8*)EHeader + PHeader->p_offset;
        for(UINT64 j = 0; j < PHeader->p_filesz; j++)
            *(VirtualAddr + j) = *(HeaderOffset + j);
        
        VirtualAddr += PHeader->p_filesz;
        KernelEnd += PHeader->p_memsz;
        for(UINT64 j = 0; j < PHeader->p_memsz - PHeader->p_filesz; j++)
            *(VirtualAddr + j) = 0;
    }
    return (KernelInfo){EHeader->e_entry, KernelStart, KernelEnd};
}