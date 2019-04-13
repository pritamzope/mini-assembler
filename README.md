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
 &nbsp;inc, dec, mov, add, je, jmp, ja, jb, jae, jbe, jne, jle, pusha, pushad, popa, popad, push, pop, loop, cmp, global, extern 
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
<h5>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; sh run_tests.sh 1 </h5>
<br/>


