#include <stdio.h>
#include <stdlib.h> 
#define ARRAY_SIZE 500

//initalize base
int base( int BP, int L);
//initalize pas array of ARRAY_SIZE
int pas[ARRAY_SIZE];
//instruction register
int IR[3]; //IR  = 0 0 0

//pointer and halt initalization
int BP = 499; 
int SP = 500; 
int PC = 0;
int OP, M, L;
int halt = 1;

int main()
{
    while(1)
    {
      scanf("%d %d %d",&OP,&M, &L);
      pas[PC+1] = OP;
      pas[PC+2] = M;
      pas[PC+3] = L;
      PC= PC +3;
      if(OP == 9 && L ==3) break;
    }
    PC = 0;

    while (halt) // stops scanning when no inputs are detected
    {
      IR[0] = pas[PC]; 
      IR[1] = pas[PC + 1];  
      IR[2] = pas[PC + 2];
      //use nested swtich statemnts
      switch(IR[0]) {
        //LIT, Literal push
        case 1:
          if(L == 0)
          {
            SP = SP-1;
            pas[SP] = IR[1];
          }
          break;
        case 2: 
          //not done yet, the ariethmetic needs to be implemented
          switch(M) {
            //RTN Returns from a subroutine is encoded 0 0 0 and restores the caller’s AR
            case 0:
              SP = BP+1;
              BP = pas[SP-2];
              PC = pas[SP-3];
              pas[SP] = IR[1];
              break;
            //ADD addition
            case 1:
              pas[SP+1]= pas[SP+1] + pas[SP];
              SP = SP+1;
              break;
            //SUB subtraction
            case 2:
              pas[SP+1]= pas[SP+1] - pas[SP];
              SP = SP+1;
              break;
            //MUL multiplication
            case 3:
              pas[SP+1]= pas[SP+1] * pas[SP];
              SP = SP+1;
              break;
            //DIV division
            case 4:
              pas[SP+1]= pas[SP+1] / pas[SP];
              SP = SP+1;
              break;
            //EQL equality 
            case 5:
              pas[SP+1]= (pas[SP+1] == pas[SP]);
              SP = SP+1;            
              break;
            //NEQ not equal
            case 6:
              pas[SP+1]= (pas[SP+1] != pas[SP]);
              SP = SP+1;
              break;
            //LSS less than
            case 7:
              pas[SP+1]= (pas[SP+1] < pas[SP]);
              SP = SP+1;
              break;
            //LEQ less than or equal to
            case 8:
              pas[SP+1]= (pas[SP+1] <= pas[SP]);
              SP = SP+1;
              break;
            //GTR greater than 
            case 9:
              pas[SP+1]= (pas[SP+1] > pas[SP]);
              SP = SP+1;
              break;
            //GEQ greater than or equal to
            case 10:
              pas[SP+1]= (pas[SP+1] >= pas[SP]);
              SP = SP+1;
              break;
          }
        // LOD load value to top of the stack from location at offset M from L
        // lexicographical levels down of the stack
        case 3:
          SP = SP-1;
          pas[SP] = pas[base(BP,IR[2])-IR[1]];
          break;
        // STO Store value at top of the stack in the ocation at offset M from L
        // lexicographical levels down of the stack
        case 4:
          pas[SP] = pas[base(BP,IR[2])-IR[1]];
          SP = SP+1;
          break;
        // CAL Call the procedure at code index p, 
        // generating a new activation record and 
        // setting PC to M
        case 5:
          pas[SP - 1]  =  base(BP, IR[2]); 
          pas[SP - 2]  = BP; 
          pas[SP - 3]  = PC;  
          BP = SP - 1;
          PC = IR[1];
          break;
        // INC allocate M locals on the stack 
        case 6:
          SP = SP-IR[1];
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
            PC = IR[1]; 
          } 
          SP = SP+1;
          break;
        // GENERAL SYSTEM [SYS] 
        case 9:
          // SOU Output of the value in stack[SP] to standard 
          // output as a character and pop:
          if(IR[2] == 1)
          {
            printf("%d",pas[SP]); 
            SP = SP+1;
          }
          // SIN Read in input from the user and store it on top of the stack
          else if(IR[2] == 2)
          {
            SP = SP-1;
            scanf("%d",&pas[SP]);
          }
          // EOP End of program (Set “eop” flag to zero)
          if(IR[2] == 3)
          {
            halt = 0;//"eop" flag ?? how do this, exit() is not allowed
          }
          break;
      }
      //fetch cycle
      PC = PC +3;
    } 
}
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