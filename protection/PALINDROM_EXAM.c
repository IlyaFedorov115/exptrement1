#include <stdio.h>
#include <stdlib.h>

int is_palindrome(int start, int end, char* str)
{
    if (str[start] != str[end])
        return 0;
    if (str[start] == str[end])
      {
        if (start <= end)
        return is_palindrome(start+1, end-1, str);
        return 1;
      }
}


int main()
{
  int a;
 char tmp[20];
 scanf("%s", tmp);
 a = is_palindrome(0,strlen(tmp)-1,tmp);
 if (a == 1)
    printf("true");
 else 
    printf("false");
    return 0;
}
