               
section .bss
    len:resd 2
    sssss : resd 10

section .data
     mystr db "hello world",0
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

    dec byte[len]
    dec word[len]
    dec dword[len]
                           
    mov eax,10
    mov al,byte[msg]
    mov ax,40
    mov bx,60
    mov byte[mystr],al

    mov al, eax
    mov eax, al

    mov ax, edx
    
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

print1:
   pusha
   push str1
   call printf
   add esp,8
   popa
   add eax,4
   loop print1
