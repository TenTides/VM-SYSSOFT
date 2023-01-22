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
    }
    
}