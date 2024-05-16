#include <types.h>
#include <kernel.h>

#include "uefi.h"
#include "gdt.h"
#include "idt.h"


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


void uefi_init(void *image, struct efi_system_table *systab) {
  systab->ConOut->OutputString(systab->ConOut, L"UEFI BOOTLOADER ESTABLISHED\r\n");

  // Get ImageBase
  struct efi_loaded_image_protocol *LIP = NULL;
  struct efi_guid lip_guid = EFI_LIP_GUID;
  systab->BootServices->HandleProtocol(image, &lip_guid, (void **)&LIP);
  systab->ConOut->OutputString(systab->ConOut, L"ImageBase=0x");
  output_u64hex(systab->ConOut, (u64)LIP->ImageBase);
  systab->ConOut->OutputString(systab->ConOut, L"\r\n");
  // ExitBootServices
  systab->ConOut->OutputString(systab->ConOut, L"Attempting to ExitBootServices...\r\n");
  u64 mmap_sz = 0, map_key = 0, desc_sz = 0;
  void *mmap = NULL;
  // TODO: Should attempt multiplie times
  systab->BootServices->GetMemoryMap(&mmap_sz, mmap, &map_key, &desc_sz, NULL);
  systab->BootServices->ExitBootServices(image, map_key);

  // Setup GDT
  gdt_load();

  // Setup IDT
  idt_load();

  // Begin executing kernel
  kmain();

  while (1);
}

