#include <stdio.h>
#include <stdlib.h> 
#define ARRAY_SIZE 500

//initalize base
int base( int BP, int L);
//initalize pas array of ARRAY_SIZE
int pas[ARRAY_SIZE];
int arBarZeroOne[ARRAY_SIZE];
//instruction register
int IR[3]; //IR  = 0 0 0

//pointer and halt initalization
int BP = 499; 
int SP = 500; 
int PC = 0;
int OP = 0; 
int M = 0; 
int L = 0;
int halt = 1;

int main()
{
    //scanning method may have to change if file pointers are used 

    //The input file name should be read as a command line argument at runtime, for 
    //example: $ ./a.out input.txt (A deduction of 5 points will be applied to submissions 
    //that do not implement this).

   

    //Initial values: 0 499 500
    while(1)
    {
      scanf("%d %d %d",&OP,&M, &L);
      pas[PC] = OP;
      pas[PC+1] = M;
      pas[PC+2] = L;
      PC= PC +3;
      // upon finding the halt code, scanning stops
      if(OP == 9 && L ==3) break;
    }
    //for(int i = 0;i<9;i++){
   //   printf("%d ",pas[i]);
    //}
    //printf("\n");
    PC = 0;
    printf("                  PC     BP     SP     stack\n");
    printf("inital values:     %d     %d     %d\n",PC,BP,SP);
    while (halt) // stops scanning when no inputs are detected
    {
      //fetch cycle
      IR[0] = pas[PC]; //op
      IR[1] = pas[PC + 1];  // m
      IR[2] = pas[PC + 2]; // l
      PC = PC +3;
      //use nested swtich statemnts
      switch(IR[0]) {
        //LIT, Literal push
        case 1:
            // Test case u can use to see how this works out
            //  6 0 5
            //  1 0 4
            //  9 0 3
            SP = SP-1;
            pas[SP] = IR[2];
            printf("LIT 0    %d       %d     %d     %d     ",IR[2],PC,BP,SP);
            //for loop to print stack with check for activation bars
            for(int i = BP; i>SP-1;i--)
            {
              printf("%d ",pas[i]);
              if(arBarZeroOne[i])
              {
                printf("| ");
              }
            }
            printf("\n");
          break;
        case 2: 
          switch(IR[1]) {
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
          SP = SP-IR[2];
          break;
        // JMP jump to the address in stack
        case 7:
          PC = IR[1];
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
            printf("Output result is: %d",pas[SP]); 
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