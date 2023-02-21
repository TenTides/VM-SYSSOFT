// Team Leader: Tyler Crawford   
// Member: Nelson Herrera Gamboa
// Class: COP3402    
// Date of Last Edit: 1/27/2023
#include <stdio.h>
#include <stdlib.h> 
#define ARRAY_SIZE 500

// initalize base
int base( int BP, int L);
// initalize pas array of ARRAY_SIZE
int pas[ARRAY_SIZE];
// arBarZeroOne array is used to mark where
// the activation bars on print go
int arBarZeroOne[ARRAY_SIZE];
// instruction register
int IR[3]; //IR  = 0 0 0

// pointer and halt initalization
int BP = 499; 
int SP = 500; 
int PC = 0;
int OP = 0; 
int M = 0; 
int L = 0;
int halt = 1;

int main(int argc, char *argv[])
{
    //Reads in input file from cmd line
    FILE *fp;
    fp = fopen(argv[1], "r");
    //Initial values: 0 499 500
    while(1)
    {
      fscanf(fp, "%d %d %d", &OP,&L, &M);
      
      pas[PC] = OP;
      pas[PC+1] = L;
      pas[PC+2] = M;
      PC= PC +3;
      // upon finding the halt code, scanning stops
      if(OP == 9 && M ==3) break;
    }
    fclose(fp);
    
    printf("\n");
    PC = 0;
    printf("                 PC   BP   SP   stack\n");
    printf("inital values:    %d   %d  %d\n",PC,BP,SP);
    while (halt) 
    {
      //Fetch Cycle
      IR[0] = pas[PC]; // OP
      IR[1] = pas[PC + 1];  // L
      IR[2] = pas[PC + 2]; // M
      PC = PC +3;
      //use nested swtich statements for instuctions
      switch(IR[0]) {
        //LIT, Pushes a constant value (literal) M onto the stack
        case 1:
            SP = SP-1;
            pas[SP] = IR[2];
            printf("    LIT 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
            //for loop to print stack with check for activation bars
            for(int i = 499; i>SP-1;i--)
            {
              if (pas[i] < 10)
              {
                printf("%3d",pas[i]);
              }
              else if (pas[i] < 100)
              {
                printf("%4d",pas[i]);
              } 
              else
              {
                printf("%5d",pas[i]);
              }
              if(arBarZeroOne[i])
              {
                printf(" | ");
              }
            }
            printf("\n");
          break;
        case 2: 
          //Operation to be performed on the data at the top of the stack.(or return from function)
          switch(IR[2]) {
            //RTN Returns from a subroutine is encoded 0 0 0 and restores the caller’s AR
            case 0:
              SP = BP+1;
              BP = pas[SP-2];
              PC = pas[SP-3];
              printf("    RTN 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //ADD addition
            case 1:
              pas[SP+1]= pas[SP+1] + pas[SP];
              SP = SP+1;
              printf("    ADD 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //SUB subtraction
            case 2:
              pas[SP+1]= pas[SP+1] - pas[SP];
              SP = SP+1;
              printf("    SUB 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //MUL multiplication
            case 3:
              pas[SP+1]= pas[SP+1] * pas[SP];
              SP = SP+1;
              printf("    MUL 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //DIV division
            case 4:
              pas[SP+1]= pas[SP+1] / pas[SP];
              SP = SP+1;
              printf("    DIV 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //EQL equality 
            case 5:
              pas[SP+1]= (pas[SP+1] == pas[SP]);
              SP = SP+1; 
              printf("    EQL 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
           
              break;
            //NEQ not equal
            case 6:
              pas[SP+1]= (pas[SP+1] != pas[SP]);
              SP = SP+1;
              printf("    NEQ 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //LSS less than
            case 7:
              pas[SP+1]= (pas[SP+1] < pas[SP]);
              SP = SP+1;
              printf("    LSS 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //LEQ less than or equal to
            case 8:
              pas[SP+1]= (pas[SP+1] <= pas[SP]);
              SP = SP+1;
              printf("    LEQ 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //GTR greater than 
            case 9:
              pas[SP+1]= (pas[SP+1] > pas[SP]);
              SP = SP+1;
              printf("    GTR 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
            //GEQ greater than or equal to
            case 10:
              pas[SP+1]= (pas[SP+1] >= pas[SP]);
              SP = SP+1;
              printf("    GEQ 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
              break;
          }
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // LOD load value to top of the stack from location at offset M from L
        // lexicographical levels down of the stack
        case 3:
          SP = SP-1;
          pas[SP] = pas[base(BP,IR[1])-IR[2]];
          printf("    LOD%2d%5d%5d%6d%5d",IR[1],IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // Store value at top of stack in the stack location at offset M 
        // from L lexicographical levels down
        case 4:
          pas[base(BP,IR[1])-IR[2]] = pas[SP];
          SP = SP+1;
          printf("    STO%2d%5d%5d%6d%5d",IR[1],IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // CAL Call the procedure at code index p, 
        // generating a new activation record and 
        // setting PC to M
        case 5:
          pas[SP - 1]  =  base(BP, IR[1]); 
          pas[SP - 2]  = BP; 
          pas[SP - 3]  = PC;  
          BP = SP - 1;
          PC = IR[2];
          arBarZeroOne[BP+1] = 1;
          printf("    CAL%2d%5d%5d%6d%5d",IR[1],IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // INC Allocate  M  memory  words  (increment  SP  by  M).  First  four  
        // are  reserved  to      Static  Link  (SL),  Dynamic  Link  (DL),                     
        // and Return Address (RA)
        case 6:
          SP = SP-IR[2];
          printf("    INC 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
            printf("%5d",pas[i]);
            }

            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // JMP Jump to instruction M (PC <- M)
        case 7:
          PC = IR[2];
          printf("    JMP 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // JPC jump conditionally,  Jump to instruction M 
        // if top stack element is 0
        case 8:
          if (pas[SP] == 0)
          { 
            PC = IR[2]; 
          } 
          SP = SP+1;
          printf("    JPC 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
          for(int i = 499; i>SP-1;i--)
          {
            if (pas[i] < 10)
            {
              printf("%3d",pas[i]);
            }
            else if (pas[i] < 100)
            {
              printf("%4d",pas[i]);
            } 
            else
            {
              printf("%5d",pas[i]);
            }
            if(arBarZeroOne[i])
            {
              printf(" | ");
            }
          }
          printf("\n");
          break;
        // GENERAL SYSTEM [SYS] 
        case 9:
          // Output of the value in stack[SP] to standard 
          // output as a character and pop:
          if(IR[2] == 1)
          {
            printf("Output result is:  %d\n",pas[SP]); 
            SP = SP+1;
            printf("    SOU 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
            for(int i = 499; i>SP-1;i--)
            {
              if (pas[i] < 10)
              {
                printf("%3d",pas[i]);
              }
              else if (pas[i] < 100)
              {
                printf("%4d",pas[i]);
              } 
              else
              {
                printf("%5d",pas[i]);
              }
              if(arBarZeroOne[i])
              {
                printf(" | ");
              }
            }
            printf("\n");
            break;
          }
          // Read in input from the user and store it on top of the stack
          else if(IR[2] == 2)
          {
            SP = SP-1;
            printf("Please Enter an integer:  "); 
            scanf("%d",&pas[SP]);
            printf("    SIN 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
            for(int i = 499; i>SP-1;i--)
            {
              if (pas[i] < 10)
              {
                printf("%3d",pas[i]);
              }
              else if (pas[i] < 100)
              {
                printf("%4d",pas[i]);
              } 
              else
              {
                printf("%5d",pas[i]);
              }
              if(arBarZeroOne[i])
              {
                printf(" | ");
              }
            }
            printf("\n");
            break;
          }
          // End of program (Set “eop” flag to zero)
          if(IR[2] == 3)
          {
            halt = 0;//"eop" flag ?? how do this, exit() is not allowed
            printf("    EOP 0%5d%5d%6d%5d",IR[2],PC,BP,SP);
            for(int i = 499; i>SP-1;i--)
            {
              if (pas[i] < 10)
              {
                printf("%3d",pas[i]);
              }
              else if (pas[i] < 100)
              {
                printf("%4d",pas[i]);
              } 
              else
              {
                printf("%5d",pas[i]);
              }
              if(arBarZeroOne[i])
              {
                printf(" | ");
              }
            }
            printf("\n");
          }
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