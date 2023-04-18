#include <ntddk.h>
#include "shellinject.h"
#ifndef COMMON
#define COMMON


#define REPULSARTAG 0x52655075 // 'RePu'
#define TAGSIZE sizeof(ULONG)
#define CMDSIZE sizeof(UCHAR)
#define SIZEFILED sizeof(USHORT);


#define RVA(base, offset) ((PVOID)((PUCHAR)(base) + (ULONG)(offset)))

__int64 CallbackTable[13];
PVOID DriverImageBase;


typedef VOID(NTAPI* PKNORMAL_ROUTINE)(
    PVOID NormalContext,
    PVOID SystemArgument1,
    PVOID SystemArgument2
    );

typedef VOID KKERNEL_ROUTINE(
    PRKAPC Apc,
    PKNORMAL_ROUTINE* NormalRoutine,
    PVOID* NormalContext,
    PVOID* SystemArgument1,
    PVOID* SystemArgument2
);
typedef VOID(NTAPI* PKRUNDOWN_ROUTINE)(
    PRKAPC Apc
    );


typedef KKERNEL_ROUTINE(NTAPI* PKKERNEL_ROUTINE);

typedef NTSTATUS(__stdcall *SRVNETSTOPCLIENT)(_In_ HANDLE Handle);
typedef NTSTATUS(__stdcall* SRVNETSTARTCLIENT)(_In_ HANDLE Handle);
typedef NTSTATUS(__stdcall *RTLQUERYMODULEINFORMATION)(ULONG *InformationLength, ULONG SizePerModule, PVOID InformationBuffer);
typedef NTSTATUS(__stdcall* SRVNETREGISTERCLIENT)(_In_ __int64 Table, _Out_ PHANDLE HandleToEntry);
typedef PVOID(__stdcall *RTLIMAGEDIRECTORYENTRYTODATA)(IN PVOID Base, IN BOOLEAN MappedAsImage, IN USHORT DirectoryEntry, OUT PULONG Size);
typedef NTSTATUS(__stdcall* SRVNETDEREGISTERCLIENT)(_In_ HANDLE Handle);



typedef struct _RTL_MODULE_EXTENDED_INFO
{
	PVOID BasicInfo;
	ULONG ImageSize;
	USHORT FileNameOffset;
	CHAR FullPathName[258];
} RTL_MODULE_EXTENDED_INFO, *PRTL_MODULE_EXTENDED_INFO;


ZW_CREATE_WORKER_FACTORY __ZwCreateWorkerFactory;
ZW_SET_INFORMATION_WORKER_FACTORY __ZwSetInformationWorkerFactory;

// exported:
PS_GET_PROCESS_IMAGE_FILE_NAME __PsGetProcessImageFileName;
ZW_CREATE_IO_COMPLETION __ZwCreateIoCompletion;



#endif