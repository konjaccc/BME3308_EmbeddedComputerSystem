/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "dr_tft.h"

unsigned char tdata[]={'0','\0'}; //transmit data
unsigned char rdata[]={'0','\0'}; //receive data
void UART_Init(void);
void initClock(void);
volatile unsigned int data= 0; //��ǰADCת������
volatile unsigned int flag=0; //=1:���µ�ADC���ݲ���
volatile unsigned int y_last=0; //ǰһ��ADC����
volatile unsigned int x=4; //ʱ���������
volatile unsigned int y=0; //��һ��ADC����
volatile unsigned int mid=4; //����ƽ��ֵ����ƽ����ʾ
volatile char disp[4]="000"; //��ʾ��Ƶ������

void main(void)
{
		WDTCTL = WDTPW + WDTHOLD; //�رտ��Ź�
		UART_Init(); //��ʼ��UART
		TA0CTL |= MC_1 + TASSEL_2 + TACLR; //ʱ��ΪSMCLK,�Ƚ�ģʽ,���������
		TA0CCTL0 = CCIE; //�򿪱Ƚ����ж�ʹ��
		TA0CCR0  = 50000; //�൱��50ms��ʱ����
		P5DIR |= BIT7;
		P8DIR |= BIT0;
		ADC12CTL0 |= ADC12MSC; //�Զ�ѭ������ת��
		ADC12CTL0 |= ADC12ON; //����ADC12ģ��
		ADC12CTL1 |= ADC12CONSEQ1 ;//ѡ��ͨ��ѭ������ת��
		ADC12CTL1 |= ADC12SHP; //��������ģʽ
		ADC12MCTL0 |= ADC12INCH_12; //ѡ��ͨ��15,ѡ��p7.4
		ADC12CTL0 |= ADC12ENC;
		_DINT();
		initClock(); //��ʼ��ʱ��
		initTFT(); //��ʼ��TFT��
		etft_AreaSet(0,0,319,239,31);
		_EINT();
		__bis_SR_register(LPM0_bits + GIE);//����͹���,�������ж�
}

volatile int n=0; //�������鳤��
volatile int pulse[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //��������
volatile int sum=0; //�����������ܺ�
volatile int pflag=0; //pulseflag=1 ��ʾ��ʼ�������
volatile int count=0; //�������

int freq_counter(int y,int t0) //����y_last,y �������鲢��������
{
	sum-=pulse[n]; //ά�������������ܺ�
	pulse[n]=y-y_last; //�����ֵ,��������
	sum+=pulse[n]; //�����ܺ�
	n++;
	if(n>=15) n=0; //���������

	// ���δ��⵽����Ŀ�ʼ�������ܺʹ��ڵ���40
	if((pflag==0)&&(sum>=40))
	{
		pflag=1; //��⵽���忪ʼ
		int tmp=count; //���浱ǰ����ֵ
		count=0;
		return 6000/tmp; //����Ƶ��
		}
	else if(sum<40) pflag=0; //�������
	count++;
	return -1;
}
void UART_Init(void)	//UART��ʼ��:P3.4��P3.5��P4.4��P4.5
{
	P3DIR|=(1<<4)|(1<<5);
	P4DIR|=(1<<4)|(1<<5);
	P4OUT|=(1<<4);
	P4OUT&=~(1<<5);
	P3OUT|=(1<<5);
	P3OUT&=~(1<<4);
	P8SEL|=0x0c;
	UCA1CTL1|=UCSWRST;	//��λUSCI
	UCA1CTL1|=UCSSEL_1;
	UCA1BR0=0x03;		//���ò�����
	UCA1BR1=0x00;
	UCA1MCTL=UCBRS_3+UCBRF_0;
	UCA1CTL1&=~UCSWRST;	//������λ
	UCA1IE|=UCRXIE;		//ʹ�ܽ����ж�
}
void initClock()
{
	while(BAKCTL & LOCKIO)                 // Unlock XT1 pins for operation
		BAKCTL &= ~(LOCKIO);
	UCSCTL6 &= ~XT1OFF;
	P7SEL |= BIT2 + BIT3;
	UCSCTL6 &= ~XT2OFF;
	while (SFRIFG1 & OFIFG) {
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}
	UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK;
	UCSCTL1 = DCORSEL_5;                           //6000kHz~23.7MHz
	UCSCTL2 = 20000000 / (4000000 / 16);
	UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16;
	while (SFRIFG1 & OFIFG) {
		UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
		SFRIFG1 &= ~OFIFG;
	}
	UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1;
	UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK;
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void) //Timer_A�ж�:��ʱ����ADC�����к����������ʾ
{
	ADC12CTL0 |= ADC12SC; //����ADCת��
	data = ADC12MEM0; //��ADCת�������ֵ������data
	x++; //x�����1
	if (flag==0)
	{
		flag=1; //����flag
		y_last=232-data/18; //���㲢����y_last
		UCA1TXBUF=data;
		ADC12CTL0 |= ADC12SC; //��ʼ��һ��ADCת��
		data = ADC12MEM0; //ת�������ֵ��data

	}
	y=232-data/18; //���㲢����y
	UCA1TXBUF=data; //UART����

	if (x>=309) //���������
	{
		x=4;
		etft_AreaSet(x,4,x+12,235,0); //������ɫ
		x++; //x�����1
	}
	mid=(y_last+y)/2; //mid:ƽ��ֵ,���ڼӵ�ƽ����ʾ
	etft_AreaSet(x+1,4,x+6,235,0); //������ɫ
	if (y_last<y) //������ʾ
	{
		etft_AreaSet(x,y_last,x,mid,0xf800);
		etft_AreaSet(x+1,mid,x+1,y,0xf800);
	}
	else
	{
		etft_AreaSet(x,mid,x,y_last,0xf800);
		etft_AreaSet(x+1,y,x+1,mid,0xf800);
	}

	int freq=freq_counter(y,y_last); //�������
	if(freq>0)
	{
		disp[0]=freq/100+'0'; //��λ��
		disp[1]=(freq%100)/10+'0'; //ʮλ��
		disp[2]=freq%10+'0'; //��λ��
	}
	//etft_DisplayString('heart rate :', 280,180,0xffff, 0x0000); //��ʾ�ַ���
	etft_DisplayString(disp, 280,215,0xffff, 0x0000); //��ʾƵ��
	y_last=y; //��y_last��ֵ��y0
}


