// Team Leader: Tyler Crawford
// Member: Nelson Herrera Gamboa
// Class: COP3402
// Date of Last Edit: 2/15/2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define CMAX 11    // maximum number of chars for idents
#define STRMAX 125 // Assuming max codeline length is 50
#define MAX_NAME_TABLE_SIZE 500

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//

char *lexicalParse(char *codeLine);
int numberOfFileLines(char *filename);
int characterInSymbolTableBS(char c, char *symTbl);
int isStatementReserved(char *word);
char *subString(int start, int end, char *line);
int isWordValid(char *word);
void PROGRAM();
int BLOCK();
void printSourceCode(char *filename);

char *GET_Token();
int Get_TokenInteger();
void CONST_DECLARATION();
void PROC_DECLARATION();
void VAR_DECLARATION(int *dx);
// void VAR_DECLARATION();

int SYMBOLTABLECHECK(char *name);
void SYMBOLTABLEDELETELEVEL(int level);
int SYMBOLTABLECHECKLEVEL(char *name);

// int SYMBOLTABLECHECK(char* name, int level);

void EXPRESSION();
void STATEMENT();
void CONDITION();
void TERM();
void FACTOR();
void printSymTbl();
void printAssCodes();
char *assemblyConvertOP(int OP, int M);
void freeAll();

// All this for recursive functions
void addNewFunctionCallToFunctionTable(char *name, int jpxIdx);
void ulitmateUpdateAllAddresses();

int base(int BP, int L);
void mainVM();

//==========================================================================================================================HW1 NO TOUCH
int pas[MAX_NAME_TABLE_SIZE];
// arBarZeroOne array is used to mark where
// the activation bars on print go
int arBarZeroOne[MAX_NAME_TABLE_SIZE];
// instruction register
int IR[3]; // IR  = 0 0 0

// pointer and halt initalization
int BP = 499;
int SP = 500;
int PC = 0;
int OP = 0;
int M = 0;
int L = 0;
int halt = 1;

