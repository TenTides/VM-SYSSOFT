# Virtual Machine and Compiler Project
Team Members: Tyler Crawford and Nelson Herrera Gamboa
Class: COP3402
Last Edit Dates: Various (from 1/27/2023 to 4/8/2023)

Project Overview:
This project involves creating a virtual machine and a compiler, with an embedded lexigraphical analyzer and parse code generator. The system is developed in C and contains multiple components: Virtual Machine (VM), LEX-SYSSOFT LEX Scanner, LEX-SYSSOFT Parse Code Generator, and COMPILER-SYSSOFT COMPILER.

1. Virtual Machine (VM):
Compile using "gcc -g vm.c" to create the executable.
To run, use "./a.out input.txt" format.
Input files are processed during runtime, and the program halts when it reaches the EOP command.
Passing no input file will result in a segmentation fault.

2. LEX-SYSSOFT LEX Scanner:
Compile using "gcc -g lex.c" to create the executable.
To run, use "./a.out input.txt" format.
The scanner tokenizes the code and outputs a lexeme list.
Passing no input file will result in a segmentation fault.

3. LEX-SYSSOFT Parse Code Generator:
Compile using "gcc -g parsecodegen.c" to create the executable.
To run, use "./a.out input.txt" format.
The generator halts on syntax errors and outputs error messages.
If successful, assembly codes are printed to "outputAssemblyCode.txt".
Passing no input file will result in an error output to the terminal.


4. COMPILER-SYSSOFT COMPILER:
Compile using "gcc -g h4compiler.c" to create the executable.
To run, use "./a.out input.txt" format.
The compiler halts on syntax errors and outputs error messages.
If successful, assembly codes are printed to "elf.txt", and the VM executes the code, displaying the process on the screen.
Passing no input file will result in an error output to the terminal.




# Virtual Machine
Team Leader: Tyler Crawford   
Member: Nelson Herrera Gamboa
Class: COP3402    
Date of Last Edit: 1/27/2023

To properly run or vm, the file must be properly 
compliled first. Use "gcc -g vm.c" 
to generate a file from the vm.c file to run.

Thereafter, input files can be passed through the vm as
command line arugments at runtime. The format to do so can 
be seen below:

$ ./a.out input.txt

During runtime, insert any required numbers until 
the program reaches EOP command and halts.

*not passing an input file will result in a segmentation fault*

# LEX-SYSSOFT LEX Scanner
Team Leader: Tyler Crawford   
Member: Nelson Herrera Gamboa
Class: COP3402    
Date of Last Edit: 2/14/2023

To properly run or vm, the file must be properly 
compliled first. Use "gcc -g lex.c" 
to generate a file from the vm.c file to run.

Thereafter, input files can be passed through the lex as
command line arugments at runtime. The format to do so can 
be seen below:

$ ./a.out input.txt

During runtime, insert any required numbers until 
the program reaches EOP command and halts.

The program will not stop when tokenizing the code. Take the following code segment
for example:


```
var if abcabcabacabss%cabc end % var /*   
%%%    
898989%89
*/
,ty,ty,
%tly%tly%
&&
```

```
The output will be:

Source Program:
var if abcabcabacabss%cabc end % var /*   
%%%    
898989%89
*/
,ty,ty,
%tly%tly%
&&


Lexeme      Token Type
var         29
if          23
abcabcabacabss Invalid Identifier, exceeds maxLength of 11
%         (Invalid Symbol)
cabc         2
end         22
%         (Invalid Symbol)
var         29
/*       Unresolved In Line Comment Error
%         (Invalid Symbol)
%         (Invalid Symbol)
%         (Invalid Symbol)
898989    Invalid Number, exceeds maxDigits of 5
%         (Invalid Symbol)
89           3
*            6
/            7
,           17
ty           2
,           17
ty           2
,           17
%         (Invalid Symbol)
tly          2
%         (Invalid Symbol)
tly          2
%         (Invalid Symbol)
&         (Invalid Symbol)
&         (Invalid Symbol)


Lexeme List:
29 23 2 cabc 22 29 3 89 6 7 17 2 ty 17 2 ty 17 2 tly 2 tly
```
*not passing an input file will result in a segmentation fault*

# LEX-SYSSOFT Parse Code Generator
Team Leader: Tyler Crawford   
Member: Nelson Herrera Gamboa
Class: COP3402    
Date of Last Edit: 3/8/2023

To properly run our Parse Code Generator, the file must be properly 
compliled first. Use "gcc -g parsecodegen.c" 
to generate a file from the parsecodegen.c file to run.

Thereafter, input files can be passed through the parsecodegen as
command line arugments at runtime. The format to do so can 
be seen below:

$ ./a.out input.txt

The program will not stop when tokenizing the code only if there are valid symbols.
During runtime, syntax Errors within the code will halt the program and output an error to the terminal.
*not passing an input file will result in a Error being output to the Terminal*

At the end of the program the assembly codes are printed out into the outputAssemblyCode.txt if the program runs
with no errors.

# COMPILER-SYSSOFT COMPILER
Team Leader: Tyler Crawford   
Member: Nelson Herrera Gamboa
Class: COP3402    
Date of Last Edit: 4/8/2023

To properly run our Parse Code Generator, the file must be properly 
compliled first. Use "gcc -g h4compiler.c" 
to generate a file from the h4compiler.c file to run.

Thereafter, input files can be passed through the parsecodegen as
command line arugments at runtime. The format to do so can 
be seen below:

$ ./a.out input.txt

The program will not stop when tokenizing the code only if there are valid symbols.
During runtime, syntax Errors within the code will halt the program and output an error to the terminal.
*not passing an input file will result in a Error being output to the Terminal*

At the end of the program the assembly codes are printed out into the elf.txt if the program runs
with no errors. And then the vm executes the code and shows its process on screen
