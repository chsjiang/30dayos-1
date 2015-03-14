/*
 * graphic.h
 *
 *  Created on: 2015年2月27日
 *      Author: lanjin
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int xsize, int ysize);
void putfont8(char *vram, int xsize, int x, int y, char color, unsigned char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char color, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

enum  COLOR{
	col_black, col_red, col_green, col_yellow, \
	col_blue, col_pul, col_blue_l, col_white, \
	col_gray, col_red_d, col_green_d, col_yellow_d,\
	col_blue_d, col_pul_d, col_blue_l_d, col_gray_d
};

/*
 * Display
 */
struct SHEET{
	unsigned char *buf;
	int bxsize, bysize, vx0, vy0, col_inv;
	int status;
	struct SHEET *pre, *next;
	struct SHTCTL *ctl;
	int id;
};
#define MAX_SHEETS	256
#define SHEET_USE 	1

#define ENABLE_SHEET 1
#define DISABLE_SHEET -1

struct SHTCTL{
	unsigned char *vram, *map;
	int xsize, ysize;
	struct SHEET *nil;
	struct MEMMAN *mem_man;
	int id_next;
};
struct SHTCTL* shtctl_init(struct MEMMAN *mem_man, \
		unsigned char *vram, int xsize, int ysize);
struct SHEET* sheet_alloc(struct SHTCTL *ctl);
void sheet_free(struct SHEET *sht);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf,\
		int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht,\
		int height);
struct SHEET* sheet_top(struct SHTCTL *ctl);
struct SHEET* sheet_bot(struct SHTCTL *ctl);
void sheet_refreshsub(struct SHEET *sht,\
		int vx0, int vy0, int vx1, int vy1);
void sheet_refresh(struct SHTCTL *ctl);
void sheet_slide(struct SHEET *sht,\
		int vx0, int vy0);
void putblocksub(char *vram, int vxsize, \
		struct SHEET *sht, int vx0, int vy0, \
		int vx1, int vy1);
void putblock(char *vram, int vxsize, struct SHEET *sht);
void sheet_enable(struct SHEET *sht, int enable);

void make_window8(char *buf, int xsize, \
		int ysize, char * title);
void putblock_map(char *map, int vxsize, \
		struct SHEET *sht, int vx0, int vy0, \
		int vx1, int vy1);

#endif /* GRAPHIC_H_ */
