// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/15/2023

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   125       // Assuming max codeline length is 50

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//

char* lexicalParse(char* codeLine);
int numberOfFileLines(char* filename);
int characterInSymbolTableBS(char c, char* symTbl);
int isStatementReserved(char* word);            
char* subString(int start, int end,char* line);  
int isWordValid(char* word);
void PROGRAM();
void BLOCK();
int GET_TOKEN();
void EXPRESSION();
void STATEMENT();
void CONDTION();
void TERM();
void FACTOR();
namerecord_t* initializeNameRecord(int _kind, char* _name, int _val, int _level, int _adr, int _mark);

typedef enum {  
    identsym = 2, 
    numbersym = 3, 
    plussym = 4 , 
    minussym = 5, 
    multsym = 6,  
    slashsym = 7, 
    oddsym = 8, 
    eqsym = 9, 
    neqsym = 10, 
    lessym = 11, 
    leqsym = 12, 
    gtrsym = 13, 
    geqsym = 14, 
    lparentsym = 15, 
    rparentsym = 16, 
    commasym = 17, 
    semicolonsym = 18, 
    periodsym = 19, 
    becomessym = 20, 
    beginsym = 21, 
    endsym = 22, 
    ifsym = 23, 
    thensym = 24,  
    whilesym = 25, 
    dosym = 26, 
    constsym = 28, 
    varsym = 29, 
    writesym = 31, 
    readsym = 32, 
}token_type; 

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//

char* resWords[] = {"odd", "begin", "end", "if", "then", "while", "do", "const",  "var",  "write", "read"}; 
int resWordsTokens[] = {oddsym, beginsym, endsym, ifsym, thensym, whilesym, dosym, constsym,  varsym, writesym , readsym};   
//Symbol table is in order of ascii value, can be used with binary search
char  symbolTableOrdered[] = {'\t','\r',' ','(',')','*', '+',',', '-' ,'.', '/', '0', '1', '2','3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
                        '=', '>','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                        'X', 'Y', 'Z','a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 
                        'v', 'w', 'x', 'y', 'z'};   


//Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
char specialTerminalSymbolsOrdered[] = {'\t','\r',' ', '(',')','*', '+',',', '-' ,'.', '/', ':', ';','<','=','>'}; // ' ' isn't a term sym, it was put here
int specialTerminalSymbolsTokens[] = {-3,-2,-1, lparentsym, rparentsym, multsym, plussym, commasym, minussym ,periodsym, slashsym, 0, semicolonsym ,lessym, eqsym, gtrsym}; // -1 is for spaces and 0 is for colons and -2 for tabs,                                                                                                                            
int halt_flag = 1; 
int EndProgramFlag = 1;   

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//
// char  letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
//                         'X', 'Y', 'Z','a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 
//                         'v', 'w', 'x', 'y', 'z'};   

// char numbers[] = {'0', '1', '2','3', '4', '5', '6', '7', '8', '9'};
char* relOP[] = {"=","<>","<","<=",">",">="};

