#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  IMAX   99999       // maximum integer value 
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   50    

char* resWords[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
                 "odd", "procedure", "read", "then", "var", "while", "write"}; 
                 
int reservedWordSumTable[] = {309, 331, 377, 393, 414, 446, 451, 482, 492, 466, 534, 554, 607, 671, 851};

// null: 466
// begin: 607
// call: 482
// const: 534
// do: 309
// else: 492
// end: 414
// if: 331
// odd: 377
// procedure: 851
// read: 451
// then: 446
// var: 393
// while: 671
// write: 554

//Symbol table isn't in order of ascii, cannot be used with binary search
// char  symbolTable[] = {'a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
//                         'w', 'x', 'y', 'z', '0', '1', '2','3', '4', '5', '6', '7', '8', '9',' ', '+', '-', '*', '/', '<', 
//                         '=', '>', ':','.' , ',' , ';' }; 

char  symbolTableOrdered[] = {' ','*', '+',',', '-' ,'.', '/', '0', '1', '2','3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
                        '=', '>','a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 
                        'v', 'w', 'x', 'y', 'z'};   

//Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
//char specialTerminalSymbols[] = {'+', '-', '*', '/', '<', '=', '>', ':','.' , ',' , ';'};    
char specialTerminalSymbolsOrdered[] = {' ','*', '+',',', '-' ,'.', '/', ':', ';','<','=', '>'}; // ' ' isn't a term sym, it was put here
int halt_flag = 1;                               
              
char* lexicalParse(char* codeLine);
int numberOfFileLines(char* filename);
int characterInSymbolTableBS(char c, char* symTbl);
int isStatementReserved(char* word);            
char* subString(int start, int end,char* line);  
int isWordValid(char* word);

typedef enum {  
    nulsym = 1, //was skipsys, but skip isn't in PL/0, this must be null
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
    callsym = 27, 
    constsym = 28, 
    varsym = 29, 
    procsym = 30, 
    writesym = 31, 
    readsym = 32, 
    elsesym = 33  
}token_type; 
//Intent is to iterate character by character from a given
//line of code to produce the token version of that code 
//and thereafter return that string
char* lexicalParse(char* codeLine)
{
    char* parsedString = malloc(sizeof(codeLine));
    parsedString[0] = '\0';
    int start = 0;
    for(int i = 0; i<strlen(codeLine)-1;i++)
    {
        if(characterInSymbolTableBS(codeLine[i], symbolTableOrdered) != -1)
        {
            int lookAhead = characterInSymbolTableBS(codeLine[i+1], specialTerminalSymbolsOrdered);
            if(lookAhead != -1 )
            {
               char* word = subString(start, i+1,codeLine);
               if(word != NULL)
               {
                    int valid  = isWordValid(word);
                    int reservedInt = -1; 
                    int token = identsym;
                    switch(valid) 
                    {
                        case -1:
                            printf("%s is an invalid Identifier, exceeds maxLength of 11", word);
                            break;
                        case -2:
                            printf("%s is an invalid Identifier, starts with an Integer", word);
                            break;
                        case -3:
                            printf("%s is an invalid integer, exceeds the maximum number of digits of 5", word);
                            break;
                        case 1:
                            reservedInt = isStatementReserved(word);
                            if(reservedInt != -1)
                            {
                                // determine which token to add into the parsed string
                            }
                            break;
                        case 2:
                            token = numbersym;
                            break;
                    }       
               }
               //if null add the special character to the parsed string
               //this is where the check for comments would be
            }
        }
    }
    return codeLine; //temp return statement
}
int isStatementReserved(char* word)
{
    int retval = -1;
    for(int x = 0; x < 15; x++)
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
            if(word[x] < '0' && word[x] > '9')
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
    //If the word is longer than 11 characters
    if(strlen(word) > 11)
    {
        retval = -1;
    }
    return retval;
}

char* subString(int start, int end,char* line)
{
    if(characterInSymbolTableBS(line[0], specialTerminalSymbolsOrdered)) return NULL;
    char* word = malloc(sizeof(char)*(end-start));
    word[0] = '\0';
    for(int i = start; i<end;i++)
    {
        word[strlen(word)] = line[i];
        word[strlen(word) + 1] = '\0';
    }
    return word;
}

int numberOfFileLines(char* filename)
{
    FILE *fp = fopen(filename, "r");
    int numLines = 0;
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

int main(int argc, char *argv[])
{
    int numLines = numberOfFileLines(argv[1]);
    //Initalize input file for viewing
    FILE *fp;
    fp = fopen(argv[1], "r");
    //Initalize main code array
    char* codePL = malloc(sizeof(char)*STRMAX*numLines);
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    //This while loop doesn't ommit comments 
    while(halt_flag)
    {
        char* line = malloc(sizeof(char)*STRMAX);
        if(fscanf(fp, "%[^\n]s", line) == EOF)
        {
            halt_flag = 0;
            break;
        }
        line = realloc(line,sizeof(char)*strlen(line));
        line = lexicalParse(line); // lex parse
        if(strlen(line) >= 0)//
        { 
            strcat(codePL,line);
        } 
        free(line);
    }   
}