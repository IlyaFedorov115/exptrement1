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

void Push(Stack** S, char* tag){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    strcpy(Newnode->data, tag);
    Newnode->prev = (*S)->head;
    (*S)->head = Newnode;
    printf("\t\n Добавили = %s", (*S)->head->data);
}


void Pop (Stack** S){
    if ((*S)->head == NULL){
        printf("Стек пуст");
        return;
    }
    Node* tmp = (*S)->head;
    char *tag;
  //  strcpy(tag, (*S)->head->data);
//    Node* tmp = (*S)->head;
    (*S)->head = (*S)->head->prev;
    free(tmp);
}

Node* top(Stack* S){
    return S->head;
}

int is_Empty (Stack* S)
  {
    if (S->head) {
        printf("no pysto");
        return 0;
    }
    return 1;
}

void print_list(Stack* S){
    Node* tmp = S->head;
    while (tmp){
        printf("\nEl = %s", tmp->data);
        tmp = tmp->prev;
    }
}

int main() {
    int result = 1;
    int flag = 1;  char c;
    Stack *S = malloc(sizeof(Stack));
    S->head = NULL;
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
                    if (strcmp(tag, top(S)->data) != 0) {result = 0; break; }
                    Pop(&S);
               }
           }

        }
    }
    if (is_Empty(S) == 0) result = 0;
     print_list(S);

    if (result == 0)  printf("wrong");
    else  printf("correct");
    return 0;
}
