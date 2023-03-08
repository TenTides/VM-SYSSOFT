// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/15/2023

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   125       // Assuming max codeline length is 50
#define MAX_NAME_TABLE_SIZE 500

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
char* GET_Token();
int Get_TokenInteger();
void CONST_DECLARATION();
void VAR_DECLARATION();
int SYMBOLTABLECHECK(char* name);

void EXPRESSION();
void STATEMENT();
void CONDITION();
void TERM();
void FACTOR();
void printSymTbl();
void printAssCodes();
char* assemblyConvertOP(int OP,int M);

//==========================================================================================================================
//==========================================================================================================================

char* global_Lexeme;
int universialIndex = 0;

int universalCodeText = 1;// This keeps track of how many opcodes we have (This should be "Line" in the terminal)
int universalCodeAddress = 3;// This keeps track of the addresses in the assembly text, we start at 3 for the 0 0 0 then we go and read the next opcode

int variableCount = 0;// This keeps track of how many Variables have been stored into the Symbol table 
int universalSymbolIndex = 1;// This keeps track of what index we must store into next. This also is where we start our search. Searching starts at universal Symbol Index and decrements until 0. 

typedef struct{
    int kind; //const = 1, var = 2, proc = 3. -- proc is not in use for now. 
    char name[12]; // name up to 11 chars not including '\0'.(identifer)
    int val; // number (ASCII value)
    int level; // L level -- assume 0 for the most part
    int adr; // M address
    int mark; // 0 = in use for code generation, 1 = unavailable.

} namerecord_t;

// This struct will be used to store the opcodes as be "emit" them to the screen for processing later. 
typedef struct{
    int OP; // name up to 11 chars.(identifer)
    int L; // number (ASCII value)
    int M; // L level
} assembly_Node;  

// Function Signitures for functions that will insert structs into the arrays below.
namerecord_t* initializeNameRecord(int _kind, char* _name, int _val, int _level, int _adr, int _mark);
assembly_Node* initializeAssemblyRecord(int OP, int L, int M);

// Arrays that will hold the structs. 
namerecord_t* symbol_Table[MAX_NAME_TABLE_SIZE];
assembly_Node* assembly_Code[MAX_NAME_TABLE_SIZE];// this is where we will be storing the the assembly code

int TOKEN = -1;
//==========================================================================================================================
//==========================================================================================================================

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
    > When we are inserting a procedure into the symbol table, why do we leave its address to a question mark? Answer: we will not be working linearly

    > How will be connecting our vm and our lexeme? 

    > how exactly should we be changing scanner for the future assignments? answer: remove whatever is not in the grammar and view them as identifiers.  

    > How many lines of assmebly code will we have? For the assembly code struct?
    
    > Operations MUL 0 3   -OR-  OPP 0 3

    > will we have to create a little vm with a stack within this code to keep track of where things are in memory. Answer: handles itself

    > on page 11 of the HW3 why does it include modsym when that was not apart of HW2?
    

    > CAN WE BE CERTAIN ADDRESSES for assembly codes like jmp and jpc are 3 incremented. Eg. after each code address increment by 3 like in the vm assignment

    > When "emmiting", are we printing the assembly code to the screen or are we just storing it for later printing. For example, in the case that there is an error should have printed the valid assembly code to the screen that preceded that error. 
    
    >if token == whilesym
        get next token
        loopIdx = current code index <----- what index is this refering to? Code PL?
        CONDITION
        if token != dosym
        error
        get next token
        jpcIdx = current code index <--
        emit JPC
        STATEMENT
        emit JMP (M = loopIdx)
        code[jpcIdx].M = current code index <--
        return

    >if token == ifsym
        get next token
        CONDITION
        jpcIdx = current code index
        emit JPC
        if token != thensym
        error
        get next token
        STATEMENT
        code[jpcIdx].M = current code index
        return
    
    > what is CURRENT CODE INDEX and what is code[], it is represented as a struct here.
    
    
    > why do we need to have jumps if we are not using procedures and why are they apart of if
    > what is meant by "emit read" and "emit write" ? IF THE ERROR STATEMENT IS ALL THAT IS PRINTED ON ERROR,
    > what do we do for all the sudo he put up?
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

    lexeme: 21 xx 18 xx 18 xx 22

    lexeme:  21 xx 18 xx 18 22

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

    
    y := 7 * (u * 3)
    y := 7 * 3u
    y := 21u

    ===========================
    {Ambiguity}

    order of operations (left most derivation)

    a + b * c 
    is should produce the same output as
    b * c + a

    but with how our program is, the first one will do (a+b)*c when it should be
    a+(b*c). 

    with in expression() and term() we must make it so that multiplication has precedence over addition. Look at pg 20 in slides and 23. it is already built in within the psuedocode. 
    ===========================
    {Recursive Descent parsing with left recursion}

    > basically if we do not have  basecases within our recursive function we could end up having a stack overflow. 
    ===========================
    {Left factoring}
    > This causes a compiler to be slower 

    {example}

    we will not want this:

    IF<C>THEN<ST>
    IF<C>THEN<ST>ELSE<ST>

    instead we will do:

    IF<C>THEN<ST><X>
    
    where <x> ::= ELSE<ST> | STOP   

    > What this does it remove the rule of backtracking making our compiler slower. 
    > we basically need to create a method that acts as the basecase. Exam question
    ===========================

    {very easy test case}

    var a, b,c; // populate the symbol table
    a := b + c.

    {output should be}

    LOD 0 4
    LOD 0 5
    ADD 0 2
    STO 0 3
    SYS 0 3

    ===========================
    We have to update our code so that it follows left factoring.


    ==================After class questions answers============
    > For this program we will print to the terminal like we saw in the HW3 file. PL/0 the code and then the assembly code. But that is not all. Along with printing each line of assembly code, we must store that code in an array/datastructure and save it to the file where for example LOD 0 3 -> 7 0 3. This is so that we can run our assembly code within the vm. 

    > For this project we do not need to create a stack nor do we need to have the the text(assembly code)
    written somewhere for us to parse. Instead, we must just keep track of where we are with in those concepts so that when we run our assembly code within the vm, the stack will be created properly. Therefore the address of variables etc. must increment by 1, and the address of the assembly code(text) must increment by 3. That is all we have to worry about in this project.

    > The psuedo code he gave us already takes care of the properly perfoming Recursive Descent parsing with left recursion to avoid infinite recurisve loops. 

    ===================================
    > Syntax Graphs 

    A way to represent the the function of grammar.


    ===================================
    

    >We can take out emmit positve and negative
    and he will send out an announcement.
