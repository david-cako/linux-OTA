#include <Uefi.h>
#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/ShellVariableGuid.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  CHAR16 *VarKey = L"BOOTTOGGLE";
  void *OutputEnvVar[10];
  CHAR16 *TextPath;
  UINTN BufferSize = 0;
  EFI_HANDLE *Buffer = NULL;
  UINTN HandleCounter;
 
  // Get environment variable, testing for L"A" or L"B" value 
  gRT->GetVariable(VarKey, &gShellVariableGuid, 0, &BuffSize, OutputEnvVar);  
  switch (OutputEnvVar) { 
    case L"A":
      *TextPath = L"FS1:\\boot_a.nsh";
      break; 
    case L"B":
      *TextPath = L"FS1:\\boot_b.nsh";
      break;
  }
 
  // Call LocateHandle to get BufferSize
  Status = gBS->LocateHandle(ByProtocol, &gEfiShellProtocolGuid, NULL, &BufferSize, Buffer);  
  
  // AllocateZeroPool using BufferSize
  Buffer = AllocateZeroPool(BufferSize);
  Status = gBS->LocateHandle(ByProtocol, &gEfiShellProtocolGuid, NULL, &BufferSize, Buffer);
  Print(L"locate status: %x\n", Status);
  
  // Iterate through handles
  for (HandleCounter = 0 ; HandleCounter < (BufferSize/sizeof(EFI_HANDLE)) ; HandleCounter++){ 
    Status = gBS->OpenProtocol(Buffer[HandleCounter],
                               &gEfiShellProtocolGuid,
                               (VOID**)&gEfiShellProtocol,
                               ImageHandle, 
                               NULL,
                               EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  }

  Status = gEfiShellProtocol->Execute(&ImageHandle, TextPath, NULL, NULL);
  Print(L"execute status: %p\n", Status);

  return EFI_SUCCESS;
}
