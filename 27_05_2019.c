#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include <png.h>
#include <getopt.h>
#define RED "\033[0;31m"
#define NONE "\033[0m"
#define GREEN "\033[0;92m"
#define YELLOW "\033[0;33m"
#define YELLOW "\033[0;33m"

struct png{

    int width;       ///ширина 	(from IHDR)
	int height;       // высота     (from IHDR)
    png_byte color_type;   // Тип цвета, состоит из 3 флагов 1 (используется палитра), 2 (используется цвет, не монохромное изображение), 
	//and 4 (присутствует альфа-канал), 1 байт
    png_byte bit_depth; // Битовая глубина , определяет количество бит на каждый сэмпл(не пиксель), 1 байт

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};

struct global_args{
    int reflect;    	/* параметр -r */
    char *langCode; 
    int start[2];              /* начало и конец отрезка */
    int end[2];
	int N ;            /* количество фигур */
    int centre[2];             /* центр окружности */
    int R;                     /* радиус */
    char axis;                  /* ось координат */
    float width;                 /* ширина линии */
    png_byte color[4];         /* цвет линии */
};
 
static const char *optstring = "lrpfzW:E:S:C:A:R:B:N:ih?";
/* Возможно всех с NULL, а не флагом
struct option longOpts[] = {
        {"reflect", no_argument, &action, 'r'},
        {"copy", no_argument, &action, 'c'},
        {"line", no_argument, &action, 'd'},
        {"help", required_argument, &action, 'h'},
        {"start", required_argument, NULL, 'S'}, 
       // {"centre", required_argument, NULL, 'C'},
        {"end", required_argument, NULL, 'E'},
        {"color", required_argument, NULL, 'C'},
        {"axis", required_argument, NULL, 'A'},
        {"width", required_argument, NULL, 'W'},
        {"radius", required_argument, NULL, 'R'},
        {"info", required_argument, &action, 'i'},
        {NULL,  no_argument, NULL, 0}
};
*/

void read_png_file(struct png* image, char* fileName)
{
    int x,y;
    char header[8];

    FILE *fp = fopen(fileName, "rb");

    if(!fp) 
    {
		printf("[read_png_file] File %s could not be opened for reading", fileName);
        return;    //abort_
		//Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);  //считывает и потока в массив 8 объектов размера 1
	
	if (png_sig_cmp(header , 0 , 8)){ //функция, возвращает 0, если байты совпали с подписью png и !=0 в противном случае
		printf("[read_png_file] File %s is not recognized as a PNG file", fileName);
	}   

/*инициализация материала*/
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    image->info_ptr = png_create_info_struct(image->png_ptr);

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8); 
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    
	//image->number_of_passes = png_set_interlace_handing(image->png_ptr);
	//png_read_update_info(image->png_ptr, image->info_ptr);
    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);

    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    
    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
    
}

void doNewFile(struct png* image, char* fileName)
{
    int x, y;

    FILE *fp = fopen(fileName, "wb");
    
    if(!fp) printf("file is not open {doNewFile}\n");
     /*инициализация материала*/
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    image->info_ptr = png_create_info_struct(image->png_ptr);

    png_init_io(image->png_ptr, fp);

	/*написание header*/
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);
    /*write bytes*/
    png_write_image(image->png_ptr, image->row_pointers);
  
    png_write_end(image->png_ptr, NULL);
