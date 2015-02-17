#include <stdio.h>

#include "head.h"


struct FIFO8 keyfifo;
struct FIFO8 mousefifo;

struct MOUSE_DEC mdec;

void wait_KBC_sendready(void){
	for(;;){
		if( (io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
			break;
	}
	return;
}

void init_keyboard(void){
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

void enable_mouse(struct MOUSE_DEC *mdec){
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	mdec->phase=0;
	mdec->mx=0;
	mdec->my=0;

	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, int data){
	if(mdec->phase == 0){
		if(data == 0xfa)
			mdec->phase = 1;
		return 0;
	}else if(mdec->phase == 1){
		if((data & 0xc8) == 0x08){
			mdec->buf[0] = data;
			mdec->phase = 2;
		}
		return 0;
	}else if(mdec->phase == 2){
		mdec->buf[1] = data;
		mdec->phase = 3;
		return 0;
	}else if(mdec->phase == 3){
		mdec->buf[2] = data;
		mdec->phase = 1;

		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];

		if((mdec->buf[0] & 0x10)  != 0)
			mdec->x |= 0xffffff00;
		if((mdec->buf[0] & 0x20)  != 0)
			mdec->y |= 0xffffff00;
		mdec->y = -mdec->y;

		return 1;	
	}
	return -1;
}

void HariMain(void){
	char *vram;	
	int xsize, ysize;
	struct BOOTINFO *binfo;

	char mcursor[16*8];
	char s[30];
	unsigned char data;

	char keybuf[KEYBUF_LEN];
	char mousebuf[MOUSEBUF_LEN];
	
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
	putblock8_8(binfo->vram, binfo->scrnx, 8, 16, 32, 32, mcursor, 8);



	sprintf(s, "*scrnx = %d", binfo->scrnx);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, col_white, s);
	
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
				boxfill8(binfo->vram, binfo->scrnx, col_blue_l_d, 0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, col_white, s);
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

					boxfill8(binfo->vram, binfo->scrnx, col_blue_l_d, 32, 16, 32 + 15*8-1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, col_white, s);
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
					putblock8_8(binfo->vram, binfo->scrnx, 8, 16, mdec.mx, mdec.my, mcursor, 8);
				}

			}	
		}
	}
		// io_hlt();	
}
// 14, 8,7,8,7,7,15,15,0,0,15,15,7,7

// col_white, col_gray
// col_black
// col_blue_l_d, col_gray_d

