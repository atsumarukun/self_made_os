#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/BlockIo.h>
#include <Guid/FileInfo.h>

#include "memory_map.hpp"

EFI_STATUS OpenRootDir(EFI_HANDLE IMAGE_HANDLE, EFI_FILE_PROTOCOL** root_dir) {
  EFI_STATUS status;

  EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
    status = gBS->OpenProtocol(
      IMAGE_HANDLE,
      &gEfiLoadedImageProtocolGuid,
      (VOID**)&loaded_image,
      IMAGE_HANDLE,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
      );
    if (EFI_ERROR(status)) {
      return status;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* simple_file;
    status = gBS->OpenProtocol(
      loaded_image->DeviceHandle,
      &gEfiSimpleFileSystemProtocolGuid,
      (VOID**)&simple_file,
      IMAGE_HANDLE,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
      );
    if (EFI_ERROR(status)) {
      return status;
    }

    simple_file->OpenVolume(simple_file, root_dir);
    return EFI_SUCCESS;
}

EFI_PHYSICAL_ADDRESS LoadKernelFile(EFI_FILE_PROTOCOL* root_dir) {
  EFI_FILE_PROTOCOL* kernel_file;
  root_dir->Open(root_dir, &kernel_file, L"kernel.elf", EFI_FILE_MODE_READ, 0);

  UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
  UINT8 file_info_buffer[file_info_size];
  kernel_file->GetInfo(kernel_file, &gEfiFileInfoGuid, &file_info_size, file_info_buffer);

  EFI_FILE_INFO* file_info = (EFI_FILE_INFO*)file_info_buffer;
  UINTN kernel_file_size = file_info->FileSize;

  EFI_PHYSICAL_ADDRESS kernel_base_addr = 0x100000;
  gBS->AllocatePages(AllocateAddress, EfiLoaderData, (kernel_file_size + 0xfff) / 0x1000, &kernel_base_addr);
  kernel_file->Read(kernel_file, &kernel_file_size, (VOID*)kernel_base_addr);
  Print(L"Kernel: 0x%0lx (%lu bytes)\n", kernel_base_addr, kernel_file_size);
  return kernel_base_addr;
}

EFI_STATUS GetMemoryMap(struct MemoryMap* map) {
  if (map->map_buf == NULL) {
    return EFI_BUFFER_TOO_SMALL;
  }

  map->map_size = map->map_buf_size;
  return gBS->GetMemoryMap(
    &map->map_size,
    (EFI_MEMORY_DESCRIPTOR*) map->map_buf,
    &map->map_key,
    &map->descriptor_size,
    &map->descriptor_version
  );
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

  EFI_PHYSICAL_ADDRESS kernel_base_addr = LoadKernelFile(root_dir);

  CHAR8 memory_map_buf[4096 * 4];
  Print(L"%ld\n", sizeof(memory_map_buf));
  struct MemoryMap memory_map = {sizeof(memory_map_buf), memory_map_buf, 0, 0, 0, 0};
  status = GetMemoryMap(&memory_map);
  ErrorHandling(status);
  Print(L"Get memory map is successed.\n");

  status = gBS->ExitBootServices(IMAGE_HANDLE, memory_map.map_key);
  if (EFI_ERROR(status)) {
    status = GetMemoryMap(&memory_map);
    if (EFI_ERROR(status)) {
      Print(L"failed to get memory map: %r\n", status);
      while (1);
    }
    status = gBS->ExitBootServices(IMAGE_HANDLE, memory_map.map_key);
    if (EFI_ERROR(status)) {
      Print(L"Could not exit boot service: %r\n", status);
      while (1);
    }
  }

  UINT64 entry_addr = *(UINT64*) (kernel_base_addr + 24);
  typedef void EntryPointType(void);
  EntryPointType* entry_point = (EntryPointType*)entry_addr;
  entry_point();

  while (1) __asm__("hlt");
  return EFI_SUCCESS;
}