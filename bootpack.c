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

	char mcursor[16*8];
	char s[30];
	unsigned char data;

	char keybuf[KEYBUF_LEN];
	char mousebuf[MOUSEBUF_LEN];
	
	//前4M为保留空间，如果内存小于4M本系统讲无法运行
	unsigned int mem_size = memtest(0x00400000, 0xbfffffff) / 0x1000;



	fifo8_init(&keyfifo, KEYBUF_LEN, keybuf);
	fifo8_init(&mousefifo, MOUSEBUF_LEN, mousebuf);

	binfo = (struct BOOTINFO*) ADR_BOOTINFO;
	xsize = binfo->scrnx;
	ysize = binfo->scrny;
	vram  = binfo->vram;

	init_gdtidt();
	init_pic();
	io_sti();

	init_palette();
	init_screen8(vram, xsize, ysize);

	putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, col_white, "Hello World");
	init_mouse_cursor8(mcursor, col_blue_l_d);
//	putblock8_8(binfo->vram, binfo->scrnx, 8, 16, 32, 32, mcursor, 8);

	sprintf(s, "*scrnx = %d", binfo->scrnx);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 40, col_white, s);
	
	mem_init(&mem_man, mem_map, mem_size);
	//内存总数
	sprintf(s, "memory : %d * 4K", mem_man.total);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 56, col_white, s);
	//内存剩余量以4K为单位
	sprintf(s, "free: %08X pages", mem_man.frees);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 72, col_white, s);

	io_out8(PIC0_IMR, 0xf9); /* PIC1‚ÆƒL[ƒ{[ƒh‚ð‹–‰Â(11111001) */
	io_out8(PIC1_IMR, 0xef); /* ƒ}ƒEƒX‚ð‹–‰Â(11101111) */

	init_keyboard();
	enable_mouse(&mdec);
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
				boxfill8(binfo->vram, binfo->scrnx, col_blue_l_d, 0, 24, 15, 40-1);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 24, col_white, s);
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
					boxfill8(binfo->vram, binfo->scrnx, col_blue_l_d, 32, 24, 32 + 15*8-1, 40-1);
					//新的文字
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 24, col_white, s);
					//擦掉原来的鼠标
					boxfill8(binfo->vram, binfo->scrnx, col_blue_l_d, mdec.mx, mdec.my, mdec.mx+8, mdec.my+16);
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
					//更新鼠标
					putblock8_8(binfo->vram, binfo->scrnx, 8, 16, mdec.mx, mdec.my, mcursor, 8);
				}

			}	
		}
	}
}
