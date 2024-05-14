#include <types.h>
#include <uefi.h>
#include <kernel.h>

void uefi_init(void *image, struct efi_system_table *systab) {
  systab->ConOut->OutputString(systab->ConOut, L"Hello World\r\n");
  // ExitBootServices
  u64 mmap_sz = 0, map_key = 0, desc_sz = 0;
  void *mmap = NULL;
  // TODO: Should attempt multiplie times
  systab->BootServices->GetMemoryMap(&mmap_sz, mmap, &map_key, &desc_sz, NULL);
  systab->BootServices->ExitBootServices(image, map_key);

  // Begin executing kernel
  kmain();

  while (1);
}