void mainVM()
{
    // Reads in input file from cmd line

    FILE *fp;
    fp = fopen("./elf.txt", "r");
    // Initial values: 0 499 500
    while (1)
    {
        fscanf(fp, "%d %d %d", &OP, &L, &M);

        pas[PC] = OP;
        pas[PC + 1] = L;
        pas[PC + 2] = M;
        PC = PC + 3;
        // upon finding the halt code, scanning stops
        if (OP == 9 && M == 3)
            break;
    }
    fclose(fp);

    printf("\n");
    PC = 0;
    printf("                 PC   BP   SP   stack\n");
    printf("inital values:    %d   %d  %d\n", PC, BP, SP);
    while (halt)
    {
        // Fetch Cycle
        IR[0] = pas[PC];     // OP
        IR[1] = pas[PC + 1]; // L
        IR[2] = pas[PC + 2]; // M
        PC = PC + 3;
        // use nested swtich statements for instuctions
        switch (IR[0])
        {
        // LIT, Pushes a constant value (literal) M onto the stack
        case 1:
            SP = SP - 1;
            pas[SP] = IR[2];
            printf("    LIT 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
            // for loop to print stack with check for activation bars
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        case 2:
            // Operation to be performed on the data at the top of the stack.(or return from function)
            switch (IR[2])
            {
            // RTN Returns from a subroutine is encoded 0 0 0 and restores the caller’s AR
            case 0:
                SP = BP + 1;
                BP = pas[SP - 2];
                PC = pas[SP - 3];
                printf("    RTN 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // ADD addition
            case 1:
                pas[SP + 1] = pas[SP + 1] + pas[SP];
                SP = SP + 1;
                printf("    ADD 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // SUB subtraction
            case 2:
                pas[SP + 1] = pas[SP + 1] - pas[SP];
                SP = SP + 1;
                printf("    SUB 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // MUL multiplication
            case 3:
                pas[SP + 1] = pas[SP + 1] * pas[SP];
                SP = SP + 1;
                printf("    MUL 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // DIV division
            case 4:
                pas[SP + 1] = pas[SP + 1] / pas[SP];
                SP = SP + 1;
                printf("    DIV 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // EQL equality
            case 5:
                pas[SP + 1] = (pas[SP + 1] == pas[SP]);
                SP = SP + 1;
                printf("    EQL 0%5d%5d%6d%5d", IR[2], PC, BP, SP);

                break;
            // NEQ not equal
            case 6:
                pas[SP + 1] = (pas[SP + 1] != pas[SP]);
                SP = SP + 1;
                printf("    NEQ 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // LSS less than
            case 7:
                pas[SP + 1] = (pas[SP + 1] < pas[SP]);
                SP = SP + 1;
                printf("    LSS 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // LEQ less than or equal to
            case 8:
                pas[SP + 1] = (pas[SP + 1] <= pas[SP]);
                SP = SP + 1;
                printf("    LEQ 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // GTR greater than
            case 9:
                pas[SP + 1] = (pas[SP + 1] > pas[SP]);
                SP = SP + 1;
                printf("    GTR 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            // GEQ greater than or equal to
            case 10:
                pas[SP + 1] = (pas[SP + 1] >= pas[SP]);
                SP = SP + 1;
                printf("    GEQ 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                break;
            }
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // LOD load value to top of the stack from location at offset M from L
        // lexicographical levels down of the stack
        case 3:
            SP = SP - 1;
            pas[SP] = pas[base(BP, IR[1]) - IR[2]];
            printf("    LOD%2d%5d%5d%6d%5d", IR[1], IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // Store value at top of stack in the stack location at offset M
        // from L lexicographical levels down
        case 4:
            pas[base(BP, IR[1]) - IR[2]] = pas[SP];
            SP = SP + 1;
            printf("    STO%2d%5d%5d%6d%5d", IR[1], IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // CAL Call the procedure at code index p,
        // generating a new activation record and
        // setting PC to M
        case 5:
            pas[SP - 1] = base(BP, IR[1]);
            pas[SP - 2] = BP;
            pas[SP - 3] = PC;
            BP = SP - 1;
            PC = IR[2];
            arBarZeroOne[BP + 1] = 1;
            printf("    CAL%2d%5d%5d%6d%5d", IR[1], IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // INC Allocate  M  memory  words  (increment  SP  by  M).  First  four
        // are  reserved  to      Static  Link  (SL),  Dynamic  Link  (DL),
        // and Return Address (RA)
        case 6:
            SP = SP - IR[2];
            printf("    INC 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }

                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // JMP Jump to instruction M (PC <- M)
        case 7:
            PC = IR[2];
            printf("    JMP 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // JPC jump conditionally,  Jump to instruction M
        // if top stack element is 0
        case 8:
            if (pas[SP] == 0)
            {
                PC = IR[2];
            }
            SP = SP + 1;
            printf("    JPC 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
            for (int i = 499; i > SP - 1; i--)
            {
                if (pas[i] < 10)
                {
                    printf("%3d", pas[i]);
                }
                else if (pas[i] < 100)
                {
                    printf("%4d", pas[i]);
                }
                else
                {
                    printf("%5d", pas[i]);
                }
                if (arBarZeroOne[i])
                {
                    printf(" | ");
                }
            }
            printf("\n");
            break;
        // GENERAL SYSTEM [SYS]
        case 9:
            // Output of the value in stack[SP] to standard
            // output as a character and pop:
            if (IR[2] == 1)
            {
                printf("Output result is:  %d\n", pas[SP]);
                SP = SP + 1;
                printf("    SOU 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                for (int i = 499; i > SP - 1; i--)
                {
                    if (pas[i] < 10)
                    {
                        printf("%3d", pas[i]);
                    }
                    else if (pas[i] < 100)
                    {
                        printf("%4d", pas[i]);
                    }
                    else
                    {
                        printf("%5d", pas[i]);
                    }
                    if (arBarZeroOne[i])
                    {
                        printf(" | ");
                    }
                }
                printf("\n");
                break;
            }
            // Read in input from the user and store it on top of the stack
            else if (IR[2] == 2)
            {
                SP = SP - 1;
                printf("Please Enter an integer:  ");
                scanf("%d", &pas[SP]);
                printf("    SIN 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                for (int i = 499; i > SP - 1; i--)
                {
                    if (pas[i] < 10)
                    {
                        printf("%3d", pas[i]);
                    }
                    else if (pas[i] < 100)
                    {
                        printf("%4d", pas[i]);
                    }
                    else
                    {
                        printf("%5d", pas[i]);
                    }
                    if (arBarZeroOne[i])
                    {
                        printf(" | ");
                    }
                }
                printf("\n");
                break;
            }
            // End of program (Set “eop” flag to zero)
            if (IR[2] == 3)
            {
                halt = 0; //"eop" flag ?? how do this, exit() is not allowed
                printf("    EOP 0%5d%5d%6d%5d", IR[2], PC, BP, SP);
                for (int i = 499; i > SP - 1; i--)
                {
                    if (pas[i] < 10)
                    {
                        printf("%3d", pas[i]);
                    }
                    else if (pas[i] < 100)
                    {
                        printf("%4d", pas[i]);
                    }
                    else
                    {
                        printf("%5d", pas[i]);
                    }
                    if (arBarZeroOne[i])
                    {
                        printf(" | ");
                    }
                }
                printf("\n");
            }
        }
    }
}

int base(int BP, int L)
{
    int arb = BP; // arb = activation record base
    while (L > 0) // find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}

//==========================================================================================================================DO NOT TOUCH THE ABOVE

char *global_Lexeme;
int universialIndex = 0;
int tokencount = 0;

int universalCodeText = 0; // This keeps track of how many opcodes we have (This should be "Line" in the terminal)

int variableCount = 0;        // This keeps track of how many Variables have been stored into the Symbol table
int universalSymbolIndex = 2; // This keeps track of what index we must store into next. This also is where we start our search. Searching starts at universal Symbol Index and decrements until 0.
int universalLevel = 0;
int universalRecursiveTableIdx = 1;

typedef struct
{
    int kind;      // const = 1, var = 2, proc = 3. -- proc is not in use for now.
    char name[12]; // name up to 11 chars not including '\0'.(identifer)
    int val;       // number (ASCII value)
    int level;     // L level -- assume 0 for the most part
    int adr;       // M address
    int mark;      // 0 = in use for code generation, 1 = unavailable.

} namerecord_t;

// This struct will be used to store the opcodes as be "emit" them to the screen for processing later.
typedef struct
{
    int OP; // name up to 11 chars.(identifer)
    int L;  // number (ASCII value)
    int M;  // L level
} assembly_Node;

typedef struct
{
    char name[12];
    int nameInstancesJmpIdx[MAX_NAME_TABLE_SIZE];
    int adr;
} recursiveNameRecord;

// Function Signitures for functions that will insert structs into the arrays below.
namerecord_t *initializeNameRecord(int _kind, char *_name, int _val, int _level, int _adr, int _mark);
assembly_Node *initializeAssemblyRecord(int OP, int L, int M);
recursiveNameRecord *initializeRecursiveNameRecord(char *_name, int _adr);

// Arrays that will hold the structs.
namerecord_t *symbol_Table[MAX_NAME_TABLE_SIZE];
assembly_Node *assembly_Code[MAX_NAME_TABLE_SIZE]; // this is where we will be storing the the assembly code
recursiveNameRecord *functionCall_Table[MAX_NAME_TABLE_SIZE];

int TOKEN = -1;
//==========================================================================================================================
//==========================================================================================================================

typedef enum
{
    identsym = 2,
    numbersym = 3,
    plussym = 4,
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
    callsym = 27,
    constsym = 28,
    varsym = 29,
    procsym = 30,
    writesym = 31,
    readsym = 32,
} token_type;

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//

char *resWords[] = {"odd", "begin", "end", "if", "then", "while", "do", "call", "const", "var", "procedure", "write", "read"};

int resWordsTokens[] = {oddsym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym};
// Symbol table is in order of ascii value, can be used with binary search
char symbolTableOrdered[] = {'\t', '\r', ' ', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
                             '=', '>', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                             'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                             'v', 'w', 'x', 'y', 'z'};

// Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
char specialTerminalSymbolsOrdered[] = {'\t', '\r', ' ', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '~'};                                                  // ' ' isn't a term sym, it was put here
int specialTerminalSymbolsTokens[] = {-3, -2, -1, lparentsym, rparentsym, multsym, plussym, commasym, minussym, periodsym, slashsym, 0, semicolonsym, lessym, eqsym, gtrsym, 0}; // -1 is for spaces and 0 is for colons and -2 for tabs,
int halt_flag = 1;
int EndProgramFlag = 1;

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//
char *lexicalParse(char *codeLine)
{
    // Copy space of parent string
    char *parsedString = malloc(sizeof(codeLine));
    parsedString[0] = '\0';
    // Start is set to zero as no special characters have been detected yet
    int start = 0;
    for (int i = 0; i < strlen(codeLine); i++)
    {
        // Check if character is valid within the symbol table
        // Check if the next character from current index is a special symbol
        int lookAheadPlus = characterInSymbolTableBS(codeLine[i + 1], symbolTableOrdered);
        int lookAhead = characterInSymbolTableBS(codeLine[i + 1], specialTerminalSymbolsOrdered);
        // Check if the current character from current index is a special symbol
        int specialIndex = characterInSymbolTableBS(codeLine[i], specialTerminalSymbolsOrdered);
        int current = characterInSymbolTableBS(codeLine[i], symbolTableOrdered);
        // reserved index set to -1, used only if a reserved word is detected in deeper code
        // where it would take up the index of a reserved word in resWords that connects to
        // resWordsTokens in a 1 to 1 relationship.
        int reservedIndex = -1;
        // If look ahead is a special character or i is on the last iteration
        // try to make a word from indexes start to i+1 exclusively (i+1 is where the
        // special char is)
        if (current == -1)
        {
            char invalid_string[4];
            invalid_string[0] = '2';
            invalid_string[1] = ' ';
            invalid_string[2] = codeLine[i];
            invalid_string[3] = '\0';
            strcat(parsedString, invalid_string);
            strcat(parsedString, " ");
            start = start + 1;
            // continue;
        }
        else if ((lookAhead != -1 || lookAheadPlus == -1) || i == strlen(codeLine) - 1 || (lookAhead == -1 && (specialIndex != -1)))
        {
            // asdfas % *
            // Substring attempts to create a word that is either a reserved word or
            // identifier. The substring will return null if the start is a special character

            char *word = subString(start, i + 1, codeLine);
            // token is set as default to the identifier token value
            int token = identsym;
            // If the word exists check if the word is both valid or a reserved word.
            // If it is a resseved word, tokenize it, if it is not valid, stop the parse,
            // free the WIP parse string and entirely and return null from the entire function
            // to indicate an error occured. Prints out to console on what type of error occured
            // on the given line, before exiting on error.
            if (word != NULL)
            {
                // checks if word is valid, errors will return null
                int valid = isWordValid(word);
                // printf("%s\n",word);
                reservedIndex = isStatementReserved(word);
                if (reservedIndex != -1)
                {
                    token = resWordsTokens[reservedIndex];
                }
                if (valid == 2 || valid == -3)
                {
                    token = numbersym;
                }
                start = i + 1;
                // if(lookAheadPlus == -1) start = start + 1;
            }
            else
            {
                // Take the index of the special character in the codeline (that failed to create a word) and
                // find what special char token it corresponds to
                specialIndex = characterInSymbolTableBS(codeLine[start], specialTerminalSymbolsOrdered);
                token = specialTerminalSymbolsTokens[specialIndex];
                // printf("TOKEN LOOKING FOR 13 %d\n",token);
                //  if(specialIndex == -1)
                //  {
                //      token  = -1;
                //  }
                //  Some tokens can merge with other tokens to create special cases.
                //  This checks for those and merges the tokens accordingly and shifts the
                //  start and i by 1, as in this case we "truncate" the string by one char
                //  when completeing a merge.

                //  The switch also tests for comments to ignore when tokenizing, IT DOESN'T CHECK CONTENT
                //  of comments. Invalid characters could be inside the commments. Also, it doesn't assume
                //  there to be nested comments like: /*/**/*/
                switch (token)
                {
                case 0:
                    if (codeLine[start + 1] == '=')
                    {
                        token = becomessym; // :=
                        start = start + 1;
                        i++;
                    }
                    break;
                case lessym:
                    if (codeLine[start + 1] == '=')
                    {
                        token = leqsym; // <=
                        start = start + 1;
                        i++;
                    }
                    else if (codeLine[start + 1] == '>')
                    {
                        token = neqsym; // <>
                        start = start + 1;
                        i++;
                    }
                    break;
                case gtrsym:
                    if (codeLine[start + 1] == '=')
                    {
                        token = geqsym; // >=
                        start = start + 1;
                        i++;
                    }
                    break;
                case slashsym:                      // Slashsym is for /
                    if (codeLine[start + 1] == '*') // checks for a /* case to indicate a comment start
                    {
                        token = -1; // From comment start onwards is to be ignored, so token is set to -1
                        int commentError = 1;
                        // Check to see if based on where the comment start is, if it would be
                        // possible to have a */ in the remaining space of the codeLine passed in
                        if ((strlen(codeLine) - (i + 1)) >= 2)
                        {
                            // Loop through and check to find if a corresponding */ exists
                            for (int x = start + 2; x < strlen(codeLine) - 1; x++)
                            {
                                char twoChars[] = {codeLine[x], codeLine[x + 1]}; // probably needs to change to checking only last two chars,
                                if (twoChars[0] == '*' && twoChars[1] == '/')     // instead of checking whole thing
                                {
                                    commentError = 0;
                                    //  if the comment ends on the last character of the codeLine
                                    //  break out and end the parse.
                                    if (x + 1 == strlen(codeLine) - 1)
                                    {
                                        halt_flag = 0;
                                    }
                                    else
                                    {
                                        start = x + 1;
                                        i = x + 1;
                                    }
                                    break;
                                }
                            }
                        }
                        if (commentError) // no end of comment was found, return an error
                        {
                            token = -5;
                            start = start + 1;
                            i++;
                            // printf("Code line: '%s' has an unresolved comment, not ended with '*/' \n", codeLine);
                            // free(parsedString);
                            // return NULL;
                        }
                    }
                    break;
                }
                if (halt_flag == 0)
                {
                    halt_flag = 1; // reset the halt flag
                    break;
                }
                start = start + 1; // increment start
            }
            // Negative tokens are to be ignored, only positive are to be added on the string
            if (token > 0 || token == -5)
            {
                char int_string[4];
                sprintf(int_string, "%d ", token);
                strcat(parsedString, int_string);
                if (token == identsym || token == numbersym || reservedIndex != -1)
                {
                    if (reservedIndex == -1)
                    {
                        strcat(parsedString, word);
                        strcat(parsedString, " ");
                    }
                }
            }
            //    if(token == 2 && characterInSymbolTableBS(word[0], symbolTableOrdered) == -1)
            //    {
            //         start = start + 1;
            //    }
        }
        // else
        // {
        //     if(current == -1)
        //     {
        //         char invalid_string[4];
        //         invalid_string[0] = '2';
        //         invalid_string[1] = ' ';
        //         invalid_string[2] = codeLine[i];
        //         invalid_string[3] = '\0';
        //         strcat(parsedString, invalid_string);
        //         strcat(parsedString, " ");
        //         //continue;
        //     }
        // }
    }
    return parsedString;
}
int isStatementReserved(char *word)
{
    int retval = -1;
    for (int x = 0; x < 13; x++)
    {
        if (strcmp(word, resWords[x]) == 0)
        {
            retval = x;
            break;
        }
    }
    return retval;
}
int isWordValid(char *word)
{
    int retval = 1;
    // If it starts with an integer make sure it only has integers
    if (word[0] >= '0' && word[0] <= '9')
    {
        for (int x = 1; x < strlen(word); x++)
        {
            if (word[x] < '0' || word[x] > '9')
            {
                retval = -2;
                break;
            }
        }
        // If the integer is longer than 5 digits
        if (retval != -2)
        {
            if (strlen(word) > 5)
            {
                retval = -3;
            }
            else
            {
                retval = 2;
            }
        }
    }
    // If the word is longer than CMAX characters
    if (strlen(word) > CMAX && retval == 1)
    {
        retval = -1;
    }
    return retval;
}
char *subString(int start, int end, char *line)
{
    if (characterInSymbolTableBS(line[start], specialTerminalSymbolsOrdered) != -1 && characterInSymbolTableBS(line[start], symbolTableOrdered) != -1)
        return NULL;
    char *word = malloc(sizeof(char) * (end - start + 1));
    word[0] = '\0';
    for (int i = start; i < end; i++)
    {
        if (characterInSymbolTableBS(line[i], symbolTableOrdered) != -1)
        {
            word[strlen(word)] = line[i];
            word[strlen(word) + 1] = '\0';
        }
        if (characterInSymbolTableBS(line[i], symbolTableOrdered) == -1)
        {
            break;
        }
    }
    if (word[0] == '\0')
    {
        return NULL;
    }
    else
    {
        return word;
    }
}
int numberOfFileLines(char *filename)
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
int characterInSymbolTableBS(char c, char *symTbl)
{
    int low = 0;
    int high = strlen(symTbl) - 1;
    while (low <= high)
    {
        int mid = (high + low) / 2;
        if (symTbl[mid] == c)
            return mid;
        else if (symTbl[mid] > c)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}
int binarySearch(int arr[], int left, int right, int x)
{
    while (left <= right)
    {
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

void freeAll()
{
    for (int i = 0; i < MAX_NAME_TABLE_SIZE; i++)
    {
        if (symbol_Table[i] != NULL)
        {
            free(symbol_Table[i]);
        }
        if (assembly_Code[i] != NULL)
        {
            free(assembly_Code[i]);
        }
        if (functionCall_Table[i] != NULL)
        {
            free(functionCall_Table[i]);
        }
        if (assembly_Code[i] == NULL && symbol_Table[i] == NULL && functionCall_Table[i] != NULL)
        {
            break;
        }
    }
}

//==================================================================================================================================================================//
//==================================================================================================================================================================//
//==================================================================================================================================================================//
void outputAssemblyToFile()
{
    // printf("\nPRINTING TO \"outputAssemblyCode.txt\" ...\n");
    FILE *file = fopen("elf.txt", "w");
    if (file == NULL)
    {
        printf("Failed to pen File!\n");
        return;
    }

    for (int i = 0; i < universalCodeText; i++)
    {
        fprintf(file, "%d %d %d\n", assembly_Code[i]->OP, assembly_Code[i]->L, assembly_Code[i]->M);
    }

    fclose(file);
    // printf("SUCCESS!\n");
}
char *GET_Token()
{
    // printf("\n\nEntering the GET_Token\n");
    // initializing variables
    char *RETVAL = (char *)malloc(sizeof(char) * 12);
    // printf("This is Global Lexeme: %s\n", global_Lexeme);
    int length = strlen(global_Lexeme);
    int foundIt = 0;
    // initializing token and word
    char *token = (char *)malloc(sizeof(char) * 1000);
    token[0] = '\0';

    char specialCharacter;
    int index = 0;
    int tokenToInt;
    char stopChar = ' ';

    if (universialIndex == length - 1)
    {
        RETVAL = NULL;
        return RETVAL;
    }
    // printf("This is universalIndex: %d\n", universialIndex);
    for (int i = universialIndex; i < length; i++, universialIndex++)
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
        if (global_Lexeme[i + 1] != ' ')
        {
            continue;
        }

        if (strcmp(token, "20") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token); // is this psossible when retval destination is null? I will check it

            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "10") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);

            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "12") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }
        if (strcmp(token, "13") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }
        if (strcmp(token, "14") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }
        if (strcmp(token, "-5") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);
            break;
        }

        if (strcmp(token, "3") == 0)
        {
            // copy whatever the token is to RETVAL
            strcpy(RETVAL, token);
            memset(token, '\0', 1000);

            break;
        }

        // done
        if (strcmp(token, "2") == 0)
        {
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
            for (int j = 0; j < 15; j++)
            {
                if (specialTerminalSymbolsTokens[j] == tokenToInt)
                {
                    // store token in retval

                    strcpy(RETVAL, token);
                    memset(token, '\0', 1000);
                    foundIt = 1;
                    break;
                }
            }

            if (foundIt)
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
        if (index != -1)
        {
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
    // printf("GET_TokenInteger enter\n ");
    char *temp = GET_Token();
    if (temp != NULL)
    {
        tokencount++;
        // printf("Get token: %s\n", temp);
        int stringLength = strlen(temp);
        // printf("Get token len: %d\n", stringLength);

        for (int i = 0; i < stringLength; i++)
        {
            if (isdigit(temp[i]) == 0)
            {
                return -10;
            }
        }
        int ret = atoi(temp);
        free(temp);
        return ret;
    }
    return -11;
}

void PROGRAM()
{
    namerecord_t *newCode = initializeNameRecord(0, " ", 0, 0, 0, 0); // used for sentinel value
    symbol_Table[0] = newCode;
    newCode = initializeNameRecord(3, "main", 0, 0, 3, 1); // hardset proc at the begining
    symbol_Table[1] = newCode;
    // printf("Just created main\n");
    recursiveNameRecord *newProcedure = initializeRecursiveNameRecord("main", 3);
    functionCall_Table[0] = newProcedure;

    // COMMMENTED OUT ASSUMED THAT BLOCK WILL DO IT NOW
    // assembly_Node* newAssCode;
    // newAssCode = initializeAssemblyRecord(7, 0, 3);
    // assembly_Code[0] = newAssCode;

    while (TOKEN != endsym)
    {
        BLOCK();
        if (TOKEN == endsym)
        {
            break;
        }
        int temp = TOKEN;
        TOKEN = Get_TokenInteger();
        if (TOKEN == -11)
        {
            TOKEN = temp;
            break;
        }
    }
    // printf("TOKEN is %d after While loop\n", TOKEN);
    // printf("reentering Block with %d\n", TOKEN);
    // Call token looking for '.' ENDING PROGRAM
    // printf("out Token Grab While %d\n",TOKEN);

    TOKEN = Get_TokenInteger();
    while (TOKEN == endsym)
        TOKEN = Get_TokenInteger(); // <-this needs to be checked out a bit more

    // printf("Block post token grab %d\n", TOKEN);

    if (TOKEN != periodsym || TOKEN == -11)
    {
        printf("-Error: program must end with period-\n");
        exit(0);
    }
    else
    {
        assembly_Node *newAssCode;
        newAssCode = initializeAssemblyRecord(9, 0, 3);
        assembly_Code[universalCodeText] = newAssCode;
        universalCodeText++;

        // printf("No errors Detected. Compile executed Successfully.\n");

        // printf("\ncalling ulimateUpdateAllAddresses\n");
        ulitmateUpdateAllAddresses();
        // Print Assembly code
        printAssCodes(); // will need to be changed for HW4
        printSymTbl();
        outputAssemblyToFile();
        // printf("\n Executable Output\n\n ");
        mainVM();
    }
}

int BLOCK()
{
    TOKEN = Get_TokenInteger();
    // printf("Block Enter Area %d\n",TOKEN);

    // universalLevel++;
    int dx = 2; // 2 or 3? I think 2
    assembly_Node *newCode = initializeAssemblyRecord(7, 0, 0);
    int jmpIdx = universalCodeText;
    // printf("%d    JMP    0    %d\n",universalCodeText,loopIdx*3);
    assembly_Code[jmpIdx] = newCode;
    universalCodeText++;
    // do
    //{
    if (constsym == TOKEN)
    {
        CONST_DECLARATION();
        TOKEN = Get_TokenInteger();
    }
    if (varsym == TOKEN)
    {
        // printf("Var Block Enter Area %d\n",TOKEN);
        VAR_DECLARATION(&dx); // handles looping aspect
        TOKEN = Get_TokenInteger();
        // printSymTbl();
    }
    while (procsym == TOKEN)
    {
        // lv and dx
        // printSymTbl();

        universalLevel++;
        PROC_DECLARATION();
        SYMBOLTABLEDELETELEVEL(universalLevel);
        universalLevel--;
        TOKEN = Get_TokenInteger();
    }
    //}while(TOKEN = constsym || TOKEN = procsym || TOKEN = varsym)
    assembly_Code[jmpIdx]->M = universalCodeText * 3; // fixing up jmp address
    // dx will be flexible to any number of declarations hence why the declarations need to be altered

    newCode = initializeAssemblyRecord(6, 0, (dx + 1));
    // printf("%d    INC    0    %d\n",universalCodeText, (3+variableCount));
    assembly_Code[universalCodeText] = newCode;
    // int incrementAddress = universalCodeText;
    universalCodeText++;
    STATEMENT();

    if (universalLevel != 0 && universalLevel > 0)
    {
        newCode = initializeAssemblyRecord(2, 0, 0); // return operation?
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
    }
    // clean house? for symbol table?
    // eliminate level? <--- to be implemented
    // SYMBOLTABLEDELETELEVEL(universalLevel);
    // universalLevel--;
    return assembly_Code[jmpIdx]->M; // will this work?
}

void PROC_DECLARATION()
{
    int address = -5;
    TOKEN = Get_TokenInteger();
    if (TOKEN == identsym)
    {
        // grab identifier function that grabs and saves variable
        char *nameIdent = GET_Token();
        if (SYMBOLTABLECHECKLEVEL(nameIdent) != -1) // needs changing
        {
            printf("Error: symbol name has already been declared\n");
            exit(0);
        }
        // initializeNameRecord(int _kind, char* _name, int _val, int _level, int _adr, int _mark);
        namerecord_t *newPrc = initializeNameRecord(3, nameIdent, 0, universalLevel - 1, -5, 0);

        // Store object in main name array.
        int tempPrcInd = universalSymbolIndex;
        symbol_Table[universalSymbolIndex] = newPrc;
        universalSymbolIndex++;
        // printSymTbl();

        // Create a new function struct;
        recursiveNameRecord *newProcedureRecord = initializeRecursiveNameRecord(nameIdent, -5);
        // printf("creating a new function in funcitonCall_Table with name %s\n", nameIdent);
        int tempRecursiveTableIdx = universalRecursiveTableIdx;
        functionCall_Table[universalRecursiveTableIdx] = newProcedureRecord;
        universalRecursiveTableIdx++;

        TOKEN = Get_TokenInteger();
        if (TOKEN == semicolonsym)
        {
            // TOKEN = Get_TokenInteger(); // goes one block higher
            // printf("Perc AD pre %d\n",symbol_Table[tempPrcInd]->adr);
            address = BLOCK();
            symbol_Table[tempPrcInd]->adr = address;

            // Store address in function struct;
            // printf("storing address %d in %s\n", address, nameIdent);
            functionCall_Table[tempRecursiveTableIdx]->adr = address;
            // printf("Perc AD post %d\n",symbol_Table[tempPrcInd]->adr);
            // printf("Perc Token %d\n",TOKEN);

            TOKEN = Get_TokenInteger();
            if (TOKEN != semicolonsym)
            {
                printf("Error: block statement in procedures must be followed by a semicolon\n");
                exit(0);
            }
        }
        else
        {
            printf("Error: procedure, constant, and variable declarations must be followed by a semicolon\n");
            exit(0);
        }
    }
}

// hasn't changed too much
void CONST_DECLARATION()
{
    while (1)
    {
        // check for identifier
        TOKEN = Get_TokenInteger();
        if (TOKEN == identsym)
        {
            // grab identifier function that grabs and saves variable
            char *nameIdent = GET_Token();
            if (SYMBOLTABLECHECK(nameIdent) != -1)
            {
                // VERIFIED
                printf("Error: symbol name has already been declared\n");
                exit(0);
            }
            // create named object for record
            // keep reference
            TOKEN = Get_TokenInteger();
            if (TOKEN == eqsym)
            {
                TOKEN = Get_TokenInteger();
                if (TOKEN == numbersym)
                {
                    // grab number
                    // store number in from reference
                    // main array name record addition
                    //  call initialize add in the value and the identifer name

                    namerecord_t *newConst = initializeNameRecord(1, nameIdent, Get_TokenInteger(), universalLevel, 0, 0);
                    // Store object in main name array.
                    symbol_Table[universalSymbolIndex] = newConst;
                    universalSymbolIndex++;

                    // Sudo code sets the address to 0 always, when setting a constant why?
                    // "add to symbol table (kind 1, saved name, number, 0, 0)"

                    TOKEN = Get_TokenInteger();
                    if (TOKEN == commasym)
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
                    // Error
                    exit(0);
                }
            }
            else
            {
                // VERIFIED
                printf("Error: constants must be assigned with =\n");
                // Error
                exit(0);
            }
        }
        else
        {
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            // Error
            exit(0);
        }
    }
}

void VAR_DECLARATION(int *dx)
{
    // printf("Var enter Area %d\n",TOKEN);

    // checking until semicolon
    // TOKEN = Get_TokenInteger();
    variableCount = 0;
    while (1)
    {
        // check for identifier
        // printf("var Before call %d\n",TOKEN);
        TOKEN = Get_TokenInteger();
        // printf(" Var 1 token %d\n",TOKEN);
        if (TOKEN == identsym)
        {
            // Grab identifier, function that grabs and saves variable
            char *nameIdent = GET_Token();
            // printf("Var Identifier Name %s\n",nameIdent);
            if (SYMBOLTABLECHECKLEVEL(nameIdent) != -1)
            {
                // VERIFIED
                printf("Error: symbol name has already been declared\n");
                exit(0);
            }
            // Create named object for record
            // printf("Block Var name Pass \n");
            variableCount++; // might cause a problem
            namerecord_t *newVar = initializeNameRecord(2, nameIdent, 0, universalLevel, variableCount + *dx, 0);
            // Store object in main name array.
            symbol_Table[universalSymbolIndex] = newVar;
            universalSymbolIndex++;
            free(nameIdent); // must free becuase we used Get_Token().
            // increment VAR counter

            // The addresses of the variables added to name table MUST be
            // correct with regards to what is already there (var# +2)
            TOKEN = Get_TokenInteger();
            if (TOKEN == commasym)
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
                // Error
                exit(0);
            }
        }
        else
        {
            // NO IDENTIFIER, INVALID TOKEN
            //  VERIFIED
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            // Error
            exit(0);
        }
    }
    (*dx) += variableCount;
}

void STATEMENT()
{
    // printf("Statement enter Area %d\n",TOKEN);
    // printf("Statement index %d\n",tokencount);

    // MASSIVE SWITCH STATEMENT BEWARE
    int symbolIndex;
    char *nameIdent;
    int jpcIdx;
    // int jpcIdxAdd;
    int M;
    int L;
    assembly_Node *newCode;
    // printf("TOKEN %d\n",TOKEN);

    switch (TOKEN)
    {
        //-----------------------------------------------------------------------------------------
    case identsym:
        // Grab identifier, function that grabs and saves variable
        // printf("Statement enter Ident %d\n",TOKEN);
        nameIdent = GET_Token();
        // printf("Statement name Ident %s\n",nameIdent);
        symbolIndex = SYMBOLTABLECHECK(nameIdent); // check that the identifer exists
        // printSymTbl();
        // printf("post symbolTable Check %d\n",symbolIndex);
        if (symbolIndex == -1)
        {
            // VERIFIED
            printf("Error: undeclared identifier: %s\n", nameIdent);
            exit(0);
            // The exit() function in C. The exit() function is used to terminate a process or function calling immediately in the program.
        }
        if (symbol_Table[symbolIndex]->kind != 2)
        {
            printf("Error: only variable values may be altered\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger();
        // printf("Statement Identifier Area  %d\n",TOKEN);
        // printf("Statement Identifier Area Name  %s\n",nameIdent);

        if (TOKEN != becomessym)
        {
            // ERROR , become operator missing, expected for the statement
            // VERIFIED
            printf("Error: assignment statements must use :=\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger();

        // printf("Statement Identifier Area Pre Expression %d\n",TOKEN);
        EXPRESSION();
        // printf("Statement Identifier Area Post Expression %d\n",TOKEN);
        if (TOKEN == identsym)
        {
            STATEMENT();
        }

        M = symbol_Table[symbolIndex]->adr;
        L = universalLevel - symbol_Table[symbolIndex]->level;

        newCode = initializeAssemblyRecord(4, L, M);
        // printf("%d    STO    0    %d\n",universalCodeText, M);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        // emit STO (M = table[symIdx].addr)
        break;
        //-----------------------------------------------------------------------------------------
    case beginsym:
        while (1)
        {
            // if (TOKEN == beginsym)
            //     printf("Statement enter in Begin %d\n",TOKEN);
            // else
            //     printf("Statement re-entering loop in Begin %d\n",TOKEN);
            // printf("Statement pre in Begin  %d\n",TOKEN);
            TOKEN = Get_TokenInteger();
            // printf("pre Statement %d\n", TOKEN);
            if (TOKEN != -11)
            {
                STATEMENT();
            }
            // printf("Statement Post in Begin  %d\n",TOKEN);

            if (TOKEN != semicolonsym)
            {
                break;
            }
        }
        // printf("Statement Post near end  %d\n",TOKEN);

        if (TOKEN != endsym)
        {
            // char* name = GET_Token();
            // printf("Statement Post in end error  %s\n",name);
            // TOKEN = Get_TokenInteger();
            // printf("Statement Post in end error  %d\n",TOKEN);

            printf("Error: begin must be followed by end\n");
            // printf("False Exit\n");
            exit(0);
        }

        break;
        //-----------------------------------------------------------------------------------------
    case ifsym: // FIXXXXXX MEEE!!!!!! look at pg 22 of newest slides 000
        TOKEN = Get_TokenInteger();
        CONDITION();
        // emit jpc, add to assembly code struct array

        jpcIdx = universalCodeText;
        // jpcIdxAdd = universalCodeText;

        newCode = initializeAssemblyRecord(8, 0, 0);
        // printf("%d    JPC    0    0\n",universalCodeText);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;

        if (TOKEN != thensym)
        {
            printf("Error: if must be followed by then\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger();
        STATEMENT();
        assembly_Code[jpcIdx]->M = universalCodeText * 3;
        // code[jpcIdx].M = current code index
        break;
        //-----------------------------------------------------------------------------------------
    case whilesym: // FIXXXXXX MEEE!!!!!! look at pg 24 of newest slides
        TOKEN = Get_TokenInteger();
        int loopIdx = universalCodeText;
        CONDITION();
        if (TOKEN != dosym)
        {
            printf("Error: while must be followed by do\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger();
        jpcIdx = universalCodeText;
        newCode = initializeAssemblyRecord(8, 0, 0);
        // printf("%d    JPC    0    %d\n",universalCodeText,universalCodeText*3);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        STATEMENT();

        newCode = initializeAssemblyRecord(7, 0, loopIdx * 3);
        // printf("%d    JMP    0    %d\n",universalCodeText,loopIdx*3);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        // JPC REASSIGNMENT
        assembly_Code[jpcIdx]->M = universalCodeText * 3;

        break;
        //-----------------------------------------------------------------------------------------
    case readsym:
        // Grab identifier, function that grabs and saves variable
        TOKEN = Get_TokenInteger();
        if (TOKEN == 2)
        {
            nameIdent = GET_Token();
            symbolIndex = SYMBOLTABLECHECK(nameIdent);
            if (symbolIndex == -1)
            {
                printf("Error: undeclared identifier: %s\n", nameIdent);
                exit(0);
                // The exit() function in C. The exit() function is used to terminate a process or function calling immediately in the program.
            }
            if (symbol_Table[symbolIndex]->kind != 2)
            {
                printf("Error: only variable values may be altered\n");
                exit(0);
            }
            TOKEN = Get_TokenInteger();
            newCode = initializeAssemblyRecord(9, 0, 2);
            // printf("%d    SYS    0    2\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;

            M = symbol_Table[symbolIndex]->adr;
            L = universalLevel - symbol_Table[symbolIndex]->level;

            newCode = initializeAssemblyRecord(4, L, M);
            // printf("%d    STO    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
        }
        else
        {
            printf("Error: const, var, and read keywords must be followed by identifier\n");
            exit(0);
        }
        break;
        //-----------------------------------------------------------------------------------------
    case writesym:
        TOKEN = Get_TokenInteger();
        EXPRESSION();
        // emit WRITE
        newCode = initializeAssemblyRecord(9, 0, 1);
        // printf("%d    SYS    0    1\n",universalCodeText);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        break;
        //-----------------------------------------------------------------------------------------
    case endsym:
        // if we see the endsym, the statement(beginsym) will check wether or not it exists.
        // we were hitting the default before :(.
        break;
    case callsym: // needs call to op code
        TOKEN = Get_TokenInteger();
        if (TOKEN == 2)
        {
            nameIdent = GET_Token();
            symbolIndex = SYMBOLTABLECHECK(nameIdent);
            if (symbolIndex == -1)
            {
                printf("Error: undeclared identifier: %s\n", nameIdent);
                exit(0);
            }
            if (symbol_Table[symbolIndex]->kind != 3)
            {
                printf("Error: only variable values may be altered\n");
                exit(0);
            }

            // printf("%d    CALL ADD\n",symbol_Table[symbolIndex]->adr);

            newCode = initializeAssemblyRecord(5, universalLevel - symbol_Table[symbolIndex]->level, symbol_Table[symbolIndex]->adr);
            // if(newCode->adr)
            // printf("%d    CALL    L    M\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;

            // printf("addNewFunctionCallFunction table with the name %s which in on line %d\n ", nameIdent, universalCodeText - 1);
            addNewFunctionCallToFunctionTable(nameIdent, universalCodeText);
        }
        else
        {
            printf("Error: call  must be followed by identifier\n");
            exit(0);
        }
        TOKEN = Get_TokenInteger(); // <careful with this one
        // printf("Statement Post in CALL  %d\n",TOKEN);

        break;

        //-----------------------------------------------------------------------------------------
    default:
        printf("Error: Invalid operation for statement when TOKEN is %d\n", TOKEN);
        exit(0);
        break;
        //-----------------------------------------------------------------------------------------
    }
}
// level-var level
void EXPRESSION()
{
    assembly_Node *newCode;
    // printf("Entering Term with %d\n",TOKEN);
    TERM();
    // printf("Leaving Term and entering Expression with %d\n",TOKEN);

    if (TOKEN == minussym)
    {
        // printf("minus if statement\n");
        //-------------------------------------------------------
        //  newCode = initializeAssemblyRecord(11, 0, 0);
        //  printf("%d    NEGATIVE    0    0\n",universalCodeText);
        //  assembly_Code[universalCodeText] = newCode;
        //  universalCodeText++;
        //  universalCodeAddress += 3;
        //-------------------------------------------------------
        while (TOKEN == plussym || TOKEN == minussym)
        {
            if (TOKEN == plussym)
            {
                TOKEN = Get_TokenInteger();
                TERM();
                // emit ADD
                newCode = initializeAssemblyRecord(2, 0, 1);
                // printf("%d    ADD    0    1\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
            }
            else
            {
                // printf("entering else statement\n");
                TOKEN = Get_TokenInteger();
                TERM();
                // emit SUB
                newCode = initializeAssemblyRecord(2, 0, 2);
                // printf("%d    SUB    0    2\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
            }
        }
    }
    else if (TOKEN == plussym)
    {
        while (TOKEN == plussym || TOKEN == minussym)
        {
            if (TOKEN == plussym)
            {
                TOKEN = Get_TokenInteger();
                TERM();
                // emit add
                //-------------------------------------------------------
                newCode = initializeAssemblyRecord(2, 0, 1);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                //-------------------------------------------------------
            }
            else
            {
                TOKEN = Get_TokenInteger();
                TERM();
                // emit SUB
                //-------------------------------------------------------
                newCode = initializeAssemblyRecord(2, 0, 2);
                // printf("%d    SUB    0    2\n",universalCodeText);
                assembly_Code[universalCodeText] = newCode;
                universalCodeText++;
                //-------------------------------------------------------
            }
        }
    }
}

void CONDITION()
{
    // printf("Entering CONDITION with: %d\n",TOKEN);
    // TOKEN = Get_TokenInteger();
    // printf("Post grab CONDITION with: %d\n",TOKEN);

    assembly_Node *newCode;
    if (TOKEN == oddsym)
    {
        TOKEN = Get_TokenInteger();
        EXPRESSION();
        // emit ODD,
        //-------------------------------------------------------
        newCode = initializeAssemblyRecord(2, 0, 11);
        // printf("%d    ODD    0    11\n",universalCodeText);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;
        //-------------------------------------------------------
    }
    else
    {
        // no get token here?
        EXPRESSION();
        switch (TOKEN)
        {
            //-----------------------------------------------------------------------------------------
        case eqsym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit EQL
            newCode = initializeAssemblyRecord(2, 0, 5);
            // printf("%d    EQL    0    5\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        case neqsym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit NEQ
            newCode = initializeAssemblyRecord(2, 0, 6);
            // printf("%d    NEQ    0    6\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        case lessym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit LSS
            newCode = initializeAssemblyRecord(2, 0, 7);
            // printf("%d    LSS    0    7\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        case leqsym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit LEQ
            newCode = initializeAssemblyRecord(2, 0, 8);
            // printf("%d    LEQ    0    8\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        case gtrsym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit GTR
            newCode = initializeAssemblyRecord(2, 0, 9);
            // printf("%d    GTR    0    9\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        case geqsym:
            TOKEN = Get_TokenInteger();
            EXPRESSION();
            // emit GEQ
            newCode = initializeAssemblyRecord(2, 0, 10);
            // printf("%d    GEQ    0    10\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
            break;
        default:
            printf("Error: condition must contain comparison operator\n");
            exit(0);
            break;
            //-----------------------------------------------------------------------------------------
        }
    }
}

int SYMBOLTABLECHECK(char *name)
{
    // printf("SYM TBL %s\n",name);

    // // printf("pre strcpy\n");
    strcpy(symbol_Table[0]->name, name);
    // // printf("post strcpy\n");

    // printf("SYM TBL %s\n",symbol_Table[0]->name);

    // // printf("pre loop \n");
    for (int i = universalSymbolIndex - 1; i > 0; i--)
    {
        if (strcmp(symbol_Table[i]->name, symbol_Table[0]->name) == 0 && i != 0)
        {
            return i;
        }
    }

    // printf("post strcpy\n");
    return -1;
}

int SYMBOLTABLECHECKLEVEL(char *name)
{
    // printf("SYM TBL %s\n",name);

    // // printf("pre strcpy\n");
    strcpy(symbol_Table[0]->name, name);
    // // printf("post strcpy\n");

    // printf("SYM TBL %s\n",symbol_Table[0]->name);

    // // printf("pre loop \n");
    for (int i = universalSymbolIndex - 1; i > 0; i--)
    {
        if (strcmp(symbol_Table[i]->name, symbol_Table[0]->name) == 0 && i != 0 && symbol_Table[i]->level == universalLevel)
        {
            return i;
        }
    }

    // printf("post strcpy\n");
    return -1;
}

void SYMBOLTABLEDELETELEVEL(int level)
{
    if (level == 0)
        return;
    // printf("SYM TBL %s\n",name);

    // // printf("pre strcpy\n");
    for (int i = universalSymbolIndex - 1; i > 0; i--)
    {
        if (symbol_Table[i]->level == level)
        {
            symbol_Table[i] = NULL;
            universalSymbolIndex--;
        }
        else
        {
            break;
        }
    }
}
// int SYMBOLTABLECHECK(char* name, int level)
// {

//     strcpy(symbol_Table[0]->name,name);

//     for(int i = universalSymbolIndex - 1; i > 0; i--)
//     {
//         if(strcmp(symbol_Table[i]->name, symbol_Table[0]->name) == 0 && i != 0 && symbol_Table[i]->level == level)
//         {
//             return i;
//         }
//         if(symbol_Table[i]->level == 0)
//         {
//             break;//start of procedure storage
//         }
//     }

//     //printf("post strcpy\n");
//     return -1;
// }

void TERM()
{
    // printf("Entering Factor with: %d\n",TOKEN);
    FACTOR();
    // TOKEN = Get_TokenInteger();
    // printf("SemiColon Term Or 4 which is +? %d\n",TOKEN);
    while (TOKEN == multsym || TOKEN == slashsym)
    {
        if (TOKEN == multsym)
        {
            TOKEN = Get_TokenInteger();
            FACTOR();
            // emit MUL
            assembly_Node *newCode = initializeAssemblyRecord(2, 0, 3);
            // printf("%d    MUL    0    3\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
        }
        else if (TOKEN == slashsym)
        {
            TOKEN = Get_TokenInteger();
            FACTOR();
            // emit DIV
            assembly_Node *newCode = initializeAssemblyRecord(2, 0, 4);
            // printf("%d    DIV    0    4\n",universalCodeText);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
        }
    }
}

void FACTOR()
{
    // printf("TOKEN %d\n",TOKEN);

    if (TOKEN == identsym)
    {
        // printf("identsym Pre token grab %d\n",TOKEN);
        char *nameIdent = GET_Token();

        int symIdx = SYMBOLTABLECHECK(nameIdent);
        // printSymTbl();
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
            int L = universalLevel - symbol_Table[symIdx]->level;

            assembly_Node *newCode = initializeAssemblyRecord(1, L, symbol_Table[symIdx]->val);
            // printf("%d    LIT    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
        }
        else // 2 = var
        {
            // emit LIT (M = symbol_Table[symIdx].Value)
            int M = symbol_Table[symIdx]->adr;
            int L = universalLevel - symbol_Table[symIdx]->level;

            assembly_Node *newCode = initializeAssemblyRecord(3, L, symbol_Table[symIdx]->adr);
            // printf("%d    LOD    0    %d\n",universalCodeText,M);
            assembly_Code[universalCodeText] = newCode;
            universalCodeText++;
        }
        //  printf("TOKEN PRE LOD: %d\n",TOKEN);
        TOKEN = Get_TokenInteger();
        //    printf("TOKEN POST LOD: %d\n",TOKEN);
    }
    else if (TOKEN == numbersym)
    {
        // printf("Num Sys Pre token grab %d\n",TOKEN);
        int M = Get_TokenInteger();
        //  printf("Num Sys Post token grab %d\n",M);

        assembly_Node *newCode = initializeAssemblyRecord(1, 0, M); // changed this from 3 to 1            PAY ATTENTION THIS I BELEIVE SHOULD STAY 0
                                                                    //  printf("%d    LIT    0    %d\n",universalCodeText,M);
        assembly_Code[universalCodeText] = newCode;
        universalCodeText++;

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

namerecord_t *initializeNameRecord(int _kind, char *_name, int _val, int _level, int _adr, int _mark)
{
    namerecord_t *new_record = malloc(sizeof(namerecord_t));
    new_record->kind = _kind;
    strcpy(new_record->name, _name);
    new_record->val = _val;
    new_record->level = _level;
    new_record->adr = _adr;
    new_record->mark = _mark;
    return new_record;
}

recursiveNameRecord *initializeRecursiveNameRecord(char *_name, int _adr)
{
    recursiveNameRecord *newRecursiveNameRecord = malloc(sizeof(recursiveNameRecord));
    strcpy(newRecursiveNameRecord->name, _name);
    for (int i = 0; i < MAX_NAME_TABLE_SIZE; i++)
    {
        newRecursiveNameRecord->nameInstancesJmpIdx[i] = -1;
    }

    newRecursiveNameRecord->adr = _adr; // UniversalCodeText
    return newRecursiveNameRecord;
}

void addNewFunctionCallToFunctionTable(char *name, int jpxIdx)
{
    int i = 0;
    int j = 0;
    int currentJmpIdx;

    // Find the function struct with that name, and set its most recent indicie of nameInstanceJmpIdx to the jmpIdx.
    for (i = universalRecursiveTableIdx - 1; i > 0; i--)
    {
        if (strcmp(functionCall_Table[i]->name, name) == 0)
        {
            for (j = 0; j < MAX_NAME_TABLE_SIZE; j++)
            {
                if (functionCall_Table[i]->nameInstancesJmpIdx[j] == -1)
                {
                    functionCall_Table[i]->nameInstancesJmpIdx[j] = jpxIdx - 1;
                    return;
                }
            }
        }
    }
}

void ulitmateUpdateAllAddresses()
{
    int i = 0;
    int j = 0;
    int currentJmpIdx;

    // For each Function struct update all of its function calls to the right address.
    for (i = universalRecursiveTableIdx - 1; i > 0; i--)
    {
        // printf("entered for loop when i is %d\n", i);
        // printf("Function struct being accessed is '%s'\n", functionCall_Table[i]->name);
        //  update all assembly_codes with that function name to the address.
        for (j = 0; j < MAX_NAME_TABLE_SIZE; j++)
        {
            currentJmpIdx = functionCall_Table[i]->nameInstancesJmpIdx[j];
            // printf("When j is %d currentJmpIdx is %d\n", j, currentJmpIdx);
            if (currentJmpIdx == -1)
            {
                return;
            }
            else if (currentJmpIdx != -1)
            {
                // printf("updating assemblycode with JpmIdx %d with address %d\n", currentJmpIdx, functionCall_Table[i]->adr);
                assembly_Code[currentJmpIdx]->M = functionCall_Table[i]->adr;
            }
        }
    }
}

void printSymTbl()
{
    printf("\nIndex | Kind | Name | Value | Level | Address | Mark \n");
    for (int i = 1; i < universalSymbolIndex; i++)
    {
        printf("  %d      %d     '%s'      %d       %d      %d         %d\n", i - 1, symbol_Table[i]->kind, symbol_Table[i]->name, symbol_Table[i]->val, symbol_Table[i]->level, symbol_Table[i]->adr, 1);
    }
    printf("\n");
}

void printAssCodes()
{
    printf("\nLine    OP     L    M\n");
    for (int i = 0; i < universalCodeText; i++)
    {
        char *temp = assemblyConvertOP(assembly_Code[i]->OP, assembly_Code[i]->M);
        printf("%d       %s    %d    %d\n", i, temp, assembly_Code[i]->L, assembly_Code[i]->M);
        free(temp);
    }
    printf("\n");
}

char *assemblyConvertOP(int OP, int M)
{
    char *result = malloc(sizeof(char) * 4);
    result[0] = '\0';
    switch (OP)
    {
    case 1:
        strcpy(result, "LIT");
        break;
    case 2:
        switch (M)
        {
        case 0:
            strcpy(result, "RTN");
            break;
        // ADD addition
        case 1:
            strcpy(result, "ADD");
            break;
        // SUB subtraction
        case 2:
            strcpy(result, "SUB");
            break;
        // MUL multiplication
        case 3:
            strcpy(result, "MUL");
            break;
        // DIV division
        case 4:
            strcpy(result, "DIV");
            break;
        // EQL equality
        case 5:
            strcpy(result, "EQL");
            break;
        // NEQ not equal
        case 6:
            strcpy(result, "NEQ");
            break;
        // LSS less than
        case 7:
            strcpy(result, "LSS");
            break;
        // LEQ less than or equal to
        case 8:
            strcpy(result, "LEQ");
            break;
        // GTR greater than
        case 9:
            strcpy(result, "GTR");
            break;
        // GEQ greater than or equal to
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
        if (M == 1)
        {
            strcpy(result, "SOU");
            break;
        }
        else if (M == 2)
        {
            strcpy(result, "SIN");
            break;
        }
        if (M == 3)
        {
            strcpy(result, "EOP");
        }
    }
    return result;
}
assembly_Node *initializeAssemblyRecord(int OP, int L, int M)
{
    assembly_Node *new_record = malloc(sizeof(assembly_Node));
    new_record->OP = OP;
    new_record->L = L;
    new_record->M = M;
    return new_record;
}

void printSourceCode(char *filename)
{

    // Initializing variables
    char buffer[STRMAX];
    int length;
    char tyler;

    // =========================================
    // Initalize input file for viewing
    int lines = numberOfFileLines(filename);
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error opening file. \n");
        return;
    }

    // =========================================
    // =========================================
    // Initalize main code array
    char *codePL = (char *)malloc(sizeof(char) * (STRMAX * lines));
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    printf("Source Program:\n\n");
    // This while loop doesn't ommit comments
    // int errorFlag = 1;
    while (fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        fscanf(fp, "%c", &tyler);

        length = strlen(buffer);

        char *line = (char *)malloc(sizeof(char) * (length + 1));
        line[0] = '\0';
        strcpy(line, buffer);
        printf("%s\n", line);
        free(line);
    }

    return;
}

//==========MAIN===========//
int main(int argc, char *argv[])
{
    //  HW2 MAIN START //
    // Initializing variables
    char buffer[STRMAX];
    int length;
    char tyler;
    int errorFlag = 1;

    // =========================================
    // Initalize input file for viewing
    int lines = numberOfFileLines(argv[1]);
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error opening file. \n");
        return 1;
    }

    // =========================================
    // =========================================
    // Initalize main code array
    char *codePL = (char *)malloc(sizeof(char) * (STRMAX * lines));
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    while (fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        fscanf(fp, "%c", &tyler);
        length = strlen(buffer);

        char *line = (char *)malloc(sizeof(char) * (length + 1));
        line[0] = '\0';
        strcpy(line, buffer);

        line = lexicalParse(line); // lex parse
        if (line != NULL)
        {
            strcat(codePL, line);
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

    if (errorFlag != 0)
    {
        length = strlen(codePL);
        char *token = (char *)malloc(sizeof(char) * 1000);
        token[0] = '\0';
        char *word = (char *)malloc(sizeof(char) * 1000);
        word[0] = '\0';
        char specialCharacter;
        int index = 0;
        int tokenToInt;
        char stopChar = ' ';

        if (EndProgramFlag)
        {
            for (int i = 0; i < length && EndProgramFlag; i++)
            {
                if (codePL[i] == ' ')
                {
                    continue;
                }

                strncat(token, codePL + i, 1);

                if (codePL[i + 1] != ' ')
                {
                    continue;
                }

                if (strcmp(token, "0") == 0)
                {
                    EndProgramFlag = 0;
                    printf("%-9s%5s\n", ":", " Error: symbol invalid when not followed by =");
                    memset(token, '\0', 1000);
                    continue;
                }
                if (strcmp(token, "20") == 0)
                {
                    // printf("we've enterd the token, := if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "10") == 0)
                {
                    // printf("we've enterd the token, <> if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "12") == 0)
                {
                    // printf("we've enterd the token, <= if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }
                if (strcmp(token, "13") == 0)
                {
                    // printf("we've enterd the token, <= if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }
                if (strcmp(token, "14") == 0)
                {
                    // printf("we've enterd the token, >= if statement\n");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "-5") == 0)
                {
                    EndProgramFlag = 0;
                    printf("%-9s%5s\n", "/*", "Error: Unresolved In Line Comment Error");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "3") == 0)
                {
                    char *pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2); // 1
                    strncpy(word, codePL + i + 2, num_chars);
                    int valid = isWordValid(word);
                    if (valid == -3)
                    {
                        EndProgramFlag = 0;
                        printf("%-9s%5s\n", word, "Error: Invalid Number, exceeds max digits of 5");
                    }
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    i += num_chars + 1;
                    continue;
                }

                if (strcmp(token, "2") == 0)
                {
                    char *pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2); // 1
                    strncpy(word, codePL + i + 2, num_chars);
                    if (strlen(word) == 1 && characterInSymbolTableBS(word[0], symbolTableOrdered) == -1)
                    {
                        EndProgramFlag = 0;
                        printf("%-9s%5s\n", word, "Error:  Invalid Symbol");
                    }
                    else
                    {
                        int valid = isWordValid(word);
                        if (valid == -1)
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
                for (int j = 0; j < 15; j++)
                {
                    if (specialTerminalSymbolsTokens[j] == tokenToInt)
                    {
                        index = j;
                        break;
                    }
                }
                if (index != -1)
                {
                    specialCharacter = specialTerminalSymbolsOrdered[index];
                    memset(token, '\0', 1000);
                    continue;
                }
                index = binarySearch(resWordsTokens, 0, 11, tokenToInt);
                if (index != -1)
                {
                    strcat(word, resWords[index]);
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    continue;
                }
            }
        }
        length = strlen(codePL);
        global_Lexeme = (char *)malloc(sizeof(char) * (length));
        strcpy(global_Lexeme, codePL);

        memset(token, '\0', 1000);
        memset(word, '\0', 1000);
        free(token);
        free(word);
    }
    printSourceCode(argv[1]);
    printf("\n");

    // printf("\n%d",isStatementReserved("read"));

    // printf("\n%s\n\n",global_Lexeme);

    PROGRAM();
    freeAll();

    free(global_Lexeme);
    free(codePL);
    return 0;
}