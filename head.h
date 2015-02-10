#ifndef _MY_HEAD_
#define _MY_HEAD_
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
struct SEGMENT_DESCRIPTOR{		//8字节
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_hight, base_hight;
};



struct GATE_DESCRIPTOR			//8字节
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int size, int base);
void load_idtr(int size, int base);
enum  COLOR{
	col_black, col_red, col_green, col_yellow, col_blue, col_pul,
	col_blue_l, col_white, col_gray, col_red_d, col_green_d,
	col_yellow_d, col_blue_d, col_pul_d, col_blue_l_d, col_gray_d
};

#endif