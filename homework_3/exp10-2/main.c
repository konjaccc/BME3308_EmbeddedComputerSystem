/*
 * main.c
 */

#include<msp430f6638.h>
#include<stdint.h>
int flag;
void main(void)
{
	int sinnum[20]={0x80F,0xA7F,0xCAF,0xE7F,0xF9F,0xFFF,0xF9F,0xE7F,0xCAF,0xA7F,0x80F,0x58F,0x35F,0x18F,0x06F,0x00F,0x06F,0x18F,0x35F,0x58F}; //sin������ɢֵ
	int n=0;
	 WDTCTL = WDTPW + WDTHOLD;
	 P7DIR |= BIT6;
	 P7SEL |= BIT6;  //�ڶ�����λ
	 TA0CTL |= MC_1 + TASSEL_2 + TACLR;
	 TA0CCTL0 = CCIE;
	 	 //f=10hz T=100ms t=5ms
	 //TA0CCR0  = 50000;
	 TA0CCR0  = 5000;
	 _bis_SR_register(GIE);
	 while(1)
	 {
		 if(flag)  //���ݶ�ʱ��ѭ��
		 {
			 DAC12_0CTL0 |= DAC12IR;
			 DAC12_0CTL0 |= DAC12SREF_1;
			 DAC12_0CTL0 |= DAC12AMP_5;
			 DAC12_0CTL0 |= DAC12CALON; //����У��
			 DAC12_0CTL0 |= DAC12OPS;   //ѡ��ͨ��
			 DAC12_0CTL0 |= DAC12ENC;  //ת��ʹ��
			 DAC12_0DAT = sinnum[n];
			 n++;
			 if(n==20) n=0;
			 flag=0;
		 }
	 }
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	flag=1;
}
