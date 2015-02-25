TOOLPATH = ../z_tools
INCPATH  = $(TOOLPATH)/haribote/
MAKE     = make -r
NASK     = $(TOOLPATH)/nask
EDIMG    = $(TOOLPATH)/edimg
IMGTOL   = $(TOOLPATH)/imgtol
COPY     = cp
DEL      = rm

CC1 = $(TOOLPATH)/cc1 -I $(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)/gas2nask -a
OBJ2BIM = $(TOOLPATH)/obj2bim
BIN2OBJ = $(TOOLPATH)/bin2obj
BIM2HRB = $(TOOLPATH)/bim2hrb
RULEFILE = $(TOOLPATH)/haribote/haribote.rul
MAKEFONT = $(TOOLPATH)/makefont
GOLIB = $(TOOLPATH)/golib00
ASM2NASMFONT=$(TOOLPATH)/make_asm_font

#默认值

default :
	$(MAKE) run

#一般规则
bin/%.gas : %.c %.h head.h Makefile
	$(CC1) -o bin/$*.gas $*.c

bin/%.gas : %.c head.h Makefile
	$(CC1) -o bin/$*.gas $*.c
	
bin/%.nas : bin/%.gas Makefile
	$(GAS2NASK) bin/$*.gas bin/$*.nas

bin/%.obj : bin/%.nas Makefile
	$(NASK) bin/$*.nas bin/$*.obj bin/$*.lst
	
bin/%.obj : %.nas Makefile
	$(NASK) $*.nas bin/$*.obj bin/$*.lst
	
bin/%.bin : %.nas Makefile
	$(NASK) $*.nas bin/$*.bin bin/$*.lst
	
bin/%.bin : bin/%.nas Makefile
	$(NASK) bin/$*.nas bin/$*.bin bin/$*.lst
################生成字体##############################################
hankaku.bin :  hankaku.txt   Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin
bin/asmfont.nas.font : hankaku.bin  Makefile
	$(ASM2NASMFONT)  > tmp
	rm hankaku.bin
	cat    asmfonthead.nas > bin/asmfont.nas.font
	cat    tmp >> bin/asmfont.nas.font
	rm tmp
bin/asmfont.obj : bin/asmfont.nas.font Makefile
	$(NASK) bin/asmfont.nas.font bin/asmfont.obj bin/asmfont.lst
####################################################################

bin/bootpack.bim : bin/bootpack.obj bin/naskfunc.obj bin/asmfont.obj bin/graphic.obj bin/dsctbl.obj bin/int.obj \
		bin/fifo.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bin/bootpack.bim stack:3136k map:bin/bootpack.map \
		bin/bootpack.obj bin/naskfunc.obj bin/asmfont.obj bin/graphic.obj bin/dsctbl.obj bin/int.obj bin/fifo.obj
# 3MB+64KB=3136KB

bin/bootpack.hrb : bin/bootpack.bim Makefile
	$(BIM2HRB) bin/bootpack.bim bin/bootpack.hrb 0	
# ?R?}???h

bin/haribote.sys : bin/asmhead.bin bin/bootpack.hrb Makefile
	cat bin/asmhead.bin > bin/haribote.sys
	cat bin/bootpack.hrb >> bin/haribote.sys

bin/haribote.img : bin/ipl.bin bin/haribote.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:bin/ipl.bin len:512 from:0 to:0  \
		copy from:bin/haribote.sys to:@: imgout:bin/haribote.img

asm :
	$(MAKE) bin/ipl.bin

img :
	$(MAKE) bin/haribote.img

run : bin
	$(MAKE) img
	qemu-system-x86_64 -drive file=bin/haribote.img,if=floppy

bin :
	mkdir bin
	
all : src_only bin
	$(MAKE) img
	


clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) *.font
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) haribote.sys

src_only :
	-$(DEL) -rf bin/
