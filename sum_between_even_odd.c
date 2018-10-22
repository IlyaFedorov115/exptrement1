#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "index_first_even.h"
#include "index_last_odd.h"
#include "sum_between_even_odd.h"


 int sum_between_even_odd (int mainArray[], int a, int b)
{
  int i;
  int sum = 0;
  int mainEvenIndex = index_first_even (a, b);
  int mainOddIndex = index_last_odd (a, b);
  for (i = a; i < b; ++i)
    {
      sum = sum + fabs (mainArray[i]);
    }
  return sum;
}
