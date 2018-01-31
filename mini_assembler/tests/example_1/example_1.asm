section .data
     mystr db "hello world",0
     x dd 123

section .text
    global main
    extern printf

main : 
                       
    mov eax,10
    mov eax,ebx
    mov ecx,61
    mov al,90
    add eax,12
    add eax,ebx
    add al,5

