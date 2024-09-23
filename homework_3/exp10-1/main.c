/*
 * main.c
 */

#include <msp430f6638.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dr_lcdseg.h"
#define MCLK_FREQ 16000000
void main(void){
	WDTCTL = WDTPW + WDTHOLD;
	initLcdSeg();
	P4DIR |= BIT5 + BIT6 + BIT7;
	P5DIR |= BIT7;
	P8DIR |= BIT0;
	ADC12CTL0 |= ADC12MSC;
	ADC12CTL0 |= ADC12ON;
	ADC12CTL1 |= ADC12CONSEQ1;
	ADC12CTL1 |= ADC12SHP;
	ADC12MCTL0 |= ADC12INCH_15;
	ADC12CTL0 |= ADC12ENC;
	volatile unsigned int value = 0;
	while(1)
	{
		ADC12CTL0 |= ADC12SC;
		value = ADC12MEM0;
		LCDSEG_DisplayNumber(value,0);
	}

}
