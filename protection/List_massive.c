#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#define MAX_SIZE 10000000

typedef struct Stack{
    int data[MAX_SIZE];
    int size;
}Stack;


void pop(Stack* S){ 
    S->size--;
    return S->data[S->size];   
}


void push(Stack* S, int x) {
    S->data[S->size] = x;   
    S->size++; 
}


int top(Stack* S){
   return S->data[S->size - 1];
}

int main(){
    Stack* S= (Stack*)malloc(sizeof(Stack));
    S->size = 0;
    long long ttime;
    ttime = time(NULL);
    clock_t c1 = clock();
    for(int i = 1; i <= MAX_SIZE; i++){
        push(S, i);
    }
    int t = (int)((clock() - c1));
    printf("time of push: %.3f sec \n", (float)t/CLOCKS_PER_SEC);
    clock_t c2 = clock();
    for(int i = 1; i <= MAX_SIZE; i++){
        pop(S);
    }
  t = (int)((clock() - c2));
    printf("time of pop: %.3f sec", (float)t/CLOCKS_PER_SEC);
    return 0;    
} 
