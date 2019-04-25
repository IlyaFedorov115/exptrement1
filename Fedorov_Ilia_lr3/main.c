#include <stdio.h>                                                              
#include <sys/types.h>                                                          
#include <dirent.h>                                                             
#include <string.h>                                                             
#include <stdlib.h>     
de->d_name[0] != '.'
int dirsearch(const char *dirPath, char *prevDir){                                   
        DIR *dir = opendir(dirPath);                                                                                           
        char *dirName;                                                          
        int length = 0;                                                         
        int tmp;                                                                
        int sum_part;                                                           
        int result = 0;                                                         
                                                                                
        if (strcmp(prevDir, "add") == 0 || strcmp(prevDir, "tmp") == 0)             
                sum_part = 0;                                                   
        else if (strcmp(prevDir, "mul") == 0)                                   
                sum_part = 1;
     struct dirent *de = readdir(dir);                               
                while (de){                                                     
                    dirName = malloc((strlen(dirPath) + strlen(de->d_name) + 2)* sizeof(char));
                        strcpy(dirName, dirPath);                               
                        strcat(dirName, "/");                                   
                        strcat(dirName, de->d_name);                            
                if ((strcmp(de->d_name, "add") == 0) ||(strcmp(de->d_name, "mul")==0)
                    {                                                           
                       if (strcmp(prevDir, "add") == 0 || strcmp(prevDir, "tmp")==0)
                            sum_part += dirsearch(dirName, de->d_name); 
                        if (strcmp(prevDir, "mul") == 0)                         
                            sum_part *= dirsearch(dirName, de->d_name);              
                        free(dirName);                                          
                    }                                                           
                                                                                
                    else if (strstr(de->d_name, ".txt")){                       
                                FILE *file = fopen(dirName, "r");                                                                                                                                 
                                while(fscanf(file, "%d", &tmp) != EOF){         
                                  if (strcmp(prevDir, "add") == 0 || strcmp(prevDir, "tmp")==0)
                                                sum_part += tmp;                
                                  else if (strcmp(prevDir, "mul")== 0)          
                                                sum_part *= tmp;                
                                }                                               
                                free(dirName);                                  
                                fclose(file);                                   
                                }       
         de = readdir(dir);                                      
                }                                                                                                                                      
        closedir(dir);                                                                                                                     
        return sum_part;                                                      
}                                                                               
int main(){                                                                     
        FILE *fp;                                                               
        int result = 0;                          
        fp = fopen("result.txt", "w");                                          
        result = dirsearch("tmp","tmp");  
           fprintf(fp, "%d", result);                                              
        fclose(fp);                                                             
}    
