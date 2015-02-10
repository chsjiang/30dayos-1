#include <stdio.h>

#include "head.h"


void HariMain(void){
	char *vram;	
	int xsize, ysize;
	struct BOOTINFO *binfo;

	char mcursor[16*8];
	char s[30];

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

	for( ; ; )
		io_hlt();	
}
// 14, 8,7,8,7,7,15,15,0,0,15,15,7,7

// col_white, col_gray
// col_black
// col_blue_l_d, col_gray_d

