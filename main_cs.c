#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define RED "\033[0;31m"
#define NONE "\033[0m"
#define GREEN "\033[0;32m"
#define MAX 1190000
#define YELLOW "\033[0;33m"
typedef struct {
	int year;
	int month;
	int day;
}date;

int compare(const date *a, const date *b);
char** memory_text(char **dynamic_array, int *num_sent);
char** delete_same(char **dynamic_array, int *num_sent);
void find_date(char **dynamic_array, int *num_sent);
char** delete_XIX_century(char **dynamic_array, int *num_sent);
void min_date_and_max_date(char **dynamic_array, int *num_sent);
void output_text(char **dynamic_array, int *num_sent);
char** sort_Sentence(char** dynamic_array, int* num_sent);
int comp(const char* a, const char* b);

int main(){
    printf("Введите текст , разделителями между словами является пробел, а между предложениями - точка\n");
    int len_text = 0;
    char choice;
    char opp;
    char **dynamic_array = (char**)malloc(sizeof(char*));
    int i = 0;
    int q = 0;
    int k, n , m = 0;    
	int num_sent = 0;
	dynamic_array = memory_text(dynamic_array, &num_sent);
	dynamic_array = delete_same(dynamic_array, &num_sent);
  while (q != 1) {
    printf("\nВведите 1, ecли вы хотите вывести все предложения, в которых есть дата с текущем месяцем и годом.\n");
    printf("\nВведите 2, ecли вы хотите отсортировать предложения по увеличению минимальной даты в них.\n");
    printf("\nВведите 3, ecли вы хотите для каждого предложения вывести самую раннюю и позднюю дату.\n");
    printf("\nВведите 4, ecли вы хотите удалить все предложения в которых все даты относятся к 19 веку\n");
    printf("\nВведите 5, если вы хотите закончить работу программы.\n");
    choice = getchar();
    opp = getchar();
        switch (choice) {
             case '1':
				find_date(dynamic_array, &num_sent);
				break;
			 case '2':
				delete_XIX_century(dynamic_array,&num_sent);
				output_text(dynamic_array, &num_sent);
				break;
			 case '3':
				min_date_and_max_date(dynamic_array,&num_sent);
				break;
		     case '4':
				qsort(dynamic_array, num_sent, sizeof(char*), comp);
				output_text(dynamic_array, &num_sent);
				break; 
			 case '5':
				printf("%s\nGoodbye, User\n%s", GREEN, NONE);
				free_Memory(dynamic_array, num_sent);
				q = 1;
				break;
			 default:
				printf("%sНеправильный ввод!%s", RED, NONE);
        }
     }
    return 0;
}


int compare(const date *a, const date *b){
	return a->year * 10000 + a->month * 100 + a->day - b->year * 10000 - b->month * 100 - b->day;
}

char** memory_text(char **dynamic_array, int *num_sent){
	int str_len = 12; 
	char symb;
 	int letters = 0;

	while (symb != '\n'){
		dynamic_array[*num_sent] = (char*)malloc(str_len * sizeof(char));
		while (symb != '.' && symb != '\n'){
			symb = getchar();
			if (symb == '\n'){
			break;
			}
			if (letters >= str_len - 1){
				str_len += str_len;
				dynamic_array[*num_sent] = (char*)realloc(dynamic_array[*num_sent], str_len * sizeof(char));
			}
			dynamic_array[*num_sent][letters++] = symb;
		}
		if (symb == '\n'){
		break;
		}
		symb = '\0';
		dynamic_array[*num_sent][letters] = symb;
		*num_sent += 1 ;
		dynamic_array = (char**)realloc(dynamic_array, (*num_sent + 1) * sizeof(char*));
		letters = 0;
		str_len = 12;
	}
	return dynamic_array;
}



