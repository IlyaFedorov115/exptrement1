#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <png.h>
#include <getopt.h>

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
    int reflect;               /* параметр -r */
    char *langCode; 
    int start[2];              /* начало и конец отрезка */
    int end[2];
    int centre[2];             /* центр окружности */
    int R;                     /* радиус */
    char axis;                  /* ось координат */
    int width;                 /* ширина линии */
    png_byte color[4];         /* цвет линии */
};
 
static const char *optString = "lrpW:E:C:A:R:B:ih?";;

 struct option longOpts[] = {
        {"reflect", no_argument, &flag, 'r'},
        {"copy", no_argument, &flag, 'c'},
        {"line", no_argument, &flag, 'd'},
        {"help", required_argument, &flag, 'h'},
        {"start", required_argument, NULL, 'S'}, 
       // {"centre", required_argument, NULL, 'C'},
        {"end", required_argument, NULL, 'E'},
        {"color", required_argument, NULL, 'C'},
        {"axis", required_argument, NULL, 'A'},
        {"width", required_argument, NULL, 'W'},
        {"radius", required_argument, NULL, 'R'},
        {"info", required_argument, &flag, 'i'},
        {NULL,  no_argument, NULL, 0}
};


void read_png_file(struct png* image, char* fileName)
{
    int x,y;
    char header[8];

    FILE *fp = fopen(fileName, "rb");

    if(!fp) 
    {
		printf("[read_png_file] File %s could not be opened for reading", fileName);
        return 1;    //abort_
		//Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);  //считывает и потока в массив 8 объектов размера 1
	
	if (png_sig_cmp(header , 0 , 8){//функция, возвращает 0, если байты совпали с подписью png и !=0 в противном случае
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
	if (x1<x2) 
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
	if (width > 2)  
	{
		draw_line(image, x_1-1, y_1, x_2 -1, y_2, 1, rgb);
	    draw_line(image, x_1-1, y_1-1, x_2-1 , y_2-1, 1, rgb);
	}
}



void line_dither(struct png* image,int x1, int y1, int x2, int y2, int width, png_byte* rgb)
{
	// Целочисленные значения координат начала и конца отрезка,
	// округленные до ближайшего целого
/*	int iX1 = roundf(x1);
	int iY1 = roundf(y1);
	int iX2 = roundf(x2);
	int iY2 = roundf(y2);
 */
	// Длина и высота линии
	int deltaX = abs(x1 - x2);
	int deltaY = abs(y1 - y2);
 
	// Считаем минимальное количество итераций, необходимое
	// для отрисовки отрезка. Выбирая максимум из длины и высоты
	// линии, обеспечиваем связность линии
	int length = max(deltaX, deltaY);
 
	// особый случай, на экране закрашивается ровно один пиксел
	if (length == 0)
	{
		set_pixel(image,x2, y2, rgb);
		return;
	}
 
	// Вычисляем приращения на каждом шаге по осям абсцисс и ординат
	double dX = (x2 - x1) / length;
	double dY = (y2 - y1) / length;
 
	// Начальные значения
	double x = x1;
	double y = y1;
 
	// Основной цикл
	length++;
	while (length--)
	{
		x += dX;
		y += dY;
		int x_= roundf(x);
		int y_= roundf(y);
		set_pixel(image,x_, y_, rgb);
	}
}


void swap_pixel(png_byte* pix1, png_byte* pix2)
{
    int tmp[4];
	
    tmp[0] = pix2[0];    //  pix2[0] = pix2[0];    
    tmp[1] = pix2[1];		//pix2[1] = pix2[1];
    tmp[2] = pix2[2];		//pix2[2] = pix2[2];
    tmp[3] = pix2[3];		//pix2[3] = pix2[3];
	
	pix2[0] = pix2[0];
    pix2[1] = pix2[1];
    pix2[2] = pix2[2];
    pix2[3] = pix2[3];

    pix1[0] = tmp[0];
    pix1[1] = tmp[1];
    pix1[2] = tmp[2];
    pix1[3] = tmp[3];

}


/*

void line_Wy(struct png* image,int x0, int y0, int x1, int y1, int width, png_byte* rgb)
{
    var steep = Math.Abs(y1 - y0) > Math.Abs(x1 - x0);
	int a = abs(y1 - y0);
	int b = abs(x1 - x0);
	int flag = 0;
    if (a > b)
    {
		flag++;
		int tmp = x0;
		x0 = y0;
		y0 = tmp;
		tmp = x1;
		x1 = y1;
		y1 = tmp;
    }
    if (x0 > x1)
    {
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
		tmp = y0;
		y0 = y1;
		y1 = tmp;
    }
    if (flag = 0) 
    	{
		    set_pixel(image, int x1, int y1, rgb);
			set_pixel(image, int x0, int y0, rgb);
		}
    else 
	    {
			set_pixel(image, int y0, int x0, rgb);
		    set_pixel(image, int y1, int x1, rgb);
		}
		
    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = dy / dx;
    float y = y0 + gradient;
    for (var x = x0 + 1; x <= x1 - 1; x++)
    {
        DrawPoint(steep, x, (int)y, 1 - (y - (int)y));
        DrawPoint(steep, x, (int)y + 1, y - (int)y);
        y += gradient;
    }
}

*/



void draw_circle(struct png* image, int Sx, int Sy, int R, int width, png_byte *rgb)
{
    int Ex = Sx + R - 1;
    int Ey = Sy + R - 1;
    int i,j;

    for(i = 0; i <= R; i++)
    {
        for(j = 0; j <= R; j++){

            if( (i*i + j*j) <= R*R && (i*i + j*j) >= (R -width)*(R - width))
            {
               
                setPixel(image, Sx + i, Sy + j, rgb);
                setPixel(image, Sx + i, Sy - j, rgb);
                setPixel(image, Sx - i, Sy + j, rgb);
                setPixel(image, Sx - i, Sy - j, rgb);
            }
        }
    } 
    
}


/*	png_byte color[4];
	color[0] = 0;
    color[1] = 50;
    color[2] = 47;
    color[3] = 255;
	draw_line(&image, 10 , 10 , 40 , 40, 2, png_byte *rgb);
	doNewFile(&image, "lolcopy.png");*/
int is_digit(char *str)
{
    for(str; *str; str++)
    {
        if(!isdigit(*str))
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]){
   
    if (argc == 1) {
        prinf("Ошибка, не введены парметры");
        return 0;
    }

	struct png image;   /*    optarg – указатель на текущий аргумент, если таковой имеется.
    optind – индекс на следующий указатель argv, который будет обработан при следующем вызове getopt().
    optopt – последний из известных параметров.*/
	int opt;
	struct global_args argument;
    int flag = 0;
    struct png image;
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

    if(read_png_file(&image, argv[1]) || !strstr(argv[1], ".png"))
    {
        printf("Невозможно открыть файл\n");
        return 0;
    }

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
                    printf("Цвета %s  в палитре нет",  optarg);
                }
                break;

            case 'A':

                if(!strcmp("x", optarg)
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
                }
                break;
            
            case '?':
            case 'h':
                printHelp(); //////////////конец вчерашней
                break;
                
            case 'i':
                flag = 'i';
                break;

            case 'r':
                flag = 'r';
                break;
            
            case 'c':
                flag = 'c';
                break;
            
            case 'd':
                flag = 'd';
                break;
        }
           
        opt = getopt_long(argc, argv, optstring , longOpts, &longIndex);
        
    }

    switch(flag){
        case'r':

            if(a.start[0] < 0 || 
               a.start[1] < 0 || 
               a.end[0] > image.width || 
               a.end[1] > (image.height - 1) ||
               a.start[0] >= a.end[0] || 
               a.start[1] >= a.end[1] )
               {
                   printf("Область отражениея не является корректной\n");
                   break;
               }
            else
            {
                reflect(&image, a.start[0], a.start[1], a.end[0], a.end[1], a.Os);
            }
            
        break;

        case'c':

            if(a.start[0] < 0 || 
               a.start[1] < 0 || 
               a.end[0] > image.width || 
               a.end[1] > (image.height - 1) ||
               a.start[0] > a.end[0] || 
               a.start[1] > a.end[1])
               {
                   printf("Область копирования не является корректной\n");
                   break;
               }
            else
            {
                doCopy(&image, a.start[0], a.start[1], a.end[0], a.end[1]);
            }
            
        break;

        case'd':

            if(a.start[0] < 0 || 
               a.start[1] < 0 || 
               a.start[0] + a.R > image.width || 
               a.start[1] + a.R > (image.height - 1)||
               a.start[0] - a.R < 0 ||
               a.start[1] - a.R < 0 ||
               a.R <= 0 ||
               a.width > a.R ||
               a.width < 0)
               {
                   printf("Невозможно нарисовать круг по введенным данным\n");
                   break;
               }
            else
            {
                drawCircle(&image,a.start[0], a.start[1], a.R, a.width, a.color);
            }
            
        break;

        case 'h':
            printHelp();
        break;

        case 'i':
            printf("Width = %d\n", image.width);
            printf("Heigth = %d\n", image.height);
        break;
    }
  
    doNewFile(&image, argv[argc - 1]);

}