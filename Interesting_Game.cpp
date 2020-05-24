/******************************************************************************
for (int i = 0; i < hight; i++) {
        for (int j = 0; j <width; j++){
            //map[i][j]->show(window);  
            mvwaddch(gameWindow, i, j, a);
        } 
     }  
     wrefresh(gameWindow);
   

*******************************************************************************/

#include <iostream>
#include <typeinfo>
    #include <stdio.h>

    #include <unistd.h>

    #include <stdlib.h>

    #include <string.h>

    #include <curses.h>
#include <vector>
using namespace std;



// Константы

#define Y_WIN 3
#define X_WIN 3  

const int HUMANCLERIC_DM = 55;
const int HUMANGUARD_DM = 130;
const int HUMANTREBUCHET_DM = 60;


const int HUMANCLERIC_HP = 55;
const int HUMANGUARD_HP = 130;
const int HUMANTREBUCHET_HP = 60;
const int HUMANCLERIC_ARM = 5;
const int HUMANGUARD_ARM = 15;
const int HUMANTREBUCHET_ARM = 5;


const char HUMANCLERIC_VIEW = '@';
const char HUMANGUARD_VIEW  = '&';
const char HUMANTREBUCHET_VIEW  = '=';


const char GRETCHIN_VIEW = '&';
const char TROLSMAGIC_VIEW = '@';
const char GOBLINCATAPULT_VIEW = '=';


const int GRETCHIN_DM = 45;
const int TROLSMAGIC_DM = 70;
const int GOBLINCATAPULT_DM = 40;

const int GRETCHIN_HP = 45;
const int TROLSMAGIC_HP = 70;
const int GOBLINCATAPULT_HP = 40;
const int GRETCHIN_ARM = 0;
const int TROLSMAGIC_ARM = 5;
const int GOBLINCATAPULT_ARM = 0;


const int ACTION_POINT = 60;
//



const int HEAL_HP = 25;
const int HEALING_CHARACTER = '+';

const int MINE_DAMAGE = 35;
const int MINE_CHARACTER = '*';

const int PORTAL_CHARACTER = 'O';
const int UNKNOWN_CHARACTER = '?';


class Cell;
class GameField;
class GameObj;
class Terrain;
typedef GameObj* Obj;
typedef Cell* pCell;
typedef Terrain* pTerrain;


class Unit;
enum Event
{
  UNIT_DIE,
  UNIT_CREATE,
  /*......*/
};


enum UnitType{
  MAGIC,
  WARRIOR,
  SIEGE
};


class Observer
{
 public:
  virtual ~Observer() {}
  virtual void updateNotify(Unit* someone, Event event) = 0;
};


class PublicObj
{
 private:
    std::vector<Observer*> observers_;
 public:
  virtual void addObserver(Observer* observer);
  virtual void removeObserver(Observer* observer);
 protected:
  virtual void notify(Unit* someone, Event event);  
};


void PublicObj::addObserver(Observer* observer)
{
  observers_.push_back(observer);
}


void PublicObj::removeObserver(Observer* observer)
{
    int index = 0;
  for(vector<Observer*>::size_type i = 0; i != observers_.size(); i++, index++){
         if (observers_[i] == observer)
         {
          observers_.erase(observers_.begin() + index);
          break;
         }
    }
}


void PublicObj::notify(Unit* someone, Event event)
{
  for(vector<Observer*>::size_type i = 0; i != observers_.size(); i++)
  {
    observers_[i]->updateNotify(someone, event);
    }
}








class GameObj : public PublicObj
{
    friend GameField;
    private:
        void initVariables();
    protected:
        static int Count;
        int x_;
        int y_; 
        GameField* field_;
    public:
        virtual void setPosition(GameField* field, int x, int y);
        GameObj();
        virtual void show() {} 
        virtual ~GameObj() {}
        virtual int getSpace(){
           return 1;
        }
        virtual bool isHero() { return false; }
        virtual bool isNeutral() { return false; }
        virtual GameObj* copy(GameField* field) {};
        virtual void affectBlock() {}
        virtual void update() {}
        virtual void updateState() {}
        static int getCount()
        {
            return Count;
        }     
};
int GameObj::Count = 0;





/* ВОТ ТУТ ПИПЕЦ
ПОТОМУ ЧТО
МЕТОД SHOW ЕСТЬ И В 
ОБЪЕКТЕ И В 
ИНТЕРФЕЙСУ */




class IUnit{
  public:
 //  virtual void show() = 0;
   virtual bool move(int x, int y) = 0;
   //virtual bool isDead() = 0;
   /* если мертв, то удалить */
   //virtual void loseHp() = 0;
   //virtual int getHp() = 0;
    //void moveTo(int x, int y) = 0;
};

const int GRASS_COST = 10;
//const int ...
const bool GRASS_MOVEABLE = true;
const int GRASS_BACK_COLOR = 10;
const int GRASS_CHAR_COLOR = 10;
const int GRASS_CHARACTER = '.';



const int RIVER_COST = 10;
//const int ...
const bool RIVER_MOVEABLE = true;
const int RIVER_BACK_COLOR = 10;
const int RIVER_CHAR_COLOR = 10;
const int RIVER_CHARACTER = '~';


const int ROCK_COST = 10;
//const int ...
const bool ROCK_MOVEABLE = false;
const int ROCK_BACK_COLOR = 10;
const int ROCK_CHAR_COLOR = 10;
const int ROCK_CHARACTER = 'a';




class TerrainTexture
{
protected:
    int background_color;
    int char_color;
    int character;
public:
    void getTexture(int&,int&,int&);
    TerrainTexture();
    //virtual ~TerrainTexture();
    
};

TerrainTexture::TerrainTexture():
 background_color(0),char_color(0),character(0)
{}


void TerrainTexture::getTexture(int& background_color_, int& char_color_, int& character_)
{
    background_color_ = this->background_color;
    char_color_ = this->char_color;
    character_ = this->character;
}



