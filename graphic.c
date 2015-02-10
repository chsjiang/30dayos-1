#include "head.h"


void init_mouse_cursor8(char *mouse, char background)
{
	static char cursor[16][8] = {
		"10000000",
		"11000000",
		"12100000",
		"12210000",
		"12221000",
		"12222100",
		"12222210",
		"12222221",
		"12222110",
		"12221100",
		"12112100",
		"11001210",
		"00001210",
		"00000110",
		"00000000",
		"00000000"
	};
	int x, y;
	for(y = 0; y < 16; y++)
	{
		for(x = 0; x < 8; x++)
		{
			if(cursor[y][x] == '1')
				mouse[y * 8 + x] = col_black;
			else if(cursor[y][x] == '2')
				mouse[y * 8 + x] = col_white;
			else
				mouse[y * 8 + x] = background;
		}
	}
	return;
}
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for(y = 0; y < pysize; y++)
	{
		for(x=0; x < pxsize; x++)
		{
			vram[(py0+y) * vxsize +(px0 +x)] = buf[y*bxsize+x];
		}
	}
	return;
}

void init_palette(void){
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,
		0xff, 0x00, 0x00,
		0x00, 0xff, 0x00,
		0xff, 0xff, 0x00,
		0x00, 0x00, 0xff,
		0xff, 0x00, 0xff,
		0x00, 0xff, 0xff,
		0xff, 0xff, 0xff,
		0xc6, 0xc6, 0xc6,
		0x84, 0x00, 0x00,
		0x00, 0x84, 0x00,
		0x84, 0x84, 0x00,
		0x00, 0x00, 0x84,
		0x84, 0x00, 0x84,
		0x00, 0x84, 0x84,
		0x84, 0x84, 0x84
	};
	set_palette(0, 15, table_rgb);
	return;
}


void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	
	io_cli();
	io_out8(0x03c8, start);
	for(i = start; i <= end; i++){
		io_out8(0x03c9, rgb[0]/4);
		io_out8(0x03c9, rgb[1]/4);
		io_out8(0x03c9, rgb[2]/4);
		rgb += 3;
	}
	io_store_eflags(eflags);
	return ;
}
void boxfill8(unsigned char *vram, int xsize, \
	unsigned char color, int x0, int y0, int x1, int y1)
{
	unsigned char *p = vram;
	int x, y;
	for(y = y0; y <= y1; y++)
	{
		for(x = x0; x <= x1; x++)
		{
			*(p + xsize*y + x) = color;
		}
	}
}

void putfont8(char *vram, int xsize, int x, int y, char color, unsigned char *font){
	int i, j;
	char *p, d;
	unsigned char t = 0x80;
	for(i = 0; i < 16; i++)
	{
		j = 0;
		t = 0x80;
		p = vram + (y + i) * xsize + x;
		d = font[i];
		do{
			if(d & t)
				p[j] = color;
			j++;
			t = t >> 1;
		}while(t);
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char color, unsigned char *str){
	extern char hankanku[4096];
	for(; *str != '\0'; str++){
		putfont8(vram, xsize, x, y, color, hankanku + *str * 16);
		x += 8;
	}
	return;
}


void init_screen8(char *vram, int xsize, int ysize)
{
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
	return;
}

