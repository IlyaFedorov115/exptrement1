#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

char* del_file(const char *dirPath, char* str){
    DIR *dir = opendir(dirPath);
    if(dir) {
        struct  dirent *de = readdir(dir);
        char* newDirPath = (char*)malloc(100*sizeof(char));
        newDirPath[0] = '\0';
        strcat(newDirPath, dirPath);
        strcat(newDirPath, "/");
        while (de) {
            if (de->d_name[0] != '.') {
                int n = strlen(str);
                if(strncmp(de->d_name, str, n) == 0){
                    strcat(newDirPath, de->d_name);
                    remove(newDirPath);
                }
            }
            de = readdir(dir);
            newDirPath[0] = '\0';
            strcat(newDirPath, dirPath);
            strcat(newDirPath, "/");
        }
        closedir(dir);
    }
}

int main(){
    char str[255];
    scanf("%s", str);
    del_file("tmp0\0", str);
    return 0;
}
