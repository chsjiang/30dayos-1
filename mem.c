/*
 * mem.c
 *
 *  Created on: 2015年2月26日
 *      Author: lanjin
 */
#include "mem.h"
#include "io.h"


unsigned int memtest_sub(unsigned int start, unsigned int end){
	unsigned int i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
	for(i = start; i <= end; i += 4 * 1024){
		p = (unsigned int*) i;
		old = *p;
		*p = pat0;
		*p ^= 0xffffffff;
		if(*p != pat1){
			*p = old;
			break;
		}
		*p = pat1;
		*p ^= 0xffffffff;
		if(*p != pat0){
			*p = old;
			break;
		}
		*p = old;
	}
	return i;
}
unsigned int memtest(unsigned int start, unsigned int end){
	char flg486 = 0;
	unsigned int eflg, cr0, size;
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT;
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if((eflg & EFLAGS_AC_BIT) != 0)
		flg486 = 1;
	eflg &= ~EFLAGS_AC_BIT;
	io_store_eflags(eflg);

	if(flg486){
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE;
		store_cr0(cr0);
	}
	size = memtest_sub_nas(start, end);
	if(flg486 != 0){
		cr0 = load_cr0();
		cr0 &= ~ CR0_CACHE_DISABLE;
		store_cr0(cr0);
	}
	return size;
}

char mem_init(struct MEMMAN *mem_man, char *mem_map, unsigned int size){
	unsigned int i = 0;
	mem_man->mem_map =  mem_map;

	for( i = 0; i < size; i++){
		mem_man->mem_map[i] = 0;
	}
	//保留前4M空间1024*4k
	for( i = 0; i < 1024; i++){
		mem_man->mem_map[i] = 1;
	}
	mem_man->total = size;
	mem_man->frees = size - 1024;
	return 0;
}

unsigned int memman_alloc_4k(struct MEMMAN *mem_man,\
		unsigned int size){
	int i = 0;
	int start = -1;
	int size4k = (size+0xfff) / 0x1000;
	for(i=0; i<mem_man->total; i++){
		if(mem_man->mem_map[i] == 0){
			start = i;
			for(i++; i < start+size4k; i++)
				if(mem_man->mem_map[i]!=0)
					break;
			//如果当前区域没有足够的内存区域
			if(start + size4k != i)
				start = -1;
			else
				break;
		}
	}
	if(start != -1){
		for(i=start; i< start+size4k; i++)
			mem_man->mem_map[i]=1;
		mem_man->frees -= size4k;
		return start*0x1000;
	}
	return 0;
}

int memman_free_4k(struct MEMMAN *mem_man, \
		unsigned int addr, unsigned int size)
{
	int i;
	int size4k = (size+0xfff) / 0x1000;
	int addr4k = addr/0x1000;
	for(i=addr4k; i<addr4k+size4k && i<mem_man->total; i++){
		mem_man->mem_map[i] = 0;
	}
	mem_man->frees += i-addr4k;
	return 1;
}
int mem_avl(struct MEMMAN *mem_man){
	return mem_man->frees;
}
