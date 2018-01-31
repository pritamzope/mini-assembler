               
section .bss
    len resd 2

section .data
     mystr db "hello world",0
     msg:db "res=%d",0
     aa2  :   	db "this is string"
     arr       dd 32, 78  , 2,	32,	212134562

section .text
    global main
    extern printf

main : 
                       
    inc word[aa34]
    inc dword[len2]
    dec byte[len]
    dec word[len]
    dec dword[len]
                           
    mov dword[len2],eax

iszero:
    mov ecx,dword[len]
    mov dword[temp],ecx
    mov ebx,arr
                             
    cmp dword[len2],0
    je exit
                               
    dec dword[len2]
                                  

