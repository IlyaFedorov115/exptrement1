#include <stdio.h>                                                              
#include <sys/types.h>                                                          
#include <dirent.h>                                                             
#include <string.h> 
#include <sys/stat.h>                                                            
#include <stdlib.h>
#define DIRENT "(([mM][uU][lL])|([aA][dD][dD]))"
#define NAME ".+\.txt"

void lower_string(char* p, int n){ 
  int i = 0;
  while (i < n){
    *p = (char)tolower(*p);
    p++;
    i++;
  }
}

int isValid(char *filename, const char* regexp){
	regex_t regexComp;
	if(regcomp(&regexComp, regexp, REG_EXTENDED)){
		return 0;
	}
	return regexec(&regexComp, filename, 0, NULL, 0) == 0;
}

int dirsearch(const char *dirPath, char *prevDir){                                   
        DIR *dir = opendir(dirPath);     
        char *dirName;                                                          
        int length = 0;                                                         
        int tmp;                                                                
        int sum_part;                                                           
        int result = 0;  
   //     lower_string(prevDir, strlen(prevDir));
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
            //    if ((strcmp(de->d_name, "add") == 0) ||(strcmp(de->d_name, "mul")==0))
                  if (de->d_type == DT_DIR && strcmp(de->d_name, ".")!=0 && strcmp(de->d_name, "..")!=0 && isValid(de->d_name, DIRENT))
			
                    {       
                       if (strcmp(prevDir, "add") == 0 || strcmp(prevDir, "tmp")==0)
                            sum_part += dirsearch(dirName, de->d_name); 
                        if (strcmp(prevDir, "mul") == 0)                         
                            sum_part *= dirsearch(dirName, de->d_name);              
                        free(dirName);                                          
                    }                                                           
                                                                                
		else if (de->d_type == DT_REG && isValid(de->d_name, NAME))                    
			//else if (strstr(de->d_name, ".txt"))
		{                       
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
