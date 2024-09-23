ORG 0000H
	LJMP MAIN
ORG 0023H
	LJMP RINT
ORG 1000H
MAIN:
	MOV  PCON,#00H       ;SET UART
    MOV  SCON,#50H
	SHOW:
MOV A,0F9H
N30:      CJNE A,#0,N31
             MOV  R2,#11000000B
             JMP  N20
N31:      CJNE A,#1,N32;
             MOV  R2,#11001111B
             JMP  N20
N32:     CJNE A,#2,N33;
            MOV  R2,#10100100B;
            JMP  N20
N33:     CJNE A,#3,N34;
            MOV  R2,#10110000B
            JMP  N20
N34:     CJNE A,#4,N35;
            MOV  R2,#10011001B
            JMP  N20
N35:     CJNE A,#5,N36;
            MOV  R2,#10010010B
            JMP  N20
N36:     CJNE A,#6,N37;
            MOV  R2,#10000010B
            JMP  N20
N37:     CJNE A,#7,N38;
            MOV  R2,#11111000B
            JMP  N20
N38:     CJNE A,#8,N39;
            MOV  R2,#10000000B
            JMP  N20
N39:     MOV  R2,#10010000B

N20:     MOV A,0F8H
            CJNE A,#0,N21;
            MOV  R3,#01000000B;0
            JMP  N10
N21:     CJNE A,#1,N22;
            MOV  R3,#01001111B;1
            JMP  N10
N22:     CJNE A,#2,N23;
            MOV  R3,#00100100B;2
            JMP  N10
N23:     CJNE A,#3,N24;
            MOV  R3,#00110000B;3
            JMP  N10
N24:     CJNE A,#4,N25;
            MOV  R3,#00011001B;4
            JMP  N10
N25:     CJNE A,#5,N26;
            MOV  R3,#00010010B;5
            JMP  N10
N26:     CJNE A,#6,N27;
            MOV  R3,#00000010B;6
            JMP  N10
N27:     CJNE A,#7,N28;
            MOV  R3,#01111000B;7
            JMP  N10
N28:     CJNE A,#8,N29;
            MOV  R3,#00000000B;8
            JMP  N10
N29:     MOV  R3,#00010000B

N10:     MOV A,0F7H
            CJNE A,#0,N11;
            MOV  R4,#11000000B
            JMP  LOOOP
N11:     CJNE A,#1,N12;
            MOV  R4,#11001111B
            JMP  LOOOP
N12:     CJNE A,#2,N13;
            MOV  R4,#10100100B
            JMP  LOOOP
N13:     CJNE A,#3,N14;
            MOV  R4,#10110000B
            JMP  LOOOP
N14:     CJNE A,#4,N15;
            MOV  R4,#10011001B
            JMP  LOOOP
N15:     CJNE A,#5,N16;
            MOV  R4,#10010010B
            JMP  LOOOP
N16:     CJNE A,#6,N17;
            MOV  R4,#10000010B
            JMP  LOOOP
N17:     CJNE A,#7,N18;
            MOV  R4,#11111000B
            JMP  LOOOP
N18:     CJNE A,#8,N19;
            MOV  R4,#10000000B
            JMP  LOOOP
N19:     MOV  R4,#10010000B
LOOOP:  MOV  P2,#00000010B
               MOV  P1,R2
               MOV  P1,#11111111B
	MOV  P2,#00000100B
	MOV  P1,R3
	MOV  P1,#11111111B
	MOV  P2,#00001000B
	MOV  P1,R4
	MOV  P1,#11111111B	
	LJMP SHOW
RINT:
	JB TI,INT_OVER
	CLR RI
	MOV A,SBUF
	MOV  B,#10
    DIV  AB
	MOV 0F7H, B
    MOV  B,#10
    DIV  AB;
	MOV 0F8H, B
    MOV  B,#10;
    DIV  AB;
	MOV 0F9H, B
	INT_OVER:
	RETI
END
		