int universialIndex = 0;

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//
char* lexicalParse(char* codeLine)
{
    //Copy space of parent string
    char* parsedString = malloc(sizeof(codeLine));
    parsedString[0] = '\0';
    //Start is set to zero as no special characters have been detected yet
    int start = 0;
    for(int i = 0; i<strlen(codeLine);i++)
    {
        //Check if character is valid within the symbol table
            //Check if the next character from current index is a special symbol
            int lookAheadPlus = characterInSymbolTableBS(codeLine[i+1], symbolTableOrdered);
            int lookAhead = characterInSymbolTableBS(codeLine[i+1], specialTerminalSymbolsOrdered);
            //Check if the current character from current index is a special symbol
            int specialIndex = characterInSymbolTableBS(codeLine[i], specialTerminalSymbolsOrdered);
            int current = characterInSymbolTableBS(codeLine[i], symbolTableOrdered);
            // reserved index set to -1, used only if a reserved word is detected in deeper code
            // where it would take up the index of a reserved word in resWords that connects to 
            // resWordsTokens in a 1 to 1 relationship.
            int reservedIndex = -1; 
            // If look ahead is a special character or i is on the last iteration
            // try to make a word from indexes start to i+1 exclusively (i+1 is where the 
            // special char is)
            if(current == -1)
            {
                char invalid_string[4];
                invalid_string[0] = '2';
                invalid_string[1] = ' ';
                invalid_string[2] = codeLine[i];
                invalid_string[3] = '\0';
                strcat(parsedString, invalid_string);
                strcat(parsedString, " ");
                start = start +1;
                //continue;
            }                                                             
            else if((lookAhead != -1 || lookAheadPlus == -1)|| i == strlen(codeLine)-1 || (lookAhead == -1 && (specialIndex != -1)))
            {
                // asdfas % *
               // Substring attempts to create a word that is either a reserved word or 
               // identifier. The substring will return null if the start is a special character
               
               char* word = subString(start, i+1 ,codeLine);
               //token is set as default to the identifier token value
               int token = identsym;
               // If the word exists check if the word is both valid or a reserved word.
               // If it is a resseved word, tokenize it, if it is not valid, stop the parse,
               // free the WIP parse string and entirely and return null from the entire function 
               // to indicate an error occured. Prints out to console on what type of error occured
               // on the given line, before exiting on error.
               if(word != NULL)
               {
                    //checks if word is valid, errors will return null
                    int valid  = isWordValid(word);
                    //printf("%s\n",word);
                    reservedIndex = isStatementReserved(word);
                    if(reservedIndex != -1)
                    {
                        token = resWordsTokens[reservedIndex];
                    }
                    if(valid == 2 || valid == -3)
                    {
                        token = numbersym;
                    }
                    start = i + 1;    
                    //if(lookAheadPlus == -1) start = start + 1;   
               }
               else
               {    
                    // Take the index of the special character in the codeline (that failed to create a word) and 
                    // find what special char token it corresponds to
                    specialIndex = characterInSymbolTableBS(codeLine[start], specialTerminalSymbolsOrdered);
                    token =  specialTerminalSymbolsTokens[specialIndex];
                    // if(specialIndex == -1)
                    // {
                    //     token  = -1;
                    // }
                    // Some tokens can merge with other tokens to create special cases.
                    // This checks for those and merges the tokens accordingly and shifts the 
                    // start and i by 1, as in this case we "truncate" the string by one char
                    // when completeing a merge.
                    
                    //  The switch also tests for comments to ignore when tokenizing, IT DOESN'T CHECK CONTENT 
                    //  of comments. Invalid characters could be inside the commments. Also, it doesn't assume
                    //  there to be nested comments like: /*/**/*/
                    switch(token)
                    {
                        case 0:
                            if(codeLine[start+1] == '=')
                            {
                                token = becomessym; // :=
                                start = start + 1;
                                i++;
                            } 
                            break;
                        case lessym:
                            if(codeLine[start+1] == '=')
                            {
                                token = leqsym; // <=
                                start = start + 1;
                                i++;
                            } 
                            else if(codeLine[start+1] == '>')
                            {
                                token = neqsym; // <>
                                start = start + 1;
                                i++;
                            } 
                            break;
                        case gtrsym:
                            if(codeLine[start+1] == '=') 
                            {   
                                token = geqsym; // >=
                                start = start + 1;
                                i++;
                            }
                            break;
                        case slashsym:                   // Slashsym is for /
                            if(codeLine[start+1] == '*') // checks for a /* case to indicate a comment start
                            {
                                token = -1; // From comment start onwards is to be ignored, so token is set to -1
                                int commentError = 1;
                                // Check to see if based on where the comment start is, if it would be 
                                // possible to have a */ in the remaining space of the codeLine passed in
                                if((strlen(codeLine) - (i+1))>=2)
                                {
                                    //Loop through and check to find if a corresponding */ exists
                                    for(int x = start+2; x<strlen(codeLine)-1;x++)
                                    {
                                        char twoChars[] = {codeLine[x],codeLine[x+1]}; // probably needs to change to checking only last two chars, 
                                        if(twoChars[0] == '*' && twoChars[1]== '/')    // instead of checking whole thing
                                        {
                                            commentError = 0;
                                            //  if the comment ends on the last character of the codeLine
                                            //  break out and end the parse.
                                            if(x+1 == strlen(codeLine)-1)
                                            {
                                                halt_flag = 0;
                                            }
                                            else
                                            {
                                                start = x+1;
                                                i = x+1; 
                                            }
                                            break;
                                        }
                                    }
                                }
                                if(commentError) // no end of comment was found, return an error
                                {
                                    token = -5;
                                    start = start + 1;
                                    i++;
                                    //printf("Code line: '%s' has an unresolved comment, not ended with '*/' \n", codeLine);
                                    //free(parsedString);
                                    //return NULL;
                                }
                            }
                            break;
                    }
                    if(halt_flag == 0)
                    {
                        halt_flag = 1; // reset the halt flag
                        break;
                    }
                    start = start + 1; // increment start
               }
               // Negative tokens are to be ignored, only positive are to be added on the string
               if(token > 0 || token == -5) 
               {
                    char int_string[4]; 
                    sprintf(int_string, "%d ",token);
                    strcat(parsedString, int_string);
                    if(token == identsym || token == numbersym || reservedIndex != -1)
                    {    
                            if(reservedIndex == -1)
                            {
                                strcat(parsedString, word);
                                strcat(parsedString, " ");
                            }
                            
                    }
               }

            }
    }
    return parsedString; 
}
int isStatementReserved(char* word)
{
    int retval = -1;
    for(int x = 0; x < 11; x++)
    {
        if(strcmp(word,resWords[x]) == 0)
        {
            retval = x;
            break;
        }
    }
    return retval;
}
int isWordValid(char* word)
{
    int retval = 1;
    //If it starts with an integer make sure it only has integers
    if(word[0] >= '0' && word[0] <= '9')
    {
        for(int x = 1; x < strlen(word); x++)
        {
            if(word[x] < '0' || word[x] > '9')
            {
                retval = -2;
                break;
            }
        }
        //If the integer is longer than 5 digits
        if(retval != -2)
        {
            if(strlen(word)>5)
            {
                retval = -3;
            }
            else
            {   
                retval = 2;
            }
        }
    }
    //If the word is longer than CMAX characters
    if(strlen(word) > CMAX && retval == 1)
    {
        retval = -1;
    }
    return retval;
}
char* subString(int start, int end,char* line)
{   
    if(characterInSymbolTableBS(line[start], specialTerminalSymbolsOrdered) != -1 
        && characterInSymbolTableBS(line[start], symbolTableOrdered) != -1) return NULL;
    char* word = malloc(sizeof(char)*(end-start+1));
    word[0] = '\0';
    for(int i = start; i<end;i++)
    {
        if(characterInSymbolTableBS(line[i], symbolTableOrdered) != -1)
        {
            word[strlen(word)] = line[i];
            word[strlen(word) + 1] = '\0';
        }
        if(characterInSymbolTableBS(line[i], symbolTableOrdered) == -1)
        {
            break;
        }
    }
    if(word[0] == '\0')
    {
        return NULL;
    } 
    else
    {
        return word;
    }
}
int numberOfFileLines(char* filename)
{
    FILE *fp = fopen(filename, "r");
    int numLines = 1;
    char ch = ' ';
    while ((ch = fgetc(fp)) != EOF)
    {
      if (ch == '\n')
      {
            numLines++;
      }
    }
    fclose(fp);
    return numLines;
}
int characterInSymbolTableBS(char c, char* symTbl)
{
    int low = 0;
    int high = strlen(symTbl)-1; 
    while (low <= high) 
    {
        int mid = (high + low) / 2;
        if (symTbl[mid] == c) return mid;
        else if (symTbl[mid] > c) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}
int binarySearch(int arr[], int left, int right, int x) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if x is present at mid
        if (arr[mid] == x)
            return mid;

        // If x greater, ignore left half
        if (arr[mid] < x)
            left = mid + 1;

        // If x is smaller, ignore right half
        else
            right = mid - 1;
    }

    // If we reach here, then element was not present
    return -1;
}
//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//




