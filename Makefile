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
%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

%.bin : %.nas Makefile
	$(NASK) $*.nas $*.bin $*.lst
################生成字体##############################################
hankaku.bin :  hankaku.txt   Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin
asmfont.nas.font : hankaku.bin  Makefile
	$(ASM2NASMFONT)  > tmp
	cat    asmfonthead.nas > asmfont.nas.font
	cat    tmp >> asmfont.nas.font
	rm tmp
asmfont.obj : asmfont.nas.font Makefile
	$(NASK) asmfont.nas.font asmfont.obj asmfont.lst
####################################################################

bootpack.bim : bootpack.obj naskfunc.obj asmfont.obj graphic.obj dsctbl.obj int.obj fifo.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj asmfont.obj graphic.obj dsctbl.obj int.obj fifo.obj
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0	
# ?R?}???h

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin > haribote.sys
	cat bootpack.hrb >> haribote.sys

haribote.img : ipl.bin haribote.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0  \
		copy from:haribote.sys to:@: imgout:haribote.img

asm :
	$(MAKE) ipl.bin

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	qemu-system-x86_64 -drive file=haribote.img,if=floppy

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
	$(MAKE) clean
	-$(DEL) haribote.img
