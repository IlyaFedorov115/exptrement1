#include <stdlib.h>
#include <stdio.h>
#include "index_last_odd.h"

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
  return index;
}