//Project Questions//
/*
    > When we are inserting a procedure into the symbol table, why do we leave its address to a question mark? Answer: we will not be worki

    > How will be connecting our vm and our lexeme? 

    > how exactly should we be changing scanner for the future assignments? answer: remove whatever is not in the grammar and view them as identifiers.  

    > How many lines of assmebly code will we have? 
    
    > Operations MUL 0 3   -OR-  OPP 0 3

    > will we have to create a little vm with a stack within this code to keep track of where things are.

*/

//======================Project Rules======================//
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

    {Example code}

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


    > Remove any keywords that are not part of  grammer inside of scanner

    > Scanner and Parser should be within the same program. 


    {Example Code}

    Begin
        statement;
        statment;
        statment
    end

    (equals)

    Begin
        statement;
        statment;
        
    end

    > when we print out the symbol table the mark on everything should be 1



    // ASSEMBLY CODE PART
    
    > We will have two datastructures one will be the stack that will be linear since there are no 
        procedures and therefore no jumps and we will also have the text which is the assembly code which we will print out from our the dataStructure of our choosing. 
    
    > Assembly code jumps by 3
    > Stack goes up by one
    > SL DL RV  | X Y Z TY 

      
    > Assembly code: 7 0 3 
    
    // Symbol table
    // assembly code increment for addresses is linear increment by 3 always. However, variable count 
    // can affect the M values that get plugged for lod and sto when adding it into struct
    
