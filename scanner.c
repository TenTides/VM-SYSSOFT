#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  IMAX   32767       // maximum integer value 
#define  CMAX      11       // maximum number of chars for idents 
#define  STRMAX   50    

char* word[] = {"null", "begin", "call", "const", "do", "else", "end", "if",``
                 "odd", "procedure", "read", "then", "var", "while", "write"}; 
//Symbol table isn't in order of ascii, cannot be used with a hash search method yet.
char  symbolTable[] = {'a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
                        'w', 'x', 'y', 'z', '0', '1', '2','3', '4', '5', '6', '7', '8', '9',' ', '+', '-', '*', '/', '<', 
                        '=', '>', ':','.' , ',' , ';' };   
//Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
char specialTerminalSymbols[] = {'+', '-', '*', '/', '<', '=', '>', ':','.' , ',' , ';'};    
                            
              
char* lexicalParse(char* codeLine);
              
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

    return codeLine; //temp return statement
}
int main(int argc, char *argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");
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
    char* codePL = malloc(sizeof(char)*STRMAX*numLines);
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    fp = fopen(argv[1], "r");
    while(1)
    {
        char* line = malloc(sizeof(char)*STRMAX);
        if(fscanf(fp, "%[^\n]", line) == EOF)
        {
            break;
        }
        line = realloc(line,sizeof(char)*strlen(line));
        line = lexicalParse(line);
        if(strlen(line) >= 0)// Because a line, or a part of it, can be a comment
        { 
            strcat(codePL,line);
        } 
        else
        {
            break; // an error was encountered prematurely
        }
        free(line);
    }   
}