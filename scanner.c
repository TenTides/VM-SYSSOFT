#include <stdio.h>
#include <stdlib.h> 
#include <limits.h>
#include <string.h>
typedef enum {  
    skipsym = 1, 
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
    char* codePL = malloc(sizeof(char)*LINE_MAX*numLines);
    codePL[0] = '\0';
    fp = fopen(argv[1], "r");
    while(1)
    {
        char* line = malloc(sizeof(char)*LINE_MAX);
        if(fscanf(fp, "%[^\n]", line) == EOF)
        {
            break;
        }
        line = realloc(line,sizeof(char)*strlen(line));
        strcat(codePL,line);
        free(line);
    }
     
}