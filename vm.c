#include <stdio.h>
#include <stdlib.h> 
#define ARRAY_SIZE 500

//initalize base
int base( int BP, int L);
//initalize pas array of ARRAY_SIZE
int pas[ARRAY_SIZE];
int IR[3]; //IR  = 0 0 0
 
int base( int BP, int L)
{
  int arb = BP; // arb = activation record base
  while (L > 0)     //find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}
int main()
{
  
    int BP = 499; 
    int SP = BP + 1; 
    int PC = 0;
    int OP, M, L;
    
    while (scanf("%d %d %d",&OP,&M, &L) !=EOF) // stops scanning when no inputs are detected
    {
      //use nested swtich statemnts
      switch(OP) {
        case 1:
          if(L == 0)
          {
            SP = SP-1;
            pas[SP] = M;
          }
          break;
        case 2:
          //not done yet, the ariethmetic needs to be implemented
          if(L == 0)
          {
            SP = BP+1;
            BP = pas[SP+2];
            PC = pas[SP+3];
            pas[SP] = M;
          }
        case 3:
          SP = SP-1;
          pas[SP] = pas[base(BP,L)-M];
          break;
        case 4:
          pas[SP] = pas[base(BP,L)-M];
          SP = SP+1;
          break;
        case 5:
          pas[SP - 1]  =  base(BP, n); 
          pas[SP - 2]  = BP; 
          pas[SP - 3]  = PC;  
          BP = SP - 1;
          PC = M;
          break;
        case 6:
          SP = SP-M;
          break;
        case 7:
          PC = pas[SP-1];
          SP = SP-1;
          break;
        case 8:
          if (pas[SP] == 0)
          { 
            PC = M; 
          } 
          SP = SP+1;
          break;
        case 9:
          if(L == 1)
          {
            putc(pas[SP]); // I don't understand this
            SP = SP+1;
          }
          else if(L == 2)
          {
            SP = SP-1;
            pas[SP] = getc(); // or this, they supposedly need to 
                              // connect with a file pointer
          }
          if(L == 3)
          {
            
          }
          break;
      }
      //fetch cycle, but I don't understand why an instruction register is even needed
      IR[0] = pas[PC]; 
      IR[1] = pas[PC + 1];  
      IR[2] = pas[PC + 2];
      PC = PC +3;
    }
    
}