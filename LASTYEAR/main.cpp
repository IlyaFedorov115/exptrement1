#include <iostream>
#include <cstring>
#include <fstream>
#define QUIT "quit"
#define SIZE 30

using namespace std;

int step = 1;

class Vector {
 public:
    int* data;
    int size = 0;
};


ostream &operator<<(ostream &stream, Vector obj){
    int i = 0; stream << "(";
    for (i; i < obj.size-1 ; i++)
    {
        stream << obj.data[i] << ", ";
    }
    stream << obj.data[i] << ")"<<"\n";
    return stream;
}


void readFile(const char *fileName, Vector &obj, int number){
    char ch; int count = 0;
    ifstream in(fileName, ios::in | ios::binary);
    while (!in){
        cout << "Не удается открыть файл.Введи корректное имя: \n";
        char buf[SIZE];
        cin.get(buf,SIZE, '\n');
        ifstream in(fileName, ios::in | ios::binary);
        //return ;
    }
    char buf[20];
    while(in && number != 0 && in.getline(buf, 20, ','))
      {
         obj.data[count++] = atoi(buf);
         number--;
      }
      obj.size = count; in.close();
      cout << "Считанный из файла вектор: " << obj << "\n";
}


void recDepth(Vector &obj, int pos , int length, int depth){
    cout <<"\n\n" << "\033[1;32mГлубина \033[0m" << depth;
    cout <<"\n" << "Шаг "<< step << "  Вектор = "<< obj; step++;
    cout << "Обрабатываемая часть : " ;
    for (int i = pos; i < pos+length; i++)
        cout << obj.data[i] << " ";
    cout << "\n";    
}


void transformVector(Vector &obj, int pos , int length,int depth = 0){
    recDepth(obj, pos, length, depth);
    if(length > 2)
     {
        if(length % 2 == 1)
           {
            transformVector(obj,pos,length/2+1, depth+1);
            transformVector(obj,pos + length/2 +1,length/2, depth+1);
           }
        else
           {
             transformVector(obj,pos,length/2,depth+1);
             transformVector(obj,pos + length/2,length/2, depth+1);
           }
    }

    if(length == 2 && (obj.data[pos+1] < obj.data[pos]))
      {
        int x = obj.data[pos];
        obj.data[pos] = obj.data[pos+1];
        obj.data[pos+1] = x;
       }
    else if (length == 2 && (obj.data[pos+1]) >= obj.data[pos]){
    }   

    if(length == 1) return;
}


void toLower(string &str){
    int i = 0;
    while (str[i]){
        str[i] = tolower(str[i]);
        i++;
    }
}


void readConsole(Vector &obj){
  int tmp;
  cout << "\033[1;34mВведите координаты вектора: \033[0m\n";
  for (int i = 0; i < obj.size ;i++)
    {
       cout << i+1 << "-e число: ";
       while (!(cin >> tmp)|| (cin.peek() != '\n'))
         {
            cin.clear();
            while (cin.get() != '\n');
            cout << "\033[1;31mОшибка ввода. Попробуйте еще раз ввести число: \033[0m";
         }
       obj.data[i] = tmp;
    }
}

string file = "file";
string con = "con";
string stop = "n";
string again = "y";

int main()
{
    string str;
    do{
        Vector obj; 
        cout << "Введите длину вектора: ";
        while (!(cin >> obj.size)|| (cin.peek() != '\n') || obj.size <= 0)
           {
            cin.clear();
            while (cin.get() != '\n');
            cout << "\033[1;31mОшибка ввода. Попробуйте ввести положительное число: \033[0m";
            }
        obj.data = new int[obj.size];
        cout << "Считать из файла - \033[1;34m\'file\' , \033[0m или с консоли - \033[1;34m\'con\': \033[0m";
        cin >> str; toLower(str);
        while (str!=file && str!=con)
          {
            cout << "\033[1;31mОшибка ввода, попробуйте снова: \033[0m" ;
            cin >> str;
          }
          if (str == file)
            {
              string fileName;  
              cout << "Введите имя файла: ";
              cin >> fileName;
              readFile(fileName.c_str(),obj,obj.size);
            }
          else {
            readConsole(obj);
             }  
        transformVector(obj, 0, obj.size, 1);
        cout << "\n" << "Результат: " << obj;
        cout << "\n" << "Вы хотите продолжить \033[1;34m[y/n]\033[0m ? : ";
        cin >> str;
        toLower(str);
        while (str!=stop && str!=again){
            cout << "Ошибка, введите [y/n] :";
            cin >> str; toLower(str);
        }
        cout << "\n";
        step = 1;
    }while(str != "n");
    
    return 0;
}