class GrassTexture : public TerrainTexture
{   
 /* something else */
public:
     GrassTexture();
     virtual ~GrassTexture() {}
    
};

/*
GrassTexture::GrassTexture() : 
background_color(GRASS_BACK_COLOR), char_color(GRASS_CHAR_COLOR), character(GRASS_CHARACTER)
{ } */
GrassTexture::GrassTexture()
{
    background_color = GRASS_BACK_COLOR; 
    char_color = GRASS_CHAR_COLOR; 
    character =  GRASS_CHARACTER;
}


/* Перегрузить конструторк чтобы перелавался боол для открытия всей краты */
class AttributeTerrain
{
  protected:
    Obj obj;
    enum TerrainState {
        OPEN,
        CLOSED,
        /* ... */
    };
    TerrainState state;
};


class ITerrain
{
public:
    virtual int getMovementCost() = 0;
    virtual bool isMovement() = 0;
    virtual void show(WINDOW*) {}
    virtual void affect() = 0;
    virtual void open() = 0;
    virtual void deleteObj() {}
    virtual Obj getObj() = 0;
    virtual bool setObj(Obj) = 0;
    virtual void addObj(Obj obj_) {}
    //virtual void close() = 0;
    //virtual void update() = 0;
  //virtual void copy(Terrain& other, GameField* field) = 0;
};


/* Переопределить стандартные методы или же */

class Mine;
class Healing;
class GameField;
class Unit;








class Terrain : public ITerrain, public AttributeTerrain
{
protected:
   int cellX, cellY;
public:
    virtual bool setObj(Obj obj_) override;
    virtual Obj getObj() override;
    virtual void addObj(Obj obj_) override;
    virtual void deleteObj() override;  
    virtual void open() override;
    Terrain(int, int);
};


Terrain::Terrain(int x, int y)
 : cellX(x), cellY(y) {}


Obj Terrain::getObj()
{
   return obj;
}


void Terrain::deleteObj(){
  if (this->obj)
  {
       delete this->obj;
       setObj(nullptr);
  }
}
  
  
  
Unit& operator+=(Unit& left, const Mine& right);
Unit& operator+=(Unit& left, const Healing& right);
  
  
  


bool Terrain::setObj(Obj obj_)
{
  this->obj = obj_;
  if(obj){
      if (obj->isHero())
           open();
  }
  return true;
}


void Terrain::open()
{
    this->state = OPEN;
    show(stdscr);
}




class Grass : public Terrain
{
private:
    static GrassTexture* texture;
public:
    bool isMovement() override;
    int getMovementCost() override;
    void show(WINDOW*) override;
    void affect() override {}
    Grass(int,int);
    //virtual ~Grass();
};
GrassTexture* Grass::texture = nullptr;



bool Grass::isMovement()
{
  /* STATE ВЛИЯЕТ */
    if (obj && obj->isNeutral()) return true;
    if (obj == nullptr) return true;
    return false;
} 


int Grass::getMovementCost()
{
    ///return movementCost;
  return GRASS_COST;
}



Grass::Grass(int x, int y) //: movementCost(GRASS_COST),  moveable(GRASS_MOVEABLE), 
 : Terrain(x, y)
{
    if (texture == nullptr){
        texture = new GrassTexture;
    }
    this->obj = nullptr;
    this->state = CLOSED;
    //open();
}


void Grass::show(WINDOW* window)   /* need do set style */
{
    int background_color, char_color; 
    int character;
    texture->getTexture(background_color, char_color, character);
    wmove(window, cellY, cellX);
    if (state == OPEN){
       waddch(window, character | COLOR_PAIR(1) | A_BOLD);
       if(obj)  obj->show();
    }
    else {
      waddch(window, '.' | A_DIM);
    }
}



class TerrainProxy : public ITerrain
{
   protected:
      Terrain* realTerrain;
      int cellX, cellY;
   public:
      void show(WINDOW*) override;
      Obj getObj() override;
      void deleteObj() override; 
      void affect() {}
      TerrainProxy(int x, int y);
};


TerrainProxy::TerrainProxy(int x, int y)
 : cellX(x), cellY(y), realTerrain(nullptr) {}


void TerrainProxy::deleteObj()
{
  if (realTerrain != nullptr){
     realTerrain->deleteObj();
  }
}

void TerrainProxy::show(WINDOW* window)
{
    if (realTerrain == nullptr){
        /* установка приглушенного серового */
        mvwaddch(window, cellY, cellX, '.' | A_DIM);
    }
    else{
        realTerrain->show(window);
    }
}



Obj TerrainProxy::getObj()
{
   if (realTerrain == nullptr){
      return nullptr;
   }  
   return realTerrain->getObj();
}





class GrassProxy : public TerrainProxy{
    public:
        int getMovementCost() override;
        bool isMovement() override;
        void open() override;
        bool setObj(Obj obj_) override;
        GrassProxy(int x, int y);
        GrassProxy(int x, int y, bool tmp);
        virtual ~GrassProxy();
};


bool GrassProxy::setObj(Obj obj_)
{
   if (realTerrain == nullptr){
     realTerrain = new Grass(cellX, cellY);
   }
  
   return realTerrain->setObj(obj_);
}



GrassProxy::~GrassProxy()
{
    if (realTerrain != nullptr){
        delete realTerrain;
    }
}


GrassProxy::GrassProxy(int x, int y) 
: TerrainProxy(x,y) {}



GrassProxy::GrassProxy(int x, int y, bool tmp) 
 : TerrainProxy(x,y)
{
    if (tmp == true){
        realTerrain = new Grass(cellX, cellY);
        realTerrain->open();
    }
    //realGrass = nullptr;
}



int GrassProxy::getMovementCost()
{
    return GRASS_COST;
}

