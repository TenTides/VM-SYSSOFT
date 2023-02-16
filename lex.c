// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 2/15/2023
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define  CMAX     11       // maximum number of chars for idents 
#define  STRMAX   125       // Assuming max codeline length is 50
      
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

char* resWords[] = {"null", "odd", "begin", "end", "if", "then", "while", "do", "call", "const",  "var", "procedure",  "write", "read", "else"}; 

int resWordsTokens[] = {nulsym, oddsym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,  varsym, procsym,  writesym , readsym , elsesym};   
//Symbol table is in order of ascii value, can be used with binary search
char  symbolTableOrdered[] = {'\t','\r',' ','(',')','*', '+',',', '-' ,'.', '/', '0', '1', '2','3', '4', '5', '6', '7', '8', '9', ':', ';', '<',
                        '=', '>','a','b','c','d','e','f','g','h','i','j','k','l' ,'m' ,'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 
                        'v', 'w', 'x', 'y', 'z'};   



//Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
char specialTerminalSymbolsOrdered[] = {'\t','\r',' ', '(',')','*', '+',',', '-' ,'.', '/', ':', ';','<','=','>'}; // ' ' isn't a term sym, it was put here

int specialTerminalSymbolsTokens[] = {-3,-2,-1, lparentsym, rparentsym, multsym, plussym, commasym, minussym ,periodsym, slashsym, 0, semicolonsym ,lessym, eqsym, gtrsym}; // -1 is for spaces and 0 is for colons and -2 for tabs, 

// //=======new=======//
// //Symbols which are essentially breakpoints and line enders // is not in here, a lookahead check is necessary for that one
// char specialTerminalSymbolsOrdered[] = {'\t',' ', ':', '+', '-', '*', '/', '=', '<', '>','(',')', ',',';', '.'}; // ' ' isn't a term sym, it was put here
// int specialTerminalSymbolsTokens[] = {-2,-1, 0, plussym, minussym , multsym, slashsym,  eqsym, lessym, gtrsym, lparentsym, rparentsym,  commasym, semicolonsym, periodsym }; // -1 is for spaces and 0 is for colons and -2 for tabs,
                                                                                                                                                                                // there is no colonsym, so I assume it can only be within becomesym
//halt flag global is used in main                                                                                                                                       
int halt_flag = 1; 
int EndProgramFlag = 1;   

