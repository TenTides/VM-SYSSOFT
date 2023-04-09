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