/* cleanup heap-куча allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);

}

void set_pixel(struct png *image, int x, int y, char *rgb)
{
   png_byte *row = image->row_pointers[y];
    png_byte *ptr = &(row[x*4]);
      
    ptr[0] = rgb[0];
    ptr[1] = rgb[1];
    ptr[2] = rgb[2];
    ptr[3] = rgb[3]; 
}


void draw_line(struct png* image, int x1 , int y1 , int x2 , int y2, int width, png_byte *rgb) {
	int x_1, x_2, y_1, y_2;
	x_1 = x1;   x_2 = x2;
	y_1 = y1;   y_2 = y2;
	if (x1>x2) 
	   {	
			int c;
			c = x1;   x1 = x2;  x2 = c;
			c = y1;   y1 = y2;  y2 = c;
	   }		
    int deltaX = abs(x2 - x1);
    int deltaY = abs(y2 - y1);
    int signX = x1 < x2 ? 1 : -1;
    int signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    set_pixel(image,x2, y2, rgb);
    while(x1 != x2 || y1 != y2) 
   {
        set_pixel(image,x1, y1,rgb);
        int error2 = error * 2;
        //
        if(error2 > -deltaY) 
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
    }
	
    if (width > 1)  draw_line(image, x_1, y_1-1, x_2 , y_2-1, 1, rgb);
/*	for (int i = 1; i < width ; i++) {
	       draw_line(image, 
	}  */
	if (width > 2)  
	{
		draw_line(image, x_1-1, y_1, x_2 -1, y_2, 1, rgb);
	    draw_line(image, x_1-1, y_1-1, x_2-1 , y_2-1, 1, rgb);
		draw_line(image, x_1-1, y_1-1, x_2-1 , y_2-1, 1, rgb);
		draw_line(image, x_1-1, y_1-1, x_2-1 , y_2-1, 1, rgb);
	}
}

void rect(struct png* image, int x1, int y1 , int x4, int y4, int width, png_byte *rgb) {
     srand(time(NULL));
/*	 int h  = 5 + rand()%1000;
	 srand(time(NULL));
	 int w = 5 + rand()%1000;
    while ((h>(image->height)) || (w>(image->width)) || ((x1+w)>(image->width)) || ((y1+h)> (image->height))) {
	  srand(time(NULL));
	 h  = 5 + rand()%(image->height);
	 srand(time(NULL));
	 w = 5 + rand()%(image->width);  
	} */
	int x2 = x4;  int x3 = x1;
    int y2 = y1 ; int y3 = y4;
	 while ((y1 != y4) && (y2 != y4)){
			draw_line(image, x1, y1, x2, y2, 1, rgb);
			y1++;
			y2++;
	 }
  png_byte rgba[4];
  rgba[0] = rand()%255 - 1;
  rgba[1] = rand()%255 - 1;
  rgba[2] = rand()%255 - 1;
  rgba[3] = 255; 
  /* if { (x1 - width - 2 < 0) draw_line1(image,((x1-2)/2), y1, x3 , y3, x1 - 2, &rgba,0); }
    else draw_line1(image,x1-width/2, y1, x3 , y3, width, &rgba,0);

  if { (y1 - width - 2 < 0) draw_line1(image,((x1-2)/2), y1, x3 , y3, x1 - 2, &rgba,0); }
    else draw_line1(image,x1-width/2, y1, x3 , y3, width, &rgba,0);
    
  if { (x1 - width - 1 < 0) draw_line1(image,((x1-2)/2), y1, x3 , y3, x1 - 2, &rgba,0); }
    else draw_line1(image,x1-width/2, y1, x3 , y3, width, &rgba,0);
    
  if { (x1 - width - 1 < 0) draw_line1(image,((x1-2)/2), y1, x3 , y3, x1 - 2, &rgba,0); }
    else draw_line1(image,x1-width/2, y1, x3 , y3, width, &rgba,0);    

*/

    
    if((x1 - width - 2) < 0) width = x1 - 1;
    if((y1 - width - 2) < 0) width = y1 - 1;
    if((x4 + width + 2) > (image->width)) width = image->width - x4 - 1;
    if((y4 + width + 2) > (image->height)) width = image->height - y4 - 1;
    
    
    
    
    
}


