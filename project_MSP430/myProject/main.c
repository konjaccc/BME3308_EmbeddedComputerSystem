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
volatile unsigned int data= 0; //当前ADC转换数据
volatile unsigned int flag=0; //=1:有新的ADC数据产生
volatile unsigned int y_last=0; //前一个ADC数据
volatile unsigned int x=4; //时间轴横坐标
volatile unsigned int y=0; //后一个ADC数据
volatile unsigned int mid=4; //计算平均值用于平滑显示
volatile char disp[4]="000"; //显示的频率数组

void main(void)
{
		WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
		UART_Init(); //初始化UART
		TA0CTL |= MC_1 + TASSEL_2 + TACLR; //时钟为SMCLK,比较模式,清零计数器
		TA0CCTL0 = CCIE; //打开比较器中断使能
		TA0CCR0  = 50000; //相当于50ms的时间间隔
		P5DIR |= BIT7;
		P8DIR |= BIT0;
		ADC12CTL0 |= ADC12MSC; //自动循环采样转换
		ADC12CTL0 |= ADC12ON; //启动ADC12模块
		ADC12CTL1 |= ADC12CONSEQ1 ;//选择单通道循环采样转换
		ADC12CTL1 |= ADC12SHP; //采样保持模式
		ADC12MCTL0 |= ADC12INCH_12; //选择通道15,选择p7.4
		ADC12CTL0 |= ADC12ENC;
		_DINT();
		initClock(); //初始化时钟
		initTFT(); //初始化TFT屏
		etft_AreaSet(0,0,319,239,31);
		_EINT();
		__bis_SR_register(LPM0_bits + GIE);//进入低功耗,开启总中断
}

volatile int n=0; //滑窗数组长度
volatile int pulse[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //滑窗数组
volatile int sum=0; //窗口内数据总和
volatile int pflag=0; //pulseflag=1 表示开始脉冲计数
volatile int count=0; //计数结果

int freq_counter(int y,int t0) //根据y_last,y 存入数组并计算心率
{
	sum-=pulse[n]; //维护窗口内数据总和
	pulse[n]=y-y_last; //计算差值,更新数组
	sum+=pulse[n]; //更新总和
	n++;
	if(n>=15) n=0; //溢出则清零

	// 如果未检测到脉冲的开始且脉冲总和大于等于40
	if((pflag==0)&&(sum>=40))
	{
		pflag=1; //检测到脉冲开始
		int tmp=count; //保存当前计数值
		count=0;
		return 6000/tmp; //返回频率
		}
	else if(sum<40) pflag=0; //脉冲结束
	count++;
	return -1;
}
void UART_Init(void)	//UART初始化:P3.4。P3.5，P4.4，P4.5
{
	P3DIR|=(1<<4)|(1<<5);
	P4DIR|=(1<<4)|(1<<5);
	P4OUT|=(1<<4);
	P4OUT&=~(1<<5);
	P3OUT|=(1<<5);
	P3OUT&=~(1<<4);
	P8SEL|=0x0c;
	UCA1CTL1|=UCSWRST;	//复位USCI
	UCA1CTL1|=UCSSEL_1;
	UCA1BR0=0x03;		//设置波特率
	UCA1BR1=0x00;
	UCA1MCTL=UCBRS_3+UCBRF_0;
	UCA1CTL1&=~UCSWRST;	//结束复位
	UCA1IE|=UCRXIE;		//使能接收中断
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
__interrupt void Timer_A (void) //Timer_A中断:定时启动ADC并进行后续计算和显示
{
	ADC12CTL0 |= ADC12SC; //启动ADC转换
	data = ADC12MEM0; //将ADC转换结果赋值给变量data
	x++; //x坐标加1
	if (flag==0)
	{
		flag=1; //更新flag
		y_last=232-data/18; //换算并存入y_last
		UCA1TXBUF=data;
		ADC12CTL0 |= ADC12SC; //开始下一次ADC转换
		data = ADC12MEM0; //转换结果赋值给data

	}
	y=232-data/18; //换算并存入y
	UCA1TXBUF=data; //UART发送

	if (x>=309) //横坐标出界
	{
		x=4;
		etft_AreaSet(x,4,x+12,235,0); //背景颜色
		x++; //x坐标加1
	}
	mid=(y_last+y)/2; //mid:平均值,用于加点平滑显示
	etft_AreaSet(x+1,4,x+6,235,0); //背景颜色
	if (y_last<y) //波形显示
	{
		etft_AreaSet(x,y_last,x,mid,0xf800);
		etft_AreaSet(x+1,mid,x+1,y,0xf800);
	}
	else
	{
		etft_AreaSet(x,mid,x,y_last,0xf800);
		etft_AreaSet(x+1,y,x+1,mid,0xf800);
	}

	int freq=freq_counter(y,y_last); //脉冲计数
	if(freq>0)
	{
		disp[0]=freq/100+'0'; //百位数
		disp[1]=(freq%100)/10+'0'; //十位数
		disp[2]=freq%10+'0'; //个位数
	}
	//etft_DisplayString('heart rate :', 280,180,0xffff, 0x0000); //显示字符串
	etft_DisplayString(disp, 280,215,0xffff, 0x0000); //显示频率
	y_last=y; //将y_last赋值给y0
}


