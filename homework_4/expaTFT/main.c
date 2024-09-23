/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "dr_tft.h"
#include "image.h"
void initClock()
{
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //启动XT1
  P7SEL |= BIT2 + BIT3; //XT2引脚功能选择
  UCSCTL6 &= ~XT2OFF; //启动XT2
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }

  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //避免DCO调整中跑飞

  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = 20000000 / (4000000 / 16); //XT2频率较高，分频后作为基准可获得更高的精度
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2进行16分频后作为基准
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //设定几个CLK的分频
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //设定几个CLK的时钟源
}

int main( void )
{

	  // Stop watchdog timer to prevent time out reset
	  WDTCTL = WDTPW + WDTHOLD;

	  _DINT();

	  initClock();
	  initTFT();

	  _EINT();

	  etft_AreaSet(0,0,319,239,0);


	  while(1){
		  etft_DisplayString(" ",100,80,65535,0);  //这是显示文字
		  __delay_cycles(MCLK_FREQ*3);
		  etft_DisplayImage(img,60,60, 60, 60);   //这是显示图片
		  __delay_cycles(MCLK_FREQ*3);
	  }
}
