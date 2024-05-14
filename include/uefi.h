#ifndef _UEFI_H
#define _UEFI_H
#include <types.h>

struct efi_output_interface {
  void* Reset;
  u64 (*OutputString)(void *, u16 *);
  // Other stuff...
};


struct efi_boot_services {
  char _skip[24];

  void* RaiseTPL;
  void* RestoreTPL;

  void* AllocatePages;
  void* FreePages;
  u64 (*GetMemoryMap)(u64 *, void *, u64 *, u64 *, u32 *);
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

