#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

// Объявление шаблонов функций
struct sExpr;   // структура узла списка
void readLisp(istream &,sExpr*&);
void writeLisp (sExpr* x,ostream &stream);
void writeSeq (sExpr* x,ostream &stream);
void readSexpr(istream &,char, sExpr*& ); 
void readSeq (istream &,sExpr*& );


struct twoPtr{       // вспомогательная структура  
	sExpr *head;    // хранящая указатели на head и tail
	sExpr *tail;
};	

struct sExpr{
  bool tag;            // true: если atom, false:если pair
  union  
	 {
	   char atom;      // значение атома
	   twoPtr pair;	
	  }node;		
};			 

struct node{
	char *hd;
	node *tl;
	node()     // конструктор
	    {
	       hd = NULL; 
	       tl = NULL; 
	    }
};	

typedef node* list;
typedef sExpr* lisp;
int step = 0;  
string file = "f";
string con = "c";
string stop = "n";
string again = "y";


bool isAtom(const lisp obj){	   // проверка на атомарность
    if(obj == NULL) return false;
	else return (obj->tag);
}

bool isNull(const lisp obj){ 
    return (obj == NULL);
}


lisp retHead(const lisp obj){      // Получение указателя на head 
   if (obj != NULL)                // Условие работы: не null (obj)
	   {
	     if (!isAtom(obj)) 
	         return obj->node.pair.head;
	     else{ 
	         cerr << "Ошибка: ф-я retHead(передан Атом) \n";  exit(1); 
	        }
	   }
   else { 
	   cerr << "Ошибка: ф-я retHead(передан NULL) \n"; exit(1);
	  }
}


lisp retTail(const lisp obj){   // Получение указателя на tail
   if (obj != NULL)               // Условие работы: не null (obj)
     {
        if (!isAtom(obj))  
           return obj->node.pair.tail;
	    else { 
	       cerr << "Ошибка: ф-я retTail(передан Атом) \n"; exit(1); }
     }
   else 
	  { 
	    cerr << "Ошибка: ф-я retTail(передан NULL) \n"; exit(1);
	  }
}



// Функция-конструктор
lisp cons(lisp hd, lisp tl){    // Условие работы: not isAtom (tl)
   lisp obj;
   if (isAtom(tl)){ 
       cerr << "Ошибка: функция cons(*,Атом) 45\n"; exit(1);}
   obj = new sExpr; 
   if (obj == NULL){
       cerr << "Ошибка: функция cons Память не выделена\n"; exit(1); } 	// не выделена динамическая память
   obj->tag = false;
   obj->node.pair.head = hd;
   obj->node.pair.tail = tl;
   return obj;	
}


// Функция создания атома
lisp makeAtom(char x){
    lisp obj;
	obj = new sExpr;
	obj->tag = true;
	obj->node.atom = x;
	return obj;
}


void destroy(lisp obj) {
   if (obj != NULL) 
     {
	  if (!isAtom(obj)) 
	     {
			destroy(retHead(obj));
			destroy(retTail(obj));
		 } 
	 delete obj;
	}
}

void destroy(list s){
   if (s != NULL) 
      {
		destroy(s->tl);
		delete s->hd;
		delete s;
	  }
}


/*
// получение значения атома
char getAtom(const lisp obj){
   if (!isAtom(obj)) 
    { 
     cerr << "Error: getAtom(obj) for !isAtom(obj) \n";
     exit(1);
    }
   else return (obj->node.atom);
}
*/


/// ФУНКЦИИ СЧИТЫВАНИЯ//...........................

void readLisp(istream &stream, lisp& obj){
  char x;
  do 
    stream >> x;
  while (x == ' ');
  readSexpr(stream , x , obj);
} 


void readSexpr(istream &stream,char prev, lisp& obj){   //prev - ранее прочитанный символ
  if (prev == ')'){
      cerr << "\033[1;31mОшибка!\033[0m Символ \033[1;34m\')\'\033[0m встречен раньше \033[1;34m\'(\'\033[0m " << endl; exit(1);  } 
  else if (prev != '(') 
    {
     obj = makeAtom(prev);
    }  
  else 
     readSeq(stream,obj);
} 


void readSeq(istream &stream, lisp& obj){	
  char x; 
  lisp p1;
  lisp p2;
  if (!(stream >> x)){
      cerr << "\033[1;31mОшибка!\033[0m Не найден конец списка." << endl; exit(1);
  }
  while(x == ' ') 
	 stream >> x;
  if (x == ')') 
     obj = NULL;
  else{
	 readSexpr(stream,x, p1);
	 readSeq(stream,p2);
	 obj = cons(p1, p2);
	} 
} 



// функции вывода 

