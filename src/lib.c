#include <lib.h>

EFI_SYSTEM_TABLE* ST;
EFI_BOOT_SERVICES* BS;
EFI_RUNTIME_SERVICES* RS;

void memset(void* Buffer, UINT8 n, UINTN count)
{
    for(UINTN i = 0; i < count; i++)
        ((UINT8*)Buffer)[i] = n;
}

int memcmp(void* B1, void* B2, UINTN count)
{
    UINT8* a = (UINT8*)B1;
	UINT8* b = (UINT8*)B2;
	for (UINTN i = 0; i < count; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

int strcmp(CHAR16* S1, CHAR16* S2)
{
    while(*S1 && (*S1 == *S2))
    {
        S1++;
        S2++;
    }
    return *S1 - *S2;
}

void strcpy(CHAR16* Dest, CHAR16* Src)
{
    while(*Src)
    {
        *Dest = *Src;
        Dest++;
        Src++;
    }
}