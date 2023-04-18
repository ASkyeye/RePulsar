#include <minwindef.h>
typedef struct _INTERNAL_EXECUTE {
	UINT8 SmbHeader[4];
	UINT8 BackdoorCommand;
	UINT16 ShellcodeLength;
	UCHAR shellcode[4000];
}INTERNAL_EXECUTE, *PINTERNAL_EXECUTE;