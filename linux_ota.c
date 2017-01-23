#include <Uefi.h>
#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/ShellVariableGuid.h>

UINTN VarBuffSize = 10;
CHAR16 *UpdateKey = L"UPDATEFLAG";  
CHAR16 *ToggleKey = L"BOOTTOGGLE";
CHAR16 *CountKey = L"BOOTCOUNT";
void *UpdateValue[10]; // these might need to be larger
void *ToggleValue[10];
void *CountValue[10];
CHAR16 *TextPath;
CONST CHAR16 *PathA = L"FS1:\\boot_a.nsh";
CONST CHAR16 *PathB = L"FS1:\\boot_b.nsh";
UINTN HandleSize = 0;
UINTN HandleCounter;

void SetVar (
    IN CHAR16 *VarKey,
    IN CHAR16 *VarVal
    )
{
    gRT->SetVariable(VarKey, 
                    &gShellVariableGuid, 
                    EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS,
                    VarBuffSize, 
                    VarVal);
}

EFI_STATUS
EFIAPI
UefiMain (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
    )
{
    EFI_STATUS Status;
    EFI_HANDLE *Buffer = NULL;

    // Get toggle variable
    gRT->GetVariable(ToggleKey, &gShellVariableGuid, 0, &VarBuffSize, ToggleValue);  

    // Get update flag value
    gRT->GetVariable(UpdateKey, &gShellVariableGuid, 0, &VarBuffSize, UpdateValue); 

    // test if update flag is enabled
    if (*UpdateValue == L"0"){ // no update in process
            if (*ToggleValue == L"A"){
                *TextPath = *PathA;
            } 
            else if (*ToggleValue == L"B"){ 
                *TextPath = *PathB;
            }
    }
    else if (*UpdateValue == L"1"){  // update in process
        // Get count variable, ensuring previous boot wasn't failed
        gRT->GetVariable(CountKey, &gShellVariableGuid, 0, &VarBuffSize, CountValue); 
        if (*CountValue == L"0"){ 
            if (*ToggleValue == L"A"){ 
                *TextPath = *PathA;
            }
            else if (*ToggleValue == L"B"){
                *TextPath = *PathB;
            }
            SetVar(CountKey, L"1");
        } 
        else if (*CountValue == L"1"){ // Previous boot unsuccessful, set to opposite boot toggle value
            if (*ToggleValue == L"A"){ 
                SetVar(ToggleKey, L"B");
                *TextPath = *PathB;
            }
            else if (*ToggleValue == L"B"){
                SetVar(ToggleKey, L"A");
                *TextPath = *PathA;
            }
            SetVar(UpdateKey, L"0");
            SetVar(CountKey, L"0");
        }
    }

    // Call LocateHandle to get BufferSize
    Status = gBS->LocateHandle(ByProtocol, &gEfiShellProtocolGuid, NULL, &HandleSize, Buffer);  
  
    // AllocateZeroPool using BufferSize
    Buffer = AllocateZeroPool(HandleSize);
    Status = gBS->LocateHandle(ByProtocol, &gEfiShellProtocolGuid, NULL, &HandleSize, Buffer);
    Print(L"locate status: %x\n", Status);
  
    // Iterate through handles
    for (HandleCounter = 0 ; HandleCounter < (HandleSize/sizeof(EFI_HANDLE)) ; HandleCounter++){ 
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
