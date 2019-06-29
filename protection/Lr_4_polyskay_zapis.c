#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 300

typedef struct Node{
    int data;
    struct Node* prev;
}Node;


typedef struct Stack{
    struct Node* head;
}Stack;


void push(Stack* S, int a){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    Newnode->data = a;
    Newnode->prev = S->head;
    S->head = Newnode;
}

int pop(Stack* S){
    if(is_Empty(S)){
        printf("error\n");
        return;
    }
    else {
    Node* tmp = S->head;
    int x = S->head->data;
    S->head = S->head->prev;
    free(tmp);
    return x;
    }
}

int count(Stack* S){
    int res = 0;
    Node* tmp = S->head;
    while (tmp){
        res++;
        tmp = tmp->prev;
    }
    return res;
}

int is_Empty (Stack* S)
  {
    if (S->head) {
        return 0;
    }
    return 1;
}

int main() {
    int result = 1;
    Stack *S = malloc(sizeof(Stack));
    S->head = NULL;
    char tmp[20];
    char c = 'a';
    while (c != '\n'){
        scanf("%s", tmp);
        c = getchar();
        if (tmp[0] >= '0' && tmp[0] <= '9'){
            push(S, atoi(tmp));
        }
        else if (tmp[0] == '-' && tmp[1] >= '0' && tmp[1] <= '9') push(S, atoi(tmp));
       else {
              int a = pop(S); printf("\n pop = %d ", a);
              int b = pop(S); printf("\n pop = %d", b);
              if (!(a && b)) return 0;
         if (tmp[0] == '+'){
            push(S, a+b);
            printf("\npush + = %d", S->head->data);
         }  
         if (tmp[0] == '-'){
            push(S, b-a);
            printf("\npush - = %d", S->head->data);
         }
         if (tmp[0] == '*'){
            push(S, a*b);
            printf("\npush * = %d", S->head->data);
         }
         if (tmp[0] == '/'){
            push(S, b/a);
            printf("\npush / = %d", S->head->data);
         }
     }    
        
    }
    if (count(S) == 1) printf("res = %d", pop(S));
    else printf("error");
    return 0;
}
