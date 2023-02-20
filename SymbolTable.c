// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/15/2023

//Project Questions//
/*
    > When we are inserting a procedure into the symbol table, why do we leave its address to a question mark?

    > How will be connecting our vm and our lexeme? 





*/

//Project Rules//
/*
    // Symbol Table

    > When seeking a variable name during a lookup. The seek pointer should be pointing at the same location as the table pointer(tp), and then iterate the back towards index 0. (seek = tp)

    > When we call a new procedure the we must set mark == 1 for all of the variables in the previous procedure.s

    > Everytime a variable is called we must complete a lookup throught the Symbol table

    > If we try to use a variable that was not declared we can use the sentinel technique. This technique is where we leave the index 0 of the array empty to store the name of the varible we are seeking so that if that variable is found and seek == 0, then the variable was not declared and an error in the message must be emmited.

    //Grammar/Syntax (Context Free Grammar)

    > to check for sytanx we will create function that check sytax rules of each section of the code. Each time we call a function we must check if the rules are followed or print an error to the screen if the sytnax is wrong. This will allow us to check the sytanx of many differnt looking peices of code. 

    > A context free language is defined by a 4-tuple(T,N,R,S)
        1 terminal symbols (valid words that are a within  syntatic class)(T)
        2 non-terminal Symbols (aka the syntatic classes)(N)
        3 Syntatic equations(R)
        4 Start Symbol(S)

    > rule get a token and call the <program> function all the way down to a terminal symbol or sytanx error. By doing this we create a sort of tree structure more specifically inorder search. 


    > basically while parsing the lexeme token by token we will use rules (also known as left most derivation).

    Example code

    <expr> {

        if (  "("  == token) {
            call <expr>

            if ( ")" == token){
                return; 
            } else {
                
            }
             
        }
        ...
        ...
        ...
    }

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