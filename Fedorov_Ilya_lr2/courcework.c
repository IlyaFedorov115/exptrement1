#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define RED "\033[0;31m"
#define NONE "\033[0m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"

char** memory_text(char **dynamic_array, int *num_sent){
	int str_len = 12;  ///äëÿ îäíîé äàòû
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
			//letters++ ;
		}
		if (symb == '\n'){
		break;
		}
		symb = '\0';
		dynamic_array[*num_sent][letters] = symb;
		//symb = None;
		*num_sent += 1 ;
		dynamic_array = (char**)realloc(dynamic_array, (*num_sent + 1) * sizeof(char*));
		letters = 0;
		str_len = 12;
	}
	return dynamic_array;
}



char** delete_same(char **dynamic_array, int *num_sent)
{
	int point = 0;
	for(int i = 0; i < *num_sent - 1; i++)
    {
		for (int k = i+1; k < *num_sent; k++)
          {
		   if (strlen(dynamic_array[i]) == strlen(dynamic_array[k]))
              {
		      for(int j = 0; j < strlen(dynamic_array[i]); j++)
                  {
				   if (toupper(dynamic_array[i][j]) == toupper(dynamic_array[k][j]))   ///èëè while
                      {
						++point; //// èëè point++;
                      }
                  }
               if (point == strlen(dynamic_array[i]))
                {
				   free(dynamic_array[k]);
				   for (int q = k; q < *num_sent; q++)
					  {
						dynamic_array[q] = dynamic_array[q+1];
					  }
					dynamic_array[*num_sent - 1] = NULL;
					k -= 1;
					*num_sent -= 1;
					point = 0;   
                }
			   }
			else continue;  /// ìîæíî óáðàòü 
		   } 
	 }
  return dynamic_array;
}

//////âûâîä òåêñòà /////
void output_text(char ***dynamic_array, int *num_sent)
   {
    if (*num_sent == 0){
		printf("%sÏðåäëîæåíèé íåò :(%s", YELLOW, NONE);
		printf("\n");
	} 
	else {
	   for (int i = 0; i < *num_sent; i++)
	     {
	    	if ((*dynamic_array)[i] != NULL)
		     {
		    	printf("%s%s%s", GREEN, (*dynamic_array)[i], NONE);    //////ðàçîáðàòüñÿ ÷òî ýòî
		     }
    	 }
	}
}


void find_date(char **dynamic_array,int *num_sent) {       ////////////ìîæåò áûòü 3 ***
    char str[] = "12/2018";
    char *instr;
    int count = 0;
    for (int i = 0 ; i < *num_sent; i++)
      {
        instr = strstr(dynamic_array[i],str);
        if (instr != NULL){
             count ++;
             for (int j = 0; j < strlen(dynamic_array[i]); j++)  // öèêë ïî ñòîëáöàì
               {
               printf("%c", dynamic_array[i][j]); // 5 çíàêîìåñò ïîä ýëåìåíò ìàññèâà
               }
               printf("\n");
        }
      }
    if (count==0) printf("Äàòû íå íàøëîñü");
    
}

void odd_delete(char ***dynamic_array, int *num_sent){
    int count1 = 0;    ////÷èñëî äàò â ïðèíöèïå
    int count2 = 0;    //// ÷èñëî äàò 19 âåêà
    char *bufer;
    char *instr;
    char fstr[] = "/18";
    for (int i = 0; i < *num_sent; i++){
        bufer = (char*)malloc((strlen((*dynamic_array)[i]))*sizeof(char));
		memcpy(bufer, (*dynamic_array)[i], (strlen((*dynamic_array)[i])+1));
		char *str = strtok(bufer, " "); /////" ,.\0"
		while(str != NULL){
			count1++;
			instr = strstr(str,fstr);
			if (instr != NULL) count2++;
			str = strtok(NULL, " ");
		}
		if (count1 == count2){
			for (int j = i; j < *num_sent - 1; j++){
				(*dynamic_array)[j] = (*dynamic_array)[j+1];
			}
			i -= 1;
			dynamic_array[*num_sent - 1] = NULL;
			*num_sent -= 1;
		}
		free(bufer);
		count1 = 0;
		count2 = 0;
        
    }
}



///////////////////////////////// MENUMENUMEN

void menu(char ***dynamic_array, int *num_sent){
	int quit = 0;
	char opp;
	char tmp;
	while (quit != 1){
		printf("\nÂâåäèòå:\n\n1: äëÿ âûâîäà äàò èç òåêñòà â âîçðàñòàþùåì ïîðÿäêå\n\n2: äëÿ óäàëåíèÿ âñåõ ïðåäëîæåíèé ñ íå÷¸òíûì êîëè÷åñòâîì ñëîâ\n\n3: äëÿ ïðåîáðàçîâàíèÿ âñåõ ñëîâ â êîòîðûõ íåò öèôð ïðîïèñíûìè (êðîìå ïîñëåäíåé áóêâû)\n\n4: äëÿ âûâîäà âñåõ ïðåäëîæåíèé, â êîòîðûõ íåò çàãëàâíûõ áóêâ\n\n5: Äëÿ âûõîäà èç ïðîãðàììû\n\nÂâîä: ");
		opp = getchar();
		tmp = getchar();
		if (tmp != '\n'){
			opp = '7';
		}
		while (tmp != '\n'){
			tmp = getchar();
		}
		switch(opp){
			case '1':
				find_date(*dynamic_array, num_sent);
				break;
			case '2':
				odd_delete(dynamic_array, num_sent);
				output_text(dynamic_array, num_sent);
				break;
			/* case '3':
				to_upper(text, sentence);
				text_output(text, sentence);
				break;
			case '4':
				no_upper(*text, sentence);
				break; */
			case '5':
				printf("%s\nAu revoir\n%s", GREEN, NONE);
				quit = 1;
				break;
			default:
				printf("%sÍåïðàâèëüíûé ââîä!%s", RED, NONE);
		}
	}
}

void main()
{
    char **dynamic_array = malloc(sizeof(char*)); /// èëè òàê char **dynamic_array =(char**)malloc(sizeof(char*));
    int i = 0;
    int k, n , m = 0;    
	int num_sent = 0;
	printf(" Ââåäèòå òåêñò. Èñïîëüçîâàòüñÿ äîëæíû òîëüêî ëàòèíñêèå áóêâû è öèôðû. Ñëîâà îòäåëÿþòñÿ ïðîáåëàìè èëè çàïÿòûìè, ïðåäëîæåíèÿ — òî÷êàìè.\n\n");
	dynamic_array = memory_text(dynamic_array, &num_sent);
	dynamic_array = delete_same(dynamic_array, &num_sent);
	menu(&dynamic_array, &num_sent);
	
}




