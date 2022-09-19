/*
@file Loader.c

カーネルを起動するためのbootファイル.
edk2を利用.
*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

#include "elf.hpp"
#include "frame_buffer.hpp"
#include "memory_map.hpp"

EFI_STATUS OpenRootDir(EFI_HANDLE IMAGE_HANDLE, EFI_FILE_PROTOCOL** root_dir) {
    EFI_STATUS status;

    EFI_LOADED_IMAGE_PROTOCOL* loaded_image_protocol;
    status = gBS->OpenProtocol(
        IMAGE_HANDLE,
        &gEfiLoadedImageProtocolGuid,
        (VOID**) &loaded_image_protocol,
        IMAGE_HANDLE,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    if (EFI_ERROR(status)) {
        return status;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* simple_file_protocol;
    status = gBS->OpenProtocol(
        loaded_image_protocol->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**) &simple_file_protocol,
        IMAGE_HANDLE,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    if (EFI_ERROR(status)) {
        return status;
    }

    return simple_file_protocol->OpenVolume(simple_file_protocol, root_dir);
}

EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* file, VOID** buffer) {
    EFI_STATUS status;

    UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    UINT8 file_info_buffer[file_info_size];
    status = file->GetInfo(file, &gEfiFileInfoGuid, &file_info_size, file_info_buffer);
    if (EFI_ERROR(status)) {
        return status;
    }

    EFI_FILE_INFO* file_info = (EFI_FILE_INFO*) file_info_buffer;
    UINTN file_size = file_info->FileSize;

    status = gBS->AllocatePool(EfiLoaderData, file_size, buffer);
    if (EFI_ERROR(status)) {
        return status;
    }

    return file->Read(file, &file_size, *buffer);
}

EFI_STATUS ReadKernelFile(EFI_FILE_PROTOCOL* root_dir, UINT64* kernel_file_addr, UINT64* kernel_file_size) {
    EFI_STATUS status;

    EFI_FILE_PROTOCOL* kernel_file;
    status = root_dir->Open(root_dir, &kernel_file, L"kernel.elf", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        return status;
    }

    VOID* kernel_file_buf;
    status = ReadFile(kernel_file, &kernel_file_buf);
    if (EFI_ERROR(status)) {
        return status;
    }

    Elf64_Ehdr* kernel_file_header = (Elf64_Ehdr*) kernel_file_buf;
    Elf64_Phdr* kernel_program_header = (Elf64_Phdr*) ((UINT64) kernel_file_header + kernel_file_header->e_phoff);

    *kernel_file_addr = MAX_UINT64;
    UINTN kernel_file_end_addr = 0;
    for (uint16_t i = 0; i < kernel_file_header->e_phnum; i++) {
        if (kernel_program_header[i].p_type != PT_LOAD) continue;
        *kernel_file_addr = MIN(*kernel_file_addr, kernel_program_header[i].p_vaddr);
        kernel_file_end_addr = MAX(kernel_file_end_addr, kernel_program_header[i].p_vaddr + kernel_program_header[i].p_memsz);
    }
    *kernel_file_size = kernel_file_end_addr - *kernel_file_addr;
    UINTN page_num = (*kernel_file_size + 0xfff) / 0x1000;
    status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, page_num, kernel_file_addr);
    if (EFI_ERROR(status)) {
        return status;
    }

    for (uint16_t i = 0; i < kernel_file_header->e_phnum; i++) {
        if (kernel_program_header[i].p_type != PT_LOAD) continue;
        UINT64 kernel_file_segment = (UINT64) kernel_file_header + kernel_program_header[i].p_offset;
        CopyMem((VOID*) kernel_program_header[i].p_vaddr, (VOID*) kernel_file_segment, kernel_program_header[i].p_filesz);
        UINTN remain_bytes = kernel_program_header[i].p_memsz - kernel_program_header[i].p_filesz;
        SetMem((VOID*) (kernel_program_header[i].p_vaddr + kernel_program_header[i].p_filesz), remain_bytes, 0);
    }

    return gBS->FreePool(kernel_file_buf);
}

EFI_STATUS OpenGOP(EFI_HANDLE IMAGE_HANDLE, EFI_GRAPHICS_OUTPUT_PROTOCOL** gop) {
    EFI_STATUS status;

    UINTN gop_handle_num = 0;
    EFI_HANDLE* gop_handles = NULL;
    status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &gop_handle_num,
        &gop_handles
    );
    if (EFI_ERROR(status)) {
        return status;
    }

    status = gBS->OpenProtocol(
        gop_handles[0],
        &gEfiGraphicsOutputProtocolGuid,
        (VOID**) gop,
        IMAGE_HANDLE,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    if (EFI_ERROR(status)) {
        return status;
    }

    return gBS->FreePool(gop_handles);
}

EFI_STATUS GetMemoryMap(MemoryMap* memory_map) {
    if (!memory_map->map_buf) {
        return EFI_BUFFER_TOO_SMALL;
    }

    memory_map->map_size = memory_map->map_buf_size;
    return gBS->GetMemoryMap(
        &memory_map->map_size,
        (EFI_MEMORY_DESCRIPTOR*) memory_map->map_buf,
        &memory_map->map_key,
        &memory_map->descriptor_size,
        &memory_map->descriptor_version
    );
}

EFI_STATUS ExitBootServices(EFI_HANDLE IMAGE_HANDLE, MemoryMap* memory_map) {
    EFI_STATUS status;

    status = gBS->ExitBootServices(IMAGE_HANDLE, memory_map->map_key);
    if (EFI_ERROR(status)) {
        status = GetMemoryMap(memory_map);
        if EFI_ERROR(status) {
            return status;
        }

        status = gBS->ExitBootServices(IMAGE_HANDLE, memory_map->map_key);
        if (EFI_ERROR(status)){
            return status;
        }
    }
    return EFI_SUCCESS;
}

void ErrorHandling(EFI_STATUS status) {
    if (EFI_ERROR(status)) {
        Print(L"%r\n", status);
        while (1) __asm__("hlt");
    }
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE IMAGE_HANDLE, EFI_SYSTEM_TABLE *SYSTEM_TABLE) {
    EFI_STATUS status;

    EFI_FILE_PROTOCOL* root_dir;
    status = OpenRootDir(IMAGE_HANDLE, &root_dir);
    ErrorHandling(status);
    Print(L"Open root directory is successed.\n");

    UINT64 kernel_file_addr, kernel_file_size;
    status = ReadKernelFile(root_dir, &kernel_file_addr, &kernel_file_size);
    ErrorHandling(status);
    Print(L"Read kernel file is successed. address: 0x%0lx, size: %lu bytes\n", kernel_file_addr, kernel_file_size);

    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    status = OpenGOP(IMAGE_HANDLE, &gop);
    ErrorHandling(status);
    Print(L"Open Graphics Output Protocol is successed.\n");

    FrameBuffer frame_buffer = {
        (UINT8*) gop->Mode->FrameBufferBase,
        gop->Mode->Info->PixelsPerScanLine,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        0
    };

    switch (gop->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
            frame_buffer.pixel_format = RGBResv8bit;
            break;
        case PixelBlueGreenRedReserved8BitPerColor:
            frame_buffer.pixel_format = BGRResv8bit;
            break;
        default:
            Print(L"This pixel format is not supported: %d\n", gop->Mode->Info->PixelFormat);
            while (1) __asm__("hlt");
    }

    CHAR8 memory_map_buf[4096 * 4];
    MemoryMap memory_map = {sizeof(memory_map_buf), memory_map_buf, 0, 0, 0, 0};
    status = GetMemoryMap(&memory_map);
    ErrorHandling(status);
    Print(L"Get memory map is successed.\n");

    Print(L"Exit boot services and execute kernel file.\n");
    status = ExitBootServices(IMAGE_HANDLE, &memory_map);
    ErrorHandling(status);

    UINT64 entry_addr = *(UINT64*) (kernel_file_addr + 24);
    typedef void EntryPointType(FrameBuffer*, MemoryMap*);
    EntryPointType* entry_point = (EntryPointType*) entry_addr;
    entry_point(&frame_buffer, &memory_map);

    while (1) __asm__("hlt");
    return EFI_SUCCESS;
}