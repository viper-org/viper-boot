#include <file.h>

EFI_FILE_HANDLE GetVolume(EFI_HANDLE ImageHandle)
{
    EFI_LOADED_IMAGE* LoadedImage = NULL;
    EFI_GUID LipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_FILE_IO_INTERFACE* IOVolume;
    EFI_GUID FsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE Volume;

    BS->HandleProtocol(ImageHandle, &LipGuid, (void**)&LoadedImage);
    BS->HandleProtocol(LoadedImage->DeviceHandle, &FsGuid, (void*)&IOVolume);
    IOVolume->OpenVolume(IOVolume, &Volume);

    return Volume;
}

UINT64 FileSize(EFI_FILE_HANDLE FileHandle)
{
    UINT64 result;
    EFI_FILE_INFO* FileInfo;
    FileInfo = LibFileInfo(FileHandle);
    result = FileInfo->FileSize;
    FreePool(FileInfo);
    return result;
}

File ReadFile(EFI_HANDLE ImageHandle, CHAR16* FileName)
{
    EFI_FILE_HANDLE Volume = GetVolume(ImageHandle);
    EFI_FILE_HANDLE FileHandle;

    Volume->Open(Volume, &FileHandle, FileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

    UINT64 ReadSize = FileSize(FileHandle);
    UINT8* Buffer = AllocatePool(ReadSize);
    FileHandle->Read(FileHandle, &ReadSize, Buffer);
    FileHandle->Close(FileHandle);
    return (File){Buffer, ReadSize};
}