bool GrassProxy::isMovement()
{
    if (realTerrain == nullptr){
        return GRASS_MOVEABLE;
    }

    else{
        return realTerrain->isMovement();
    }
}   


void GrassProxy::open()
{
    if (realTerrain == nullptr){
        realTerrain = new Grass(cellX, cellY);
    }
    realTerrain->open();
}



class RiverTexture : public TerrainTexture
{   
 /* something else */
public:
     RiverTexture();
     virtual ~RiverTexture() {}
    
};


RiverTexture::RiverTexture()
{
    background_color = RIVER_CHAR_COLOR; 
    char_color = RIVER_CHAR_COLOR; 
    character =  RIVER_CHARACTER;
}



class River : public Terrain
{
private:
    static RiverTexture* texture;
public:
    bool isMovement() override;
    int getMovementCost() override;
    void show(WINDOW*) override;
    void affect() override;
    bool setObj(Obj obj_) override;
    River(int,int);
    //virtual ~Grass();
};
RiverTexture* River::texture = nullptr;


bool River::setObj(Obj obj_)
{
  obj = obj_;
  if (obj){
    obj->affectBlock();
  }
}



bool River::isMovement()
{
    if (obj && obj->isNeutral()) return true;
    if (obj == nullptr) return true;
    return false;
}


int River::getMovementCost()
{
    ///return movementCost;
  return RIVER_COST;
}


River::River(int x, int y) //: movementCost(GRASS_COST),  moveable(GRASS_MOVEABLE), 
 : Terrain(x,y)
{
    if (texture == nullptr){
        texture = new RiverTexture;
    }
    this->obj = nullptr;
    this->state = CLOSED;
}


void River::show(WINDOW* window)   /* need do set style */
{
    int background_color, char_color; 
    int character;
    texture->getTexture(background_color, char_color, character);
    wmove(window, cellY, cellX);
    if (state == OPEN){
       waddch(window, character | COLOR_PAIR(2) | A_BOLD);
       if(obj)  obj->show();
    }
    else {
      waddch(window, '.' | A_DIM);
    }
}



void River::affect()
{
  if (this->obj){
    obj->affectBlock();
  }
}






class RiverProxy : public TerrainProxy{
    public:
        int getMovementCost() override;
        bool isMovement() override;
        void affect() override {}
        void open() override;
        bool setObj(Obj obj_) override;
        RiverProxy(int x, int y);
        RiverProxy(int x, int y, bool tmp);
        virtual ~RiverProxy();
};


bool RiverProxy::setObj(Obj obj_)
{
   if (realTerrain == nullptr){
     realTerrain = new River(cellX, cellY);
   }
  
   return realTerrain->setObj(obj_);
}



RiverProxy::~RiverProxy()
{
    if (realTerrain != nullptr){
        delete realTerrain;
    }
}


RiverProxy::RiverProxy(int x, int y) 
: TerrainProxy(x,y)  {}


RiverProxy::RiverProxy(int x, int y, bool tmp) 
 : TerrainProxy(x,y)
{
    if (tmp == true){
        realTerrain = new River(cellX, cellY);
        realTerrain->open();
    }
    //realGrass = nullptr;
}



int RiverProxy::getMovementCost()
{
    return RIVER_COST;
}

bool RiverProxy::isMovement()
{
    if (realTerrain == nullptr){
        return RIVER_MOVEABLE;
    }

    else{
        return realTerrain->isMovement();
    }
}   


void RiverProxy::open()
{
    if (realTerrain == nullptr){
        realTerrain = new River(cellX, cellY);
    }
    realTerrain->open();
}

















class RockTexture : public TerrainTexture
{   
 /* something else */
public:
     RockTexture();
     virtual ~RockTexture() {}
    
};

/*
GrassTexture::GrassTexture() : 
background_color(GRASS_BACK_COLOR), char_color(GRASS_CHAR_COLOR), character(GRASS_CHARACTER)
{ } */ 
RockTexture::RockTexture()
{
    background_color = ROCK_CHAR_COLOR; 
    char_color = ROCK_CHAR_COLOR; 
    character =  ROCK_CHARACTER;
}



class Rock : public Terrain
{
private:
    static RockTexture* texture;
public:
  Obj getObj() override;
  bool setObj(Obj obj_) override;
    bool isMovement() override;
    int getMovementCost() override;
    void show(WINDOW*) override;
    void affect() override {}
    Rock(int,int);
    //virtual ~Grass();
};
RockTexture* Rock::texture = nullptr;



Obj Rock::getObj()
{
  return nullptr;
}


  
/* ПРОВЕРКА ДОБАВЛИСЯ ЛИ */
bool Rock::setObj(Obj obj_)
{
  return false;
}



bool Rock::isMovement()
{
  /* STATE ВЛИЯЕТ */
    return false;
}


int Rock::getMovementCost()
{
    ///return movementCost;
  return ROCK_COST;
}


Rock::Rock(int x, int y) //: movementCost(GRASS_COST),  moveable(GRASS_MOVEABLE), 
 : Terrain(x,y)
{
    if (texture == nullptr){
        texture = new RockTexture;
    }
    this->obj = nullptr;
    this->state = CLOSED;
}


void Rock::show(WINDOW* window)   /* need do set style */
{
    int background_color, char_color; 
    int character;
    texture->getTexture(background_color, char_color, character);
    wmove(window, cellY, cellX);
    if (state == OPEN){
       waddch(window, character | COLOR_PAIR(3) | A_ALTCHARSET | A_BOLD);
    }
    else {
      waddch(window, '.' | A_DIM);
    }
}





class RockProxy : public TerrainProxy{
    public:
        int getMovementCost() override;
        bool isMovement() override;
        void affect() override {}
        void open() override;
        Obj getObj() override;
        bool setObj(Obj obj_) override;
        RockProxy(int x, int y);
        RockProxy(int x, int y, bool tmp);
        virtual ~RockProxy();
};


