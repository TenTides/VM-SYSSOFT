// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/15/2023

//Project Questions//
/*
    > When we are inserting a procedure into the symbol table, why do we leave its address to a question mark?

    >How will be connecting our vm and our lexeme?

    >




*/

//Project Rules//
/*
    > When seeking a variable name during a lookup. The seek pointer should be pointing at the same location as the table pointer(tp), and then iterate the back towards index 0. (seek = tp)

    > When we call a new procedure the we must set mark == 1 for all of the variables in the previous procedure.s

    > Everytime a variable is called we must complete a lookup throught the Symbol table

    > If we try to use a variable that was not declared we can use the sentinel technique. This technique is where we leave the index 0 of the array empty to store the name of the varible we are seeking so that if that variable is found and seek == 0, then the variable was not declared and an error in the message must be emmited. 


*/


typedef struct{
    int kind; //const = 1, var = 2, proc = 3.
    char name[11]; // name up to 11 chars.(identifer)
    int val; // number (ASCII value)
    int level; // L level
    int adr; // M address
    int mark; // 0 = in use for code generation, 1 = unavailable.

} namerecord_t;

const MAX_NAME_TABLE_SIZE = 0;

int main(int argc, char* argv[]) {
    


    return 0;
}