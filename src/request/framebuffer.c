#include <request/framebuffer.h>
#include <lib.h>

struct ViperFramebufferResponse fb;

EFI_GRAPHICS_OUTPUT_PROTOCOL* GetGOP()
{
    EFI_STATUS status;
    EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;

    status = BS->LocateProtocol(&GopGuid, NULL, (void**)&GOP);
    if(EFI_ERROR(status))
    {
        ST->ConOut->OutputString(ST->ConOut, L"Unable to locate GOP");
        for(;;);
    }

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info;
    UINTN InfoSize, NumModes;

    status = GOP->QueryMode(GOP, 0, &InfoSize, &Info);
    if(status == EFI_NOT_STARTED)
        status = GOP->SetMode(GOP, 0);
    if(EFI_ERROR(status))
    {
        ST->ConOut->OutputString(ST->ConOut, L"Unable to get native GOP mode");
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
    return GOP;
    //ST->ConOut->OutputString(ST->ConOut, L"Unable to find suitable GOP mode");
    //for(;;);
}

struct ViperFramebufferResponse GetFramebuffer()
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = GetGOP();
    fb = (struct ViperFramebufferResponse){
        (void*)gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize, gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution, 4 * gop->Mode->Info->PixelsPerScanLine
    };
    return fb;
}