void writeLisp(lisp x,ostream &stream = cout){  //процедура вывода списка с обрамляющими его скобками
	if (isNull(x))                            //пустой список выводитс¤ как () 
	   stream << " ()";
	else if (isAtom(x)) 
	   stream << ' ' << x->node.atom;
	else { 
	   stream << " (" ;
	   writeSeq(x,stream);
	   stream << " )";
	}
} 


void writeSeq(lisp x,ostream &stream = cout)    //выводит элементы без скобок
{
	if (!isNull(x)) {
		writeLisp(retHead(x),stream); 
		writeSeq(retTail(x),stream);
	}
}


void printList(list s)
{	if (s != NULL){
		cout << *s->hd << " " ;
		printList(s->tl);
	}
}

void printList2(list s){
    cout << "( ";
    printList(s);
    cout << ")";
}

/*
lisp copy(const lisp x)
{	if  (isNull(x)) 
        return NULL;
	else if (isAtom(x)) 
	    return makeAtom(x->node.atom);
	else 
	    return cons(copy(retHead(x)), copy(retTail(x)));
} //end copy-lisp
*/

// функция соединения списков
lisp concat(const lisp y, lisp z){
  if (isNull(y))
	  return z;
  else 
      return cons(retHead(y), concat(retTail(y), z));
} 


/*
void recDepth(const lisp obj, int depth){
    cout <<"\n\n" << "\033[1;32mГлубина \033[0m" << depth;
    cout <<"\n" << "Шаг "<< step << "  Промежуточный результат = "; step++;
    write_seq(cout, obj);
    cout << "\n";    
} */


/*
lisp align(lisp obj, int depth = 0){
    if (isNull(obj)) 
        return NULL;
    else 
      {
        if (isAtom(obj)) 
           return cons(obj, NULL);
        else 
          {
            if (isAtom(retHead(obj))) {
               return cons(retHead(obj),alignment(retTail(obj), depth+1));    }
            else {  
               return concat(alignment(retHead(obj)), alignment(retTail(obj), depth+1));   }
          }
      }
} */



void toLower(string &str){
    int i = 0;
    while (str[i]){
        str[i] = tolower(str[i]);
        i++;
    }
}

void fixed(){
    cin.clear(); 
    while (cin.get() != '\n');
}


void printInfo(list s, int depth, ostream &stream = cout){
    stream << "Шаг " << step << ":";
    for (int i = 0; i < depth; i++)
        stream << " ";
    printList2(s);
    cout << "\n";
}


list consList(char x, list s, int depth = 0){	
    list p;
	p = new node; 
	if ( p != NULL)	{ 	
		p->hd = new char;
		*p->hd = x;
		p->tl = s;
		step++;
		printInfo(p,depth);
		return p;	
	}
	else {
	    cerr << "Memory not enough\n";exit(1);}
}

list align(lisp obj, list t, int depth = 0){
    if(isNull(obj))
        return t;
    else 
       {
         if(isAtom(obj))
              return consList(obj->node.atom,t,depth);
         else
              return align(retHead(obj),align(retTail(obj),t,depth+1),depth);
        }
}


int main()
{
    string str;
    do{
        list s; lisp obj;
        cout << "Считать из файла - \033[1;34m\'f\' , \033[0m или с консоли - \033[1;34m\'c\': \033[0m";
        cin >> str; toLower(str);
        while (str!=file && str!=con)
          {
            cout << "\033[1;31mОшибка ввода, попробуйте снова: \033[0m" ;
            cin >> str;
          }
        if (str == file)
            {
              string fileName;  
              cout << "\033[0;32mВведите имя файла:\033[0m ";
              cin >> fileName;
              ifstream in(fileName.c_str(),ios::in | ios::binary);
              while(!in)
                  {
                   cout <<"\033[1;31mНе удается открыть файл.Введите корректное имя: \033[0m";
                   cin >> fileName;
                   in.open(fileName.c_str(),ios::in | ios::binary);
                  }
              readLisp(in,obj);
            }
         else 
            {
              cout << "\033[0;32mВведите список в обрамляющих скобках ():\033[0m ";    
              readLisp(cin,obj);
              fixed();
            } 
        cout << "Считанный список: ";
        writeLisp(obj); cout << "\n";
        s = align(obj,NULL);
        cout << "\n" << "\033[0;32mРезультат:\033[0m " ;
        printList2(s);
        cout << "\n" << "Вы хотите продолжить \033[1;34m[y/n]\033[0m ? : ";
        cin >> str;
        toLower(str);
        while (str!=stop && str!=again){
            cout << "\nОшибка, введите [y/n] :";
            cin >> str; toLower(str);
        }
        cout << "\n"; step = 1;
        destroy(obj); destroy(s);
    }while(str != "n");

    return 0;
}