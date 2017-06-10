;Routine: computeCheckSum. The algorithm is:
;												1.Compute how many dwords the string has
;												2.Iterate over those and chain-xor the results
;												3.Padd the remaining bytes and xor with the results of the loop
;
;Arguments: (pointer to string (DWORD), size of string (DWORD))       

;use32 For FASM

computeCheckSum: push ebp
				 mov ebp, esp
				 
				 mov eax, [ebp + 0xC] ; Size of string 
				 mov bl, 0x4
				 div bl ; AL <- has now the number of words on the string 
				 movzx ecx, al; ecx <- number of dwords on the string
				 shr eax, 0x8 ; remove al
				 movzx edx, al ; dl <- number of incomplete dword bytes

				 mov eax, 0x00000000 ; default eax.
				 cmp ecx, 0x0
				 jz checksumEnd

				 mov esi, [esp + 0x8] ; Pointer to string
				 mov ebx, 0x00000000 ; initially xor is with 0x00000000

xorLoop:	     lodsd ; loads dword from esi to eax
				 xor ebx, eax ; result so far xor new dword
				 loop xorLoop

				 xor eax, eax
				 cmp edx, 0x0
				 jz checksumEnd

padThree:		 cmp edx, 0x1 ; dword with three bytes missing
				 jnz padTwo ; 
				 lodsb 
				 shl eax, 0x18 ; byte must be shifted to the end of a word. Padding with zeros
				 xor ebx, eax
				 mov eax, ebx 
				 jmp checksumEnd

padTwo:			 cmp edx, 0x2 ; dword with two bytes missing
				 jnz padOne
				 lodsw
				 shl eax, 0x10 ; pad two bytes with zero
				 xor ebx, eax 
				 mov eax, ebx
				 jmp checksumEnd

padOne:			 lodsd ; dword with one byte missing. We load the full dword and then clear the last byte
				 and eax, 0xFFFFFF00
				 xor ebx, eax
				 mov eax, ebx
checksumEnd:
				 pop ebp
				 ret


