#ifndef _UEFI_H
#define _UEFI_H
#include <types.h>

#define EFI_BUFFER_TOO_SMALL (((s64) 5) < 0)

struct efi_output_interface {
  void* Reset;
  u64 (*OutputString)(void *, u16 *);
  // Other stuff...
};

struct efi_guid {
  u32 Data1;
  u16 Data2;
  u16 Data3;
  u8  Data4[8];
};

#define EFI_LIP_GUID { 0x5B1B31A1, 0x9562, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} }

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiUnacceptedMemoryType,
  EfiMaxMemoryType
} efi_memory_type_t;

struct efi_loaded_image_protocol {
  u32   Revision;
  void *ParentHandle;
  void *SystemTable;
  void *DeviceHandle;
  void *FilePath;
  void *Reserved;
  u32   LoadOptionsSize;
  void *LoadOptions;
  void *ImageBase;
  u64   ImageSize;
  efi_memory_type_t ImageCodeType;
  efi_memory_type_t ImageDataType;
};

struct efi_memory_desc {
  u32 Type;
  u32 Pad;
  u64 PhysicalStart;
  u64 VirtualStart;
  u64 NumberOfPages;
  u64 Attribute;
};

#define NEXT_DESC(Ptr,Size) ((struct efi_memory_desc *) (((u8 *) Ptr) + Size))

struct efi_boot_services {
  char _skip[24];

  void* RaiseTPL;
  void* RestoreTPL;

  void* AllocatePages;
  void* FreePages;
  u64 (*GetMemoryMap)(u64 *, void *, u64 *, u64 *, u32 *);
  u64 (*AllocatePool)(efi_memory_type_t, u64, void **);
  void* FreePool;

  void* CreateEvent;
  void* SetTimer;
  void* WaitForEvent;
  void* SignalEvent;
  void* CloseEvent;
  void* CheckEvent;

  void* InstallProtocolInterface;
  void* ReinstallProtocolInterface;
  void* UninstallProtocolInterface;
  u64 (*HandleProtocol)(void *, struct efi_guid *, void **);
  void* PCHandleProtocol;
  void* RegisterProtocolNotify;
  void* LocateHandle;
  void* LocateDevicePath;
  void* InstallConfigurationTable;

  void* LoadImage;
  void* StartImage;
  void* Exit;
  void* UnloadImage;
  u64 (*ExitBootServices)(void *, u64 MapKey);

  void* GetNextHighMonotonicCount;
  void* Stall;
  void* SetWatchdogTimer;

  void* ConnectController;
  void* DisconnectController;

  void* OpenProtocol;
  void* CloseProtocol;
  void* OpenProtocolInformation;

  void* ProtocolsPerHandle;
  void* LocateHandleBuffer;
  void* LocateProtocol;
  void* InstallMultipleProtocolInterfaces;
  void* UninstallMultipleProtocolInterfaces;

  void* CalculateCrc32;
};

struct efi_system_table {
  char   _skip0[24];
  void  *_skip1;
  u32    _skip2;
  void  *_skip3;
  void  *_skip4;
  void  *_skip5;
  struct efi_output_interface *ConOut;
  void  *_skip6;
  struct efi_output_interface *ConErr;
  void  *_skip7;
  struct efi_boot_services *BootServices;
  // More data, we ignore...
};

#endif

