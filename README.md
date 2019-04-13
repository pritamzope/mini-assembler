<h1>Mini Assembler</h1>
<br/>
<h4>A simple translation of an intel x86 assembly language into opcodes that generates listing files.</h4>
<br/>
<br/>
<strong>symtab.hpp :-</strong> This file contains the declarations of the main symbol table and class SymbolTable to bind it.
<br/>
<strong>symtab.cpp :-</strong> This file contains the implementation of declared functions in symtab.hpp.
<br/>
<strong>errortab.hpp :-</strong> This file contains the list of errors and functions in class ErrorTable.
<br/>
<strong>errortab.cpp :-</strong> This file contains the implementation of function defined in errortab.hpp
<br/>
<strong>optab.hpp :-</strong> This file contains the hashtable for storing the mnemonics and their required parameters with class OPCodeTable.
<br/>
<strong>optab.cpp :-</strong> This file contains the function that stores mnemonics and its information in OPCodeTable.
<br/>
<strong>pass_1.cpp :-</strong> This file performs the pass 1 of the assembler.
<br/>
<strong>pass_2.cpp :-</strong> This file performs the pass 2 of the assembler
<br/> 
<br/>
Pass 1 is performed by file pass_1.cpp,it reads the source code,validate the declarartions and mnemonics by parsing the assembly language source code,generate addresses and write it to the intermediate file.
the intermediate file contains four entries each separated by hash(#) sign.
<br/><br/>

&nbsp;&nbsp;&nbsp;&nbsp;  &lt;linenumber&gt;#&lt;address&gt;#&lt;symbol&gt;#&lt;original_file_content&gt;#

<br/>

here symbol is the name of label defined in the .data or .bss section
<br/>
Pass 2 reads this intermediate file,assemble the instructions by looking opcode in OPCodeTable and write it to lst file
<br/>
The supported instructions are:-<br/>
 &nbsp;<b>inc, dec, mov, add, je, jmp, ja, jb, jae, jbe, jne, jle, pusha, pushad, popa, popad, push, pop, loop, cmp, global, extern </b>
<br/>
</br>
defined in a file opttab.cpp<br/>
More instructions can be added in opttab.cpp source file by adding an entry of mnemnic in table with required parameter such as how many parameters this mnemonic require, its opcode, is it register or memory etc.
<br/>
<h2>Compilation :-</h2>
<br/>
<strong>Requirements :-</strong> GNU/Linux Operating System<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;                G++ - GNU Compiler Collection with C++ Compiler
<br/>
Compile it using following command:-
<h5>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; make </h5>
<br/>
it will create a minassm executable file.it requires two file names separated by -i and -o command line options.
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;-i :- Input file name(.asm)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;-o :- Ouput lst filename;<br/>

<br/>
<h5>&nbsp;&nbsp;&nbsp;&nbsp; e.g.:  ./minassm -i &lt;inputfile.asm&gt; -o lstfile.lst </h5>
<br/>
<h2>Test :-</h2>
<br/>
Run shell script file(run_tests.sh) to test the examples given.<br/>
it takes an arguments of example file numbers such as 1, 2, 3 and 4.
<br/>
e.g.:<br/>
<h5>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; sh run_tests.sh 4 </h5>
<br/>
<b>output</b>
<br/>


asm file = tests/example_4/example.asm
lst file = tests/example_4/example.lst

     1                                  
     2                                  section .bss
     3 00000000 <res 00000008>              len2:resd 2
     5 00000008 <res 00000028>              sssss : resd 10
     6                                      aaaa resb 1
     7                                  
     8                                  section .data
     9 00000000 68656C6C6F20776F72          mystr db "hello world, how are you ?",0
     9 00000009 6C642C20686F772061-
     9 00000012 726520796F75203F00
    10 0000001B 7265733D256400              msg:db "res=%d",0
    11                                       aa2  :   	db "this is string"
    12 0000002B 200000004E00000002          arr       dd 32, 78  , 2,	32,	212134562
    12 00000034 00000020000000CA4E-
    12 00000022 AA20
    13 00000036 0C00220005004300            myar dw 12,34,5,67
    14                                       len dd 101
    15 00000047 776865726520646F20          aa  :   	db "where do you live ?",0
    15 00000050 796F75206C69766520-
    15 0000003E 3F00
    16                                       aa34 dw 20
    17 0000005B 0C0000002D0000004E          str1 dd 12,45,78,00,11
    17 00000064 000000000000000B00-
    17 00000052 0000
    18                                       abcd dd 2
    19                                  
    20                                  section .text
    21                                      global main
    22                                      extern printf
    23                                  
    24                                  main : 
    25                                  
    26 00000000 6640                            inc eax
    27 00000002 6643                            inc ebx
    28 00000004 FEC0                            inc al
    29 00000006 FEC3                            inc bl
    30 00000008 FEC2                            inc dl
    31 0000000A FEC1                            inc cl
    32 0000000C FEC5                            inc ch
    33 0000000E 40                              inc ax
    34 00000010 43                              inc bx
    35 00000012 FE06[1B00]                      inc byte[msg]
    36 00000016 FF06[000]                       inc word[aa34]
    37 0000001A 66FF06[000]                     inc dword[len2]
    38 0000001F FEC8                            dec al
    39 00000020 FECB                            dec bl
    40 00000021 FEC9                            dec cl
    41 00000023 6648                            dec eax
    42 00000025 664B                            dec ebx
    43 00000027 48                              dec ax
    44 00000029 4B                              dec bx
    45 0000002B 49                              dec cx
    46 0000002C FECB                            dec bl
    47 0000002E FE0E[000]                       dec byte[len]
    48 00000032 FF0E[000]                       dec word[len]
    49 00000036 66FF0E[000]                     dec dword[len]
    50                                  
    51 0000003B 66B00A000000                    mov eax,10
    52 0000003F 66C7060[000]0C000000            mov dword[len],12
    53 00000043 66BB0C000000                    mov ebx,12
    54 00000047 66A1[000]                       mov eax,dword[len]
    55 0000004B 668B1E[000]                     mov ebx,dword[len]
    56 0000004F 66A3[000]                       mov dword[len2],eax
    57 00000053 66891E[000]                     mov dword[len2],ebx
    58 00000057 6689                            mov eax,ebx
    59 0000005B 6689                            mov ecx,eax
    60 0000005F 6689                            mov eax,ebx
    61 00000063 6689                            mov edx,ebx
    62 00000067 6689                            mov eax,ecx
    63 0000006B 66B93D000000                    mov ecx,61
    64 0000006F 668B1E[000]                     mov ecx,dword[len]
    65 00000073 668B1E[000]                     mov edx,dword[len]
    66 00000075 A0[1B00]                        mov al,byte[msg]
    67 00000077 89                              mov ax,bx
    68 00000079 89                              mov cx,ax
    69 0000007B 89                              mov ah,al
    70 0000007D B05A                            mov al,90
    71 0000007F B31E                            mov bl,30
    72 00000081 B80280                          mov ax,40
    73 00000083 B833C                           mov bx,60
    74 00000085 A2[000]                         mov byte[mystr],al
    75 00000087 8A1E[000]                       mov bl,byte[mystr]
    76 00000089 880E[000]                       mov byte[mystr],cl
    77 0000008D 66C7060[000]14000000            mov dword[len],20
    78                                  
    79 0000008F 6683C00C000000                  add eax,12
    80 00000093 6601D8                          add eax,ebx
    81 00000095 04005                           add al,5
    82 00000097 01CB                            add ax,bx
    83 00000099 01D8                            add bl,cl
    84                                  
    85 00000099 60                              pusha
    86 00000099 61                              popa
    87 00000099 60                              pushad
    88 00000099 61                              popad
    89                                  
    90                                  iszero:
    91 0000009E 668B1E[000]                     mov ecx,dword[len]
    92 000000A2 66891E[000]                     mov dword[temp],ecx
    93 000000A6                                 mov ebx,arr
    94                                  
    95 000000A6 66833E0[000]00000000            cmp dword[len2],0
    96 000000A6 74                              je exit
    97                                  
    98 000000AB 66FF0E[000]                     dec dword[len2]
    99                                  
   100                                  checkloop :    	
   101 000000AB 66833E0[000]00000000            cmp dword[temp],0
   102 000000AB 74                              je iszero
   103                                  
   104 000000AF 6689                            mov eax,ebx
   105 000000B1 6681304000000                   add ebx,4
   106                                  
   107 000000B1                                 cmp eax,edx
   108 000000B1 7E                              jle checkloop2
   109                                  
   110                                  checkloop2:
   111 000000B5 66FF0E[000]                     dec dword[temp]
   112 000000B5 EB                              jmp checkloop
   113                                  
   114                                  exit:
   115 000000BA 66FF06[000]                     inc dword[temp]
   116 000000BF 668B1E[000]                     mov ecx,dword[temp]
   117 000000C3                                 mov eax,arr
   118                                  
   119 000000C3 3C000                           cmp al,0
   120 000000C3 74                              je exit
   121                                  
   122 000000C3 83F8000                         cmp ax,0
   123 000000C3 74                              je exit
   124                                  
   125 000000C3 6683F8000000000                 cmp eax,0
   126 000000C3 74                              je exit
   127                                  
   128 000000C3 803E0[1B00]00000000             cmp byte[msg],0
   129 000000C3 74                              je exit
   130                                  
   131 000000C3 833E0[000]00000000              cmp word[aa34],0
   132 000000C3 74                              je exit
   133                                  
   134                                  print1:
   135 000000C3 60                              pusha
   136 000000C3 68                              push str1
   137 000000C3 E8(0000)                        call printf
   138 000000C5 6681408000000                   add esp,8
   139 000000C5 61                              popa
   140 000000C9 6683C004000000                  add eax,4
   141 000000C9 E2                              loop print1