*/




//==========Functions===========//
//  >use a universal index that all the below functions can edit,this will go to the end of codePL
//  >it will return the NEXT token in code PL
//  > it should be able to either ignore the variable identifier names to go to the next token, or deal with it directly.
//    Ignoring it may be better, as a seperate function could be responable for grabbing the identifer from codePL 
//    and thereafter incrementing the universal index appropriately.  (helper function could be GRAB_NAME(index))
//universialIndex

char* GET_Token()
{
    // printf("\n\nEntering the GET_Token\n");
    // initializing variables
    char* RETVAL = (char*) malloc(sizeof(char) * 12);
    // printf("This is Global Lexeme: %s\n", global_Lexeme);
    int length = strlen(global_Lexeme);                  
    int foundIt = 0;
    //initializing token and word
    char* token = (char*) malloc(sizeof(char) * 1000);
    token[0] = '\0';

    char specialCharacter;
    int index = 0;
    int tokenToInt;
    char stopChar = ' ';

    if(universialIndex == length - 1)
    {
        RETVAL = NULL;
        return RETVAL;
    }

    // printf("This is universalIndex: %d\n", universialIndex);
    for(int i = universialIndex; i < length ;i++, universialIndex++)
    {   
        // printf("We have entered the for loop\n");
        // if we see a space then we continue. 
        if (global_Lexeme[i] == ' ')
        {
            continue;
        }

        strncat(token, global_Lexeme + i, 1);
        // printf("When i is %d token is %s\n", i, token);
        // continue if we have multiple numbers/chars 
        if (global_Lexeme[i+1] != ' ') {
            continue;
        }


        if (strcmp(token, "20") == 0) {
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token); // is this psossible when retval destination is null? I will check it
            
            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "10") == 0) {
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            
            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "12") == 0) {
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "14") == 0) {
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }
        if (strcmp(token, "-5") == 0) {
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "3") == 0) {            
            //copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);

            break;
        }

        // done
        if (strcmp(token, "2") == 0) {
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }

        // printf("We are about to check the specialSymbolTokens for loop token is %s\n", token);
        index = -1;

        tokenToInt = atoi(token);
        // printf("when token is %s tokenToInt is %d\n", token, tokenToInt);

        if (strcmp(token, "0") == 0 || tokenToInt < 0 || tokenToInt > 0)
        {
            for (int j = 0; j < 15; j++) {
                if (specialTerminalSymbolsTokens[j] == tokenToInt)
                {
                    // store token in retval
                    
                    strcpy(RETVAL, token);
                    memset(token, '\0', 1000);
                    foundIt = 1;
                    break;
                }
            }

            if(foundIt)
            {
                break;
            }
        }


        // for (int j = 0; j < 15; j++) {
        //     if (specialTerminalSymbolsTokens[j] == tokenToInt)
        //     {
        //         // store token in retval
                
        //         strcpy(RETVAL, token);
        //         memset(token, '\0', 1000);
        //         break;
        //     }
        // }



        // printf("We just left the specialSymbolTokens for loop token is %s\n", token);
        // printf("We are about to check the resWordsTokens for loop token is %s\n", token);
        // printf("PASSING\n");

        
        index = binarySearch(resWordsTokens, 0, 15, tokenToInt);
        if(index != -1) {
            // store token in retval
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }
        // printf("We just left thye resWordsTokens for loop token is %s\n", token);

        // printf("doing normal strcpy when token is %s\n", token);
        strcpy(RETVAL, token);
        
        memset(token, '\0', 1000);
        break;
    }

    free(token);
    // printf("about to return\n");
    universialIndex++;

    // printf("This is RETVAL before returning %s\n", RETVAL);
    return RETVAL;
}


