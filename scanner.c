#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  IMAX   99999       // maximum integer value 
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   50    
      
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

char* resWords[] = {"null", "begin", "call", "const", "do", "else", "end", "if",
                 "odd", "procedure", "read", "then", "var", "while", "write"}; 
int resWordsTokens[] = {nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym,
                        oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};           

//Symbol table is in order of ascii value, can be used with binary search
char  symbolTableOrdered[] = {'\t',' ','(',')','*', '+',',', '-' ,'.', '/', '0', '1', '2','3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
                        '=', '>','a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 
                        'v', 'w', 'x', 'y', 'z'};   

//Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
//char specialTerminalSymbols[] = {'+', '-', '*', '/', '<', '=', '>', ':','.' , ',' , ';'};    
char specialTerminalSymbolsOrdered[] = {'\t',' ', '(',')','*', '+',',', '-' ,'.', '/', ':', ';','<','=','>'}; // ' ' isn't a term sym, it was put here
int specialTerminalSymbolsTokens[] = {-2,-1, lparentsym, rparentsym, multsym, plussym, commasym, minussym ,periodsym, slashsym, 0, semicolonsym ,lessym, eqsym, gtrsym}; // -1 is for spaces and 0 is for colons and -2 for tabs, 

//halt flag global is used in main                                                                                                                                                                         // there is no colonsym, so I assume it can only be within becomesym
int halt_flag = 1;    

//Intent is to iterate character by character from a given
//line of code to produce the token version of that code 
//and thereafter return that string
char* lexicalParse(char* codeLine)
{
    char* parsedString = malloc(sizeof(codeLine));
    parsedString[0] = '\0';
    int start = 0;
    for(int i = 0; i<strlen(codeLine);i++)
    {
        //printf("%c ",codeLine[i]);
        //int key = characterInSymbolTableBS(codeLine[i], symbolTableOrdered);
        //printf("%d\n",key);
        if(characterInSymbolTableBS(codeLine[i], symbolTableOrdered) != -1)
        {
            int lookAhead = characterInSymbolTableBS(codeLine[i+1], specialTerminalSymbolsOrdered);
            //printf("Look ahead '%c' %d\n",codeLine[i],lookAhead);
            int reservedIndex = -1; 
            if(lookAhead != -1 || i == strlen(codeLine)-1)
            {
               char* word = subString(start, i+1,codeLine);
               //printf("WORD: %s\n",word);
               int token = identsym;
               if(word != NULL)
               {
                    int valid  = isWordValid(word);
                    printf("WORD IS valid?: %d\n",valid);
                    switch(valid) 
                    {
                        case -1:
                            printf("%s is an invalid Identifier, exceeds maxLength of 11\n", word);
                            return NULL;
                            break;
                        case -2:
                            printf("%s is an invalid Identifier, starts with an Integer\n", word);
                            return NULL;
                            break;
                        case -3:
                            printf("%s is an invalid integer, exceeds the maximum number of digits of 5\n", word);
                            return NULL;
                            break;
                        case 1:
                            reservedIndex = isStatementReserved(word);
                            if(reservedIndex != -1)
                            {
                                //printf("RES WORD: %s\n",word);
                                token = resWordsTokens[reservedIndex];
                            }
                            break;
                        case 2:
                            token = numbersym;
                            break;
                    }
                    start = i + 1;       
               }
               else
               {
                    int specialIndex = characterInSymbolTableBS(codeLine[start], specialTerminalSymbolsOrdered);
                    //printf("Special Symbol When word null %d\n",specialIndex);
                    token =  specialTerminalSymbolsTokens[specialIndex];
                    //Switch onwards could become its own method
                    switch(token)
                    {
                        case 0:
                            if(codeLine[i+1] == '=')
                            {
                                token = becomessym;
                                start = start + 1;
                            } 
                            break;
                        case lessym:
                            if(codeLine[i+1] == '=') token = leqsym;
                            break;
                        case gtrsym:
                            if(codeLine[i+1] == '=') token = geqsym;
                            break;
                        case slashsym: //checking for comments
                            if(codeLine[i+1] == '*')
                            {
                                token = -1;
                                int commentError = 1;
                                if((strlen(codeLine) - (i+1))>=2)
                                {
                                    for(int x = i+2; x<strlen(codeLine)-1;x++)
                                    {
                                        char twoChars[] = {codeLine[x],codeLine[x+1]}; // probably needs to change to checking only last two chars, 
                                        if(twoChars[0] == '*' && twoChars[1]== '/')    // instead of checking whole thing
                                        {
                                            commentError = 0;
                                        }
                                    }
                                }
                                if(commentError)
                                {
                                    printf("%s has an unresolved comment, not ended with '*/' \n", codeLine);
                                    return NULL;
                                }
                            }
                            break;
                    }
                    start = start + 1;                    
               }
               if(token > -1)
               {
                    char int_string[4]; 
                    sprintf(int_string, "%d ",token);
                    //printf("INT STRING: %s\n",int_string);
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
    }
    printf("PARSE STRING: %s\n",parsedString);
    return parsedString; //temp return statement
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
    //printf("%d\n", (end-start));
    if((line[start] != ' ') && (line[start] != '\t') && characterInSymbolTableBS(line[start], specialTerminalSymbolsOrdered) != -1) return NULL;
    //printf("PASS 1\n");
    char* word = malloc(sizeof(char)*(end-start+1));
    //printf("PASS 2\n");
    word[0] = '\0';
    for(int i = start; i<end;i++)
    {
        if(line[i] != ' ' && (line[i] != '\t'))
        {
            word[strlen(word)] = line[i];
            word[strlen(word) + 1] = '\0';
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

int main(int argc, char *argv[])
{
    //int numLines = numberOfFileLines(argv[1]);
    //Initalize input file for viewing
    // FILE *fp;
    // fp = fopen(argv[1], "r");
    // //Initalize main code array
    // char* codePL = malloc(sizeof(char)*STRMAX*numLines);
    // codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    // //This while loop doesn't ommit comments 
    // while(halt_flag)
    // {
    //     char* line = malloc(sizeof(char)*STRMAX);
    //     if(fscanf(fp, "%[^\n]s", line) == EOF)
    //     {
    //         halt_flag = 0;
    //         break;
    //     }
    //     line = realloc(line,sizeof(char)*strlen(line));
    //     line = lexicalParse(line); // lex parse
    //     if(strlen(line) >= 0)//
    //     { 
    //         strcat(codePL,line);
    //     } 
    //     free(line);
    // }   
    //TEST LEX PARSE FRAMEWORK
    printf(" ' ' %d ",characterInSymbolTableBS(' ', specialTerminalSymbolsOrdered));
    printf(" > %d ",characterInSymbolTableBS('>', specialTerminalSymbolsOrdered));
    printf(" , %d\n",characterInSymbolTableBS(',', specialTerminalSymbolsOrdered));
    printf(" Tab %d\n",characterInSymbolTableBS('\t', specialTerminalSymbolsOrdered));
    char* line = malloc(sizeof(char)*STRMAX);
    printf("Enter a line of text: ");
    scanf("%[^\n]%*c", line);
    line = realloc(line,sizeof(char)*strlen(line));
    printf("You entered: %s\n", line);
    char* newline = lexicalParse(line);
    newline = realloc(line,sizeof(char)*strlen(newline));
    printf("You parsed: %s\n", newline);
    free(newline);
}