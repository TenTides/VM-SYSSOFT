/*
    ===========================PROJECT 4 NOTES===========================

    >Introducing "Procedure" implementation.
    We will also include "else"

    >Predictive Parsing
    (First and follow Sets)

    >Predictive parsing table

    >We are now going from system Software Compiler to System Software Operatings
    ===============
    >We will now be working on the Assembler which is a translator between assembly code to binary code

    >We will be using a one address machine which consits of 16 bits: 4 bit instruction and 12 bit address

    > the firs step would be to convert the mneumonic into its binary representation.
    > next we would take its address and covert that as well into its binary representation

    > Assembler directive help the assembler do the translation. They tell the assembler when the program starts. To reserve memory. then we need to tell the assembler where the program ends.

    >1st step create a new symbol table
    >2nd step is to then do the translate

    >The object code file will have several sections

        >header: Size of code, name source file, size of data *
        >text sections: object code*
        >data section: Data (in binary)*
        >relocation information section: Address to be fixed up by the linker*
        >symbol table section: Global symbolas int he program, imported symbols*
        >debugging section: Source file and line number information, description of data structures.


    The location in memory wehre the text section and data section will be loaded can be in different parts in memory however each will be have information contigous in memory.

    > a.out stands for "assembler output"
    >the whole point of having assembly code is that we don't have to create a new compiler for each operating system.

*/
