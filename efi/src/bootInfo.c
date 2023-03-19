#include <efi.h>
#include <efilib.h>
#include <bootInfo.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL* GetGOP()
{
    EFI_STATUS status;
    EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;

    status = BS->LocateProtocol(&GopGuid, NULL, (void**)&GOP);
    if(EFI_ERROR(status))
    {
        Print(L"Unable to locate GOP");
        for(;;);
    }

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info;
    UINTN InfoSize, NumModes;

    status = GOP->QueryMode(GOP, 0, &InfoSize, &Info);
    if(status == EFI_NOT_STARTED)
        status = GOP->SetMode(GOP, 0);
    if(EFI_ERROR(status))
    {
        Print(L"Unable to get native GOP mode");
        for(;;);
    }
    else
        NumModes = GOP->Mode->MaxMode;

    for(UINTN i = 0; i < NumModes; i++)
    {
        GOP->QueryMode(GOP, i, &InfoSize, &Info);
        if(Info->HorizontalResolution == 1920)
        {
            if(Info->VerticalResolution == 1080)
            {
                GOP->SetMode(GOP, i);
                return GOP;
            }
        }
    }
    Print(L"Unable to find suitable GOP mode");
    for(;;);
}

VBInfo GetBootInfo()
{
    VBInfo Info;

    Info.Magic = 0x17B002;

    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP = GetGOP();

    Info.Framebuffer.Base = (UINT32*)GOP->Mode->FrameBufferBase;
    Info.Framebuffer.Size = GOP->Mode->FrameBufferSize;
    Info.Framebuffer.Horiz = GOP->Mode->Info->HorizontalResolution;
    Info.Framebuffer.Vert = GOP->Mode->Info->VerticalResolution;
    Info.Framebuffer.Pitch = 4 * GOP->Mode->Info->PixelsPerScanLine;

    

    return Info;
}