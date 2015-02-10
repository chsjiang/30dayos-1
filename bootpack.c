#include <stdio.h>


void io_hlt(void);
void write_mem8(int addr, char data);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void io_cli(void);
void io_sti(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_out8(int port, int data);

void boxfill8(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1);

void putfont8(char *vram, int xsize, int x, int y, char color, unsigned char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char color, unsigned char *str);

void init_mouse_cursor8(char *mouse, char background);
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
enum  COLOR{
	col_black, col_red, col_green, col_yellow, col_blue, col_pul,
	col_blue_l, col_white, col_gray, col_red_d, col_green_d,
	col_yellow_d, col_blue_d, col_pul_d, col_blue_l_d, col_gray_d
};

struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};


void HariMain(void){
	char *vram;	
	int xsize, ysize;
	struct BOOTINFO *binfo;

	char mcursor[16*8];
	char s[30];

	binfo = (struct BOOTINFO*) 0x0ff0;
	xsize = binfo->scrnx;
	ysize = binfo->scrny;
	vram  = binfo->vram;

	init_palette();

	
	
	vram = (char *)0xa0000;
	boxfill8(vram, xsize, col_blue_l_d, 0,           0         , xsize - 1, ysize - 29);
	boxfill8(vram, xsize, col_gray,     0,           ysize - 28, xsize - 1, ysize - 28);
	boxfill8(vram, xsize, col_white,    0,           ysize - 27, xsize - 1, ysize - 27);
	boxfill8(vram, xsize, col_gray,     0,           ysize - 26, xsize - 1, ysize - 1 );

	boxfill8(vram, xsize, col_white,    3,           ysize - 24, 59       , ysize - 24);
	boxfill8(vram, xsize, col_white,    2,           ysize - 24, 2        , ysize - 4 );
	boxfill8(vram, xsize, col_gray_d  , 3,           ysize - 4 , 59       , ysize - 4 );
	boxfill8(vram, xsize, col_gray_d  ,59,           ysize - 23, 59       , ysize - 5 );
	
	boxfill8(vram, xsize, col_black   , 2,           ysize - 3 , 59       , ysize - 3 );
	boxfill8(vram, xsize, col_black   ,60,           ysize - 24, 60       , ysize - 3 );

	boxfill8(vram, xsize, col_gray_d  ,xsize - 47,   ysize - 24, xsize-4  , ysize - 24 );
	boxfill8(vram, xsize, col_gray_d  ,xsize - 47,   ysize - 23, xsize-47 , ysize - 4  );
	boxfill8(vram, xsize, col_white   ,xsize - 47,   ysize - 3 , xsize-4 ,  ysize - 3  );
	boxfill8(vram, xsize, col_white   ,xsize - 3 ,   ysize - 23, xsize-3 ,  ysize - 3  );

	
	putfont8_asc(binfo->vram, binfo->scrnx, 8, 8, col_white, "Hello World");
	init_mouse_cursor8(mcursor, col_blue_l_d);
	putblock8_8(binfo->vram, binfo->scrnx, 8, 16, 32, 32, mcursor, 8);



	sprintf(s, "*scrnx = %d", binfo->scrnx);
	putfont8_asc(binfo->vram, binfo->scrnx, 16, 64, col_white, s);


	for( ; ; )
		io_hlt();	
}
// 14, 8,7,8,7,7,15,15,0,0,15,15,7,7

// col_white, col_gray
// col_black
// col_blue_l_d, col_gray_d

