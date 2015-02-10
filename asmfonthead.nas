; asmfont
; TAB=4

[FORMAT "WCOFF"]	; 制作目标文件的模式
[INSTRSET "i486p"]	; 
[BITS 32]			; 32bit 

; the object file info
[FILE "asmfont.nas"]	; source file's name
		GLOBAL 		_hankanku
[SECTION .data]
_hankanku: