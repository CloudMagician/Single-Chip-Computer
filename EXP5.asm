ORG 	0000H
LJMP 	START
ORG 	0040H

;choose
TAB1:
DB 00H,01H
DB 00H,02H
DB 00H,04H
DB 00H,08H
DB 00H,10H
DB 00H,20H
DB 00H,40H
DB 00H,80H
DB 01H,00H
DB 02H,00H
DB 04H,00H
DB 08H,00H
DB 10H,00H
DB 20H,00H
DB 40H,00H
DB 80H,00H

;show
TAB2:
DB 00H,00H,3FH,0FEH,20H,14H,20H,24H,20H,0C4H,3FH,04H,20H,04H,20H,04H;
DB 20H,04H,3FH,84H,20H,44H,20H,44H,20H,44H,3FH,0FEH,00H,00H,00H,00H		;"四",0

DB 21H,08H,21H,0CH,3FH,0F8H,21H,10H,21H,11H,00H,42H,1FH,84H,00H,18H;
DB 0FFH,0E0H,00H,00H,21H,04H,21H,04H,3FH,0FCH,21H,04H,21H,04H,00H,00H	;"班",1

DB 00H,00H,7FH,0FFH,40H,10H,44H,08H,5BH,10H,60H,0E0H,01H,00H,11H,3EH;
DB 11H,02H,11H,02H,0FFH,0FEH,11H,02H,11H,02H,11H,3FH,01H,00H,00H,00H	;"陆",2

DB 01H,00H,41H,00H,41H,00H,41H,00H,41H,00H,41H,02H,41H,01H,47H,0FEH;
DB 45H,00H,49H,00H,51H,00H,61H,00H,41H,00H,01H,00H,01H,00H,00H,00H		;"子",3

DB 08H,01H,08H,06H,0FFH,0F8H,08H,00H,08H,00H,0FH,0FCH,00H,02H,00H,02H;
DB 7FH,0F2H,44H,22H,44H,22H,44H,22H,7FH,0F2H,00H,02H,00H,1EH,00H,00H	;"旭",4

START:
DX 		EQU P0.0
CKX 	EQU P0.1
CKXL 	EQU P0.2
DY 		EQU P0.3
ENY 	EQU P0.4
CKY 	EQU P0.5
CKYL 	EQU P0.6
ENX 	EQU P0.7
MOV		R3,#64	;turn
MOV 	R4,#0	;count

TTT:
MOV		R7,#40

MAIN:
MOV 	R0,#0	;first
MOV 	R1,#1	;last
MOV 	R2,#16	;16L

MAINL:
	;X
	CLR 	CKXL
	MOV 	DPTR,#TAB1
	;first8
	MOV 	A,R0
	MOVC 	A,@A+DPTR
	MOV 	R5,#8
	X0:
	CLR 	CKX
	RLC 	A
	MOV 	DX,C
	SETB 	CKX
	DJNZ 	R5,X0
	;last8
	MOV 	A,R1
	MOVC 	A,@A+DPTR
	MOV 	R5,#8
	X1:
	CLR 	CKX
	RLC 	A
	MOV 	DX,C
	SETB 	CKX
	DJNZ 	R5,X1
	;Get-In
	SETB 	CKXL
	CLR 	ENX

	;Y
	CLR 	CKYL
	MOV 	DPTR,#TAB2
	;first8
	MOV 	A,R0
	ADD 	A,R4;
	MOVC 	A,@A+DPTR
	MOV 	R5,#8
	Y0:
	CLR 	CKY
	RLC 	A
	CPL 	C
	MOV 	DY,C
	SETB 	CKY
	DJNZ 	R5,Y0
	;last8
	MOV 	A,R1
	ADD 	A,R4
	MOVC 	A,@A+DPTR
	MOV 	R5,#8
	Y1:
	CLR 	CKY
	RLC 	A
	CPL 	C
	MOV 	DY,C
	SETB 	CKY
	DJNZ 	R5,Y1
	;Get-In
	SETB 	CKYL
	CLR 	ENY

	LCALL 	DELAY
	INC 	R0
	INC 	R0
	INC 	R1
	INC 	R1
DJNZ 	R2,MAINL
DJNZ 	R7,MAIN

INC 	R4
INC 	R4
DJNZ	R3,TTT
AJMP 	START

DELAY:
MOV 	R6,#255
DJNZ 	R6,$
RET

END
