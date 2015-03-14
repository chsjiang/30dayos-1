#include <stdio.h>

#include "head.h"
#include "input.h"
#include "io.h"
#include "fifo.h"
#include "graphic.h"
#include "mem.h"


struct FIFO8 keyfifo;
struct FIFO8 mousefifo;
struct MOUSE_DEC mdec;


char mem_map[MEM_MAX]; //32768=128M/4k
struct MEMMAN mem_man;

void HariMain(void){
	char *vram;	
	int xsize, ysize;
	struct BOOTINFO *binfo;
	char s[30];
	unsigned char data;

	char keybuf[KEYBUF_LEN];
	char mousebuf[MOUSEBUF_LEN];
	
	//前4M为保留空间，如果内存小于4M本系统讲无法运行
	unsigned int mem_size = memtest(0x00400000, 0xbfffffff) / 0x1000;
	mem_init(&mem_man, mem_map, mem_size);



	fifo8_init(&keyfifo, KEYBUF_LEN, keybuf);
	fifo8_init(&mousefifo, MOUSEBUF_LEN, mousebuf);

	binfo = (struct BOOTINFO*) ADR_BOOTINFO;
	xsize = binfo->scrnx;
	ysize = binfo->scrny;
	vram  = binfo->vram;

	init_gdtidt();
	init_pic();
	io_sti();



	io_out8(PIC0_IMR, 0xf9); /* PIC1‚ÆƒL[ƒ{[ƒh‚ð‹–‰Â(11111001) */
	io_out8(PIC1_IMR, 0xef); /* ƒ}ƒEƒX‚ð‹–‰Â(11101111) */

	init_keyboard();
	enable_mouse(&mdec);

	init_palette();
	char *buf_back, *buf_mouse, *buf_window;
	struct SHTCTL *shtctl = shtctl_init(&mem_man, \
				binfo->vram, binfo->scrnx, binfo->scrny);
	struct SHEET *sht_back, *sht_mouse, *sht_window;
	sht_back = sheet_alloc(shtctl);
	sht_mouse = sheet_alloc(shtctl);
	sht_window = sheet_alloc(shtctl);

	buf_back = (char*)memman_alloc_4k(&mem_man, xsize*ysize);
	buf_mouse = (char*)memman_alloc_4k(&mem_man, 8*16);
	buf_window= (char*)memman_alloc_4k(&mem_man, 160*68);
	sheet_enable(sht_back, ENABLE_SHEET);
	sheet_enable(sht_mouse, ENABLE_SHEET);
	sheet_enable(sht_window, ENABLE_SHEET);

	sheet_setbuf(sht_back, buf_back, xsize, ysize, -1);
	init_screen8(buf_back, xsize, ysize);
	putfonts8_asc(buf_back, xsize, 8, 8, col_white, "Hello World");
	sprintf(s, "*scrnx = %d", xsize);
	putfonts8_asc(buf_back, xsize, 16, 40, col_white, s);
	//内存总数
	sprintf(s, "memory : %d * 4K", mem_man.total);
	putfonts8_asc(buf_back, xsize, 16, 56, col_white, s);
	//内存剩余量以4K为单位
	sprintf(s, "free: %08X pages", mem_man.frees);
	putfonts8_asc(buf_back, xsize, 16, 72, col_white, s);
	sheet_slide(sht_back, 0, 0);

	init_mouse_cursor8(buf_mouse, 99);
//	putblock8_8(binfo->vram, binfo->scrnx, 8, 16, 32, 32, buf_mouse, 8);
	sheet_setbuf(sht_mouse, buf_mouse, 8, 16, 99);
	sheet_slide(sht_mouse, 32, 32);

	sheet_setbuf(sht_window, buf_window, 160, 68, -1);
	make_window8(buf_window, 160, 68, "window");
	sheet_slide(sht_window, 80, 87);
//	sheet_updown(sht_window,-1);
	for( ; ; )
	{
		io_cli();
		if(fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0)
			io_stihlt();
		else{
			if(fifo8_status(&keyfifo) != 0){
				data = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", data);
				boxfill8(buf_back, binfo->scrnx, col_blue_l_d, 0, 24, 15, 40-1);
				putfonts8_asc(buf_back, binfo->scrnx, 0, 24, col_white, s);
			}

			if(fifo8_status(&mousefifo) != 0){
				data = fifo8_get(&mousefifo);
				io_sti();
				if(mouse_decode(&mdec, data) == 1){
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if((mdec.btn & 0x01) != 0)
						s[1] = 'L';
					if((mdec.btn & 0x02) != 0)
						s[2] = 'C';
					if((mdec.btn & 0x04) != 0)
						s[3] = 'R';
					//擦掉文字
					boxfill8(buf_back, binfo->scrnx, col_blue_l_d, 32, 24, 32 + 15*8-1, 40-1);
					//新的文字
					putfonts8_asc(buf_back, binfo->scrnx, 32, 24, col_white, s);

					sheet_refreshsub(shtctl, 32, 24, 32+15*8-1, 40-1);
					mdec.mx += mdec.x;
					mdec.my += mdec.y;
					if(mdec.mx < 0)
						mdec.mx = 0;
					else if(mdec.mx >= xsize - 1)
						mdec.mx = xsize - 1;
					if(mdec.my < 0)
						mdec.my = 0;
					else if(mdec.my >= ysize - 1)
						mdec.my = ysize - 1;
					sheet_slide(sht_mouse, \
							mdec.mx, mdec.my);
				}

			}
		}
	}
}
