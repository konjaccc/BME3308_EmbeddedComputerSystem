;TRANSMIT
ORG 0000H
	LJMP START
ORG 0100H
START:CLR  EA		;INIT
           MOV  TMOD,#20H    
           MOV  TH1,#0F4H      
           MOV  TL1,#0F4H
           MOV  PCON,#00H       
           SETB  TR1                 
		   MOV P1,#00H
           MOV  SCON,#50H     
  ALOOP1:MOV  SBUF,#0E1H  ;SEND HANDSHAKE SIGNAL
           JNB  TI,$                 
           CLR  TI                     
           JNB  RI,$           ;RECEIVE               
           CLR  RI                    
           MOV  A,SBUF         
           XRL  A,#0E2H        ;VERIFY HANDSHAKE 
           JNZ  ALOOP1         

  ALOOP2:MOV  R0,#40H        
           MOV  R7,#10H       ;DATA LENGTH
           MOV  R6,#00H        
 ALOOP3:MOV  SBUF,@R0    ;SEND DATA
           MOV  A,R6           
           ADD  A,@R0         
           MOV  R6,A           
           INC R0
           JNB  TI,$   
           CLR  TI
           DJNZ  R7,ALOOP3   

           MOV  SBUF,R6       ;SEND CHECKSUM
          JNB  TI,$
           CLR  TI

           JNB  RI,$           ;RECEIVE VERIFICATION RESULT
           CLR  RI

           MOV  A,SBUF        
           JNZ   ALOOP2     ;WRONG  
		   MOV P1,#0FH
		   SJMP $
END
