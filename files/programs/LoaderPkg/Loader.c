#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "memory_map.hpp"

EFI_STATUS GetMemoryMap(struct MemoryMap* map) {
  EFI_STATUS status;

  if (!map->map_buf) {
    return EFI_BUFFER_TOO_SMALL;
  }

  status = gBS->GetMemoryMap(
    &map->map_size,
    (EFI_MEMORY_DESCRIPTOR*) map->map_buf,
    &map->map_key,
    &map->descriptor_size,
    &map->descriptor_version
  );
  return status;
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
  EFI_STATUS status;

  CHAR8 memory_map_buf[4096 * 4];
  struct MemoryMap memory_map = {0, memory_map_buf, 0, 0, 0};
  status = GetMemoryMap(&memory_map);
  if (EFI_ERROR(status)) {
    Print(L"%r\n", status);
    while (1) __asm__("hlt");
  }

  Print(L"%ld\n", memory_map.map_size);
  
  while (1) __asm__("hlt");
  return EFI_SUCCESS;
}