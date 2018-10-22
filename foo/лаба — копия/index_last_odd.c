#include <stdlib.h>
#include <stdio.h>
int index_last_odd (int mainArray[], int n)
{
  int index;
  int i;
  for (i = 0; i < n; i++)
    {
      if (mainArray[i] % 2 != 0)
	{
	  index = i;
	}
    }
  return i;
}
