#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 300

typedef struct Node{
    char data[20];
    struct Node* prev;
}Node;


typedef struct Stack{
    struct Node* head;
}Stack;

void Push (Stack** S, char* tag){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    strcpy(Newnode->data, tag);
    Newnode->prev = (*S)->head;
    (*S)->head = Newnode;
}
 

char* Pop (Stack** S){
    if ((*S)->head == NULL){
        printf("Стек пуст");
        return;
    }
    Node* tmp = (*S)->head;
    char *tag;
    *tag = (*S)->head->data;
  //  strcpy(tag, (*S)->head->data);
 //   Node* tmp = (*S)->head;
    (*S)->head = (*S)->head->prev;
    free(tmp);
    return tag;
}

int is_Empty (Stack* S)
  {
    if (S->head) {
        printf("no pysto");
        return 0;
    }
    return 1;
   }

int main() {
    int result = 1;
    int flag = 1;  char c;
    Stack *S = malloc(sizeof(Stack));
    printf("It is pkey 1 \n");
    S->head = NULL;
    printf("WTF");
    char tag[11];
    while((c = getchar()) != '\n')
    {
       int i = 0; flag = 1;

        if (c == '<')
        {
          tag[i++] = c;
          while(c != '>')
          {
             c = getchar();
             if (c == '/')
                {
                   flag = 0;
                }
             else  tag[i++] = c;
          }
           tag[i] = '\0';

          if (flag == 1)
          {
               if (strcmp("<br>", tag)!=0 && strcmp("<hr>", tag)!=0)
                   Push(&S, tag);   
           }
           else 
           {
               if (strcmp("<br>", tag)!=0 && strcmp("<hr>", tag)!=0)
               {
                    if (strcmp(tag, Pop(S)) != 0) {result = 0; break; }
               }
           }

        }
    }
    if (is_Empty(S) == 0) result = 0;

    if (result == 0)  printf("wrong");
    else  printf("correct");
    return 0;
}
