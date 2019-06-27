#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 10000000

typedef struct Node{
    int data;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct Queue{
  struct Node* head; 
  struct Node* tail;
}Queue;

void push(Queue* Q, int x){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    Newnode->data = x;
    Newnode->next = Q->tail; // возможно наооборот
    Newnode->prev = NULL;
    if (Q->tail) // если в списке что то было
        Q->tail->prev = Newnode;
    Q->tail = Newnode;        
    if (Q->head == NULL) Q->head = Q->tail; // если в списке ничего не было    
}

int pop(Queue** Q){
    Node* tmp = (*Q)->head;
    int x = (*Q)->head->data;
    (*Q)->head = (*Q)->head->prev;
    if ((*Q)->head == NULL) //если ничего не осталось
        (*Q)->tail = NULL;
    free(tmp);    
    return x;    
    
}

int main()
{
    Queue tmp;
    Queue* Q = &tmp;
    Q->tail = NULL;
    Q->head = NULL;
    clock_t c1 = clock();
    for (int i = 0; i < N; i++)
        push(Q,i);
    int t = (int)((clock() - c1));
    printf("time of push: %.4f sec \n", (float)t/CLOCKS_PER_SEC);    
    clock_t c2 = clock();
    for (int i = 0; i < N; i++)
        pop(&Q);
    t = (int)((clock() - c2));
    printf("time of pop: %.4f sec", (float)t/CLOCKS_PER_SEC);
    return 0;
}
