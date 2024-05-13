#include <types.h>
#include <uefi.h>
#include <kernel.h>

void uefi_init(void *image, struct efi_system_table *systab) {
  systab->ConOut->OutputString(systab->ConOut, L"Hello World\n");

  /*// ExitBootServices!
  u64 mmap_sz = 0, map_key = 0, desc_sz = 0;
  void *mmap = NULL;
  systab->BootServices->GetMemoryMap(&mmap_sz, mmap, &map_key, &desc_sz, NULL);
  systab->BootServices->ExitBootServices(image, map_key);

  // Kernel Time!
  kmain();

  while (1);*/
}
