#include <elf.h>
#include <lib.h>
#include <viper.h>
#include <request/request.h>
#include <linux/elf.h>

KernelInfo ParseKernel(FILE f)
{
    Elf64_Ehdr* EHdr = (Elf64_Ehdr*)f.Buffer;
    Elf64_Phdr* PHdr = (Elf64_Phdr*)(f.Buffer + EHdr->e_phoff);
    Elf64_Shdr* SHdr = (Elf64_Shdr*)(f.Buffer + EHdr->e_shoff);

    uint64_t kernelSize = 0;
    for(UINT32 i = 0; i < EHdr->e_phnum; i++, PHdr++)
        kernelSize += PHdr->p_memsz;
    
    PHdr = (Elf64_Phdr*)(f.Buffer + EHdr->e_phoff);

    void* KernelLocation = NULL;
    BS->AllocatePool(EfiLoaderData, kernelSize, &KernelLocation);
    EFI_PHYSICAL_ADDRESS KernelStart = (EFI_PHYSICAL_ADDRESS)KernelLocation & ~0xFFF;
    EFI_PHYSICAL_ADDRESS KernelEnd = KernelStart;
    for(UINT32 i = 0; i < EHdr->e_phnum; i++, PHdr++)
    {
        if(PHdr->p_type != PT_LOAD)
            continue;

        UINT8* VirtualAddr = (UINT8*)(PHdr->p_vaddr - 0xFFFFFFFF80000000 + KernelStart);
        UINT8* HdrOffset = f.Buffer + PHdr->p_offset;
        for(UINTN j = 0; j < PHdr->p_filesz; j++)
            *(VirtualAddr + j) = *(HdrOffset + j);
        
        VirtualAddr += PHdr->p_filesz;
        for(UINTN j = 0; j < PHdr->p_memsz - PHdr->p_filesz; j++)
            *(VirtualAddr + j) = 0;
        KernelEnd += PHdr->p_memsz;
    }

    // Parse requests
    UINT64 viperMagic[2] = {VIPER_MAGIC};
    for(UINT32 i = 0; i < EHdr->e_shnum; i++, SHdr++)
    {
        if(SHdr->sh_type != SHT_PROGBITS)
            continue;

        void* addr = (void*)(SHdr->sh_addr - 0xFFFFFFFF80000000 + KernelStart);
        for(UINTN j = 0; j < SHdr->sh_size; j++, addr++)
        {
            if(!memcmp(addr, viperMagic, 2 * sizeof(UINT64)))
                ParseRequest(addr);
        }
    }
    return (KernelInfo){EHdr->e_entry, KernelStart, KernelEnd};
}