*/

typedef struct{
    int kind; //const = 1, var = 2, proc = 3.
    char name[11]; // name up to 11 chars.(identifer)
    int val; // number (ASCII value)
    int level; // L level -- assume 0 for the most part
    int adr; // M address
    int mark; // 0 = in use for code generation, 1 = unavailable.

} namerecord_t;

typedef struct{
    int line; //const = 1, var = 2, proc = 3.
    char name[4]; // name up to 11 chars.(identifer)
    int L; // number (ASCII value)
    int M; // L level
    // assembly_Node next = null;
} assembly_Node;

#define MAX_NAME_TABLE_SIZE 500

namerecord_t symbol_Table[MAX_NAME_TABLE_SIZE];

assembly_Node assembly_Code[MAX_NAME_TABLE_SIZE];

// 7*(6*(7/4(hasdf * ( 6))))
//==========Functions===========//

int GET_TOKEN()
{
    
}


void PROGRAM()
{
    int TOKEN;
    BLOCK();

    //Call token looking for '.' ENDING PROGRAM
    TOKEN = GET_Token();
    if (TOKEN != periodsym)
    {
        // ERROR(TOKEN);
        exit();
    }
    else
    {
        // Store Assembly  SYS END is added
        //Print Assembly code
        
        //SymbolTablePrint();
    }
}

void BLOCK()
{
    int TOKEN;
    TOKEN = GET_Token();
    if(constsym == TOKEN) 
    {
        CONST_DECLARATION();
    }
    
    if(varsym == TOKEN)
    {
        VAR_DECLARATION();
    }
    
    STATEMENT();
}
// if SYMBOLTABLECHECK (token) != -1
// We need  a SYMBOLTABLECHECK() to see if something is in the symbol table, -1 otherwise don't the else 
//
void CONST_DECLARATION() 
{
    int TOKEN;
    // checking until semicolon
    TOKEN = GET_Token();
    while(1){
        //check for identifier 
        TOKEN = GET_Token();
        if(TOKEN == identsym)
        {
            //grab identifier function that grabs and saves variable  
            char* nameIdent;
            if(SYMBOLTABLECHECK(nameIdent) != -1)
            {
                // ERROR , constant with the identifier name already exists
                exit();
            }
            //create named object for record
            //keep reference
            TOKEN = GET_Token();
            if(TOKEN == eqsym)
            {
                TOKEN = GET_Token();
                if(TOKEN == numbersym)
                {
                    //grab number
                    //store number in from reference
                    //main array name record addition 
                    // call initialize add in the value and the identifer name
                    //
                    // Sudo code sets the address to 0 always, when setting a constant why? 
                
                    TOKEN = GET_Token();
                    if(TOKEN == commasym)
                    {
                        continue;
                    }
                    else if (TOKEN == semicolonsym)
                    {
                        break;
                    }
                    else
                    {
                        // SEMICOLON MISSING, INVALID TOKEN 
                        //Error
                        exit(); 
                    }
                }
                else
                {
                    // NUMBER MISSING, INVALID TOKEN 
                    //Error
                    exit(); 
                }
            }
            else
            {
                // NO EQUALS, INVALID TOKEN 
                //Error
                exit(); 
            }
        }
        else
        {
            //NO IDENTIFIER, INVALID TOKEN 
            //Error
            exit(); 
        }
    } 
}

