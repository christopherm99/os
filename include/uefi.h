#ifndef _UEFI_H
#define _UEFI_H
#include <types.h>

struct efi_output_interface {
  void* Reset;
  u64 (*OutputString)(void *This, u16 *String);
  // Other stuff...
};


struct efi_boot_services {
  char _skip[24];

  void* RaiseTPL;
  void* RestoreTPL;

  void* AllocatePages;
  void* FreePages;
  u64 (*GetMemoryMap)(u64 *MemoryMapSize, void *MemoryMap, u64 *MapKey,
      u64 *DescriptorSize, u32 *DescriptorVersion);
  void* AllocatePool;
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
  void* HandleProtocol;
  void* PCHandleProtocol;
  void* RegisterProtocolNotify;
  void* LocateHandle;
  void* LocateDevicePath;
  void* InstallConfigurationTable;

  void* LoadImage;
  void* StartImage;
  void* Exit;
  void* UnloadImage;
  u64 (*ExitBootServices)(void *ImageHandle, u64 MapKey);

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
  char _skip0[78];
  struct efi_output_interface *ConOut;
  char _skip1[16];
  void *RuntimeServices;
  struct efi_boot_services *BootServices;
  // More data, we ignore...
};

#endif

