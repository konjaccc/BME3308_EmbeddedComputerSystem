/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
typedef struct                   //��ָ����ʽ����P8�ڵĸ����Ĵ���
{
  const volatile uint8_t* PxIN;     //����һ�����ᱻ������޷����ַ��ͱ���
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
  UCSCTL6 &= ~XT1OFF; //����XT1
  while (UCSCTL7 & XT1LFOFFG) //�ȴ�XT1�ȶ�
    UCSCTL7 &= ~(XT1LFOFFG);

  UCSCTL4 = SELA__XT1CLK + SELS__REFOCLK + SELM__REFOCLK; //ʱ����ΪXT1��Ƶ�ʽϵͣ����������ʱ

  int i;
  for(i=0;i<5;++i)
    *LED_GPIO[i]->PxDIR |= LED_PORT[i]; //���ø�LED�����ڶ˿�Ϊ�������

  P4OUT |= BIT0;				// ѡ��P4.0Ϊ�����ʽ
  P4REN |= BIT0;				// P4.0ʹ��
  P4OUT |= BIT1;				// ѡ��P4.1Ϊ�����ʽ
  P4REN |= BIT1;				// P4.1ʹ��
  cnt = 0;
  while(1)
  {
	if (P4IN & BIT0) cnt = cnt;
	else { //P4.0��:S3 ���µ���
	    __delay_cycles(327); //���������� ��ʱ10ms
	    if (P4IN & BIT0) cnt = cnt;
	    else cnt = (cnt+1)%6;
	}
	if (P4IN & BIT1) cnt=cnt;
	else { //P4.1��:S4 ���µ�ȫ��
	    __delay_cycles(327); //���������� ��ʱ10ms
		if (P4IN & BIT1) cnt=cnt;
		else cnt = 0;

	}
	for(i=0;i<5;++i)
		if (i<cnt)
	        *LED_GPIO[i]->PxOUT |= LED_PORT[i];
		else
			*LED_GPIO[i]->PxOUT &= ~LED_PORT[i];
    __delay_cycles(32760); //��ʱ��Լ1s
	for(i=0;i<5;++i)
		*LED_GPIO[i]->PxOUT &= ~LED_PORT[i]; //LEDȫ���ص�
    __delay_cycles(32760); //��ʱ��Լ1s
  }
}
