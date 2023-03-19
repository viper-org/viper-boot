#include <vmm.h>
#include <efilib.h>

UINT64* pml4;

#define PAGE_SIZE 4096
#define PRESENT_WRITABLE 0x3

void VMMMapPage(UINT64 PhysAddr, UINT64 VirtAddr, UINT64 Flags)
{
    PhysAddr &= ~0xFFF;
    VirtAddr &= ~0xFFF;
    Flags    &=  0xFFF;

    UINT64 pml4i = (VirtAddr >> 39) & 0x1FF;
    UINT64 pml3i = (VirtAddr >> 30) & 0x1FF;
    UINT64 pml2i = (VirtAddr >> 21) & 0x1FF;
    UINT64 pml1i = (VirtAddr >> 12) & 0x1FF;

    UINT64* pml3;
    UINT64* pml2;
    UINT64* pml1;

    if(!(pml4[pml4i] & 0x1))
    {
        BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, (EFI_PHYSICAL_ADDRESS*)pml4[pml4i]);
        Print(L"q");
        pml4[pml4i] &= ~0xFFF;
        SetMem((void*)pml4[pml4i], PAGE_SIZE, 0);
        pml4[pml4i] |= PRESENT_WRITABLE;
    }
    pml3 = (UINT64*)pml4[pml4i];

    if(!(pml3[pml3i] & 0x1))
    {
        BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, (EFI_PHYSICAL_ADDRESS*)pml3[pml3i]);
        pml3[pml3i] &= ~0xFFF;
        SetMem((void*)pml3[pml3i], PAGE_SIZE, 0);
        pml3[pml3i] |= PRESENT_WRITABLE;
    }
    pml2 = (UINT64*)pml3[pml3i];

    if(!(pml2[pml2i] & 0x1))
    {
        BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, (EFI_PHYSICAL_ADDRESS*)pml2[pml2i]);
        pml2[pml2i] &= ~0xFFF;
        SetMem((void*)pml2[pml2i], PAGE_SIZE, 0);
        pml2[pml2i] |= PRESENT_WRITABLE;
    }
    pml1 = (UINT64*)pml2[pml2i];

    pml1[pml1i] = PhysAddr | Flags | PRESENT_WRITABLE;

    Print(L"%x         %x", pml1[pml1i] & ~0xFFF, PhysAddr);
}