void zadacha(struct png* image, int N){
	printf("\nzadacha");
	for (int i = 0; i < N; i++) 
	{
		 srand(time(NULL));
	     int x1 = rand()%(image->width - 10);
		 printf("\n x1 = %d  ", x1);
		
		 int x4 = x1 + rand()%(image->width - x1 + 1);
		 printf("\n x4 = %d", x4);
	
		 int y1 = 20 + rand()%(image->height - 10);
		 printf("\n y1 = %d", y1);
		
		 int y4 = y1 + rand()%(image->height - y1 + 1);
		 printf("\n y4 = %d", y4);
		 if(x1 > x4) { int tmp = x4;   x4 = x1; x1 = tmp;}
	     if(y1 > y4) { int tmp = y4;   y4 = y1; y1 = tmp;}

		while ( (x4-x1)<3 || (y4-y1)<3 ) //(x4 + x1)<=0 || (y4+y1)<=0 
		       {

	                 int x1 = 20 + rand()%(image->width - 20 + 1);
		              printf("\n x1 = %d  ", x1);

		             int x4 = x1 + rand()%(image->width - x1 + 1);
		             printf("\n x4 = %d", x4);
	
		             int y1 = 20 + rand()%(image->height - 20 + 1);
	            
	            	 int y4 = y1 + rand()%(image->height - y1 + 1);
	            	 if(x1 > x4) { int tmp = x4;   x4 = x1; x1 = tmp;}
	                  if(y1 > y4) { int tmp = y4;   y4 = y1; y1 = tmp;}
		        }
		png_byte rgb[4];
		
		int width = 2 + rand()%(10-2+1);
		
		rgb[0] = rand()%255 - 1;
		
		rgb[1] = rand()%255 - 1;
		
		rgb[2] = rand()%255 - 1;
		rgb[3] = 255; 

		rect(image, x1, y1 , x4, y4, width, &rgb);
	
	}

}




void setPixelColor(struct png *image, int x, int y, char *rgb, int color)

{

   png_byte *row = image->row_pointers[y];

   png_byte *ptr = &(row[x*4]);

      

    ptr[0] = rgb[0] ;

    ptr[1] = rgb[1] ;

    ptr[2] = rgb[2] ;

    ptr[3] = rgb[3]; 
   // ptr[3] = 50;
	//ptr[3] = rgb[3]-color;
}




void swap_pixel(png_byte* pix1, png_byte* pix2)
{
    int tmp[4];
	
    tmp[0] = pix2[0];    //  pix2[0] = pix2[0];    
    tmp[1] = pix2[1];		//pix2[1] = pix2[1];
    tmp[2] = pix2[2];		//pix2[2] = pix2[2];
    tmp[3] = pix2[3];		//pix2[3] = pix2[3];
	
    pix2[0] = pix1[0];
    pix2[1] = pix1[1];
    pix2[2] = pix1[2];
    pix2[3] = pix1[3];

    pix1[0] = tmp[0];
    pix1[1] = tmp[1];
    pix1[2] = tmp[2];
    pix1[3] = tmp[3];

}



void draw_circle(struct png* image, int Sx, int Sy, int R, float width, png_byte *rgb)
{
	int width1 = (int)width;
    int Ex = Sx + R - 1;
    int Ey = Sy + R - 1;
    int i,j;

    for(i = 0; i <= R; i++)
    {
        for(j = 0; j <= R; j++){

            if( (i*i + j*j) <= R*R && (i*i + j*j) >= (R - width1)*(R - width1))
            {
               
                set_pixel(image, Sx + i, Sy + j, rgb);
                set_pixel(image, Sx + i, Sy - j, rgb);
                set_pixel(image, Sx - i, Sy + j, rgb);
                set_pixel(image, Sx - i, Sy - j, rgb);
            }
        }
    } 
    
}



void draw_line1(struct png* image, int x0, int y0, int x1, int y1, float wd ,png_byte *rgb,int flag)

