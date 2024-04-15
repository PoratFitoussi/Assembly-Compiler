# Assembly-Compiler
Assembler for a lighted version of Assembly, 
The assembler receives as input a file containing a program in assembly language, and its role is to convert the source code into code machine, and build an output file containing the machine code.

Detecting errors in the source program:
The assembler performs a careful parsing of the source program in assembly language, and is able to detect and report a variety of errors, such as: an operation-name that is not defined, an incorrect number of operands, an operand type that is not suitable for the operation, a register number that does not exist, use of a symbol that is not defined, a symbol that is defined more than once, a numerical value of abnormal size, and other errors.
The assembler performs a careful parsing of the source program in assembly language, and is able to detect and report a variety of errors, such as: an operation-name that is not defined, an incorrect number of operands, an operand type that is not suitable for the operation, a register number that does not exist, use of a symbol that is not defined, a symbol that is defined more than once, a numerical value of abnormal size, and other errors.

Assembler output files:
For each input file assembled without errors, the assembler creates output files as follows.
1. object file, which contains the machine code.
2. The externals file, containing details of all the places (addresses) in the machine code where a declared symbol is encoded
as external (a symbol that appeared as an operand of the extern directive, and characterized in the symbol table as external).
3. Entries file, which contains details about each symbol that is declared as an entry point (a symbol that appeared as an operand of the instruction
entry, and characterized in the symbol table as entry).