//const xd = 5, t = 3 
// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"].
// var-declaration  ::= [ "var" ident {"," ident} “;"].

// The sudo code has var declaration returning the number of variables
// why isn't the variable count increased when a constant is defined in his
// sudo code?
void VAR_DECLARATION() 
{
    int TOKEN;
    // checking until semicolon
    TOKEN = GET_Token();
    while(1){
        //check for identifier
        TOKEN = GET_Token();
        if(TOKEN == identsym)
        {

            // Grab identifier, function that grabs and saves variable  
            char* nameIdent;
            if(SYMBOLTABLECHECK(nameIdent) != -1)
            {
                // ERROR , variable with the identifier name already exists
                exit();
            }
            // Create named object for record
            // Store object in main name array.
            // increment VAR counter

            // The addresses of the variables added to name table MUST be
            // correct with regards to what is already there (var# +2) 
            TOKEN = GET_Token();
            if(TOKEN == commasym)
            {
                continue;
            }
            else if (TOKEN == semicolonsym)
            {
                break;
            }
            else
            {
                // SEMICOLON MISSING, INVALID TOKEN 
                //Error
                exit();
            }
        }
        else
        {
            //NO IDENTIFIER, INVALID TOKEN 
            //Error
            exit();
        }
    } 
}
// statement   ::= [ ident ":=" expression
//       | "begin" statement { ";" statement } "end" 
//       | "if" condition "then" statement 
//       | "while" condition "do" statement
//       | "read" ident 
//       | "write"  expression 
//       | empty ] .  

void STATEMENT()
{
    int TOKEN;
    TOKEN = GET_Token();
    //MASSIVE SWITCH STATEMENT BEWARE
    switch(TOKEN) 
    {
    //-----------------------------------------------------------------------------------------
        case identsym:
            // Grab identifier, function that grabs and saves variable  
            char* nameIdent;
            int symbolIndex = SYMBOLTABLECHECK(nameIdent);
            if(symbolIndex == -1)
            {
                // ERROR , identifier name does not exist
                exit();
                //The exit() function in C. The exit() function is used to terminate a process or function calling immediately in the program. 
            }
            if(symbol_Table[symbolIndex].kind != 2)
            {
                // ERROR , identifier found is not of VAR type
                exit();
            } 
            TOKEN = GET_Token();
            if(TOKEN != becomessym)
            {
                // ERROR , become operator missing, expected for the statement
                exit();
            } 
            TOKEN = GET_Token();
            EXPRESSION()
            // Store the assembly code into the array 
            // emit STO (M = table[symIdx].addr)
            break;
    //-----------------------------------------------------------------------------------------
        case beginsym:

            break;
    //-----------------------------------------------------------------------------------------
        case ifsym:

            break;
    //-----------------------------------------------------------------------------------------
        case whilesym:

            break;
    //-----------------------------------------------------------------------------------------
        case readsym:

            break;
    //-----------------------------------------------------------------------------------------
        case writesym:

            break;
    //-----------------------------------------------------------------------------------------
        case -1: 
            // EMPTY SATEMENT, assuming there is no more tokens in the line, that implies the statement is valid
            break;
    //-----------------------------------------------------------------------------------------
        default:
            //ERROR, token isn't one of the above ^^^
            break;
    //-----------------------------------------------------------------------------------------
    }
}

void EXPRESSION()
{
    
}

void CONDTION()
{

}

void TERM()
{

}

void FACTOR()
{
    
}

namerecord_t* initializeNameRecord(int _kind, char* _name, int _val, int _level, int _adr, int _mark)
{
    namerecord_t* new_record =  malloc(sizeof(namerecord_t));
    new_record->kind =_kind;
    strcpy(new_record->name, _name);
    new_record->val = _val;
    new_record->level = _level;
    new_record->adr = _adr;
    new_record->mark = _mark;
    return new_record;
}


//==========MAIN===========//
int main(int argc, char* argv[]) {
//  HW2 MAIN START //
    // Initializing variables
    char buffer[STRMAX];
    int length;
    char tyler;
    int errorFlag = 1;

// =========================================
    //Initalize input file for viewing
    int lines = numberOfFileLines(argv[1]);
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Error opening file. \n");
        return 1;
    }

