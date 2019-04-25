#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 500
#define type char

struct SArray {
        type arr[N];
        int topIndex;
};

void pushArray(struct SArray *a, type value);
type popArray(struct SArray *a);
type topArray(struct SArray *a);
struct SArray* initSArray();
int isEmptyArray(struct SArray *a);


struct S{
        struct Elem *headElem;//голова списка
	struct Elem *firstElem;
};

struct Elem{
        char value;
        struct Elem *next;
};

void push(struct S *a, char value);
char pop(struct S *a);
char top(struct S *a);
struct S* initS();
int isEmpty(struct S *a);


void push(struct S *a, char value){//вставка в голову списка
        struct Elem *newElem = malloc(sizeof(struct Elem));
        newElem -> value = value;
        if(isEmpty(a)){
                newElem -> next = NULL;
		a -> firstElem = newElem;
        }
        else newElem -> next = a -> headElem;
        a -> headElem = newElem;
        return;
}

void pushArray(struct SArray *a, type value){
	a -> topIndex++;
	a -> arr[a->topIndex] = value;
	return;
}

char pop(struct S *a){
        if(!isEmpty(a)){
            char Elem = top(a);
	    struct Elem *tmp = a -> headElem;
	    while(tmp->next->next){
	    	tmp = tmp->next;
	    }
	    tmp -> next = NULL;
            a -> firstElem = tmp;
            return Elem;
        }
}

type popArray(struct SArray *a){
	if(!isEmptyArray(a)){
		type elem = a->arr[0];
		memmove(a->arr, a->arr + 1, a->topIndex);	
		return elem;
	}

}

char top(struct S *a){
        if(!isEmpty(a))
            return a->firstElem->value;
}

type topArray(struct SArray *a){
	if(!isEmptyArray(a))
		return a -> arr[0];
}

int isEmpty(struct S *a){
	return a->firstElem == NULL;
}

int isEmptyArray(struct SArray *a){
	return a->topIndex == -1;
}

struct S* initS(){
        struct S *a = malloc(sizeof(struct S));
	a -> firstElem = NULL;
        a -> headElem = NULL;
        return a;
}

struct SArray* initSArray(){
	struct SArray *a = malloc(sizeof(struct SArray));
        a -> topIndex = -1;
        return a;
}

int main(){
	struct S *a1 = initS();
	struct SArray *a2 = initSArray();
        char value = 'k';
	//printf("%ld\n", clock());	
	//pushArray(a2, value);
	//printf("%ld\n", clock());	
	push(a1, value);
	printf("%ld\n", clock());	

	return 0;
}
