#include "graphic.h"

#include "io.h"
#include "mem.h"


void init_mouse_cursor8(char *mouse, char col_inv)
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
				mouse[y * 8 + x] = col_inv;
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize, \
		int pysize, int px0, int py0, char *buf, \
		int bxsize)
{
	int x, y;
	for(y = 0; y < pysize; y++)
	{
		for(x=0; x < pxsize; x++)
		{
			vram[(py0+y) * vxsize +(px0 +x)] = \
					buf[y*bxsize+x];
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

void putfont8(char *vram, int xsize, int x, int y, char color, unsigned char *font)
{
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

void putfonts8_asc(char *vram, int xsize, int x, int y, char color, unsigned char *str)
{
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

struct SHTCTL* shtctl_init(struct MEMMAN *mem_man, \
		unsigned char *vram, int xsize, int ysize)
{
	struct SHTCTL *ctl = 0;
	struct SHEET *nil;
	nil = (struct SHEET*)memman_alloc_4k(mem_man, \
			sizeof(struct SHEET));
	if(nil == 0) goto err;
	ctl = (struct SHTCTL *)memman_alloc_4k(mem_man, \
			sizeof(struct SHTCTL));
	if(ctl == 0) goto err;

	ctl->mem_man = mem_man;
	ctl->vram = vram;
	ctl->xsize = xsize;
	ctl->ysize = ysize;
	ctl->nil = nil;
	nil->next = nil;
	nil->pre = nil;
err:
	return ctl;
}

struct SHEET* sheet_alloc(struct SHTCTL *ctl)
{
	struct SHEET *sht, *nil;
	nil = ctl->nil;
	sht = (struct SHEET*)memman_alloc_4k(ctl->mem_man,\
			sizeof(struct SHEET));
	if(sht == 0)
		goto err;
	sht->status = -1;
	//默认将申请到的图层放到最顶层
	nil->pre->next = sht;
	sht->next = nil;
	sht->pre = nil->pre;
	nil->pre = sht;
	sht->ctl = ctl;
err:
	return sht;
}

void sheet_free(struct SHEET *sht)
{
	struct SHEET *pre, *next;
	struct MEMMAN *mem_man = sht->ctl->mem_man;
	pre = sht->pre;
	next = sht->next;
	pre->next = next;
	next->pre = pre;
	memman_free_4k(mem_man,(unsigned int)sht,\
			sizeof(struct SHEET));
	return;
}

void sheet_setbuf(struct SHEET *sht, unsigned char *buf,\
		int xsize, int ysize, int col_inv)
{
	sht->buf = buf;
	sht->bxsize = xsize;
	sht->bysize = ysize;
	sht->col_inv = col_inv;
}

void sheet_updown(struct SHEET *sht, int height)
{
	struct SHEET *cur, *pre, *next;
	struct SHTCTL *ctl = sht->ctl;
	if(sht == 0 || height == 0)
		return;
	if(height > 0){
		while(height--){
			if (ctl->nil->pre == sht) //ctl->top == sht
				return;
			cur = sht;
			pre = sht->pre;
			next = sht->next;

			pre->next = next;
			next->pre = pre;
			next->next->pre = cur;
			cur->next = next->next;
			next->next = cur;
			cur->pre = next;
		}
	}else{
		while(height++){
			if (ctl->nil->next == sht) //ctl->bot==sht
				return;
			cur = sht;
			pre = sht->pre;
			next = sht->next;

			pre->next = next;
			next->pre = pre;
			pre->pre->next = cur;
			cur->pre = pre->pre;
			cur->next = pre;
			pre->pre=cur;
		}
	}
}

struct SHEET* sheet_top(struct SHTCTL *ctl)
{
	return ctl->nil->pre;
}
struct SHEET* sheet_bot(struct SHTCTL *ctl)
{
	return ctl->nil->next;
}
void putblock(char *vram, int vxsize, struct SHEET *sht)
{
	putblocksub(vram, vxsize, sht, 0, 0, \
			sht->bxsize, sht->bysize);
}
void putblocksub(char *vram, int vxsize, \
		struct SHEET *sht, int vx0, int vy0, \
		int vx1, int vy1)
{
	int x, y, col;
	int bx0, bx1, by0, by1;
	bx0 = vx0 - sht->vx0;
	bx1 = vx1 - sht->vx0;
	by0 = vy0 - sht->vy0;
	by1 = vy1 - sht->vy0;
	if(bx0 < 0) bx0 = 0;
	if(bx1 > sht->bxsize) bx1 = sht->bxsize;
	if(by0 < 0) by0 = 0;
	if(by1 > sht->bysize) by1 = sht->bysize;
	for(x=bx0; x<bx1; x++){
		for(y=by0; y<by1; y++){
			col = sht->buf[y*sht->bxsize+x];
			if(col != sht->col_inv)
				vram[(sht->vy0+y)*vxsize + (sht->vx0+x)]\
				= col;
		}
	}
}
void sheet_refresh(struct SHTCTL *ctl)
{
	struct SHEET *sht;
	sht = sheet_bot(ctl);
	if(sht != ctl->nil){
		do{
			putblock(ctl->vram, ctl->xsize, sht);
			sht = sht->next;
		}while(sht != ctl->nil);
	}
}
void sheet_refreshsub(struct SHEET *sht,\
		int vx0, int vy0, int vx1, int vy1)
{
	struct SHTCTL *ctl;
	ctl = sht->ctl;
//	sht = sheet_bot(ctl);
	if(sht != ctl->nil){
		do{
			if(vx0 < 0) vx0 = 0;
			if(vx1 > ctl->xsize)vx1=ctl->xsize;
			if(vy0 < 0) vy0=0;
			if(vy1 > ctl->ysize)vy1=ctl->ysize;

			if(sht->status == ENABLE_SHEET)
				putblocksub(ctl->vram, ctl->xsize, sht, \
					vx0, vy0, vx1, vy1);
			sht = sht->next;
		}while(sht != ctl->nil);
	}
}

void sheet_slide(struct SHEET *sht,	int vx0, int vy0)
{
	struct SHTCTL *ctl = sht->ctl;
	int vx0_old, vy0_old;
	vx0_old = sht->vx0;
	vy0_old = sht->vy0;
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	struct SHEET *bot = sheet_bot(ctl);
	sheet_refreshsub(bot, vx0_old, vy0_old, \
			vx0_old+sht->bxsize, vy0_old+sht->bysize);
	sheet_refreshsub(sht, vx0, vy0, \
			vx0+sht->bxsize, vy0+sht->bysize);
}

void make_window8(char *buf, int xsize, \
		int ysize, char * title)
{
	static char closebtn[14][16] = {
			"0000000000000003",
			"0111111111111123",
			"0111111111111123",
			"0111331111331123",
			"0111133113311123",
			"0111113333111123",
			"0111111331111123",
			"0111113333111123",
			"0111133113311123",
			"0111331111331123",
			"0111111111111123",
			"0111111111111123",
			"0222222222222223",
			"3333333333333333"
	};
	int x, y;
	char c;
	boxfill8(buf, xsize, col_gray,  0, 0, xsize-1, 0);
	boxfill8(buf, xsize, col_white, 1, 1, xsize-2, 1);
	boxfill8(buf, xsize, col_gray, 0, 0, 0, ysize-1);
	boxfill8(buf, xsize, col_white, 1, 1, 1, ysize-1);
	boxfill8(buf, xsize, col_gray_d, xsize-2, 1, xsize-2, ysize-1);
	boxfill8(buf, xsize, col_black, xsize-1, 0, xsize-1, ysize-1);
	boxfill8(buf, xsize, col_gray, 2, 2, xsize-3, ysize-3);
	boxfill8(buf, xsize, col_blue_d, 3, 3, xsize-4, 20);
	boxfill8(buf, xsize, col_gray_d, 1, ysize-2, xsize-2, ysize-2);
	boxfill8(buf, xsize, col_black, 0, ysize-1, xsize-1, ysize-1);
	putfonts8_asc(buf, xsize, 24,4,col_white, title);

	for(y=0; y<14; y++)
		for(x=0;x<16;x++){
			c=closebtn[y][x];
			if(c=='3') c=col_black;
			else if(c=='2') c=col_gray_d;
			else if(c=='1') c=col_gray;
			else c=col_white;
			buf[(5+y)*xsize+(xsize-21+x)]=c;
		}
//	};

}
void sheet_enable(struct SHEET *sht, int enable)
{
	sht->status = enable;
}
