#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SIZE 10000000

typedef struct queue{   
    int data[SIZE];  
    int size, tail, head;  
}Queue;

// функция pop

void push(Queue* S, int x){
    if (S->size == SIZE) {
        printf("Queue is over");
        return;
    }
    S->tail++;
    if (S->tail >= SIZE) S->tail = 0;
    S->data[S->tail] = x;
    S->size++;
}

int pop(Queue* S){
   if (S->size == 0) {
        printf("Queue is lost");
        return;    
     }
    int tmp = S->data[S->head];
    S->head++;
    if (S->head >= SIZE) S->head = 0;
    S->size--;
    return tmp;
}

int is_empty(Queue* S){
    if (S->size == 0) return 0;
    return 1;
}

int count(Queue* S){
    return S->size;
}

int main() {   
    Queue* S = (Queue*)malloc(sizeof(Queue));
    clock_t c1 = clock();
    for (int i = 0; i < SIZE; i++){
        push(S, i);
    }
    int t = (int)((clock() - c1));
    printf("time of push: %.4f sec \n", (float)t/CLOCKS_PER_SEC); 
     clock_t c2 = clock();
    for (int i = 0; i < SIZE; i++){
        pop(S);
    }
    t = (int)((clock() - c2));
    printf("time of pop: %.4f sec", (float)t/CLOCKS_PER_SEC);
     return 0; 
} 
