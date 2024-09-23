/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
typedef struct                   //以指针形式定义P8口的各个寄存器
{
  const volatile uint8_t* PxIN;     //定义一个不会被编译的无符号字符型变量
  volatile uint8_t* PxOUT;
  volatile uint8_t* PxDIR;
  volatile uint8_t* PxREN;
  volatile uint8_t* PxSEL;
} GPIO_TypeDef;

const GPIO_TypeDef GPIO4 =
{ &P4IN, &P4OUT, &P4DIR, &P4REN, &P4SEL};
const GPIO_TypeDef GPIO5 ={&P5IN, &P5OUT, &P5DIR, &P5REN, &P5SEL};

const GPIO_TypeDef GPIO8 ={&P8IN, &P8OUT, &P8DIR, &P8REN, &P8SEL};
const GPIO_TypeDef* LED_GPIO[5] = {&GPIO4, &GPIO4, &GPIO4, &GPIO5, &GPIO8};
const uint8_t LED_PORT[5] = {BIT5, BIT6, BIT7, BIT7, BIT0};
int cnt = 0;
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //启动XT1
  while (UCSCTL7 & XT1LFOFFG) //等待XT1稳定
    UCSCTL7 &= ~(XT1LFOFFG);

  UCSCTL4 = SELA__XT1CLK + SELS__REFOCLK + SELM__REFOCLK; //时钟设为XT1，频率较低，方便软件延时

  int i;
  for(i=0;i<5;++i)
    *LED_GPIO[i]->PxDIR |= LED_PORT[i]; //设置各LED灯所在端口为输出方向

  P4OUT |= BIT0;				// 选中P4.0为输出方式
  P4REN |= BIT0;				// P4.0使能
  P4OUT |= BIT1;				// 选中P4.1为输出方式
  P4REN |= BIT1;				// P4.1使能
  cnt = 0;
  while(1)
  {
	if (P4IN & BIT0) cnt = cnt;
	else { //P4.0口:S3 按下灯亮
	    __delay_cycles(327); //按键防抖动 延时10ms
	    if (P4IN & BIT0) cnt = cnt;
	    else cnt = (cnt+1)%6;
	}
	if (P4IN & BIT1) cnt=cnt;
	else { //P4.1口:S4 按下灯全灭
	    __delay_cycles(327); //按键防抖动 延时10ms
		if (P4IN & BIT1) cnt=cnt;
		else cnt = 0;

	}
	for(i=0;i<5;++i)
		if (i<cnt)
	        *LED_GPIO[i]->PxOUT |= LED_PORT[i];
		else
			*LED_GPIO[i]->PxOUT &= ~LED_PORT[i];
    __delay_cycles(32760); //延时大约1s
	for(i=0;i<5;++i)
		*LED_GPIO[i]->PxOUT &= ~LED_PORT[i]; //LED全部关掉
    __delay_cycles(32760); //延时大约1s
  }
}