bool RockProxy::setObj(Obj obj_)
{
   if (realTerrain == nullptr){
     realTerrain = new Rock(cellX, cellY);
   }
  
   return realTerrain->setObj(obj_);
}

Obj RockProxy::getObj()
{
    return nullptr;
}


RockProxy::~RockProxy()
{
    if (realTerrain != nullptr){
        delete realTerrain;
    }
}


RockProxy::RockProxy(int x, int y) 
: TerrainProxy(x,y) {}


RockProxy::RockProxy(int x, int y, bool tmp) 
 : TerrainProxy(x,y)
{
    if (tmp == true){
        realTerrain = new Rock(cellX, cellY);
        realTerrain->open();
    }
    //realGrass = nullptr;
}



int RockProxy::getMovementCost()
{
    return ROCK_COST;
}

bool RockProxy::isMovement()
{
    if (realTerrain == nullptr){
        return ROCK_MOVEABLE;
    }

    else{
        return realTerrain->isMovement();
    }
}   


void RockProxy::open()
{
    if (realTerrain == nullptr){
        realTerrain = new Rock(cellX, cellY);
    }
    realTerrain->open();
}




class Unit;
class Base;
class BaseConstructor;

class GameField{
  friend Base;
  friend BaseConstructor;
  private:
    int width;
    int height;
    TerrainProxy*** map;
    int count_obj;
    WINDOW* window;
public:
    void openArround(int x, int y);
    bool isOvercrowd();
    WINDOW* getWindow(){ return window;}
    void show();
    void initialise();
    bool move(Unit* unit, int x, int y);
    void add(Obj);
    void add(Obj, int);
    void deleteObj(int x, int y);
    void deleteObj(Obj tmp) { if(tmp != nullptr) delete tmp;}
    void deleteObj(int x, int y, int size);
    int get();
    GameField(int,int,WINDOW*);
    ~GameField();
    GameField(const GameField& other);

};




void GameField::openArround(int x, int y)
{
  for (int curX = x-1; curX < (x+2); curX++){
    for (int curY = y-1; curY < (y+2); curY++){
      if (!(curX < 0 || curY < 0 || curX > this->width || curY > this->height) && (curX != x || curY != y))
          map[curX][curY]->open();
    }
  }
}


void GameField::deleteObj(int x, int y, int size){
  map[x][y]->deleteObj();
  for (int i = x; i < x+size; i++){
    for (int j = y; j < y+size; j++)
        map[i][j]->setObj(nullptr);
  }
}

void GameField::deleteObj(int x, int y){
    map[x][y]->deleteObj();
}


GameField::GameField(const GameField& obj){
    this->width = obj.width;
    this->height = obj.height;
    this->count_obj = obj.count_obj;
    this->window = newwin(this->height, this->width, Y_WIN, X_WIN+obj.width+5);  /*демонстрации */
    this->initialise();
    for (size_t i = 0; i < this->height; i++){
        for (size_t j = 0; j < this->width; j++)
        {
    //        map[i][j]->copy(*obj.map[i][j], this);
        }
    }
} 



bool GameField::isOvercrowd(){
    int max_count = width*height/3;
    int real_count = GameObj::getCount();
    return max_count <= real_count;
}


int GameField::get(){
    int c = wgetch(window);
    return c;
}

void GameField::add(Obj obj){
    if (this->isOvercrowd())
    {
        /* Исключение нельзя добавить */
        return;
    }
    int cellX = obj->x_;
    int cellY = obj->y_;
    if (!map[cellX][cellY]->isMovement()) return;
    if (cellX >= this->width || cellX < this->width || cellY < this->height || cellY >= this->height)
     {
        /* исключение*/  
     }

   // if (!map[cellX][cellY]->isEmpty())
     {
        /* Исключение, в клетке есть объект */ 
     }
    if (obj->isHero()) 
        openArround(cellX, cellY);
    map[cellX][cellY]->setObj(obj);
    map[cellX][cellY]->show(window);             /*Тут проблема, т.к. в конструкторе вызывается вирт */
    
    wrefresh(window);
}


void GameObj::initVariables(){   //возможно убрать в конструтор просто
    this->x_ = 0;
    this->y_ = 0;
    this->field_ = NULL;
}


GameObj::GameObj(){
    Count++;
    this->initVariables();
}




void GameObj::setPosition(GameField* field, int x, int y){
    this->field_ = field;
    this->x_ = x;
    this->y_ = y;
    field_->add(this);
}

enum HeroState{
   BLOCK_ATTACK,
   BLOCK_MOVEABLE,
   NORMAL
   /* ... */
};

class Mine;
class Healing;
//class Portal;


class Unit : public GameObj, public IUnit {
  private:
    friend Unit& operator+=(Unit& left, const Mine& right);
    friend Unit& operator+=(Unit& left, const Healing& right);
    //friend Unit& operator+=(Unit& left, const Portal& right);
    void initVariables();  
  protected:
    int hitPoint;
    int armor;
    int actionPoint;
    char character; // int
    int damagePoint;
    HeroState state_;
  public:
    virtual bool move(int x, int y) override;
    virtual void show() override;
    virtual void update() override;                          /* Исправить потом */
    bool isDead() {return hitPoint <= 0;}
    void loseHp(int hp) {hitPoint -= hp;}
    void gainHp(int hp) { hitPoint += hp;}
    bool isHero() override { return true; }
    void affectBlock() override;
    void updateState() override;
    Unit();
    virtual ~Unit() {}
};

void Unit::initVariables(){
      this->hitPoint = 0;
      this->armor = 0;
      this->actionPoint = 0;
      this->character = '?';
      this->damagePoint = 0;
}


void Unit::update()
{
  if (isDead())
    field_->deleteObj(x_, y_);
}

void Unit::affectBlock()
{
   state_ = BLOCK_ATTACK;
}

void Unit::updateState()
{
  state_ = NORMAL;
}

Unit::Unit(){
      this->initVariables();
      state_ = NORMAL;
}


