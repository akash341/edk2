#include<uefi.h>
#include<Library\UefiApplicationEntryPoint.h>
//#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include<Protocol\SimpleTextOut.h>
#include <Protocol\SimpleTextIn.h>
#include<Library/DebugLib.h>
//#include<stdio.h>
//#include<stdlib.h>


EFI_SYSTEM_TABLE 		*gST;
EFI_BOOT_SERVICES 		*gBS;
EFI_RUNTIME_SERVICES 	*gRT;

EFI_STATUS EFIAPI
UefiMain(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable
)
{
	EFI_STATUS Status;
	EFI_TIME *Time;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *SimpleTextOutput;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL *SimpleTextInput;
	EFI_INPUT_KEY Key;
	UINTN Index;
	EFI_EVENT TimerEvent;
	EFI_EVENT WaitList[2];
	UINT64 Timeout = 0;
	
	gST = SystemTable;
	gBS = gST->BootServices;
	gRT = gST->RuntimeServices;
	
	SimpleTextOutput = gST->ConOut;
	SimpleTextInput = gST->ConIn;
	
	DEBUG ((DEBUG_INFO, "=============Test App ENTER=============\n"));
	
	Status = gST->ConOut->OutputString (gST->ConOut, L"Hello Akash\n\r");
	
	if(EFI_ERROR(Status)){
		return Status;
	}
	
	Status = gBS->AllocatePool (EfiBootServicesData, sizeof(EFI_TIME), (VOID **)&Time);
	if(EFI_ERROR(Status)){
		return Status;
	}
	

//	String[0] = (UINT16)Time->Minute;

	while(1){
	Status = gRT->GetTime(Time,NULL);
	if(EFI_ERROR(Status)){
		return Status;
	}
	AsciiPrint ("%d:%d:%d\n",Time->Hour, Time->Minute, Time->Second);
//	gBS->Stall(1000000);
//	SimpleTextOutput->ClearScreen(SimpleTextOutput);
	
	DEBUG ((DEBUG_INFO, "-----> Waiting event\n"));
	
	Status = gBS->CreateEvent(EVT_TIMER,0,NULL,NULL,&TimerEvent);
	if(!EFI_ERROR(Status)){
		
	gBS->SetTimer(TimerEvent,TimerRelative,Timeout);
		
	WaitList[0] = SimpleTextInput->WaitForKey;
    WaitList[1] = TimerEvent;
	  
	Status = gBS->WaitForEvent(1,WaitList,&Index);
	DEBUG ((DEBUG_INFO, "-----> WaitForEvent Status %r \n",Status));
	ASSERT (!EFI_ERROR (Status));
    ASSERT (Index == 0);
	if(!EFI_ERROR(Status)){
	DEBUG ((DEBUG_INFO, "-----> Event Status %r \n",Status));
	//gBS->Stall(3000000);
	SimpleTextInput->ReadKeyStroke(SimpleTextInput,&Key);
	if(Key.UnicodeChar ==CHAR_BACKSPACE){
		DEBUG ((DEBUG_INFO, "-----> Break\n"));
		break;
	}
	}
	}
	DEBUG ((DEBUG_INFO, "-----> After readkeyStroke\n"));
	
/*	switch(Key.UnicodeChar)
	{
		case CHAR_BACKSPACE:
		{
			break;
		}
	}*/
	

	
	}

/*	Status = gST->ConOut->OutputString (gST->ConOut, String);
	
	if(EFI_ERROR(Status)){
		return Status;
	}*/
	Status = gST->ConOut->OutputString (gST->ConOut, L"ExitApp\n\r");
	if(EFI_ERROR(Status)){
		return Status;
	}
	
	DEBUG ((DEBUG_INFO, "=============Test App EXIT=============\n"));
	return EFI_SUCCESS;
}