#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 10000000

typedef struct Node{   
    int data;  
    struct Node *prev;   
}Node;


typedef struct Stack{
   struct Node *head;
}Stack;



void pop(Node** head) {
  Node* a = *head;
  *head = (*head)->prev;
  free(a); 
}

void push(Node** head, int x)
{

   Node* NewNode = (Node*)malloc(sizeof(struct Node));
   NewNode->data = x;
   NewNode->prev = *head;
   *head = NewNode;
}


// функция top
int top(Node* S) {   
    return S->data;  
}


int isEmpty(Node* head){
    if (head == NULL) return 0;
    return 1;
}
               
                     
int main(){
    Node* head = NULL;
    clock_t c1 = clock();
    
    for(int i = 0; i < MAXSIZE; i++){
        push(&head, i);
    }
    int t = (int)((clock() - c1));
    printf("time of push: %.3f sec \n", (float)t/CLOCKS_PER_SEC);
    clock_t c2 = clock();
    for(int i = 0; i < MAXSIZE; i++){
        pop(&head);
    }
    t = (int)((clock() - c2));
    printf("time of pop: %.3f sec", (float)t/CLOCKS_PER_SEC);
    printf("\nhead = %p", head);
  //  printf("head->data = %d", head->data);
    return 0;    
}
