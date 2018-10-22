#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define ArraySize 100
 
 int main ()
{
  int i, value, array_Size;
  int mainEvenIndex = 0;
  int mainOddIndex = 0;
  int mainSum = 0;
  int mainArray[ArraySize];
  char endSymbol;
  scanf ("%d", &value);
  for (i = 0; i < ArraySize; ++i)
    {
      scanf ("%d", &mainArray[i]);
      endSymbol = getchar ();
      if (endSymbol == '\n')
	break;
      array_Size = i;
    }

  if (value == 0)
    {
      mainEvenIndex = index_first_even(mainArray, array_Size);
      printf ("%d", mainEvenIndex);
    }
  else if (value == 1)
    {
      mainOddIndex = index_last_odd(mainArray, array_Size);
      printf ("%d", mainOddIndex);
    }
  else if (value == 2)

    {
      mainSum = sum_between_even_odd (mainArray, mainEvenIndex, mainOddIndex);
      printf ("%d", mainSum);
    }
  else if (value == 3)
    {
      mainSum = sum_before_even_and_after_odd (mainArray, mainEvenIndex,
				       mainOddIndex);
      printf ("%d", mainSum);
    }
  else if (value > 3)
    {
      printf ("incorrect data");
      }
} 
      
      
      
      
      
      
      
      
      
      
      
