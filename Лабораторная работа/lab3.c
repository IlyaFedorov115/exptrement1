#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define NAME_OF_FILE_OUTPUT "./result.txt"
#define DIR_START "./tmp"
#define FIND_FIRST_FILE "file.txt"
#define ADD "add"
#define MUL "mul"

			
 int func(const char *dirPath){
	DIR *dir = opendir(dirPath);	
	//char symbol;
	//char *string = NULL;	char *dirName;
	int length = 0;
	int sum_part;
	int result = 0;
	short flag_sum = 0;     /// проверить dirPath если умножение, то соответ флаг
	short flag_mult = 0;
	if (dir){
		struct dirent *de = readdir(dir);
		while (de){
		/*	dirName = malloc((strlen(dirPath) + strlen(de->d_name) + 2) * sizeof(char));
			strcpy(dirName, dirPath);
			strcat(dirName, "/");
			strcat(dirName, de->d_name);
		*/	
	        if ((!strcmp(de->d_name, ADD)) || (!strcmp(de->d_name, MUL)) )  // возможно по коду выше полный путь прописывать
	            {
	               sum_part = func(de->d_name);
	            }
	            
			if (strstr(de->d_name, ".txt")){
				FILE *file = fopen(de->d_name, "r"); // был dirName
				char a;
				int c;
				int array[80];
				int len = 0;
				if (file){
					while (((c = fscanf(file,"%d", &array[len])) != 0) && ((a = fgetc(file)) != EOF)){
					    len++;
					    
						//string = realloc(string, ++length * sizeof(char) + 1);
						//string[length-1] = symbol;
					}
					    if (!strcmp(dirPath, ADD)) //dirName
					      {
		                     for (int i = 0; i <= len; i++)
		                            result += array[i];
					      }
					    if (!strcmp(dirPath, MUL))
					      {   
					          result = 1;
					          for (int i = 0; i <= len; i++)
					                result *= array[i];
					      }
					
					/* string[length] = '\0';
					*res = realloc(*res, ++(*numOfStr) * sizeof(char*));
					(*res)[(*numOfStr)-1] = malloc((length+1) * sizeof(char) + 1);
					strcpy((*res)[(*numOfStr)-1], string);
					string = NULL;
					length = 0;
					free(string); */
					fclose(file);
				}
			}
				/*	else if (!strchr(de->d_name, '.')){
				func(dirName, res, numOfStr);
 			}   */
			de = readdir(dir);
		}
	}
	closedir(dir);
    if (!strcmp(dirPath,ADD))
          return result+sum_part;
    else 
          return result*sum_part;
}


int main(){
	FILE *fp;
	int result = 0;
//	char *tmp = "/.tmp";
	fp = fopen("result.txt", "w");
	result = func(DIR_START);
	fprintf(fp, "%d", result);
	fclose(fp);
}
