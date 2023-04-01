#include <config.h>
#include <lib.h>

ConfigInfo ParseCfg(FILE ConfigFile)
{
    CHAR16* name = NULL;
    CHAR16* kernel = NULL;
    BS->AllocatePool(EfiLoaderData, 32 * sizeof(CHAR16), (VOID**)&name);
    BS->AllocatePool(EfiLoaderData, 64 * sizeof(CHAR16), (VOID**)&kernel);
    memset(name, 0, 32);
    memset(kernel, 0, 64);

    CHAR8* cfg = ConfigFile.Buffer;

    UINTN i = 0;
    while(*cfg != ' ' && *cfg != '{')
    {
        if(i >= 31)
            asm volatile("cli; hlt"); // TODO: Error
        name[i++] = *cfg++;
    }

    while(*cfg == '\n' || *cfg == ' ' || *cfg == '\t')
        cfg++;
    cfg++;

    while(*cfg != '}')
    {
        while(*cfg == '\n' || *cfg == ' ' || *cfg == '\t')
            cfg++;
        
        CHAR16 key[16] = {0};
        i = 0;
        while(*cfg != '=' && *cfg != ' ')
        {
            if(i >= 15)
                asm volatile("cli; hlt"); // TODO: Error
            key[i++] = *cfg++;
        }

        cfg++;

        CHAR16 value[64] = {0};
        i = 0;
        while(*cfg != '\n')
        {
            if(i >= 63)
                asm volatile("cli; hlt"); // TODO: Error
            value[i++] = *cfg++;
        }
        
        while(*cfg == '\n' || *cfg == ' ' || *cfg == '\t')
            cfg++;
        
        if(!strcmp(key, L"KERNEL"))
            strcpy(kernel, value);
    }
    return (ConfigInfo){name, kernel};
}