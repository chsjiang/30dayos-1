#ifndef _MY_HEAD_FILE_
#define _MY_HEAD_FILE_

//#include "fifo.h"
//#include "mem.h"
//#include "input.h"



/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* ƒu[ƒgƒZƒNƒ^‚Í‚Ç‚±‚Ü‚ÅƒfƒBƒXƒN‚ð“Ç‚ñ‚¾‚Ì‚© */
	char leds; /* ƒu[ƒgŽž‚ÌƒL[ƒ{[ƒh‚ÌLED‚Ìó‘Ô */
	char vmode; /* ƒrƒfƒIƒ‚[ƒh  ‰½ƒrƒbƒgƒJƒ‰[‚© */
	char reserve;
	short scrnx, scrny; /* ‰æ–Ê‰ð‘œ“x */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

/* naskfunc.nas */

void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);



/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

#endif
