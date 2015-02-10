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

void putfont8_asc(char *vram, int xsize, int x, int y, char color, unsigned char *str){
	extern char hankanku[4096];
	for(str ; *str != '\0'; str++){
		putfont8(vram, xsize, x, y, color, hankanku + *str * 16);
		x += 8;
	}
	return;
}

