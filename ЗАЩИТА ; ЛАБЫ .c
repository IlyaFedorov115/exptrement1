/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 1000000

typedef struct Stack {
    int data[10000000];
    int size;
}Stack;

Stack* initStack(){
    Stack* myS = (Stack*)malloc(sizeof(Stack));
    myS->topIndex = -1;
    return myS;
} ///////надо т.к не выделяет память для более 1 000 000 элементов


void push(Stack *stack, int value) {
    if (stack->size >= MAX_SIZE) {
        printf("Стек переполнен\n");
    }
    stack->data[stack->size] = value;
    stack->size++;
}

int pop(Stack *stack) {
    if (stack->size == 0) {
        printf("Стек пуст\n");;
    }
    stack->size --;
    return stack->data[stack->size];
}


int peek(Stack *stack) {
    if (stack->size <= 0) {
        printf("Стек пуст\n");
    }
    return stack->data[stack->size - 1];
}

/*
void printStackValue(const T value) {
    printf("%d", value);
}
 
void printStack(const Stack_t *stack, void (*printStackValue)(const T)) {
    int i;
    int len = stack->size - 1;
    printf("stack %d > ", stack->size);
    for (i = 0; i < len; i++) {
        printStackValue(stack->data[i]);
        printf(" | ");
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}
*/

// можно сделать с strcpy и массивами чтобы работала время

int main()
{
   long int ttime;

//   ttime = time(NULL);
    Stack s;
    s.size = 0;
 ///   ttime = time(NULL);
  //  printf("%ld",ttime);
    for (int i = 0; i < MAX_SIZE; i++) {
        push(&s, i);
    }
  //  long int ttime2 = time(NULL) - ttime;
  //  ttime = time(NULL);
 //   printf("\n%ld",ttime);
    
    
    for (int i = 0; i < MAX_SIZE; i++) {
        pop(&s);
    }
    
    
  //  printf();
    return 0;
}










#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

typedef struct Stack{
    int s[10000000];
    int topIndex;
}Stack;

Stack* initStack(){
    Stack* myS = (Stack*)malloc(sizeof(Stack));
    myS->topIndex = -1;
    return myS;
}

void pop(Stack** myS){    
    (*myS)->topIndex -= 1;    
}

void push(Stack** myS, int s1) {
    (*myS)->topIndex += 1;
    (*myS)->s[(*myS)->topIndex] = s1;
}

int top(Stack* myS){
    int q = myS->topIndex;
    return(myS->s[q]);
}

int main(){
    Stack* stack;
    stack = initStack();
    clock_t c1 = clock();
    for(int i = 1; i <= 10000000; i++){
        push(&stack, i);
    }
    printf("push: %d\n", (int)((clock() - c1)));
    clock_t c2 = clock();
    for(int i = 1; i <= 10000000; i++){
        pop(&stack);
    }
    printf("pop: %d\n", (int)((clock() - c2)));
    return 0;    
} 
