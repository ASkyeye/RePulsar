#include <iostream>
#include <Windows.h>

void shellcode_size(DWORD size, DWORD offset, PULONG buffer) {
	if (size > 0xff) {
	while (size > 0xff)
	{
		DWORD _size = size - 0xff;
		*(ULONG*)(buffer + offset) = size;
		
	}
	}
}