bool Unit::move(int x, int y){
    updateState();
    return field_->move(this, x, y);
}


void Unit::show(){
    mvwaddch(field_->getWindow(), y_, x_, character);
}







class INeutralObj : public GameObj
{ 
protected:
  int character;  
public:
  //virtual ~NeutralObj();
  bool isNeutral() override { return true; }
  virtual void selfDestr() = 0;
  //virtual void update() = 0;
  virtual ~INeutralObj() {}
};


class Healing : public INeutralObj
{
  private:
    int healHp;
    bool life;
  public:
    void update() override;
    void selfDestr() override;
    void show() override;
    Healing(GameField* field, int x, int y);
    friend Unit& operator+=(Unit& left, const Healing& right);  
    virtual ~Healing() {} 
};

Healing::Healing(GameField* field, int x , int y) 
 : healHp(HEAL_HP), life(true) 
{
  character = HEALING_CHARACTER;
  setPosition(field, x, y);
}


void Healing::selfDestr()
{
  if(!life){
    field_->deleteObj(this);
  }
}


void Healing::update(){
  if (!life){
    selfDestr();
  }
}


void Healing::show(){
  wmove(stdscr, y_, x_);
    waddch(stdscr, character | COLOR_PAIR(6) | A_BOLD);
}






class Mine : public INeutralObj
{
  private:
    int damage;
    bool life;
  public:
    void update() override;
    void selfDestr() override;
    void show() override;
    Mine(GameField* field, int x, int y);
    friend Unit& operator+=(Unit& left, const Mine& right); 
    virtual ~Mine() {}  
};



Mine::Mine(GameField* field, int x , int y) 
 : damage(MINE_DAMAGE), life(true) 
{
  character = MINE_CHARACTER;
  setPosition(field, x, y);
}



void Mine::selfDestr()
{
  if(!life){
    field_->deleteObj(this);
  }
}




void Mine::update(){
  if (!life){
    selfDestr();
  }
}

void Mine::show(){
  wmove(stdscr, y_, x_);
    waddch(stdscr, character | COLOR_PAIR(7) | A_BOLD);
}






Unit& operator+=(Unit& left, const Mine& right){
  left.hitPoint -= right.damage;
  //left.loseHP(right.damage)
  //right.setLife();
  return left;
}





Unit& operator+=(Unit& left, const Healing& right){
  left.hitPoint += right.healHp;
  //left.gainHp(right.healHp);
  //right.setLife();
  return left;
}






void Terrain::addObj(Obj obj_)
{
   if (this->obj==nullptr){
      setObj(obj_);
   }
   if (obj_->isHero() && obj!=nullptr && obj->isNeutral())
   {
      Unit* hero = (Unit*)obj_;  
      if (typeid(*obj) == typeid(Healing)){
         Healing* ch = (Healing*)obj;
         (*hero) += (*ch);
      }
      if (typeid(*obj) == typeid(Mine)){
         Mine* ch = (Mine*)obj;
         (*hero) += (*ch);
      }
 /*     if (typeid(*obj) == typeid(Healing)){
         Healing* ch = (Healing*)obj;
         (*hero) += (*ch);
      }
*/ 
      /* удаление мин и лечилки */
      setObj(obj_);
   }
   else
      return;

}








class TBattleCharacter : public Unit {
  public:
      virtual void attack() {}
      TBattleCharacter();
      virtual ~TBattleCharacter() {}
};


TBattleCharacter::TBattleCharacter() {
     
 }



class TMagicCharacter : public TBattleCharacter{
  public:
      //void attack() override {}
      virtual void magicAttack() {}
      TMagicCharacter();
      virtual ~TMagicCharacter() {}
};


 TMagicCharacter::TMagicCharacter(){
     
 }



class TSiegeCharacter: public Unit {
  protected:
    size_t distantAttack;
  public:
    virtual void shoot() {}
    TSiegeCharacter();
    virtual ~TSiegeCharacter() {}
};

TSiegeCharacter::TSiegeCharacter()
{
    
}


class HumanCleric : public TMagicCharacter{
   private:
     void initVariables();
   public:
     GameObj* copy(GameField* field);
     HumanCleric(const HumanCleric& obj);
     HumanCleric(GameField* field, int x, int y);
     virtual ~HumanCleric() {}
};


HumanCleric::HumanCleric(const HumanCleric& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* HumanCleric::copy(GameField* field)
{   
    HumanCleric* ret = new HumanCleric(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}


 
void HumanCleric::initVariables(){
    hitPoint = HUMANCLERIC_HP; 
    armor = HUMANCLERIC_ARM;
    actionPoint = ACTION_POINT; 
    damagePoint = HUMANCLERIC_DM; 
    character = HUMANCLERIC_VIEW; 
 }
 
 
 HumanCleric::HumanCleric(GameField* field, int x, int y) : TMagicCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }
 

class TrolsMagic : public TMagicCharacter{
   private:
     void initVariables();
   public:
      TrolsMagic(const TrolsMagic& obj);
      GameObj* copy(GameField* field) override;
      TrolsMagic(GameField* field, int x, int y);
      virtual ~TrolsMagic() {}
};
 

void TrolsMagic::initVariables() {
    hitPoint = TROLSMAGIC_HP; 
    armor = TROLSMAGIC_ARM;
    actionPoint = ACTION_POINT; 
    damagePoint = TROLSMAGIC_DM; 
    character = TROLSMAGIC_VIEW;
}
 
 
// hitPoint(TROLSMAGIC_HP), armor(TROLSMAGIC_ARM), actionPoint(ACTION_POINT), damagePoint(TROLSMAGIC_DM), character(TROLSMAGIC_VIEW) 
 
TrolsMagic::TrolsMagic(GameField* field, int x, int y) : TMagicCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }


