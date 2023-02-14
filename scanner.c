// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/213/2023
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   50       // Assuming max codeline length is 50
      
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
char specialTerminalSymbolsOrdered[] = {'\t',' ', '(',')','*', '+',',', '-' ,'.', '/', ':', ';','<','=','>'}; // ' ' isn't a term sym, it was put here
int specialTerminalSymbolsTokens[] = {-2,-1, lparentsym, rparentsym, multsym, plussym, commasym, minussym ,periodsym, slashsym, 0, semicolonsym ,lessym, eqsym, gtrsym}; // -1 is for spaces and 0 is for colons and -2 for tabs, 
//halt flag global is used in main                                                                                                                                       // there is no colonsym, so I assume it can only be within becomesym
int halt_flag = 1; 
int EndProgramFlag = 1;   

//Intent is to iterate character by character from a given
//line of code to produce the token version of that code 
//and thereafter return that string

// Example run for a given line
// var x,y;
// start = 0   
// sub (0,3) var i+1
// i = 2    start = 3
// i = 3    space so no token start = 4
// i = 4    sub(4,i+1(5)) x start = 5
// i = 5    sub(5, i+1(6)) fail ret null , 17 start = 6 
// i = 6    sub(6,7) y start = 7
// i = 7    sub(7,8) fail ret null , 18 
        

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
        if(characterInSymbolTableBS(codeLine[i], symbolTableOrdered) != -1)
        {
            //Check if the next character from current index is a special symbol
            int lookAhead = characterInSymbolTableBS(codeLine[i+1], specialTerminalSymbolsOrdered);
            //Check if the current character from current index is a special symbol
            int specialIndex = characterInSymbolTableBS(codeLine[i], specialTerminalSymbolsOrdered);
            // reserved index set to -1, used only if a reserved word is detected in deeper code
            // where it would take up the index of a reserved word in resWords that connects to 
            // resWordsTokens in a 1 to 1 relationship.
            int reservedIndex = -1; 
            // If look ahead is a special character or i is on the last iteration
            // try to make a word from indexes start to i+1 exclusively (i+1 is where the 
            // special char is)                                                             
            if(lookAhead != -1 || i == strlen(codeLine)-1 || (lookAhead == -1 && specialIndex != -1))
            {
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
                    //printf("WORD IS valid?: %d\n",valid);
                    switch(valid) 
                    {
                        case -1:
                            printf("%s is an invalid Identifier, exceeds maxLength of 11\n", word);
                            free(parsedString);
                            return NULL;
                            break;
                        case -2:
                            printf("%s is an invalid Identifier, starts with an Integer\n", word);
                            free(parsedString);
                            return NULL;
                            break;
                        case -3:
                            printf("%s is an invalid integer, exceeds the maximum number of digits of 5\n", word);
                            free(parsedString);
                            return NULL;
                            break;
                        case 1:
                            reservedIndex = isStatementReserved(word);
                            if(reservedIndex != -1)
                            {
                                token = resWordsTokens[reservedIndex];
                            }
                            break;
                        case 2:
                            token = numbersym;
                            break;
                    }
                    // Start is set to where the special lookahead character was detected at.
                    // It will be there were the parser will continue to try to make words. It will
                    // fail to produce a word at i +1 always as it is a special char, but by failing to do so
                    // that special char is tokenized and not considered for words to come.
                    start = i + 1;       
               }
               else
               {    
                    // Take the index of the special character in the codeline (that failed to create a word) and 
                    // find what special char token it corresponds to
                    specialIndex = characterInSymbolTableBS(codeLine[start], specialTerminalSymbolsOrdered);
                    token =  specialTerminalSymbolsTokens[specialIndex];
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
                                token = becomessym;
                                start = start + 1;
                                i++;
                            } 
                            break;
                        case lessym:
                            if(codeLine[start+1] == '=')
                            {
                                token = leqsym;
                                start = start + 1;
                                i++;
                            } 
                            else if(codeLine[start+1] == '>')
                            {
                                token = neqsym;
                                start = start + 1;
                                i++;
                            } 
                            break;
                        case gtrsym:
                            if(codeLine[start+1] == '=') 
                            {   
                                token = geqsym;
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
                                    printf("Code line: '%s' has an unresolved comment, not ended with '*/' \n", codeLine);
                                    free(parsedString);
                                    return NULL;
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
               if(token > 0) 
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
        else
        {
            //Invalid symbol, return error null
            printf(" %c is an invalid Symbol, for the line: %s\n", codeLine[i], codeLine);
            free(parsedString);
            return NULL;  
        }
    }
    return parsedString; 
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
    if(strlen(word) > CMAX)
    {
        retval = -1;
    }
    return retval;
}
char* subString(int start, int end,char* line)
{   
    if((line[start] != ' ') && (line[start] != '\t') && characterInSymbolTableBS(line[start], specialTerminalSymbolsOrdered) != -1) return NULL;
    char* word = malloc(sizeof(char)*(end-start+1));
    word[0] = '\0';
    for(int i = start; i<end;i++)
    {
        if((line[i] != ' ') && (line[i] != '\t'))
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
    // int numLines = numberOfFileLines(argv[1]); 
    char buffer[STRMAX];
    int length;
    char tyler;

    //Initalize input file for viewing
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Error opening file. \n");
        return 1;
    }

    //Initalize main code array
    char* codePL = (char*) malloc(sizeof(char)* (STRMAX*6));
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    //This while loop doesn't ommit comments 
    while(fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        fscanf(fp, "%c", &tyler);
        printf("we entered the loop\n");
        length = strlen(buffer);
        printf("buffer-> %s\n", buffer);

        if(EndProgramFlag == 0)
        {
            break;
        }

        char* line = (char*) malloc(sizeof(char) * (length+ 1));
        line[0] = '\0';
        strcpy(line, buffer);
        printf("line-> %s\n", line);
        line = lexicalParse(line); // lex parse

        if(line != NULL)
        { 
            strcat(codePL,line);
        } 

        free(line);
    }   

    printf("%s\n",codePL);

    // //TEST LEX PARSE FRAMEWORK
    // printf(" ' ' %d ",characterInSymbolTableBS(' ', specialTerminalSymbolsOrdered));
    // printf(" > %d ",characterInSymbolTableBS('>', specialTerminalSymbolsOrdered));
    // printf(" , %d\n",characterInSymbolTableBS(',', specialTerminalSymbolsOrdered));
    // printf(" Tab %d\n",characterInSymbolTableBS('\t', specialTerminalSymbolsOrdered));



    // char* line = malloc(sizeof(char)*STRMAX);
    // line[0]= '\0';  //VERY IMPORTANT it can create junk if not careful
    // printf("Enter a line of text: ");
    // scanf("%[^\n]%*c", line);
    // line = realloc(line,sizeof(char)*strlen(line));
    // printf("You entered: %s\n", line);
    // char* line2 = lexicalParse(line);
    // printf("You parsed: %s\n", line2);
    // free(line2);
    // free(line);
}