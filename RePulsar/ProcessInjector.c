/*#include <ntddk>
typedef struct _EPROCESS {
    BYTE _pad0[0x2e8];
    ULONG UniqueProcessId;
    LIST_ENTRY ActiveProcessLinks;
    BYTE _pad1[0x150];
    UCHAR ImageFileName[15];
    BYTE _pad2[0x39];
    UINT32 ActiveThreads;
} EPROCESS;
DWORD _pid(){UCHAR *name}{
    CONST PEPROCESS startProcess = PsGetCurrentProcess();

    PEPROCESS currentProcess = startProcess;

    do {
        if (!currentProcess->ActiveThreads)
            continue;

        STRING currentName;
        RtlInitString(&currentName, (PCSZ)currentProcess->ImageFileName);

        if (RtlEqualString(name, &currentName, FALSE))
            return currentProcess->UniqueProcessId;

    } while ((currentProcess = CONTAINING_RECORD(currentProcess->ActiveProcessLinks.Flink, EPROCESS, ActiveProcessLinks)) != startProcess);

}

void InjectProcess(DWORD pid, UCHAR *Buf){
  //
  //inject shellcode in process by apc
  //
}*/