{ 
   if (wd > 30) wd = 25; 
   wd = wd/2;
   int width = (int)(wd/1.7);

   int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1; 

   int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1; 

   int err = dx-dy, e2, x2, y2;                          

   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);

   

   for (wd = (wd+1)/2; ; ) {                                  

      setPixelColor(image,x0,y0,rgb,fmax(0,255*(abs(err-dx+dy)/ed-wd+1)));

      e2 = err; x2 = x0;

      if (2*e2 >= -dx) {                                        

         for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)

            setPixelColor(image,x0, y2 += sy,rgb, fmax(0,255*(abs(e2)/ed-wd+1)));

         if (x0 == x1) break;

         e2 = err; err -= dy; x0 += sx; 

      } 

      if (2*e2 <= dy) {                                          

         for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)

            setPixelColor(image,x2 += sx, y0, rgb, fmax(0,255*(abs(e2)/ed-wd+1)));

         if (y0 == y1) break;

         err += dx; y0 += sy; 

      }

   }
   if (width >= 6 && flag == 1) {
       for (int i = 1; i < width+2 ; i++){
         if( !((x1+i)>(image->width) || (x1-i)<0 || (y1+i)>(image->height) || (y1-i)<0))   draw_circle(image, x1 , y1 , i, 1, rgb);
         if( !((x0+i)>(image->width) || (x0-i)<0 || (y0+i)>(image->height) || (y0-i)<0))   draw_circle(image, x0, y0, i , 1 , rgb);
       }
           
   } 
  // draw_circle(image, x1 , y1 , wd, wd, rgb);
}


int is_digit(char *str)
{
    for(str; *str; str++)
    {
        if(!isdigit(*str))
            return 0;
    }
    return 1;
}
/*
void draw_star(struct png *image, int x0, int y0, int R, float width, png_byte* rgb){
    int R1; //малый радиусы
    int array[2][11] = {0};//массив вершин
    double a;//угол 
    R1 = round(R*0.382);
    a = M_PI/2;
    for (int i = 0; i < 10; i++){
        if (i % 2 == 0) {
            array[0][i] = x0 + round(R1*cos(a));
            array[1][i] = y0 - round(R1*sin(a));
        }
        else {
            array[0][i] = x0 + round(R*cos(a));
            array[1][i] = y0 - round(R*sin(a));
        }
        a = a + M_PI/5;
    }
    array[0][10] = array[0][0];
    array[1][10] = array[1][0];

    draw_line1(image,array[0][1], array[1][1]-1,array[0][5],array[1][5], width, rgb,0);
    draw_line1(image,array[0][1], array[1][1]-1,array[0][7],array[1][7]-1, width, rgb,0);
    draw_line1(image,array[0][3], array[1][3],array[0][7],array[1][7]-1, width, rgb,0);
    draw_line1(image,array[0][3], array[1][3],array[0][9]+1,array[1][9], width, rgb,0);
    draw_line1(image,array[0][5], array[1][5],array[0][1],array[1][1], width, rgb,0);
    draw_line1(image,array[0][5], array[1][5],array[0][9]+1,array[1][9], width, rgb,0);
    
} */


void draw_star(struct png *image, int x0, int y0, int R, float width, png_byte* rgb){
    int R1; //малый радиусы
    int array[2][11] = {0};//массив вершин
    double a;//угол 
    R1 = round(R*0.382);
    a = -M_PI/2;
    for (int i = 0; i < 10; i++){
        if (i % 2 == 0) {
            array[0][i] = x0 + round(R1*cos(a));
            array[1][i] = y0 - round(R1*sin(a));
        }
        else {
            array[0][i] = x0 + round(R*cos(a));
            array[1][i] = y0 - round(R*sin(a));
        }
        a = a + M_PI/5;
    }
    array[0][10] = array[0][0];
    array[1][10] = array[1][0];

    draw_line1(image,array[0][1], array[1][1]-2,array[0][5],array[1][5], width, rgb,0);
    draw_line1(image,array[0][1], array[1][1]-1,array[0][7],array[1][7]-1, width, rgb,0);
    draw_line1(image,array[0][3], array[1][3],array[0][7],array[1][7]-1, width, rgb,0);
    draw_line1(image,array[0][3], array[1][3],array[0][9]+1,array[1][9], width, rgb,0);
    draw_line1(image,array[0][5], array[1][5],array[0][1],array[1][1], width, rgb,0);
    draw_line1(image,array[0][5], array[1][5],array[0][9]+1,array[1][9], width, rgb,0);
    
}




