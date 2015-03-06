/*
 * input.h
 *
 *  Created on: 2015年2月26日
 *      Author: lanjin
 */

#ifndef INPUT_H_
#define INPUT_H_

/* int.c */
void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

#define PORT_KEYDAT		0x0060
#define PORT_KEYSTA		0x0064
#define PORT_KEYCMD		0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE	0x60
#define KBC_MODE		0x47
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE	0xf4

#define KEYBUF_LEN 32
#define MOUSEBUF_LEN  128


struct MOUSE_DEC
{
	int buf[3], phase;
	unsigned char btn;
	int x, y;
	int mx, my;
};

void wait_KBC_sendready(void);
void init_keyboard(void);
void enable_mouse(struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, int data);


#endif /* INPUT_H_ */
