#include "fifo.h"

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf){
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size;
	fifo->flags=0;
	fifo->nr=0;
	fifo->nw=0;
}



int fifo8_put(struct FIFO8 *fifo, unsigned char data){
	if(fifo->free == 0){
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->nw] = data;
	fifo->nw++;
	if(fifo->nw == fifo->size)
		fifo->nw == 0;
	fifo->free--;
	return 0;
}

int fifo8_get(struct FIFO8 *fifo){
	int data;
	if(fifo->free == fifo->size){
		return -1;
	}
	data = fifo->buf[fifo->nr];
	fifo->nr++;
	if(fifo->nr == fifo->size)
		fifo->nr = 0;
	fifo->free++;
	return data;
}

int fifo8_status(struct FIFO8 *fifo){
	return fifo->size - fifo->free;
}