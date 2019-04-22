#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

void group(const char *dirPath){
    char symb;
    int a[26];
    for (int i = 0; i < 26; i++){
        a[i] = 0;
    }
    DIR *dir = opendir(dirPath);
    if(dir) {
        struct  dirent *de = readdir(dir);
        while (de) {
            if (de->d_name[0] != '.') {
                if (a[(int)(tolower(de->d_name[0]))-97] == 0){
                    a[(int)(tolower(de->d_name[0]))-97] = 1;
                    char* c = (char*)malloc(100*sizeof(char));
                    strcat(c,"./tmp/");
                    c[6] = tolower(de->d_name[0]);
                    c[7] = '\0'; 
                    mkdir(c, S_IRWXU);
                }
                char* s = (char*)malloc(100*sizeof(char));
                strcat(s,"./tmp/");
                s[6] = tolower(de->d_name[0]);
                s[7] = '/';
                s[8] = '\0'; 
                strcat(s, de->d_name);
                char* q = (char*)malloc(100*sizeof(char));
                strcat(q,"./tmp/\0"); 
                strcat(q, de->d_name);
                FILE *fileNew = fopen(s,"w+");
                FILE *file = fopen(q,"r");
                symb = fgetc(file); 
                while(!feof(file)) 
                { 
                    fputc(symb, fileNew); 
                    symb = fgetc(file); 
                }
            }
            de = readdir(dir);
        }
        closedir(dir);
    }
}

int main(){
    group("tmp\0");
    return 0;
}
