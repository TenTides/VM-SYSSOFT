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
        //LIT, Literal push
        case 1:
          if(L == 0)
          {
            SP = SP-1;
            pas[SP] = M;
          }
          break;
        case 2: 
          //not done yet, the ariethmetic needs to be implemented

          //RTN Returns from a subroutine is encoded 0 0 0 and restores the caller’s AR
          if(L == 0)
          {
            SP = BP+1;
            BP = pas[SP+2];
            PC = pas[SP+3];
            pas[SP] = M;
          }
        // LOD load value to top of the stack from location at offset M from L
        // lexicographical levels down of the stack
        case 3:
          SP = SP-1;
          pas[SP] = pas[base(BP,L)-M];
          break;
        // STO Store value at top of the stack in the ocation at offset M from L
        // lexicographical levels down of the stack
        case 4:
          pas[SP] = pas[base(BP,L)-M];
          SP = SP+1;
          break;
        // CAL Call the procedure at code index p, 
        // generating a new activation record and 
        // setting PC to M
        case 5:
          pas[SP - 1]  =  base(BP, L); 
          pas[SP - 2]  = BP; 
          pas[SP - 3]  = PC;  
          BP = SP - 1;
          PC = M;
          break;
        // INC allocate M locals on the stack 
        case 6:
          SP = SP-M;
          break;
        // JMP jump to the address in stack and pop
        case 7:
          PC = pas[SP-1];
          SP = SP-1;
          break;
        // JPC jump conditionally if index SP is zero
        // jump to index m and pop
        case 8:
          if (pas[SP] == 0)
          { 
            PC = M; 
          } 
          SP = SP+1;
          break;
        // GENERAL SYSTEM [SYS]
        case 9:
          // SOU Output of the value in stack[SP] to standard 
          // output as a character and pop:
          if(L == 1)
          {
            putc(pas[SP]); // I don't understand this
            SP = SP+1;
          }
          // SIN Read in input from the user and store it on top of the stack
          else if(L == 2)
          {
            SP = SP-1;
            pas[SP] = getc(); // or this, they supposedly need to 
                              // connect with a file pointer
          }
          // EOP End of program (Set “eop” flag to zero)
          if(L == 3)
          {
            //"eop" flag ?? how do this, exit() is not allowed
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