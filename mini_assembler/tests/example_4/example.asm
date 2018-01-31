               
section .bss
    len2:resd 2
    temp:resd  5
    sssss : resd 10
    aaaa resb 1

section .data
     mystr db "hello world, how are you ?",0
     msg:db "res=%d",0
     aa2  :   	db "this is string"
     arr       dd 32, 78  , 2,	32,	212134562
     myar dw 12,34,5,67
     len dd 101
     aa  :   	db "where do you live ?",0
     aa34 dw 20
     str1 dd 12,45,78,00,11
     abcd dd 2

section .text
    global main
    extern printf

main : 
                       
    inc eax
    inc ebx
    inc al
    inc bl
    inc dl
    inc cl
    inc ch
    inc ax
    inc bx
    inc byte[msg]
    inc word[aa34]
    inc dword[len2]
    dec al
    dec bl
    dec cl
    dec eax
    dec ebx
    dec ax
    dec bx
    dec cx
    dec bl
    dec byte[len]
    dec word[len]
    dec dword[len]
                           
    mov eax,10
    mov dword[len],12
    mov ebx,12
    mov eax,dword[len]
    mov ebx,dword[len]
    mov dword[len2],eax
    mov dword[len2],ebx
    mov eax,ebx
    mov ecx,eax
    mov eax,ebx
    mov edx,ebx
    mov eax,ecx
    mov ecx,61
    mov ecx,dword[len]
    mov edx,dword[len]
    mov al,byte[msg]
    mov ax,bx
    mov cx,ax
    mov ah,al
    mov al,90
    mov bl,30
    mov ax,40
    mov bx,60
    mov byte[mystr],al
    mov bl,byte[mystr]
    mov byte[mystr],cl
    mov dword[len],20
    
    add eax,12
    add eax,ebx
    add al,5
    add ax,bx
    add bl,cl
                                 
    pusha
    popa
    pushad
    popad
                                 
iszero:
    mov ecx,dword[len]
    mov dword[temp],ecx
    mov ebx,arr
                             
    cmp dword[len2],0
    je exit
                               
    dec dword[len2]
                                  
checkloop :    	
   cmp dword[temp],0
   je iszero
                                
   mov eax,ebx
   add ebx,4
                               
   cmp eax,edx
   jle checkloop2
                             
checkloop2:
   dec dword[temp]
   jmp checkloop
                              
exit:
   inc dword[temp]
   mov ecx,dword[temp]
   mov eax,arr
                          
   cmp al,0
   je exit
                       
   cmp ax,0
   je exit
                              
   cmp eax,0
   je exit
                             
   cmp byte[msg],0
   je exit
                             
   cmp word[aa34],0
   je exit
                            
print1:
   pusha
   push str1
   call printf
   add esp,8
   popa
   add eax,4
   loop print1
