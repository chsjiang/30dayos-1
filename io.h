/*
 * io.h
 *
 *  Created on: 2015年2月26日
 *      Author: lanjin
 */

#ifndef IO_H_
#define IO_H_

void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_out8(int port, int data);
unsigned char io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
unsigned int load_cr0();
void store_cr0(int cr0);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void io_stihlt(void);



#endif /* IO_H_ */
