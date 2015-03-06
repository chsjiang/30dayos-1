/*
 * mem.h
 *
 *  Created on: 2015年2月26日
 *      Author: lanjin
 */

#ifndef MEM_H_
#define MEM_H_


#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000

#define MEM_MAX	32768
struct MEMMAN{
	char *mem_map;
	unsigned int frees;
	unsigned int total;
};

unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);
unsigned int memtest_sub_nas(unsigned int start, unsigned int end);

char mem_init(struct MEMMAN *mem_man, char *mem_map, \
		unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *mem_man, unsigned int size);
int memman_free_4k(struct MEMMAN *mem_man, unsigned int addr,\
		unsigned int size);
#endif /* MEM_H_ */
