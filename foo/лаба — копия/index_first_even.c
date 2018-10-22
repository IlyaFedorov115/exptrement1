#include <stdlib.h>
#include <stdio.h>
int index_first_even (int mainArray[], int n)
{
  int i, index;
  for (i = 0; i < n; ++i)
    {
      if (mainArray[i] % 2 == 0)
	{
	  index = i;
	  break;
	}
    }
  return index;
}
