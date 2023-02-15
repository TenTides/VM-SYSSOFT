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

The program will also stop prematurely if an error is detected 
while tokenizing the code. Hence if an error is detected, the program
will halt and the output would only be the printed out source code and as well as 
the error statement dictating the type of error along with the line of code, that
caused the error.

*not passing an input file will result in a segmentation fault*