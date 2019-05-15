#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAX_PATH 255

void sort_file(const char *dirPath){
    char symbol;
    int index = 0;
    char alphabet[26];
    int flag = 0;
   // memset(alphabet, '0', 26);
    for (int i = 0; i < 26; i++){
        alphabet[i] = '1';
  }
    DIR *dir = opendir(dirPath);
    if(dir) {
        struct  dirent *de = readdir(dir);
        while (de) {
            if (strcmp(de->d_name,"..")!=0 && strcmp(de->d_name, ".") && strstr(de->d_name, ".")) { 
                for (int i = 0; i < 26; i++)
                    { 
                        if (alphabet[i] == de->d_name[0])
                        flag = 1;
                    }    
                if (flag == 0){  
                    ////strstr(alphabet , de->d_name[0]) == NULL
                    alphabet[index] = de->d_name[0];
                    index++;
                    char* c = (char*)malloc(MAX_PATH*sizeof(char));
                    strcat(c,"tmp/");
                    char con = tolower(de->d_name[0]);
                //    strcat(c , con);
                  //  strcat(c, '\0');
                    c[4] = tolower(de->d_name[0]);
                    c[5] = '\0'; 
                    mkdir(c, S_IRWXU);
                   // free(c);
                }
                flag = 0;
                char* s = (char*)malloc(MAX_PATH*sizeof(char));
                strcat(s,"tmp/");
                s[4] = tolower(de->d_name[0]);
                //char con = tolower(de->d_name[0]);
                //strcat(s, con);
                ///strcat(s, '/');
                 s[5] = '/';
                s[6] = '\0'; 
                strcat(s, de->d_name);
               //  strcat(s, '\0');
                char* q = (char*)malloc(MAX_PATH*sizeof(char));
                strcat(q,"tmp/"); 
                strcat(q, de->d_name);
                FILE *fileNew = fopen(s,"w+");
                FILE *file = fopen(q,"r");
                symbol = fgetc(file); 
                while(!feof(file))   ///!feof(file)
                { 
                    fputc(symbol, fileNew); 
                    symbol = fgetc(file); 
                }
               // remove(s);
            }
	    /*else if (strcmp(de->d_name,"..")!=0 && strcmp(de->d_name, ".") && strstr(de->d_name,".") == NULL) {
		alphabet[index] = de->d_name[0];
		index++;
		}  */
            de = readdir(dir);
        }
        closedir(dir);
    }
}

int main(){
    sort_file("tmp");
    return 0;
}









#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

#define MAX_PATH 255


void delete(const char *dirPath, char* str){
    DIR *dir = opendir(dirPath);
     /// df
      if(dir) {
        struct  dirent *de = readdir(dir);
        char* Current_Path = (char*)malloc((MAX_PATH+1)*sizeof(char));
         Current_Path[0] = '\0';
        strcat(Current_Path, dirPath);
        strcat(Current_Path, "/");
        while (de) {
            if (strstr(de->d_name, ".")) {
               // int n = strlen(str);
                  int n = strstr(de->d_name, ".") - de->d_name - 1;
                  int c = strstr(de->d_name, str) - de->d_name;
                  int flag = 0;
                  if (strlen(str) < strlen(de->d_name)) {
                  for (int i = strlen(str) - 1; i < 0 ; i--){
                        if (str[i] == de->d_name[n]) flag++;
                        n--;
                  }
                  }
                  else { flag = 0; };
                  if (flag == strlen(str)){
                      strcat(Current_Path, de->d_name);
                      remove(Current_Path);
                  }
               /* if((strncmp(de->d_name, str, strlen(str)) == 0 && (strlen(str) - n == 0)) || ((c != NULL) && (c+strlen(str) == n) ) )
                  {
                      strcat(Current_Path, de->d_name);
                      remove(Current_Path);
                  }  */
            }
            de = readdir(dir);
            Current_Path[0] = '\0';
            strcat(Current_Path, dirPath);
            strcat(Current_Path, "/");
        }
        closedir(dir);
    }
}

int main(){
    char name[MAX_PATH+1];
    scanf("%s", name);
    delete("tmp", name);
    return 0;
}