int Get_TokenInteger()
{
    //printf("GET_TokenInteger enter\n ");
    char* temp = GET_Token();
    //printf("Get token: %s\n", temp);
    int stringLength = strlen(temp);
    //printf("Get token len: %d\n", stringLength);

    for(int i = 0 ; i<stringLength;i++)
    {
        if(isdigit(temp[i]) == 0)
        {
            return -10;
        }
    }
    int  ret = atoi(temp); // frees for you apparently, when I had free temp after this, it gave me a double free exception
    free(temp);
    return ret;
}

void PROGRAM()
{
    namerecord_t* newCode = initializeNameRecord(0," ", 0, 0, 0, 0);
    symbol_Table[0] = newCode;

    assembly_Node* newAssCode;
    newAssCode = initializeAssemblyRecord(7, 0, 3);
    printf("%d    JMP    0    3\n\n",universalCodeText);
    assembly_Code[0] = newAssCode;
    //universalCodeText++;
    //universalCodeAddress += 3;

    //printSymTbl();
    printf("Program %d\n",TOKEN);

    BLOCK();
    printf("reentering Block with %d\n", TOKEN);
    //Call token looking for '.' ENDING PROGRAM
    
    printf("Block pre token grab %d\n", TOKEN);
    TOKEN = Get_TokenInteger();
    printf("Block post token grab %d\n", TOKEN);
    if (TOKEN != periodsym)
    {
        printf("Error: program must end with period\n");
        exit(0);
    }
    else
    {
        printf("YAYYYY WE MADE IT TO THE PERIOD EXTIO!!!!\n");
        // Store Assembly  SYS END is added
        assembly_Node* newAssCode;
        newAssCode = initializeAssemblyRecord(9, 0, 3);
        printf("%d    SYS    0    3\n",universalCodeText);
        assembly_Code[universalCodeText] = newAssCode;
        universalCodeText++;
        universalCodeAddress += 3;

        //Print Assembly code
        printAssCodes();        
        printSymTbl();
    }
}