// =========================================
// =========================================
    //Initalize main code array
    char* codePL = (char*) malloc(sizeof(char)* (STRMAX*lines));
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    while(fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        printf("entering while\n");
        fscanf(fp, "%c", &tyler);
        length = strlen(buffer);

        char* line = (char*) malloc(sizeof(char) * (length+ 1));
        line[0] = '\0';
        strcpy(line, buffer);

        line = lexicalParse(line); // lex parse
        printf("passed parse\n");
        if(line != NULL)
        { 
            strcat(codePL,line);
        }
        else
        {
            errorFlag = 0;
            break;
        }

        free(line);
    }   
    printf("PASS 3\n");

// =========================================
// =========================================

    // I commented out the print statements from everything in this
    // except the error cases, that way exiting on errors will be easier 
    // and the only things that print on exit.

    if (errorFlag != 0) {
        length = strlen(codePL);
        char* token = (char*) malloc(sizeof(char) * 1000);
        token[0] = '\0';
        char* word = (char*) malloc(sizeof(char) * 1000);
        word[0] = '\0';
        char specialCharacter;
        int index = 0;
        int tokenToInt;
        char stopChar = ' ';

        if (EndProgramFlag) 
        {
            for (int i = 0; i < length && EndProgramFlag; i++) {
                if (codePL[i] == ' ')
                {
                    continue;
                }

                strncat(token, codePL + i, 1);

                if (codePL[i+1] != ' ') {
                    continue;
                }

                if (strcmp(token, "20") == 0) {
                    // printf("we've enterd the token, := if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "10") == 0) {
                    // printf("we've enterd the token, <> if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "12") == 0) {
                    // printf("we've enterd the token, <= if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "14") == 0) {
                    // printf("we've enterd the token, >= if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "-5") == 0) {
                    EndProgramFlag = 0;
                    printf("%-9s%5s\n", "/*", "Unresolved In Line Comment Error");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "3") == 0) {
                    char* pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2);// 1
                    strncpy(word, codePL + i + 2, num_chars);
                    int valid  = isWordValid(word);
                    if(valid == -3)
                    {
                        EndProgramFlag = 0;
                        printf("%-9s%5s\n", word, " Invalid Number, exceeds maxDigits of 5");
                    }
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    i += num_chars + 1;
                    continue;
                }
                
                if (strcmp(token, "2") == 0) {
                    char* pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2);// 1
                    strncpy(word, codePL + i + 2, num_chars);
                    if(strlen(word) == 1 && characterInSymbolTableBS(word[0],symbolTableOrdered) == -1)
                    {
                        EndProgramFlag = 0;
                        printf("%-9s%5s\n", word, " (Invalid Symbol)");
                    }
                    else
                    {
                        int valid  = isWordValid(word);
                        if(valid == -1)
                        {
                            EndProgramFlag = 0;
                            printf("%-9s%5s\n", word, " Invalid Identifier, exceeds maxLength of 11");
                        }
                        else if (valid == -2)
                        {
                            EndProgramFlag = 0;
                            printf("%-9s%5s\n", word, " Invalid Identifier, starts with an Integer");
                        }
                    }
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    i += num_chars + 1;
                    continue;
                }

                index = -1;
                tokenToInt = atoi(token);
                for (int j = 0; j < 15; j++) {
                    if (specialTerminalSymbolsTokens[j] == tokenToInt) {
                        index = j;
                        break;
                    }
                }
                if (index != -1) {
                    specialCharacter = specialTerminalSymbolsOrdered[index];
                    memset(token, '\0', 1000);
                    continue;
                }
                index = binarySearch(resWordsTokens, 0, 11, tokenToInt);
                if(index != -1) {
                    strcat(word, resWords[index]);
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    continue;
                }
                if(strcmp(token, "13") == 0)
                {
                    memset(token, '\0', 1000);
                    continue;
                }
            }
        }

        printf("CodePL:\n \t%s\n", codePL);// DONT FORGET TO DELETE ME LATER
        
        memset(token, '\0', 1000);
        memset(word, '\0', 1000);
        free(token);
        free(word);
    }   


    //===================HW2 MAIN END==================//

    if (EndProgramFlag)
    {
         

    }
   

  
    free(codePL);   
    
    return 0;
}