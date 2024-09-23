;RECEPT
ORG 0000H
	LJMP START
ORG 1000H
START:CLR   EA			;INIT
           MOV  TMOD,#20H
           MOV  TH1,#0F4H
           MOV  TL1,#0F4H
           MOV  PCON,#00H
           SETB  TR1
           MOV  SCON,#50H    
  BLOOP1:JNB  RI,$           ;RECEIVE HANDSHAKE
           CLR  RI               
           MOV  A,SBUF       
           XRL  A,#0E1H        ;VERIFY HANDSHAKE
           JNZ   BLOOP1         
           MOV  SBUF,#0E2H     ;SEND HANDSHAKE SIGNAL
           JNB   TI,$       
           CLR   TI  
           MOV  R0,#40H        
           MOV  R7,#10H    
           MOV  R6,#00H      
BLOOP2:JNB  RI,$			;RECEIVE DATA
           CLR   RI  
           MOV  A,SBUF
           MOV  @R0,A        
           INC   R0  
           ADD  A,R6         ;COUNT CHECKSUM
           MOV  R6,A
           DJNZ  R7,BLOOP2   
           JNB  RI,$            ;RECEIVE CHECKSUM
		   CLR  RI
           MOV  A,SBUF
           XRL  A,R6           ;VERIFY
           JZ   END1            
           MOV  SBUF,#0FFH     ;WRONG
           JNB  TI,$            
           CLR  TI
END1:MOV  SBUF,#00H				;CORRECT
           JNB  TI,$            
           CLR  TI
SJMP $
END