void star(struct png *image, int x0, int y0, int R, float width, png_byte* rgb,int n )
{
 int r = round(R*0.382);
 int a = 0;
 int array[2][30] = {0};//Массив для хранения координат вершин звезды
 int x = x0; //Расчет центра экрана по х
 int y = y0; //Расчет центра экрана по y
 
 
//Цикл расчета вершин звезды
 for (int i = 1; i < n*2+2 ;i++)
 {
  if (!(i%2)) //При выполнении условия четности следующие формулы
   {
    array[0][i] = x + r/2*cos(a*M_PI/180);
    array[1][i] = y - r/2*sin(a*M_PI/180);
   }
   else //При невыполнении условия четности следующие формулы
    {
     array[0][i] = x+R*cos(a*M_PI/180);
     array[1][i] = y-R*sin(a*M_PI/180);
    }
    a = a + 180/n;
 }
//Завершаем построение звезды соединяя её окончание с начальной точкой
 array[0][n*2+1]=array[0][1];
 array[1][n*2+1]=array[1][1];
 
//Последовательное соединение точек массива, хранящего вершины звезды
 for (int i = 1; i < n*2+1; i++)
  {
    draw_line1(image,array[0][i], array[1][i],array[0][i+1],array[1][i+1], width, rgb,0);
  }
 
}




void reflection(struct png *image, int x0, int y0, int x1, int y1, char axis)
{   //возможно оси наоборот
	int x;
    int y;
    if (x0 > x1) {
        int c = x0;
        x0 = x1;
        x1 = c;
        c = y0;
        y0 = y1;
        y1 = c;
    }
    if(axis == 'y'){
        int length = x1 - x0 + 1;
        int width = x0 +(length - 3)/2 + (length - 3)%2;
        for(x = x0; x <= width; x++){
            for(y = y0; y <= y1; y++){
                png_byte *row1 = image->row_pointers[y];
                png_byte *ptr1 = &(row1[x*4]);
                png_byte *ptr2 = &(row1[(x1 - x + x0)*4]);
                swap_pixel(ptr1, ptr2);
            }
        }
   }
    else if (axis == 'x') {
        int length = y1 - y0 + 1;
        int height = y0 +(length - 3)/2 + (length - 3)%2;
        for(y = y0; y <= height; y++){
             png_byte *row1 = image->row_pointers[y];
             png_byte *row2 = image->row_pointers[y1 - y + y0];
            for(x = x0; x <= x1; x++){
                png_byte *ptr1 = &(row1[x*4]);
                png_byte *ptr2 = &(row2[(x)*4]);
                swap_pixel(ptr1, ptr2);
            }
        }
    }
	

}