char** delete_same(char **dynamic_array, int *num_sent){
	int count = 0;
	for(int sentence1 = 0; sentence1 < *num_sent - 1; sentence1++){
		for (int sentence2 = sentence1 + 1; sentence2 < *num_sent; sentence2++){
			if (strlen(dynamic_array[sentence1]) == strlen(dynamic_array[sentence2])){
				count = 0;
				for(int j = 0; j < strlen(dynamic_array[sentence1]); j++){
					if (toupper(dynamic_array[sentence1][j]) == toupper(dynamic_array[sentence2][j])){
						++count;
					}
				}
				if (count == strlen(dynamic_array[sentence1])){
					free(dynamic_array[sentence2]);
					for (int l = sentence2; l < *num_sent; l++){
						dynamic_array[l] = dynamic_array[l+1];
					}
					sentence2--;
					(*num_sent)--;
					dynamic_array = (char**)realloc(dynamic_array,(*num_sent)*sizeof(char*));
				}
			}
		} 
	}
	return dynamic_array;
}


void output_text(char **dynamic_array, int *num_sent)
   {
    if ((*num_sent) == 0){
		printf("%sПредложений нет :(%s", YELLOW, NONE);
		printf("\n");
	} 
	else {
	   for (int i = 0; i < *num_sent; i++)
	     {
	    	if (dynamic_array[i] != NULL)
		     {
		    	printf("%s%s%s", GREEN, dynamic_array[i], NONE);
		     }
    	 }
	}
}


void find_date(char **dynamic_array,int *num_sent) 
   {       
    char str[] = "12/2018";
    char *instr;
    int count = 0;
    for (int i = 0 ; i < *num_sent; i++)
      {
        instr = strstr(dynamic_array[i],str);
        if (instr != NULL){
             count ++;
             for (int j = 0; j < strlen(dynamic_array[i]); j++)  
               {
               printf("%c", dynamic_array[i][j]); 
               }
               printf("\n");
        }
      }
    if (count==0) printf("Даты не нашлось");
    
}

char** delete_XIX_century(char **dynamic_array, int *num_sent){
    int count1 = 0;    
    int count2 = 0;    
    char *bufer;
    char *instr;
    char fstr[] = "/18";
    for (int i = 0; i < *num_sent; i++){
        bufer = (char*)malloc((strlen(dynamic_array[i]))*sizeof(char));
		memcpy(bufer, dynamic_array[i], (strlen(dynamic_array[i])+1));
		char *str = strtok(bufer, " "); /////" ,.\0"
		while(str != NULL){
			count1++;
			instr = strstr(str,fstr);
			if (instr != NULL) count2++;
			str = strtok(NULL, " ");
		}
		if (count1 == count2){
			for (int j = i; j < *num_sent - 1; j++){
				dynamic_array[j] = dynamic_array[j+1];
			}
			i -= 1;
			dynamic_array[*num_sent - 1] = NULL;
			*num_sent -= 1;
		}
		free(bufer);
		count1 = 0;
		count2 = 0;
        
    }
    return dynamic_array;
}


