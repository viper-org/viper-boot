#ifndef MEM_PMM_H
#define MEM_PMM_H
#include <efi/efi.h>
#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 0x1000
#define NPAGES(num) (((num) + PAGE_SIZE - 1) / PAGE_SIZE)
void PMMInit(EFI_MEMORY_DESCRIPTOR* memoryMap, UINTN mapSize, UINTN descSize);

bool PMMAlloc(void* address, uint32_t npages);

void* PMMGetPage();
void PMMFreePage(void* address);

void* PMMGetPages(uint32_t count);
void PMMFreePages(void* address, uint32_t count);

#endif