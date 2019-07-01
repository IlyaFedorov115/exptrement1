#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define PATTERN "(mul|add|tmp)"
#define STRING "mul"

void lower_string(char* p, int n){ 
  int i = 0;
  while (i < n){
    *p = (char)tolower(*p);
    p++;
    i++;
  }
}
 
int main() {
    char name[10] = "mul";
    char name1[10] = "MUL";
    char name2[10] = "add";
    regex_t preg;
    int err,regerr;
    err = regcomp(&preg, PATTERN, REG_EXTENDED);
    if (err != 0) {
        char buff[512];
        regerror(err, &preg, buff, sizeof(buff));
        printf("%s",buff);
    }
    regmatch_t pm;
    regerr = regexec(&preg, name, 0, &pm, 0);
    if (regerr == 0) printf("name true\n"); 
    else {
        printf("name false\n");
         char errbuf[512];
          regerror(regerr, &preg, errbuf, sizeof(errbuf));
    printf("\n%s\n",errbuf);
    }
    
    lower_string(name1,strlen(name1));
    regerr = regexec(&preg, name1, 0, &pm, 0);
    if (regerr == 0) printf("name1 true\n"); 
    else {
        printf("name1 false\n");
         char errbuf[512];
          regerror(regerr, &preg, errbuf, sizeof(errbuf));
           printf("\n%s\n",errbuf);
    }
    
    
    regerr = regexec(&preg, name2, 0, &pm, 0);
    if (regerr == 0) printf("name2 true\n"); 
    else {
        printf("name2 false\n");
         char errbuf[512];
          regerror(regerr, &preg, errbuf, sizeof(errbuf));
    printf("\n%s\n",errbuf);
    }
    return 0;
}