void min_date_and_max_date(char **dynamic_array, int *num_sent)
  {
    char *d;
	char *m;
	char *y;
	int size = 10;
	char *bufer;
	date *dates = (date*)malloc(size * sizeof(date));
	int index = 0;
	for (int i = 0; i < *num_sent; i++)
	  {
	    bufer = (char*)malloc((strlen(dynamic_array[i])+1) * sizeof(char));
		memcpy(bufer, dynamic_array[i], (strlen(dynamic_array[i])+1) * sizeof(char));
		char *str = strtok(bufer, " \0"); 
	    while(str != NULL)
	       {
	         if (strlen(str) >= 10) {
	             for (int j = 0; j < strlen(str); j++) {
	                 if (isdigit(str[j]) && isdigit(str[j + 1]) && str[j + 2] == '/' && j + 9 < strlen(str)){
	                     if (isdigit(str[j + 3]) && isdigit(str[j + 4]) && str[j + 5] == '/'){
	                         if ( isdigit(str[j + 6]) && isdigit(str[j + 7]) && isdigit(str[j + 8]) && isdigit(str[j + 9])){
	                            if (index >= size){
								   size += 10;
								   dates = (date*)realloc(dates, size * sizeof(date)); }
								d = (char*)malloc(2 * sizeof(char));
								d[0] = str[j]; d[1] = str[j + 1];
								m = (char*)malloc(2 * sizeof(char));
					            m[0] = str[j + 3]; m[1] = str[j + 4];
								y = (char*)malloc(4 * sizeof(char));
								y[0] = str[j + 6]; y[1] = str[j + 7]; y[2] = str[j + 8] ; y[3] = str[j + 9];
								dates[index].day = atoi(d);
								dates[index].month = atoi(m);
								dates[index].year = atoi(y);
								free(d); free(m); free(y);
								index += 1;   
	                         }
	                     }
	                 }
	             }
     
	         }
 
	        str = strtok(NULL, " \0");
	        }
	      free(bufer);
	      qsort(dates, index, sizeof(date),(int(*)(const void *, const void *)) compare);
	      if (index == 0 || index == 1){
		       printf("%sВ предложении нет дат или дата всего одна\n%s", YELLOW, NONE);
	           }
	     else { 
	       printf("%sМинимальная и Максимальная дата для %d -го предложения: %s",YELLOW,i+1,NONE);
	     for (int k = 0; k < index; k += (index - 1)){
	    	if (dates[k].day < 10){
	    		printf("%s0%s", GREEN, NONE);
	    	}
	    	printf("%s%d/%s", GREEN, dates[k].day, NONE);
	    	if (dates[k].month < 10){
    			printf("%s0%s", GREEN, NONE);
    		}
    		printf("%s%d/%s", GREEN, dates[k].month, NONE);
    		if (dates[k].year < 1000){
	    		printf("%s0%s", GREEN, NONE);
	    		if (dates[k].year < 100){
            			printf("%s0%s", GREEN, NONE);
              		if (dates[k].year < 10){
             				printf("%s0%s", GREEN, NONE);
            			}
      			}
    		}
	     	printf("%s%d%s\n", GREEN, dates[k].year, NONE);
	}
	       }
	index = 0;
   
  }
	free(dates);
}

int comp(const char* a, const char* b){
    const char *str1 = *( const char * const *) a;
    const char *str2 = *( const char * const *) b;
    int mins_1 = MAX;
    int min1 = 0;
    int mins_2 = MAX;
    int min2 = 0;
    int x;
    char y;
    for (int i = 0; i < strlen(str1)-9; i++){
        if (str1[i] == ' '){
            i++; 
             }
          min1 = (y=str1[i] - '0')*10 + (y=str1[i+1] - '0') + (y=str1[i+3] - '0')*100 + (y=str1[i+4] - '0')*10 + (y=str1[i+6] - '0')*120000 + (y=str1[i+7] - '0')*10000 + (y = str1[i+8] - '0')*1000 + (y = str1[i+9] - '0')*200;
        if (min1 <= mins_1) {
            mins_1 = min1;
             }
    }
    for (int k = 0; k < strlen(str2)-9; k++){
        while (str2[k] == ' ') k++;
        min2 = (y=str2[k] - '0')*10 + (y=str2[k+1] - '0') + (y=str1[k+3] - '0')*100 + (y=str2[k+4] - '0')*10 + (y=str2[k+6] - '0')*120000 + (y=str2[k+7] - '0')*10000 + (y = str2[k+8] - '0')*1000 + (y = str2[k+9] - '0')*150;
        if (min2 <= mins_2) {
            mins_2 = min2;
             }
    }
    
    if (mins_1 > mins_2){
        return 1;
    }
    if (mins_1 == mins_2){
        return 0;
    }
    if (mins_1 < mins_2){
        return -1;
    }
}

void free_Memory(char **dynamic_array, int n){
    for (int i = 0; i < n ; i++){
        free(dynamic_array[i]);
    }
   free(dynamic_array);
}  
