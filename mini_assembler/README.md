<h1>Mini Assembler</h1>
<br/><br/>
A simple translation of an intel x86 assembly language syntax into opcodes that generates an listing files.
<br/>
<br/>
symtab.hpp :- This file contains the declarations of main symbol table and class SymbolTable to bind it.
<br/>
symtab.cpp :- This file contains the implementation of declared functions in symtab.hpp.
<br/>
errortab.hpp :- This file contains the list of errors and functions in class ErrorTable.
<br/>
errortab.cpp :- This file contains the implementation of function defined in errortab.hpp
<br/>
optab.hpp :- This file contains the hashtable for storing the mnemonics and their required parameters with class OPCodeTable.
<br/>
optab.cpp :- This file contains the function that stores mnemonics and its information in OPCodeTable.
<br/>
pass_1.cpp :- This file performs the pass 1 of the assembler.
<br/>
pass_2.cpp :- This file performs the pass 2 of the assembler
<br/> 
<br/>
Pass 1 is performed by file pass_1.cpp,it reads the source code,validate it,generate addresses and write it to the intermediate file.
<br/>
the intermediate file contains four entries each separated by hash(#) sign.
<br/>

  &lt;linenumber&gt;#&lt;address&gt;#&lt;symbol&gt;#&lt;original_file_content&gt;#

<br/>

here symbol is the name of label defined in the .data or .bss section
<br/>
Pass 2 reads this intermediate file,assemble the instructions by looking opcode in OPCodeTable and write it to lst file
<br/>
The supported instructions are:-
	1) inc <br/>
	2) dec <br/>
	3) mov <br/>
	4) add <br/>
	5) je <br/>
	6) jmp <br/>
	7) ja <br/>
	8) jb <br/>
	9) jae <br/>
	10) jbe <br/>
	11) jne <br/>
	12) jle <br/>
	13) pusha <br/>
	14) pushad <br/>
	15) popa <br/>
	16) popad <br/>
	17) push <br/>
	18) pop <br/>
	19) loop <br/>
	20) cmp <br/>
	21) global <br/>
	22) extern <br/>

defined in a file opttab.cpp

<br/><br/>
Compilation :-
<br/>
Requirements :- GNU/Linux Operating System<br/>
                G++ - GNU Compiler Collection with C++ Compiler
<br/>
Compile it using following command:-
<br/>

$>  make

<br/>
it will create a minassm executable file.it requires two file names separated by -i and -o command line options.
<br/>
-i :- Input file name(.asm)<br/>
-o :- Ouput lst filename;<br/>

<br/>
e.g.:  ./minassm -i &lt;inputfile.asm&gt; -o lstfile.lst
<br/>

Test :-
<br/>
Run shell script file(run_tests.sh) to test the examples given.<br/>
it takes an arguments of example file numbers such as 1, 2, 3 and 4.
<br/>
e.g.:<br/>
$> sh run_tests.sh 1
<br/>














