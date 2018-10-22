#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "index_first_even.h"
#include "index_last_odd.h"
#include "sum_before_even_and_after_odd.h"


 int sum_before_even_and_after_odd (int mainArray[], int a, int b)
{
  int i;
  int sum = 0;
  int mainEvenIndex = index_first_even (a, b);
  int mainOddIndex = index_last_odd (a, b);
  for (i = 0; i < a; i++)
    {
      sum = sum + fabs (mainArray[i]);
    }
  for (i = b;; i++)
    {
      sum = sum + fabs (mainArray[i]);
    }
  return sum;
}