void print_help()
{
    printf("Справка ============================================================\n");
  //  printf("%s--copy, -c \t\t%s- делает копию заданного участка. Участок задается при помощи -S  и -E.\n", GREEN,NONE);
   // printf("        Пример: --copy -S 100 100 -E 200 200 fileOut.png\n");
    printf("%s--reflect, -r \t\t%s - выполняет отражения участка относительно заданной оси   \t\t\t  Участок задается при помощи -S и -E, ось задается \n\t\t\t при помощи -O\n", GREEN,NONE);
    printf("\t\t\t Пример: --reflect -S 100 200 -E 500 700 -O x result.png\n");
    printf("%s--draw_circle, -d \t%s- рисует окружность вписанную в квадрат. \n\t\t\t Квадрат задается при помощи координат левого \n \t\t\t верхнего угла -S и  радиуса окружности -R ", GREEN,NONE);
    printf("\n%s--line, -d \t\t %s- рисует линию. Задается координатами начала -S и \n \t\t\t конца -E, шириной -W и цветом -C", GREEN,NONE);
    printf("\n\t\t\t Пример:--line -S 50 50 -E 100 100 -W 5 -C red res.png");
    printf("%s\n--start, -S \t\t %s- считывает координаты (тип int) верхнего левого угла \n\t\t\t прямоугольной области,центра пентаграммы, начала линии\n", GREEN,NONE);
    printf("%s--end, -E \t\t%s- считывает координаты (тип int) нижнего правого угла \n \t\t\t прямоугольной области, либо конца линии \n", GREEN,NONE);
    printf("%s--color, -C \t\t %s- считывает цвет. Палитра: red, green, blue, white,\n \t\t\t black , orange , yellow \n", GREEN,NONE);
    printf("%s--axis, -A \t\t %s- считывает оси (x или y) \n", GREEN,NONE);
    printf("%s--width, -W \t\t %s- считывает значение ширины \n", GREEN,NONE);
    printf("%s--radius, -R \t\t %s- считывает заначение радиуса \n", GREEN,NONE);
    printf("%s--info, -i \t\t %s- выводит информацию об изображении\n", GREEN,NONE);
    printf("%s--help, -h -? \t\t %s- вызывает справку", GREEN,NONE);
    printf("\nСтрока команд может заканчиваться названием файла с изменениями, иначе будет создан файл  result.png \n");
    printf("============================================================================\n");

}

void info(struct png* image, int len, char *fileName){
    int l = image->width;
    int n = image->height;
    int ot = 0;
    if (len < 9) {ot = 9 - len; len = 9; }
   printf("┌");
    for (int i = 0; i <= len+len/2; i++)
        printf("─");
    printf("┐");
    printf("\n│ ");printf("%s", fileName);
     for (int i = 0; i <= len/2 - 1 + ot; i++)
        printf(" ");
    printf("│");
    printf("\n│ "); printf("Width= %d", l);
    for (int i = 0; i <=len+len/2 - 12; i++)
        printf(" ");
    printf("│");
    printf("\n│ "); printf("Heigth = %d", n);
    for (int i = 0; i <= len+len/2 - 13; i++)
        printf(" ");
    printf("│");
    printf("\n└");
    for (int i = 0; i <= len+len/2; i++)
        printf("─");
    printf("┘");  
    printf("\n");  
    
}