void BLOCK()
{
    printf("Block Pre Token Grab %d\n",TOKEN);
    TOKEN = Get_TokenInteger();
    printf("Block Post Token Grab %d\n",TOKEN);

    if(constsym == TOKEN) 
    {
        printf("Block Const %d\n",TOKEN);

        CONST_DECLARATION();
    }
    if(varsym == TOKEN)
    {
        printf("Var Block Enter Area %d\n",TOKEN);
        VAR_DECLARATION();
    }
    // else
    // {
    //     //Error
    //     exit(0); 
    // }
    
    // When working with the stack we must reserve three indecies for Static Link, Dynamic Link, and Return Address. 
    // Stack pointer should be at 3 now. 
    // We must then then increment stack pointer to however many variables we have just "declared". in the case of test01.txt 
    // we have two variables x and y. Therefore we must increment the stack pointer by 2. Stack pointer should now be 5. 
    // stack should look like this from left to right:
    // SL DL RA 0 0 _
    //              ^
    //              SP
    // We do this so that when we initialize a variable using LIT 0 34 the stack pointer will add it to indicie 5. 
    // SL DL RA 0 0 34
    //              ^
    //              SP
    // Then when LOD 0 3 is called, the variable will be placed at indicies 3.
    // SL DL RA 34 0 
    //          ^
    //          SP
    assembly_Node* newCode;
    newCode = initializeAssemblyRecord(6, 0, (3+variableCount));
    printf("%d    INC    0    %d\n",universalCodeText, (3+variableCount));
    assembly_Code[universalCodeText] = newCode;
    // printf("%d ",assembly_Code[universalCodeText]->OP);
    // printf("%d ",assembly_Code[universalCodeText]->L);
    // printf("%d\n",assembly_Code[universalCodeText]->M);
    universalCodeText++;
    universalCodeAddress += 3;
    STATEMENT();
}
// if SYMBOLTABLECHECK (token) != -1
// We need  a SYMBOLTABLECHECK() to see if something is in the symbol table, -1 otherwise don't the else 
//
void CONST_DECLARATION() 
{
    while(1){
        //check for identifier 
        TOKEN = Get_TokenInteger();
        if(TOKEN == identsym)
        {
            //grab identifier function that grabs and saves variable  
            char* nameIdent = GET_Token();
            if(SYMBOLTABLECHECK(nameIdent) != -1)
            {
                // VERIFIED
                printf("Error: symbol name has already been declared\n");
                exit(0);
            }
            //create named object for record
            //keep reference
            TOKEN = Get_TokenInteger();
            if(TOKEN == eqsym)
            {
                TOKEN = Get_TokenInteger();
                if(TOKEN == numbersym)
                {
                    //grab number
                    //store number in from reference
                    //main array name record addition 
                    // call initialize add in the value and the identifer name

                    namerecord_t* newConst = initializeNameRecord(1,nameIdent,Get_TokenInteger(), 0, 0,  0);
                    // Store object in main name array.
                    symbol_Table[universalSymbolIndex] = newConst;
                    universalSymbolIndex++;

                    // Sudo code sets the address to 0 always, when setting a constant why? 
                    // "add to symbol table (kind 1, saved name, number, 0, 0)"
                
                    TOKEN = Get_TokenInteger();
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
                        // VERIFIED
                        printf("Error: constant and variable declarations must be followed by a semicolon\n");
                        exit(0); 
                    }
                }
                else
                {
                    // NUMBER MISSING, INVALID TOKEN 
                    // VERIFIED
                    printf("Error: constants must be assigned an integer value\n");
                    //Error
                    exit(0); 
                }
            }
            else
            {
                // VERIFIED
                printf("Error: constants must be assigned with =\n");
                //Error
                exit(0); 
            }
        }
        else
        {
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            //Error
            exit(0); 
        }
    } 
}

//const xd = 5, t = 3 k
// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"].
// var-declaration  ::= [ "var" ident {"," ident} “;"].

