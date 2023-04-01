#include <elf.h>
#include <lib.h>
#include <linux/elf.h>

KernelInfo ParseKernel(FILE f)
{
    Elf64_Ehdr* EHdr = (Elf64_Ehdr*)f.Buffer;
    Elf64_Phdr* PHdr = (Elf64_Phdr*)(f.Buffer + EHdr->e_phoff);
    //void* KernelLocation;
    //BS->AllocatePool(EfiLoaderData, f.Size, &KernelLocation);
    EFI_PHYSICAL_ADDRESS KernelStart = (EFI_PHYSICAL_ADDRESS)PHdr->p_vaddr;
    EFI_PHYSICAL_ADDRESS KernelEnd = KernelStart;
    for(UINT32 i = 0; i < EHdr->e_phnum; i++, PHdr++)
    {
        if(PHdr->p_type != PT_LOAD)
            continue;

        UINT8* VirtualAddr = (UINT8*)PHdr->p_vaddr;
        UINT8* HdrOffset = f.Buffer + PHdr->p_offset;
        for(UINTN j = 0; j < PHdr->p_filesz; j++)
            *(VirtualAddr + j) = *(HdrOffset + j);
        
        VirtualAddr += PHdr->p_filesz;
        for(UINTN j = 0; j < PHdr->p_memsz - PHdr->p_filesz; j++)
            *(VirtualAddr + j) = 0;
        KernelEnd += PHdr->p_memsz;
    }
    return (KernelInfo){EHdr->e_entry, KernelStart, KernelEnd};
}