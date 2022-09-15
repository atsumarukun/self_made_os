#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "memory_map.hpp"

EFI_STATUS GetMemoryMap(struct MemoryMap* map) {
  EFI_STATUS status;
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
  CHAR8 memory_map_buf[4096 * 4];
  struct MemoryMap memory_map = {sizeof(memory_map_buf), memory_map_buf, 0, 0, 0};
  GetMemoryMap(&memory_map);
  Print(L"%ld\n", memory_map.map_size);
  while (1);
  return EFI_SUCCESS;
}