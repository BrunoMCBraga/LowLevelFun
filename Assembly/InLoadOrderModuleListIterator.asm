;
;
;Author: Bruno Braga
;Loaded Modules Iterator
;Usage: Skeleton code to iterate accross the InLoadOrderModuleList list. 
;
;
;
;
;



mov eax, [fs:eax + 0x30]; _PEB
mov eax, [eax + 0xC] ;Pointer _PEB_LDR_DATA
mov ebx, [eax + 0xC] ;Flink at the head of the list
mov ecx, [eax + 0x10] ;Blink at the head of the list
cmp ebx, ecx
jz endoflist


mov eax, ebx ; eax <- Flink. eax will save the current Flink during the loop

listloop:		

		mov eax, [eax]; Get next Flink
		cmp eax, ebx
		jnz listloop
	    
endoflist: