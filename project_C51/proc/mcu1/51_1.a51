ORG  0000H
	LJMP INIT
ORG 000BH
	LJMP DVT0
ORG 00A0H
 COUNT_TIME: DB 0
SUM_TIME: DB 0   
ORG  0100H		
	INIT:
	SETB EA		
MOV P1,#01H	
	MOV SP,#4FH
	MOV  TMOD,#21H
    MOV  TH1,#0FDH
    MOV  TL1,#0FDH
	MOV TH0, #0D8H    
    MOV TL0, #0F0H
    MOV  PCON,#00H       
    SETB  TR1
	SETB ES
    SETB TR0 
	SETB PT0
	SETB ET0
    MOV  SCON,#50H	
	MOV A, #12H
	PUSH ACC
	MOV A,#11H
	PUSH ACC
START:
 MOV  DPTR, #0000H 
 MOVX @DPTR, A
 LCALL DELAY1
 MOVX A, @DPTR
 PUSH ACC
 MOV DPTR, #COUNT_TIME
 MOVX  A,@DPTR

 MOV B,A
 MOV A,#00H
 MOVX  @DPTR,A
 MOV DPTR, #SUM_TIME
 MOVX A, @DPTR
 ADD A, B

 MOV DPTR, #SUM_TIME
 MOVX @DPTR,A
 POP ACC
	MOV R5,#8
	MOV R6,#00H
	LOOP1:
		MOV B,#2
		DIV AB
		PUSH ACC
		MOV A,R6
		PUSH B
		MOV B,#2
		MUL AB
		POP B
		ADD A,B
		MOV R6, A
		POP ACC
	DJNZ R5, LOOP1
	POP B
	POP ACC

	CJNE A, B, NEXTSTEP
	LJMP READOVER1
	NEXTSTEP:
	JNC READOVER1
	CLR CY
	MOV A, R6
	PUSH B
	PUSH ACC

	CJNE A,B, CHECK
LCALL CALC_RATE
	CHECK:
	JNC READOVER
	CLR CY
	LCALL CALC_RATE
	LJMP READOVER
	READOVER1:
	PUSH B
	MOV A, R6
	PUSH ACC
	READOVER:
	LJMP START	
DELAY1:
	MOV R6,#04FH
DELAY:    
	MOV  R5,#0FFH
DELAYY:
	DJNZ R5,DELAYY
DJNZ R6,DELAY
    RET 
CALC_RATE:
 CPL P1.0
	MOV DPTR, #SUM_TIME
	MOV A,#00H                
	MOVX @DPTR,A
	MOVX A, @DPTR


	MOV B, A
	MOV A, #0FFH
	DIV AB
	PUSH ACC
	MOV B, A
	MOV B,#03H
	MUL AB
	MOV B, #0EBH
	ADD A, B
	PUSH ACC
	MOV B, R1

	POP ACC
	DIV AB
	MOV A, R7
	POP ACC
	MOV B,#03H
	MUL AB
	ADD A, R7
 	MOV SBUF, A
	JNB T1, $
	CLR TI
	
	RET
DVT0:
 
    MOV TH0, #0D8H    
    MOV TL0, #0F0H
 MOV DPTR, #COUNT_TIME
 MOVX A, @DPTR
 MOV B,#01H
 ADD A,B
 MOV DPTR, #COUNT_TIME
 MOVX @DPTR, A
 RETI
END