int main(int argc, char *argv[]){
    if (argc == 1) {
        printf("Ошибка, не введены параметры");
        return 0;
    }

    if (argc == 2 && (strstr(argv[1],"h") || strstr(argv[1],"help") || strstr(argv[1],"?"))) {
        print_help();
        return 0;
    }
    
	struct png image;   /*    optarg – указатель на текущий аргумент, если таковой имеется.
    optind – индекс на следующий указатель argv, который будет обработан при следующем вызове getopt().
    optopt – последний из известных параметров.*/
	int opt;
	struct global_args argument;
    int action = 0;
    int i;

    int longIndex = 0;
    
    argument.start[0] = -100;
    argument.start[1] = -100;
    argument.end[0] = -100;
    argument.end[1] = -150;
    argument.R = -1;
    argument.axis = 'x';
    argument.width = 1;

    argument.color[0] = 0;
    argument.color[1] = 0;
    argument.color[2] = 0;
    argument.color[3] = 255; // по эксперементировать почему 255 , а не 1 или 0     

  //  char *optstring = "drcE:S:R:C:O:W:ih";
    struct option longOpts[] = {
        {"reflect", no_argument, &action, 'r'},
        {"copy", no_argument, &action, 'c'},
        {"line", no_argument, &action, 'd'},
        {"help", required_argument, &action, 'h'},
        {"start", required_argument, NULL, 'S'}, 
        {"pentagram", no_argument, &action, 'p'},
	   {"figure" , no_argument, &action , 'f'},
		   {"zadacha", no_argument , &action , 'z'},
        {"end", required_argument, NULL, 'E'},
        {"color", required_argument, NULL, 'C'},
        {"axis", required_argument, NULL, 'A'},
        {"width", required_argument, NULL, 'W'},
        {"radius", required_argument, NULL, 'R'},
        {"info", required_argument, &action, 'i'},
        {NULL,  no_argument, NULL, 0}
};

    if(!strstr(argv[1], ".png"))
    {
        printf("Невозможно открыть файл\n");
        return 0;
    }
    read_png_file(&image, argv[1]);

    opt = getopt_long(argc, argv, optstring , longOpts, &longIndex);
    
    while(opt != -1 ) {
        switch(opt) {
            case 'E':
                
                if(!is_digit(optarg))
                {
                    printf("%s - это не целое число\n", optarg);
                    return 0;
                }
                
                if(!is_digit(argv[optind]))
                {
                    printf("%s - это не целое число\n", argv[optind]);
                    return 0;
                }

                argument.end[0] = atoi(optarg);
                argument.end[1] = atoi(argv[optind]);
                break;
            
            case 'S':

                if(!is_digit(optarg))
                {
                    printf("%s - это не целое число\n", optarg);
                    return 0;
                }
                if(!is_digit(argv[optind]))
                {
                    printf("%s - это не целое число\n", argv[optind]);
                    return 0;
                }
              
                argument.start[0] = atoi(optarg);
                argument.start[1] = atoi(argv[optind]);

                break;

            case 'R':
                
                if(!is_digit(optarg))
                {
                    printf("%s - это не целое число\n", optarg);
                    return 0;
                }
               
                argument.R = atoi(optarg);
                break;
            
            case 'W':
                
                if(!is_digit(optarg))
                {
                    printf("%s - это не целое число\n", optarg);
                    return 0;
                }
                
                argument.width = atoi(optarg);
                
                break;

            case 'C':
                
                if(!strcmp("red", optarg))
                {
                    argument.color[0] = 255;
                }
                else if (!strcmp("green", optarg))
                {
                    argument.color[1] = 255;
                }
                else if (!strcmp("blue", optarg))
                {
                    argument.color[3] = 255;
                }
                else if (!strcmp("white", optarg))
                {
                    argument.color[0] = 255;
                    argument.color[1] = 255;
                    argument.color[2] = 255;
                }
                else if (!strcmp("black", optarg))
                {

                }
                else if (!strcmp("orange", optarg))
                {
                    argument.color[0] = 255;
                    argument.color[1] = 165;
                    argument.color[2] = 0;
                }
                else if (!strcmp("yellow", optarg))
                {
                    argument.color[0] = 255;
                    argument.color[1] = 255;
                    argument.color[2] = 0;
                }
                else if (!strcmp("aqua", optarg))
                {
                    argument.color[0] = 0;
                    argument.color[1] = 255;
                    argument.color[2] = 255;
                }
                else
                {
                    printf("Цвета %s в палитре нет\n",  optarg);
		    return 0;
                }
                break;

            case 'A':

                if(!strcmp("x", optarg))
                {
                    argument.axis = 'x';
                }
                else if (!strcmp("y", optarg))
                {
                    argument.axis = 'y';
                }
                else
                {
                    printf("Не верная ось координат\n");
		            return 0;
                }
                break;
            
            case '?':
            case 'h':
	        action = 'h';
             //   print_help(); //////////////конец вчерашней
                break;
                
            case 'i':
                action = 'i';
                break;

            case 'r':
                action = 'r';
                break;
				
				
			case 'z':
                action = 'z';
                break;	
            
          /*  case 'c':
                flag = 'c';
                break;  */
            case 'p':
	        	action = 'p';
                // пентаграмма    
                break;
            case 'f':
		action = 'f';
		break;
	    case 'l':
                action = 'l';
                break;
        }
        opt = getopt_long(argc, argv, optstring , longOpts, &longIndex);
        
    }

    switch(action){
        case 'r': //////возможно лишние ограничения, разобраться как исправить !!!!!!!!!!!!!!!!!!!!
            if(argument.start[0] < 0 || argument.start[1] < 0 || argument.end[0] > image.width || argument.end[1] > (image.height - 1) || argument.start[0] >= argument.end[0] || argument.start[1] >= argument.end[1])
               {
                   printf("\nОбласть отражения не является корректной\n",RED,NONE);
                   break;
               }
            else
            {
                reflection(&image, argument.start[0], argument.start[1], argument.end[0], argument.end[1], argument.axis);
            }
            
        break;
/*
        case 'c':
            if(argument.start[0] < 0 || 
               argument.start[1] < 0 || 
               argument.end[0] > image.width || 
               argument.end[1] > (image.height - 1) ||
               argument.start[0] > argument.end[0] || 
               argument.start[1] > argument.end[1])
               {
                   printf("Область копирования не является корректной\n");
                   break;
               }
            else
            {
                doCopy(&image, argument.start[0], argument.start[1], argument.end[0], argument.end[1]);
            }
            
        break;
                */
        case 'p':

            if(argument.start[0] < 0 || argument.R < 0 || (argument.start[0] + argument.R) < 0 ||
               argument.start[1] < 0 || (argument.start[1]+argument.R) < 0 || (argument.start[0] - argument.R) < 0 ||
               argument.start[0] > image.width || (argument.start[0] + argument.R) > image.width ||
               argument.start[1] > (image.height) || (argument.start[1] + argument.R) > image.height  ||
                argument.width < 0)
               {
                   printf("\nНевозможно нарисовать пентаграмму по введенным данным\n",RED,NONE);
                   break;
               }
            else
            {
                // BresenhamCircle(&image, argument.start[0] , argument.start[1], argument.end[0], argument.width, argument.color);//возмнжно &color
  draw_circle(&image, argument.start[0] , argument.start[1], argument.R, argument.width, argument.color);
  draw_star(&image, argument.start[0], argument.start[1], argument.R, argument.width, argument.color);
            }
            
        break;
		
		case 'z':
					printf("Hi wat is gone %d", argument.R);
					zadacha(&image, argument.R);
					break;
        case 'l': /// ТОЖЕ лишние ограничения глянуть !!!!!!!!!!!!
             if(argument.start[0] < 0 || argument.end[0] < 0 ||
               argument.start[1] < 0 || argument.end[1] < 0 ||
               argument.end[0] > image.width || 
               argument.end[1] > (image.height) ||
                argument.width < 0)
               {
                   printf("\nНе возможно нарисовать линию по введенным данным\n", RED , NONE);
                   break;
               }
               else
                {
                 draw_line1(&image, argument.start[0] , argument.start[1], argument.end[0], argument.end[1], argument.width, argument.color,1);//возмнжно &color
                }
            
        break;

        case 'h':
            print_help();
	    return 0;
        break;
	case 'f':
if(argument.start[0] < 0 || argument.R < 0 || (argument.start[0] + argument.R) < 0 ||
               argument.start[1] < 0 || (argument.start[1]+argument.R) < 0 || (argument.start[0] - argument.R) < 0 ||
               argument.start[0] > image.width || (argument.start[0] + argument.R) > image.width ||
               argument.start[1] > (image.height) || (argument.start[1] + argument.R) > image.height  ||
                argument.width < 0)
               {
                   printf("\nНевозможно нарисовать фигуру по введенным данным\n",RED,NONE);
                   break;
               }
            else
            {
                // BresenhamCircle(&image, argument.start[0] , argument.start[1], argument.end[0], argument.width, argument.color);//возмнжно &color
  star(&image, argument.start[0], argument.start[1], argument.R, argument.width, argument.color,6);
 
 draw_circle(&image, argument.start[0] , argument.start[1], argument.R, argument.width, argument.color);
  //star(&image, argument.start[0], argument.start[1], argument.R, argument.width, argument.color,6);
            }
            
        break;
		

        case 'i':
            info(&image,strlen(argv[2]), argv[2]);
	    return 0;		
        break;
    }
    if (strstr((argv[argc-1]), ".png"))
        doNewFile(&image, argv[argc - 1]);
    else 
        doNewFile(&image, "result_of.png");
 return 0;
}