//Intent is to iterate character by character from a given
//line of code to produce the token version of that code 
//and thereafter return that string

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
    if(strlen(word) > CMAX && retval == 1)
    {
        retval = -1;
    }
    return retval;
}
char* subString(int start, int end,char* line)
{   
    if((line[start] != ' ') && (line[start] != '\t') && characterInSymbolTableBS(line[start], specialTerminalSymbolsOrdered) != -1 
        && characterInSymbolTableBS(line[start], symbolTableOrdered) != -1) return NULL;
    char* word = malloc(sizeof(char)*(end-start+1));
    word[0] = '\0';
    for(int i = start; i<end;i++)
    {
        if((line[i] != ' ') && (line[i] != '\t') && characterInSymbolTableBS(line[i], symbolTableOrdered) != -1)
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


void printSourceCode(char* filename) {

    // Initializing variables
    char buffer[STRMAX];
    int length;
    char tyler;

    // =========================================
    //Initalize input file for viewing
    int lines = numberOfFileLines(filename);
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file. \n");
        return;
    }

    // =========================================
    // =========================================
    //Initalize main code array
    char* codePL = (char*) malloc(sizeof(char)* (STRMAX*lines));
    codePL[0] = '\0'; // Must be set to the first index to allow for smooth cats

    printf("Source Program:\n");
    //This while loop doesn't ommit comments 
    //int errorFlag = 1;
    while(fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        fscanf(fp, "%c", &tyler);
        
        length = strlen(buffer);
        

        char* line = (char*) malloc(sizeof(char) * (length+ 1));
        line[0] = '\0';
        strcpy(line, buffer);
        printf("%s\n", line);
        free(line);
    }  

    return; 
}


int main(int argc, char *argv[])
{
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

    char* EditedcodePL = (char*) malloc(sizeof(char)* (STRMAX*lines));
    EditedcodePL[0] = '\0'; // Must be set to the first index to allow for smooth cats


    printSourceCode(argv[1]);
    printf("\n");


    //This while loop doesn't ommit comments 
    //int errorFlag = 1;
    while(fscanf(fp, "%[^\n]s", buffer) != EOF)
    {
        fscanf(fp, "%c", &tyler);
        length = strlen(buffer);

        char* line = (char*) malloc(sizeof(char) * (length+ 1));
        line[0] = '\0';
        strcpy(line, buffer);

        line = lexicalParse(line); // lex parse

        if(line != NULL) // && errorDetect == 0
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

    if (errorFlag != 0) {
        printf("\nLexeme      Token Type\n"); 

        length = strlen(codePL);
        char* token = (char*) malloc(sizeof(char) * 1000);
        token[0] = '\0';
        char* word = (char*) malloc(sizeof(char) * 1000);
        word[0] = '\0';
        char specialCharacter;
        int index = 0;
        int tokenToInt;
        char stopChar = ' ';

        if (EndProgramFlag) {
            for (int i = 0; i < length; i++) {
                if (codePL[i] == ' ')
                {
                    continue;
                }

                strncat(token, codePL + i, 1);
                // printf("we've entered the for loop when i is %d and TOKEN is %s and WORD is %s\n", i, token, word);

                if (codePL[i+1] != ' ') {
                    continue;
                }

                if (strcmp(token, "20") == 0) {
                    // printf("we've enterd the token, := if statement\n");
                    printf("%-9s%5s\n", ":=",token);
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "10") == 0) {
                    // printf("we've enterd the token, <> if statement\n");
                    printf("%-9s%5s\n", "<>",token);
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "12") == 0) {
                    // printf("we've enterd the token, <= if statement\n");
                    printf("%-9s%5s\n", "<=",token);
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "14") == 0) {
                    // printf("we've enterd the token, >= if statement\n");
                    printf("%-9s%5s\n", ">=",token);
                    memset(token, '\0', 1000);
                    continue;
                }
                if (strcmp(token, "-5") == 0) {
                    // printf("we've enterd the token, >= if statement\n");
                    printf("%-9s%5s\n", "/*", "Unresolved In Line Comment Error");
                    memset(token, '\0', 1000);
                    continue;
                }

                if (strcmp(token, "3") == 0) {
                    // printf("we've enterd the token, 3 if statement\n");
                    char* pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2);// 1
                    // printf("num_chars is equal to %d\n", num_chars);
                    strncpy(word, codePL + i + 2, num_chars);
                    int valid  = isWordValid(word);
                    switch(valid) 
                    {
                        case -3:
                            printf("%-9s%5s\n", word, " Invalid Number, exceeds maxDigits of 5");
                            break;
                        case 2:
                            printf("%-9s%5s\n", word, token);
                            break;
                    }
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    i += num_chars + 1;
                    continue;
                }
            

                if (strcmp(token, "2") == 0) {
                    // printf("we've enterd the token, 2 if statement\n");
                    char* pos = strchr(codePL + i + 2, ' ');
                    int num_chars = pos - (codePL + i + 2);// 1
                    // printf("num_chars is equal to %d\n", num_chars);
                    strncpy(word, codePL + i + 2, num_chars);
                    if(strlen(word) == 1 && characterInSymbolTableBS(word[0],symbolTableOrdered) == -1)
                    {
                        printf("%-9s%5s\n", word, " (Invalid Symbol)");
                    }
                    else
                    {
                        int valid  = isWordValid(word);
                        switch(valid) 
                        {
                            case -1:
                                printf("%-9s%5s\n", word, " Invalid Identifier, exceeds maxLength of 11");
                                break;
                            case -2:
                                printf("%-9s%5s\n", word, " Invalid Identifier, starts with an Integer");
                                break;
                            case 1:
                                printf("%-9s%5s\n", word, token);
                                break; 
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
                    // printf("we've enterd the SPECIALTERMINAL IF if statement\n");
                    specialCharacter = specialTerminalSymbolsOrdered[index];
                    printf("%c%13s\n", specialCharacter, token);
                    memset(token, '\0', 1000);
                    continue;
                }
                // printf("PASSING\n");
                index = binarySearch(resWordsTokens, 0, 15, tokenToInt);
                if(index != -1) {
                    // printf("we've enterd the RESWORD if statement\n");
                    strcat(word, resWords[index]);
                    printf("%-9s%5s\n", word, token);
                    memset(token, '\0', 1000);
                    memset(word, '\0', 1000);
                    continue;
                }
                // printf("PASSING\n");
                if(strcmp(token, "13") == 0)
                {
                    memset(token, '\0', 1000);
                    continue;
                }
                printf("Token %s NOT FOUND", token);
            }
        }
        //printf("\n\nLexeme List:\n%s\n\n",codePL);
        printf("\n\n");

        memset(token, '\0', 1000);
        memset(word, '\0', 1000);
        // for(int i = 0; i<strlen(codePL);i++)
        // {   
        //     if((codePL[i] == '2' || codePL[i] == '3') && (i != 0 && codePL[i-1] == ' ')  && (i == strlen(codePL)-1 codePL[i+1] == ' ')  )
        //     {
        //         int start = i;
        //         char* tempWord = malloc(sizeof(char)*(1000));
        //         tempWord[0] = '\0';
        //         for(int x = i+2; x<strlen(codePL);x++)
        //         {   


        for(int i = 0; i<strlen(codePL);i++)
        {   
            // continue if we see a space
            if (codePL[i] == ' ')
            {
                continue;
            }

            strncat(token, codePL + i, 1);

            // continue if we have multiple numbers 
            if (codePL[i+1] != ' ') {
                continue;
            }

            // done
            if (strcmp(token, "3") == 0) {
                char buffer;
                int value;
                // printf("we've enterd the token, 2 if statement\n");
                char* pos = strchr(codePL + i + 2, ' ');
                int num_chars = pos - (codePL + i + 2);// 1
                // printf("num_chars is equal to %d\n", num_chars);
                strncpy(word, codePL + i + 2, num_chars);

                value = isWordValid(word);
                if (value >= 0){
                    strcat(EditedcodePL, token);
                    strcat(EditedcodePL, " ");
                    strcat(EditedcodePL, word);
                    strcat(EditedcodePL, " ");
                }
                
                memset(token, '\0', 1000);
                memset(word, '\0', 1000);
                i += num_chars + 1;
                continue;
            }

            // done
            if (strcmp(token, "2") == 0) {
                char buffer;
                int value;
                // printf("we've enterd the token, 2 if statement\n");
                char* pos = strchr(codePL + i + 2, ' ');
                int num_chars = pos - (codePL + i + 2);// 1
                // printf("num_chars is equal to %d\n", num_chars);
                strncpy(word, codePL + i + 2, num_chars);

                if (num_chars == 1){
                    buffer = word[0];
                    value = characterInSymbolTableBS(buffer, symbolTableOrdered);
                    if (value >= 0){
                        strcat(EditedcodePL, token);
                        strcat(EditedcodePL, " ");
                        strcat(EditedcodePL, word);
                        strcat(EditedcodePL, " ");
                    }
                }
                
                if (num_chars > 1) {
                    value = isWordValid(word);
                    if (value >= 0){
                        strcat(EditedcodePL, token);
                        strcat(EditedcodePL, " ");
                        strcat(EditedcodePL, word);
                        strcat(EditedcodePL, " ");
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
                // printf("we've enterd the SPECIALTERMINAL IF if statement\n");
                specialCharacter = specialTerminalSymbolsOrdered[index];
                strcat(EditedcodePL, token);
                strcat(EditedcodePL, " ");
                memset(token, '\0', 1000);
                continue;
            }

            // printf("PASSING\n");
            index = binarySearch(resWordsTokens, 0, 15, tokenToInt);
            if(index != -1) {
                // printf("we've enterd the RESWORD if statement\n");
                strcat(word, resWords[index]);
                strcat(EditedcodePL, token);
                strcat(EditedcodePL, " ");
                memset(token, '\0', 1000);
                memset(word, '\0', 1000);
                continue;
            }
            // printf("PASSING\n");
            if(strcmp(token, "13") == 0)
            {
                memset(token, '\0', 1000);
                continue;
            }

            // printf("Token %s NOT FOUND in Lexeme editor", token);

            memset(token, '\0', 1000);
            memset(word, '\0', 1000);
            continue;
        }

        printf("Lexeme List:\n%s\n\n",EditedcodePL);
        printf("\n\n");
        free(token);
        free(word);
        free(codePL);
    }   

// =========================================
// =========================================
}