TrolsMagic::TrolsMagic(const TrolsMagic& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* TrolsMagic::copy(GameField* field)
{   
    TrolsMagic* ret = new TrolsMagic(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}



class Gretchin : public TBattleCharacter{
  private:
    int comradeCount;
    void initVariables();
  public:
    Gretchin(const Gretchin& obj);
    GameObj* copy(GameField* field) override;
    Gretchin(GameField* field, int x, int y);
    void attack() override {}
    /* атака с бонусом от рядом находящихся */
    virtual ~Gretchin() {}
};


void Gretchin::initVariables(){
    comradeCount = 0;
    hitPoint = GRETCHIN_HP; 
    armor = GRETCHIN_ARM; 
    actionPoint = ACTION_POINT; 
    damagePoint = GRETCHIN_DM; 
    character = GRETCHIN_VIEW;
}

 Gretchin::Gretchin(GameField* field, int x, int y) : TBattleCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }

Gretchin::Gretchin(const Gretchin& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->comradeCount = obj.comradeCount;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* Gretchin::copy(GameField* field)
{   
    Gretchin* ret = new Gretchin(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}




class HumanGuard : public TBattleCharacter {
  private:
    void initVariables();
    int moralityPoint;
  public:
    HumanGuard(const HumanGuard& obj);
    GameObj* copy(GameField* field) override;
    HumanGuard(GameField* field, int x, int y);
    void attack() override {}
    /*атакa с бонусом от морали */
    virtual ~HumanGuard() {}
};

void HumanGuard::initVariables(){
   moralityPoint = 0; 
   hitPoint = HUMANGUARD_HP; 
   armor = HUMANCLERIC_ARM;
   actionPoint = ACTION_POINT;
   damagePoint = HUMANGUARD_DM; 
   character = HUMANGUARD_VIEW; 
}


HumanGuard::HumanGuard(GameField* field, int x, int y) : TBattleCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }

HumanGuard::HumanGuard(const HumanGuard& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->moralityPoint = obj.moralityPoint;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* HumanGuard::copy(GameField* field)
{   
    HumanGuard* ret = new HumanGuard(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}





 
class HumanTrebuchet : public TSiegeCharacter {
    private:
        void initVariables();
    public:
        HumanTrebuchet(const HumanTrebuchet& obj);
        GameObj* copy(GameField* field) override;
        /* обычная атака */
        HumanTrebuchet(GameField* field, int x, int y);
        virtual ~HumanTrebuchet() {}
 };


void HumanTrebuchet::initVariables(){
   distantAttack = 5; 
   hitPoint = HUMANTREBUCHET_HP; 
   armor = HUMANCLERIC_ARM;
   actionPoint = ACTION_POINT;
   damagePoint = HUMANTREBUCHET_DM; 
   character = HUMANTREBUCHET_VIEW; 
} 
 
 
HumanTrebuchet::HumanTrebuchet(GameField* field, int x, int y) : TSiegeCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }
 
 HumanTrebuchet::HumanTrebuchet(const HumanTrebuchet& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->distantAttack = obj.distantAttack;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* HumanTrebuchet::copy(GameField* field)
{   
    HumanTrebuchet* ret = new HumanTrebuchet(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}
 
 
 
 
 
 class GoblinCatapult : public TSiegeCharacter {
    private:
      void initVariables();
      int fireBall;
    public:
        GoblinCatapult(const GoblinCatapult& obj);
        GameObj* copy(GameField* field) override;
        void shoot() override {}
        GoblinCatapult(GameField* field, int x, int y);
        virtual ~GoblinCatapult() {}
 };
 
 
void GoblinCatapult::initVariables()
{
   distantAttack = 4;
   hitPoint = GOBLINCATAPULT_HP; 
   armor = GOBLINCATAPULT_ARM; 
   actionPoint = ACTION_POINT;
   damagePoint = GOBLINCATAPULT_DM; 
   character = GOBLINCATAPULT_VIEW; 
}
 
 GoblinCatapult::GoblinCatapult(GameField* field, int x, int y) : TSiegeCharacter()
 {
    this->initVariables();
    this->setPosition(field, x, y);
 }
 
 
 
 
  GoblinCatapult::GoblinCatapult(const GoblinCatapult& obj)
{
    this->hitPoint = obj.hitPoint;
    this->armor = obj.armor;
    this->damagePoint = obj.damagePoint;
    this->actionPoint = obj.actionPoint;
    this->character = obj.character;
    this->distantAttack = obj.distantAttack;
    this->x_ = obj.x_;
    this->y_ = obj.y_;
}



GameObj* GoblinCatapult::copy(GameField* field)
{   
    GoblinCatapult* ret = new GoblinCatapult(*this);
    ret->setPosition(field, this->x_, this->y_);
    return ret;
}
 
 
 
 
 /* Задавать характеристики в конструкторе некоторые по умолчанию кроме поля и координат */

 class AFactory{
   public:
      virtual TMagicCharacter* createTMagicCharacter(GameField* field, int x, int y) = 0;
      virtual TBattleCharacter * createTBattleCharacter(GameField* field, int x, int y) = 0;
      virtual TSiegeCharacter * createTSiegeCharacter(GameField* field, int x, int y) = 0;
    virtual ~AFactory() {}
     
 };
 


class HumanFactory: public AFactory{
  public:  
    TMagicCharacter* createTMagicCharacter(GameField* field, int x, int y) override { 
      return new HumanCleric(field,x, y); 
    }
    
    TBattleCharacter* createTBattleCharacter(GameField* field, int x, int y) override { 
      return new HumanGuard(field,x, y); 
    }
    
    TSiegeCharacter* createTSiegeCharacter(GameField* field, int x, int y) override { 
      return new HumanTrebuchet(field,x, y); 
    }
};
  
  
// Фабрика для создания воинов армии хаоса
class ChaosFactory: public AFactory{
  public:    
    TMagicCharacter* createTMagicCharacter(GameField* field, int x, int y) override { 
      return new TrolsMagic(field,x, y); 
    }
    
    TBattleCharacter* createTBattleCharacter(GameField* field, int x, int y) override { 
      return new Gretchin(field,x, y); 
    }
    
    TSiegeCharacter* createTSiegeCharacter(GameField* field, int x, int y) override { 
      return new GoblinCatapult(field,x, y); 
    }
};
 



const int HUMAN_BASE_COST = 200;
const int HUMAN_BASE_LIMIT = 10;
const int HUMAN_BASE_HP = 200;
const int HUMAN_BASE_CH = '#';
const int CHAOS_BASE_HP = 100;
const int CHAOS_BASE_COST = 100;
const int CHAOS_BASE_LIMIT = 15;
const int CHAOS_BASE_CH = '%';



class Base : public Observer, public GameObj
{
  protected:
    AFactory* factory_;
    std::vector<Unit*> troop;
    int unitCount;
    bool find(Unit*, int&);
    virtual bool findRespPlace(int&,int&);
    bool isNotLimit();
    int hitPoint;
    int unitLimit;
    int character;
  public:
    virtual void addUnit(Unit* unit);
    virtual void removeUnit(Unit* unit);
    virtual int getSpace() override;
    Base(int,int,int); 
    virtual void updateNotify(Unit* unit, Event event) override;
    void setPosition(GameField* field, int x, int y) override;
    virtual Unit* createUnit(UnitType type);
    virtual ~Base();
};




void Base::setPosition(GameField* field, int x, int y)
{
  field_ = field;
  x_ = x;
  y_ = y;
  field_->add(this, 2);
}


Base::Base(int hp, int lim, int ch) 
: hitPoint(hp),unitLimit(lim), unitCount(0), factory_(nullptr), character(ch)  
{}


bool Base::find(Unit* unit, int& index)
{
  for(vector<Unit*>::size_type i = 0; i != troop.size(); i++, index++){
         if (troop[i] == unit) return true;
    }
    return false;
}


void Base::removeUnit(Unit* unit)
{
  int index = 0;
  if (find(unit, index)){
    troop.erase(troop.begin() + index);
    unitCount--;
  }
  else {
    /* Исключение, не нашел */
    return;
  }
}




void Base::updateNotify(Unit* unit, Event event)
{
  switch (event) 
  {
      case UNIT_DIE:
        removeUnit(unit);
        break;
      case UNIT_CREATE:
        addUnit(unit);
        break;
  /*  default:
      выполнить, если ни один вариант не подошел
      break; */
    }
}



int Base::getSpace()
{
  int space = 4;          /// Размер базы
  for (int i = 0; i < troop.size(); i++)
  {
    space += troop[i]->getSpace();
  }
    return space;
}


void Base::addUnit(Unit* unit)
{
  troop.push_back(unit);
  unitCount++;
}

Base::~Base()
{
  if (factory_)
     delete factory_;
}



bool Base::isNotLimit()
{
  return unitCount < unitLimit; 
}



Unit* Base::createUnit(UnitType type)
{
  int x,y;
  if (findRespPlace(x,y) && isNotLimit()){
  Unit* unit; 
  switch (type) 
  {
      case MAGIC:
        unit = factory_->createTMagicCharacter(field_, x, y);
        break;
      case WARRIOR:
        unit = factory_->createTBattleCharacter(field_, x, y);
        break;
      case SIEGE:
        unit = factory_->createTSiegeCharacter(field_, x, y);
        break;  
  /*  default:
      выполнить, если ни один вариант не подошел
      break; */
    }
    addUnit(unit);
    return unit;
  } 
  else{
    return nullptr;
  }
}



bool Base::findRespPlace(int &x, int &y)
{
  int yi = y_;
  int xi = x_+2;
for (int i = y_-1; i < y_+3; i++)
{
  for (int j = x_-1; j < x_+3; j++){
    if ((i==y_||i==y_+1)&&(j==x_||j==x_+1) || i<0 || j<0 || i>(field_->height-1) || j>(field_->width-1))
        continue;
    else if (field_->map[j][i]->isMovement()){
      x = j; y = i;
      return true;
    }
  }
}

return false;
}






class HumanBase : public Base
{
public:
  HumanBase(GameField* field, int x, int y);
  void show() override;
  //virtual ~HumanBase();
};




HumanBase::HumanBase(GameField* field, int x, int y)
 : Base(HUMAN_BASE_HP, HUMAN_BASE_LIMIT, HUMAN_BASE_CH)
 {
    
  factory_ = new HumanFactory;
  setPosition(field, x, y);
 }



 void HumanBase::show()
 {  
    for (int i = x_; i < x_+2; i++){
        for (int j = y_; j < y_+2; j++){
            wmove(stdscr, j, i);
            waddch(stdscr, character | COLOR_PAIR(4) | A_BOLD);
        }
    } 
 }


/*
void HumanBase::die(){
   field_->deleteObj(x_,y_, 2);
} */



class ChaosBase : public Base
{
public:
  ChaosBase(GameField* field, int x, int y);
  void show() override;
};




ChaosBase::ChaosBase(GameField* field, int x, int y)
 : Base(CHAOS_BASE_HP, CHAOS_BASE_LIMIT, CHAOS_BASE_CH)
 {
  factory_ = new ChaosFactory;
  setPosition(field, x, y);
 }



 void ChaosBase::show()
 {  
    for (int i = x_; i < x_+2; i++){
        for (int j = y_; j < y_+2; j++){
            wmove(stdscr, j, i);
            waddch(stdscr, character | COLOR_PAIR(5) | A_BOLD);
        }
    } 
 }





class BaseConstructor
{
private:
  //HumanFactory* human_factor;
  //ChaosFactory* chaos_factor;
  GameField* field_;
  bool isMovementBase(int x, int y);
public:
  Base* createHumanBase(int x, int y);
  Base* createChaosBase(int x, int y);
  BaseConstructor(GameField*);
};



BaseConstructor::BaseConstructor(GameField* field)
: field_(field) {}

Base* BaseConstructor::createHumanBase(int x, int y)
{
  if (!isMovementBase(x,y)) return nullptr;
  return new HumanBase(field_, x, y);
}


Base* BaseConstructor::createChaosBase(int x, int y)
{
  if (!isMovementBase(x,y)) return nullptr;
  return new ChaosBase(field_, x, y);
}



bool BaseConstructor::isMovementBase(int x, int y)
{
  if (!field_->map[x][y]->isMovement() || !field_->map[x+1][y]->isMovement()
     || !field_->map[x][y+1]->isMovement() || !field_->map[x+1][y+1]->isMovement())
    return false;
  return true;
}









bool GameField::move(Unit* unit, int x, int y){
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    int oldCellX = unit->x_;
    int oldCellY = unit->y_;
    int cellX = x;
    int cellY = y;
    if (!map[cellX][cellY]->isMovement()) return false;
    unit->x_ = x;
    unit->y_ = y;
    wmove(window, oldCellY, oldCellX);
    if (oldCellX == cellX && oldCellY == cellY) return false;    //не переместились
    
    map[oldCellX][oldCellY]->setObj(nullptr);
    map[oldCellX][oldCellY]->show(window);
  //  if (!map[cellX][cellY]->isEmpty()){
        /*  ERROR */
 //   }
    beep();
    add(unit);  
  // map[cellX][cellY]->show(window);
//    wrefresh(window);
    return true;
}


GameField::GameField(int width_, int height_, WINDOW* window_) : count_obj(0){   
    width = width_;
    height = height_;
    window = window_;
   // int maxwidth,maxheight;
    //getmaxyx(window, maxheight, maxwidth);
   // keypad(window, true);
    initialise();
    show();
}


// возможно лучше в конструктор получать размеры + некоторые параметры
// а создавать матрицу позже

void GameField::initialise(){
    map = new TerrainProxy** [this->height];
    for (size_t i = 0; i < this->height; i++){
        map[i] = new TerrainProxy* [this->width];
        for (size_t j = 0; j < width; j++){
           if ( ((j > 15 && j < 25)||(j > 29 && j <35) ) && (i < 13 || (i>25 && i < 36))  ) 
                map[i][j] = new RiverProxy(i, j);
           else if (rand()%20+1 == 7) 
                map[i][j] = new RockProxy(i, j);
           else 
                map[i][j] = new GrassProxy(i, j);              /*+передачу параметров ландшафта случайным образом*/
            
        }
    }   
    //wmove(this->window, 0 , 0);
}


GameField::~GameField(){
    // возможно удаление и объектов внутри, т.к. они по указателям
   for (size_t i = 0; i < this->height; i++)
   {
       delete [] map[i];
   }
   delete [] map;
   /* something else */
}




void GameField::show(){
     /* возможно стоит ставить курсор в левый верхнмй угол*/
     wmove(window, 0 , 0);
     
     for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++){
            map[i][j]->show(window);
        } 
     }  
}




void GameField::add(Obj obj, int size)
{
    if (this->isOvercrowd()){
        /* Исключение нельзя добавить */
        return;
    }
    int cellX = obj->x_;
    int cellY = obj->y_;
    if (!map[cellX][cellY]->isMovement()) return;
    for (int i = cellX; i < cellX+size; i++){
      for (int j = cellY; j < cellY+size; j++){
           map[i][j]->setObj(obj);
           openArround(cellX, cellY);
        }
    }
    wrefresh(window);
}



class Game{
  private:
    int width = 40;
    int hight = 40;
    int xWin;
    int yWin;
    void initWindow();
    void initCurses();
    WINDOW* window;
    GameField* field;
  public:
    void run();
    Game();
    virtual ~Game();
    
};


Game::Game(){
    this->initCurses();
    this->initWindow();
    window = stdscr;
    this->field = new GameField(width, hight, window);
}

void Game::initWindow(){
    srand(time(0));
    //this->window = newwin(this->hight,this->width, Y_WIN, X_WIN);
   // refresh();
   // wrefresh(this->window);
}

void Game::initCurses(){
    initscr();
    noecho();
        curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();
   // raw(); 

    init_pair(1, COLOR_GREEN, COLOR_BLACK);   //трава
    init_pair(2, COLOR_CYAN, COLOR_BLACK);    //река
    init_pair(3, COLOR_BLACK, COLOR_BLACK);    //блок
    init_pair(4, COLOR_BLUE, COLOR_BLACK);   //база-люди
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);    //база-хаос
    init_pair(6, COLOR_RED, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
}

Game::~Game(){
     delwin(this->window);
     endwin();
}

void Game::run(){
    Base* human_base = new HumanBase(field, 1,1);
    ChaosFactory factor;
    TMagicCharacter* unit1 = factor.createTMagicCharacter(field,5, 5);
    Unit *unit2;
    char choice = '0';
    int goX, goY = 1;
    int x = 5;
    int y = 5;
    Healing* hilka1 = new Healing(field, 10, 15);
    Healing* hilka2 = new Healing(field, 9, 21);
    Healing* hilka3 = new Healing(field, 21, 21);
    Mine* mine1 = new Mine(field,17,18);
    while(choice != 'q'){
        //choice = field->get();
        choice = wgetch(field->getWindow());
        if (choice == 's')  { if (unit1->move(x,y+1)) y++; }
        if (choice == 'w')  { if (unit1->move(x,y-1)) y--; }
        if (choice == 'a')  { if(unit1->move(x-1,y)) x--; }
        if (choice == 'd')  { if(unit1->move(x+1,y)) x++; }
        if (choice == 'c')  {  unit2 = human_base->createUnit(WARRIOR); }
        wrefresh(window);
    }
}

int main(){
   
    Game game;
    game.run();
    return 0;
} 



