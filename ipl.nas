; hello-os
; TAB=4

		ORG		0x7c00			;指明程序装载地址
		;0x00007c00 - 0x00007dff 启动区内容装载地址

; 以下的记述用于标准的FAT12格式软盘

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ƒu[ƒgƒZƒNƒ^‚Ì–¼‘O‚ðŽ©—R‚É‘‚¢‚Ä‚æ‚¢i8ƒoƒCƒgj
		DW		512				; 1ƒZƒNƒ^‚Ì‘å‚«‚³i512‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DB		1				; ƒNƒ‰ƒXƒ^‚Ì‘å‚«‚³i1ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		1				; FAT‚ª‚Ç‚±‚©‚çŽn‚Ü‚é‚©i•’Ê‚Í1ƒZƒNƒ^–Ú‚©‚ç‚É‚·‚éj
		DB		2				; FAT‚ÌŒÂ”i2‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		224				; ƒ‹[ƒgƒfƒBƒŒƒNƒgƒŠ—Ìˆæ‚Ì‘å‚«‚³i•’Ê‚Í224ƒGƒ“ƒgƒŠ‚É‚·‚éj
		DW		2880			; ‚±‚Ìƒhƒ‰ƒCƒu‚Ì‘å‚«‚³i2880ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DB		0xf0			; ƒƒfƒBƒA‚Ìƒ^ƒCƒvi0xf0‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		9				; FAT—Ìˆæ‚Ì’·‚³i9ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		18				; 1ƒgƒ‰ƒbƒN‚É‚¢‚­‚Â‚ÌƒZƒNƒ^‚ª‚ ‚é‚©i18‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		2				; ƒwƒbƒh‚Ì”i2‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DD		0				; ƒp[ƒeƒBƒVƒ‡ƒ“‚ðŽg‚Á‚Ä‚È‚¢‚Ì‚Å‚±‚±‚Í•K‚¸0
		DD		2880			; ‚±‚Ìƒhƒ‰ƒCƒu‘å‚«‚³‚ð‚à‚¤ˆê“x‘‚­
		DB		0,0,0x29		; ‚æ‚­‚í‚©‚ç‚È‚¢‚¯‚Ç‚±‚Ì’l‚É‚µ‚Ä‚¨‚­‚Æ‚¢‚¢‚ç‚µ‚¢
		DD		0xffffffff		; ‚½‚Ô‚ñƒ{ƒŠƒ…[ƒ€ƒVƒŠƒAƒ‹”Ô†
		DB		"HARIBOTEOS "	; ƒfƒBƒXƒN‚Ì–¼‘Oi11ƒoƒCƒgj
		DB		"FAT12   "		; ƒtƒH[ƒ}ƒbƒg‚Ì–¼‘Oi8ƒoƒCƒgj
		RESB	18				; ‚Æ‚è‚ ‚¦‚¸18ƒoƒCƒg‚ ‚¯‚Ä‚¨‚­

; ƒvƒƒOƒ‰ƒ€–{‘Ì


entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; 读软盘
;	AH:	02:read;  03:write; 04:check; 0c:寻道
;	AL: 处理对象的扇区数（智能同时处理连续的扇区）
;	CH:	柱面号&0xff
;	CL: 扇区号
;	DH:	磁头号
;	DL: 驱动器号
;	ES:BX:	缓冲区地址
;	返回值：CF :0,无错误    1:有错，错误码在AH中
		MOV		AX, 0x0820
		MOV		ES, AX			; 将数据存放到0x820中
		MOV		CH, 0			; 柱面0
		MOV		DH, 0			; 磁头0
		MOV		CL, 2			; 扇区2
readloop:
		MOV		SI, 0 			; 记录失败次数
retry:
		MOV		AH, 0x02		; AH=0x02 : 读盘
		MOV		AL, 1			; 1个扇区
		MOV		BX, 0
		MOV		DL, 0x00		; A驱动器
		INT		0x13			; 调用BIOS调用
		JNC		next 			; 没出错的话就准备读下一个扇区
		ADD		SI, 1 			;失败次数加一
		CMP		SI, 5
		JAE		error			; 失败次数大于等于5 则显示出错

		MOV		AH, 0x00
		MOV		DL, 0x00
		INT		0x13 			;复位软驱
		JMP		retry	
next:
		MOV		AX, ES
		ADD		AX, 0x0020		;目标地址加0x20 = 512/16
		MOV		ES, AX
		ADD		CL, 1 			;扇区向后加一
		CMP		CL, 18
		JBE		readloop		; <=18时继续向后读
		MOV		CL, 1
		ADD		DH, 1 			; 磁头加一
		CMP		DH, 2 			
		JB  		readloop		
		MOV		DH, 0 			; 已读过2个磁头，翻转到第一个
		ADD		CH, 1
		CMP		CH, 10 			
		JB 		readloop		
		; 表示已经读了10个柱面了
		MOV		[0x0ff0],CH
		JMP		0xc200


; “Ç‚ÝI‚í‚Á‚½‚¯‚Ç‚Æ‚è‚ ‚¦‚¸‚â‚é‚±‚Æ‚È‚¢‚Ì‚ÅQ‚é

fin:
		HLT						; CPU停止工作，等待
		JMP		fin				; 循环


error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 给SI加一
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,15			; 指定文字颜色
		INT		0x10			; BIOS中断
		JMP		putloop
msg:
		DB		0x0a, 0x0a		; 2个换行
		DB		"load error"
		DB		0x0a			; 1个换行
		DB		0 				; 字符串结束符

		RESB	0x7dfe-$	; 保留一段的数据直到0x7dfe

		DB		0x55, 0xaa	;0x7dfe 和0x7dff 为55aa表示是可引导的软盘
