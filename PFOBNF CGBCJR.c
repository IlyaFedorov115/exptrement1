#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>


#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102
 
typedef int T;
typedef struct Node_tag {
    T value;
    struct Node_tag *next;
} Node_t;


void push(Node_t **head, T value) {
    Node_t *tmp = malloc(sizeof(Node_t));
if (tmp == NULL) {
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}



Функция pop берёт первый элемент (тот, на который указывает вершина), перекидывает указатель на следующий элемент и возвращает первый. Здесь есть два варианта – можно вернуть узел или значение. Если вернём значение, то придётся удалять узел внутри функции
?
Node_t* pop1(Node_t **head) {
    Node_t *out;
    if ((*head) == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    return out;
}



T pop2(Node_t **head) {
    Node_t *out;
    T value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}


T peek(const Node_t* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}



size_t getSize(const Node_t *head) {
    size_t size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}

Конечно, можно хранить размер отдельно, можно обернуть стек со всеми данными ещё в одну структуру и т.д. Рассмотрим всё это при более подробном изучении списков.

Тестируем
?
void main() {
    int i;
    Node_t *head = NULL;
    for (i = 0; i < 300; i++) {
        push(&head, i);
    }
    printf("size = %d\n", getSize(head));
    while (head) {
        printf("%d ", peek(head));
        printf("%d ", pop2(&head));
    }
    _getch();
}

или так
?
void main() {
    int i;
    Node_t *head = NULL;
    Node_t *tmp;
    for (i = 0; i < 300; i++) {
        push(&head, i);
    }
    printf("size = %d\n", getSize(head));
    while (head) {
        printf("%d ", peek(head));
        tmp = pop1(&head);      
        printf("%d ", tmp->value);
        free(tmp);
    }
    _getch();
}

