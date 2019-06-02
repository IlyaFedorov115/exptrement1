#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

typedef struct StackElem{
    int s;
    struct StackElem *next;
}StackElem;

typedef struct Stack{
     StackElem *topElem;
}Stack;

Stack* initStack(int s){
    StackElem* elem = (StackElem*)malloc(sizeof(StackElem));
    elem -> s = s;
    elem -> next = NULL;
    Stack* head = (Stack*)malloc(sizeof(Stack));
    head -> topElem = elem;
    return head;
}

void pop(Stack** head){
    free((*head)->topElem);
    (*head)->topElem = (*head)->topElem->next;
}

void push(int s, Stack** head){
    StackElem* elem = (StackElem*)malloc(sizeof(StackElem));
    elem -> s = s;
    elem -> next = (*head) ->topElem;
    (*head) -> topElem = elem;
}

int isEmpty(Stack *head) {
    if (head->topElem)
        return 0;
    else
        return 1;
}

int main(){
    Stack* stack;
    stack = initStack(0);
    clock_t c1 = clock();
    for(int i = 1; i <= 10000000; i++){
        push(i, &stack);
    }
    printf("push: %d\n", (int)((clock() - c1)));
    clock_t c2 = clock();
    for(int i = 1; i <= 10000000; i++){
        pop(&stack);
    }
    printf("pop: %d\n", (int)((clock() - c2)));
    return 0;    
} 
