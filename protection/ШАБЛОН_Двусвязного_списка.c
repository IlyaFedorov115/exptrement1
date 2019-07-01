
#include <stdio.h>

typedef struct Node{
    int data;
    struct Node* prev;
    struct Node* next;
}Node;

Node* createNode(int x){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    Newnode->data = x;
    Newnode->prev = NULL;
    Newnode->next = NULL;
    return Newnode;
    
}

void add_first(Node** head, Node** tail,Node* Newnode)){
    Newnode->next = *head;
    Newnode->prev = NULL;
    if (*head) (*head)->prev = Newnode;
    *head = Newnode;
    if (!(*tail)) *tail = *head;
}

void add_last(Node** head, Node** tail,Node* Newnode)){
    Newnode->prev = *tail;
    Newnode->next = NULL;
    if (*tail) (*tail)->next = Newnode;
    *tail = Newnode;
    if (!(*head)) *head = *tail;
}

void add_after(Node** head, Node** tail, Node* Newnode, Node* p){
    if (!p->next) add_last(head, tail, Newnode);
    else {
        Newnode->next = p->next;
        Newnode->prev = p;
        p->next->prev = Newnode;
        p->next = Newnode;
    }
}


void add_before(Node** head, Node** tail, Node* Newnode, Node* p){
    if (!p->prev) add_first(head, tail, Newnode);
    else {
        Newnode->next = p;
        Newnode->prev = p->prev;
        p->prev->next = Newnode;
        p->prev = Newnode;
    }
}

void delete(Node** head, Node** tail,Node* del){
    if (*head == del) {
        *head = del->next;
        if (*head) *head->prev = NULL;
        else *tail = NULL;
    }
    else {
        del->prev->next = del->next;
        if (del->next) del->next->prev = del->prev;
        else *tail = NULL; // удалили последний может быть *tail = del->prev;
    }
    free(del);
}

Node* find(Node* head, int x){
    Node* q = head;
    while (q && (q->data != x)) q = q->next;
    return q;
}

int main()
{
    printf("Hello World");

    return 0;
}
