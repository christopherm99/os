#include <types.h>
#include <kernel.h>

#include "uefi.h"
#include "gdt.h"
#include "idt.h"

#define CHECK(e,msg) { \
  u64 status = e;\
  if (status) { \
    systab->ConOut->OutputString(systab->ConOut, msg); \
    switch (status & 0xFF) { \
      case 2: \
        systab->ConOut->OutputString(systab->ConOut, L"EFI_INVALID_PARAMETER\r\n"); \
        break; \
      default: \
        systab->ConOut->OutputString(systab->ConOut, L"Unrecognized error code: 0x"); \
        u16 str[5] = {}; \
        str[0] = tohex((status / 16) % 16); \
        str[1] = tohex(status % 16); \
        str[2] = L'\r'; \
        str[3] = L'\n'; \
        systab->ConOut->OutputString(systab->ConOut, str); \
        break; \
    } \
    return status; \
  } \
}

u16 tohex(u64 v) {
  if (v < 10)
    return '0' + v;
  return 'A' + v - 10;
}

void output_u64hex(struct efi_output_interface *con, u64 v) {
  u16 str[17] = {};
  for (u32 i = 0; i < 17; i++)
    str[i] = '0';
  u32 i = 15;
  while (v > 0) {
    str[i] = tohex(v % 16);
    v /= 16;
    i--;
  }
  con->OutputString(con, str);
}

u64 uefi_init(void *image, struct efi_system_table *systab) {
  systab->ConOut->OutputString(systab->ConOut, L"UEFI BOOTLOADER ESTABLISHED\r\n");

  // Get ImageBase
  struct efi_loaded_image_protocol *LIP = NULL;
  struct efi_guid lip_guid = EFI_LIP_GUID;
  CHECK(systab->BootServices->HandleProtocol(image, &lip_guid, (void **)&LIP),
      L"Failed to get HandleProtocol: ")
  /*systab->ConOut->OutputString(systab->ConOut, L"ImageBase=0x");
  output_u64hex(systab->ConOut, (u64)LIP->ImageBase);
  systab->ConOut->OutputString(systab->ConOut, L"\r\n");
  */

  // GetMemoryMap
  // First, we need to know the size to malloc
  u64 mmap_sz = 0, map_key = 0, desc_sz = 0;
  // Note that this will fail, but will still tell us the size of the mmap
  systab->BootServices->GetMemoryMap(&mmap_sz, NULL, &map_key, &desc_sz, NULL);

  // Now we can AllocatePool for our mmap
  struct efi_memory_desc *mmap = NULL;
  mmap_sz += 4 * desc_sz; // Worst case scenario
  CHECK(systab->BootServices->AllocatePool(LIP->ImageDataType, mmap_sz, (void **)&mmap),
      L"Failed to AllocatePool: ")
  // Finally, we have the memory map
  CHECK(systab->BootServices->GetMemoryMap(&mmap_sz, mmap, &map_key, &desc_sz, NULL),
      L"Failed to GetMemoryMap (2): ")

  // ExitBootServices
  systab->ConOut->OutputString(systab->ConOut, L"Attempting to ExitBootServices...\r\n");
  u8 count = 0;
  while (systab->BootServices->ExitBootServices(image, map_key)) {
    if (count > 3) {
      systab->ConOut->OutputString(systab->ConOut, L"Failed to ExitBootServices\r\n");
      return -1;
    }
    mmap_sz += 4 * desc_sz; // Worst case scenario
    CHECK(systab->BootServices->AllocatePool(LIP->ImageDataType, mmap_sz, (void **)&mmap),
        L"Failed to AllocatePool: ")
    systab->BootServices->GetMemoryMap(&mmap_sz, mmap, &map_key, &desc_sz, NULL);
    count++;
  }

  // Setup GDT
  gdt_load();

  // Setup IDT
  idt_load();

  // Begin executing kernel
  kmain((void *)mmap, mmap_sz, desc_sz);

  while (1);
  return 0;
}