// The sudo code has var declaration returning the number of variables
// why isn't the variable count increased when a constant is defined in his
// sudo code?
void VAR_DECLARATION() 
{
    printf("Var enter Area %d\n",TOKEN);

    // checking until semicolon
    //TOKEN = Get_TokenInteger();
    while(1){
        //check for identifier
        //printf("var Before call %d\n",TOKEN);
        TOKEN = Get_TokenInteger();
        //printf(" Var 1 token %d\n",TOKEN);
        if(TOKEN == identsym)
        {
            // Grab identifier, function that grabs and saves variable  
            char* nameIdent = GET_Token();
            printf("Var Identifier Name %s\n",nameIdent);
            if(SYMBOLTABLECHECK(nameIdent) != -1)
            {
                // VERIFIED
                printf("Error: symbol name has already been declared\n");
                exit(0);
            }
            // Create named object for record
            //printf("Block Var name Pass \n");

            namerecord_t* newVar = initializeNameRecord(2 ,nameIdent,0, 0, variableCount + 2,  0);
            // Store object in main name array.
            symbol_Table[universalSymbolIndex] = newVar;
            universalSymbolIndex++;
            variableCount++;
            free(nameIdent);// must free becuase we used Get_Token(). 
            // increment VAR counter

            // The addresses of the variables added to name table MUST be
            // correct with regards to what is already there (var# +2) 
            TOKEN = Get_TokenInteger();
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
                // VERIFIED
                printf("Error: constant and variable declarations must be followed by a semicolon\n");
                //Error
                exit(0);
            }
        }
        else
        {
            //NO IDENTIFIER, INVALID TOKEN 
            // VERIFIED
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            //Error
            exit(0);
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
    printf("Statement enter Area %d\n",TOKEN);

    if(TOKEN != 2 && TOKEN != endsym) // added endsym to ensure we do not get another token before going back to statement(beginsym).
    {
        TOKEN = Get_TokenInteger();
        printf("Statement post Area Grab %d\n",TOKEN);
    }
    
   

    //MASSIVE SWITCH STATEMENT BEWARE
    int symbolIndex;
    char* nameIdent;
    int jpcIdx;
    int M; 
    assembly_Node* newCode;
    switch(TOKEN) 
    {
    //-----------------------------------------------------------------------------------------
        case identsym:
            // Grab identifier, function that grabs and saves variable  
            //printf("Statement enter Ident %d\n",TOKEN);
            nameIdent = GET_Token();
            //printf("Statement name Ident %s\n",nameIdent);
            symbolIndex = SYMBOLTABLECHECK(nameIdent); // check that the identifer exists
            //printSymTbl();
            //printf("post symbolTable Check %d\n",symbolIndex);
            if(symbolIndex == -1)
            {
                //VERIFIED
                printf("Error: undeclared identifier\n");
                exit(0);
                //The exit() function in C. The exit() function is used to terminate a process or function calling immediately in the program. 
            }
            if(symbol_Table[symbolIndex]->kind != 2)
            {
                // ERROR , identifier found is not of VAR type
                exit(0);
            } 
            TOKEN = Get_TokenInteger();
            printf("Statement Identifier Area  %d\n",TOKEN);
            printf("Statement Identifier Area Name  %s\n",nameIdent);

            if(TOKEN != becomessym)
            {
                // ERROR , become operator missing, expected for the statement
                // VERIFIED
                printf("Error: assignment statements must use :=\n");
                exit(0); 
            } 
            TOKEN = Get_TokenInteger();

            printf("Statement Identifier Area Pre Expression %d\n",TOKEN);
            EXPRESSION();
            printf("Statement Identifier Area Post Expression %d\n",TOKEN);

            M = symbol_Table[symbolIndex]->adr;
            newCode = initializeAssemblyRecord(4, 0, M);
            printf("%d    STO    0    %d\n",universalCodeText, M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            // emit STO (M = table[symIdx].addr)
            break;
    //-----------------------------------------------------------------------------------------
        case beginsym:
            while(1)
            {
                if (TOKEN == beginsym)
                    printf("Statement enter in Begin %d\n",TOKEN);
                else 
                    printf("Statement re-entering loop in Begin %d\n",TOKEN);

                TOKEN = Get_TokenInteger();
                STATEMENT();
                printf("Statement Post in Begin  %d\n",TOKEN);

                if(TOKEN != semicolonsym)
                {
                    break;
                }
            }
            if(TOKEN != endsym)
            {
                printf("Error: begin must be followed by end\n");
                //printf("False Exit\n");
                exit(0);
            }
            // !!! since we exito before going back to Block, our program does not yet check for whether there is a period at the end. 
            printf("exito without going back to block to check for periodsym :) \n");
            // exit(0);
            //TOKEN = Get_TokenInteger();
            break;
    //-----------------------------------------------------------------------------------------
        case ifsym://FIXXXXXX MEEE!!!!!! look at pg 22 of newest slides 000
            TOKEN = Get_TokenInteger();
            CONDITION();
            // emit jpc, add to assembly code struct array

            jpcIdx = universalCodeAddress;
            newCode = initializeAssemblyRecord(8, 0, 0);
            printf("%d    JPC    0    0\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
        
            if(TOKEN != thensym)
            {
                printf("Error: if must be followed by then\n");
                exit(0);
            }
            STATEMENT();
            assembly_Code[jpcIdx]->M = universalCodeText;
            //code[jpcIdx].M = current code index
            break;
    //-----------------------------------------------------------------------------------------
        case whilesym://FIXXXXXX MEEE!!!!!! look at pg 24 of newest slides
            jpcIdx =  universalCodeAddress;
            TOKEN = Get_TokenInteger();
            CONDITION();
            int loopIdx =  universalCodeAddress;
            if(TOKEN != dosym)
            {
                printf("Error: while must be followed by do\n");
                exit(0);
            }
            TOKEN = Get_TokenInteger();

            newCode = initializeAssemblyRecord(8, 0, 0);
            printf("%d    JPC    0    0\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            STATEMENT();

            newCode = initializeAssemblyRecord(7, 0, 0);
            printf("%d    JMP    0    %d\n",universalCodeText,jpcIdx);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            //JPC REASSIGNMENT
            assembly_Code[loopIdx]->M = universalCodeText;

            
            // code[jpcIdx].M = current code index 
            break;
    //-----------------------------------------------------------------------------------------
        case readsym:
              // Grab identifier, function that grabs and saves variable  
            nameIdent;
            symbolIndex = SYMBOLTABLECHECK(nameIdent);
            if(symbolIndex == -1)
            {
                printf("Error: undeclared identifier\n");
                exit(0);
                //The exit() function in C. The exit() function is used to terminate a process or function calling immediately in the program. 
            }
            if(symbol_Table[symbolIndex]->kind != 2)
            {
                printf("Error: only variable values may be altered\n");
                exit(0);
            } 
            TOKEN = Get_TokenInteger();
            // emit READ ???? guessing sys read code
            newCode = initializeAssemblyRecord(9, 0, 2);
            printf("%d    SYS    0    2\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            
            // emit  STO (M = table[symbolIndex].addr), add to assembly code struct array
            M = symbol_Table[symbolIndex]->adr;
            newCode = initializeAssemblyRecord(4, 0, M);
            printf("%d    STO    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            // emit 
            break;
    //-----------------------------------------------------------------------------------------
        case writesym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit WRITE
            newCode = initializeAssemblyRecord(9, 0, 1);
            printf("%d    SYS    0    1\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
            break;
    //-----------------------------------------------------------------------------------------
        case endsym:
            // if we see the endsym, the statement(beginsym) will check wether or not it exists.
            // we were hitting the default before :(. 
            break;
    //-----------------------------------------------------------------------------------------
        default:
            printf("Error: condition must contain comparison operator\n");
            exit(0);
            break;
    //-----------------------------------------------------------------------------------------
    }
}

void EXPRESSION()
{
    assembly_Node* newCode;
    printf("Entering Term with %d\n",TOKEN);
    TERM();   
    printf("Leaving Term and entering Expression with %d\n",TOKEN);
 
    if (TOKEN == minussym)
    {
        printf("minus if statement\n");
        //-------------------------------------------------------
        // newCode = initializeAssemblyRecord(11, 0, 0);
        // printf("%d    NEGATIVE    0    0\n",universalCodeText);
        // assembly_Code[universalCodeText] = newCode;
        // universalCodeText++;
        // universalCodeAddress += 3;
        //-------------------------------------------------------
        while (TOKEN == plussym || TOKEN == minussym)
        {
            if (TOKEN == plussym) 
            {
                TOKEN = Get_TokenInteger();
                TERM();
                //emit ADD
                newCode = initializeAssemblyRecord(2, 0, 1);
                printf("%d    ADD    0    1\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
            }
            else 
            {
                printf("entering else statement\n");
                TOKEN = Get_TokenInteger();
                TERM();
                //emit SUB
                newCode = initializeAssemblyRecord(2, 0, 2);
                printf("%d    SUB    0    2\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
            }

        }
        
    }
    else if (TOKEN == plussym)
    {  
        printf("Plus area Expression %d\n",TOKEN);
        //emit POSITIVE  ??
        //-------------------------------------------------------
        // newCode = initializeAssemblyRecord(12, 0, 0);
        // printf("%d    POSITVE    0    0\n",universalCodeText);
        // assembly_Code[universalCodeText] = newCode;
        // universalCodeText++;
        // universalCodeAddress += 3;
        //-------------------------------------------------------
        while (TOKEN == plussym || TOKEN == minussym)
        {
            if (TOKEN == plussym) 
            {
                TOKEN = Get_TokenInteger();
                printf("Expecting 3 %d and entering Term: \n",TOKEN);
                TERM();
                printf("leaving Term and reentering Expression with: %d\n", TOKEN);
                //emit add
                //-------------------------------------------------------
                newCode = initializeAssemblyRecord(2, 0, 1);
                printf("%d    ADD    0    1\n",universalCodeText);// why is it 1?
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                //-------------------------------------------------------
            }
            else 
            {
                TOKEN = Get_TokenInteger();
                TERM();
                //emit SUB
                //-------------------------------------------------------
                newCode = initializeAssemblyRecord(2, 0, 2);
                printf("%d    SUB    0    2\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                //-------------------------------------------------------
            }
    

        }

        
    }
    
}

void CONDITION()
{
    TOKEN = Get_TokenInteger();
    assembly_Node* newCode;
    if(TOKEN == oddsym)
    {
        TOKEN = Get_TokenInteger();
        EXPRESSION();
        // emit ODD, 
        //-------------------------------------------------------
        newCode = initializeAssemblyRecord(2, 0, 11);
        printf("%d    ODD    0    11\n",universalCodeText);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        universalCodeAddress += 3;
        //-------------------------------------------------------

    }
    else
    {
        // no get token here?
        EXPRESSION();
        switch(TOKEN) 
        {
        //-----------------------------------------------------------------------------------------
            case eqsym :
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit EQL
                newCode = initializeAssemblyRecord(2, 0, 5);
                printf("%d    EQL    0    5\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            case neqsym:
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit NEQ
                newCode = initializeAssemblyRecord(2, 0, 6);
                printf("%d    NEQ    0    6\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            case lessym:
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit LSS
                newCode = initializeAssemblyRecord(2, 0, 7);
                printf("%d    LSS    0    7\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            case leqsym:
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit LEQ
                newCode = initializeAssemblyRecord(2, 0, 8);
                printf("%d    LEQ    0    8\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            case gtrsym:
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit GTR
                newCode = initializeAssemblyRecord(2, 0, 9);
                printf("%d    GTR    0    9\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            case geqsym:
                TOKEN = Get_TokenInteger();
                EXPRESSION();
                // emit GEQ
                newCode = initializeAssemblyRecord(2, 0, 10);
                printf("%d    GEQ    0    10\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                universalCodeAddress += 3;
                break;
            default:
                printf("Error: condition must contain comparison operator\n");
                exit(0);
                break;
        //-----------------------------------------------------------------------------------------
        }
    }
}

int SYMBOLTABLECHECK(char* name)
{
    // printf("SYM TBL %s\n",name);
    
    // // printf("pre strcpy\n");
    strcpy(symbol_Table[0]->name,name);
    // // printf("post strcpy\n");
    
    // printf("SYM TBL %s\n",symbol_Table[0]->name);

    // // printf("pre loop \n");
    for(int i = universalSymbolIndex - 1; i > 0; i--)
    {
        if(strcmp(symbol_Table[i]->name, symbol_Table[0]->name) == 0 && i != 0)
        {
            return i;
        }
    }

    //printf("post strcpy\n");
    return -1;
}

void TERM()
{   
    printf("Entering Factor with: %d\n",TOKEN);
    FACTOR();
    printf("Leaving Factor reentering Term with:%d\n", TOKEN);
    //printf("SemiColon Term Prior? %d\n",TOKEN);
    //TOKEN = Get_TokenInteger();
    printf("SemiColon Term Or 4 which is +? %d\n",TOKEN);
    while (TOKEN == multsym || TOKEN == slashsym)
    {
        if (TOKEN == multsym)
        {
            TOKEN = Get_TokenInteger();
            FACTOR();
            //emit MUL
            assembly_Node* newCode = initializeAssemblyRecord(2, 0, 3);
            printf("%d    MUL    0    3\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
        }
        else if (TOKEN == slashsym)
        {
            TOKEN = Get_TokenInteger();
            FACTOR();
            //emit DIV
            assembly_Node* newCode = initializeAssemblyRecord(2, 0, 4);
            printf("%d    DIV    0    4\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
        }
    }

}

void FACTOR()
{       
    if (TOKEN == identsym)
    {
        printf("identsym Pre token grab %d\n",TOKEN);
        char* nameIdent = GET_Token();
        printf("identsym Post Name token grab: %s\n",nameIdent);

        int symIdx = SYMBOLTABLECHECK(nameIdent);
        //printSymTbl();
        if (symIdx == -1)
        {
            // ERROR identifier does not exist withiin the symbolTable
            // VERIFIED
            printf("Error: Identifier has not been declared\n");
            exit(0);
        }
        else if (symbol_Table[symIdx]->kind == 1) // 1 = const
        {
            // emit LIT (M = symbol_Table[symIdx].Value)
            int M = symbol_Table[symIdx]->val;
            
            assembly_Node* newCode = initializeAssemblyRecord(1, 0, symbol_Table[symIdx]->val);
            printf("%d    LIT    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;

            
        }
        else // 2 = var
        {
            // emit LIT (M = symbol_Table[symIdx].Value)
            int M = symbol_Table[symIdx]->adr;
            assembly_Node* newCode = initializeAssemblyRecord(3, 0, symbol_Table[symIdx]->adr);
            printf("%d    LOD    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            universalCodeAddress += 3;
        }
        TOKEN = Get_TokenInteger();
    } 
    else if (TOKEN == numbersym)
    {
        printf("Num Sys Pre token grab %d\n",TOKEN);
        int M = Get_TokenInteger();
        printf("Num Sys Post token grab %d\n",M);

        assembly_Node* newCode = initializeAssemblyRecord(1, 0, M); // changed this from 3 to 1
        printf("%d    LIT    0    %d\n",universalCodeText,M);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        universalCodeAddress += 3;

        TOKEN = Get_TokenInteger();
    }
    else if (TOKEN == lparentsym)
    {
        TOKEN = Get_TokenInteger();
        EXPRESSION();
        if (TOKEN != rparentsym)
        {
            printf("Error: right parenthesis must follow left parenthesis\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger();
    }
    else
    {
        printf("Error: arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
        exit(0);
    }
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

void printSymTbl()
{
    printf("\nIndex | Kind | Name | Value | Level | Address | Mark \n");
    for (int i = 0; i < universalSymbolIndex; i++)
    {
        printf("  %d      %d     '%s'      %d       %d      %d         %d\n",i, symbol_Table[i]->kind, symbol_Table[i]->name, symbol_Table[i]->val,symbol_Table[i]->level, symbol_Table[i]->adr, 1);
    }
    printf("\n");
}


void printAssCodes()
{
    printf("\nLine    OP     L    M\n");
    for (int i = 0; i < universalCodeText; i++)
    {
        char* temp = assemblyConvertOP(assembly_Code[i]->OP,assembly_Code[i]->M);
        printf("%d       %s    %d    %d\n",i, temp, assembly_Code[i]->L, assembly_Code[i]->M);
        free(temp);
    }
    printf("\n");
}


char* assemblyConvertOP(int OP,int M)
{
    char* result = malloc(sizeof(char)*4);
    result[0] = '\0';
    switch(OP) {
        case 1:
            strcpy(result, "LIT");
          break;
        case 2: 
          switch(M) {
            case 0:
              strcpy(result, "RTN");
              break;
            //ADD addition
            case 1:
              strcpy(result, "ADD");
              break;
            //SUB subtraction
            case 2:
              strcpy(result, "SUB");
              break;
            //MUL multiplication
            case 3:
              strcpy(result, "MUL");
              break;
            //DIV division
            case 4:
              strcpy(result, "DIV");
              break;
            //EQL equality 
            case 5:
              strcpy(result, "EQL");           
              break;
            //NEQ not equal
            case 6:
              strcpy(result, "NEQ");
              break;
            //LSS less than
            case 7:
              strcpy(result, "LSS");
              break;
            //LEQ less than or equal to
            case 8:
              strcpy(result, "LEQ");
              break;
            //GTR greater than 
            case 9:
              strcpy(result, "GTR");
              break;
            //GEQ greater than or equal to
            case 10:
              strcpy(result, "GEQ");
              break;
            case 11:
              strcpy(result, "ODD");
              break;
          }
          break;
        case 3:
          strcpy(result, "LOD");
          break;
        case 4:
          strcpy(result, "STO");
          break;
        case 5:
          strcpy(result, "CAL");
          break;
        case 6:
          strcpy(result, "INC");
          break;
        case 7:
          strcpy(result, "JMP");
          break;
        case 8:
          strcpy(result, "JPC");
          break;
        case 9:
          if(M == 1)
          {
            strcpy(result, "SOU");
            break;
          }
          else if(M == 2)
          {
            strcpy(result, "SIN");
            break;
          }
          if(M == 3)
          {
            strcpy(result, "EOP");
          }
      }
    return result;
} 
assembly_Node* initializeAssemblyRecord(int OP, int L, int M)
{
    assembly_Node* new_record =  malloc(sizeof(assembly_Node));
    new_record->OP =OP;
    new_record->L = L;
    new_record->M = M;
    return new_record;
}

void outputAssemblyToFile()
{
    printf("\nPRINTING TO \"output.txt\" ...\n");
    FILE*  file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Failed to pen File!\n");
        return;
    }


    for (int i = 0; i < universalCodeText; i++)
    {
        fprintf(file, "%d %d %d\n",assembly_Code[i]->OP, assembly_Code[i]->L, assembly_Code[i]->M);
    }

    fclose(file);
    printf("SUCCESS!\n");
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
        fscanf(fp, "%c", &tyler);
        length = strlen(buffer);

        char* line = (char*) malloc(sizeof(char) * (length+ 1));
        line[0] = '\0';
        strcpy(line, buffer);

        line = lexicalParse(line); // lex parse
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

                if (strcmp(token, "0") == 0) {
                    EndProgramFlag = 0;
                    printf("%-9s%5s\n", ":", " Error: symbol invalid when not followed by =");
                    memset(token, '\0', 1000);
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
                    printf("%-9s%5s\n", "/*", "Error: Unresolved In Line Comment Error");
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
                        printf("%-9s%5s\n", word, "Error: Invalid Number, exceeds max digits of 5");
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
                        printf("%-9s%5s\n", word, "Error:  Invalid Symbol");
                    }
                    else
                    {
                        int valid  = isWordValid(word);
                        if(valid == -1)
                        {
                            EndProgramFlag = 0;
                            printf("%-9s%5s\n", word, "Error: Invalid Identifier, exceeds max length of 11");
                        }
                        else if (valid == -2)
                        {
                            EndProgramFlag = 0;
                            printf("%-9s%5s\n", word, "Error: Invalid Identifier, starts with an Integer");
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
        length = strlen(codePL);
        global_Lexeme = (char*) malloc(sizeof(char)* (length));
        strcpy(global_Lexeme,codePL);


        
        memset(token, '\0', 1000);
        memset(word, '\0', 1000);
        free(token);
        free(word);
    } 
    
    // printf("\nToken Lex: %s\n",global_Lexeme);
    // printf("Tokens From Get\n");
    // char* temp = GET_Token();
    // while (temp != NULL)
    // {
    //     printf("Token: %s\n", temp);
    //     temp = GET_Token();
    // }
    PROGRAM();
    outputAssemblyToFile();

    //===================HW2 MAIN END==================//
    free(global_Lexeme);
    free(codePL);   
    return 0;
}