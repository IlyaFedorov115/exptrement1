#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    char symbol;
    struct Node* next;
}Node;

void push(Node** head, int a, char b){
    Node* Newnode = (Node*)malloc(sizeof(Node));
    Newnode->data = a;
    Newnode->symbol = b;
    Newnode->next = *head;
    *head = Newnode;
}

char find(Node* head, int x){
    while (head){
        if (head->data == x) return head->symbol;
        head = head->next;
    }
    return '0';
}


int main()
{
    int a;
    char b;
    char result[10];
    int j = 0;
    Node* head = NULL;
    for (int i = 0; i < 7; i++){
        scanf("%d %c", &a,&b);
        char c = getchar();
        push(&head,a,b);
    }
    for (int i = 0; i < 7; i++){
        scanf("%d", &a);
        b = find(head, a);
        if (b == '0') {
            printf("error");
            return 0;
        }
        result[j++] = b;
    }
    printf("%s", result);
    

    return 0;
}
