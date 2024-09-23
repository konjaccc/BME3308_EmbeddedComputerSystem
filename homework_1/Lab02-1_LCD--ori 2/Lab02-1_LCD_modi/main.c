/*
 * main.c
 */
#include <msp430f6638.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dr_lcdseg.h"   //璋冪敤娈靛紡娑叉櫠椹卞姩澶存枃浠�//#include "lcd.h"

#define XT2_FREQ   4000000

#define MCLK_FREQ 16000000
#define SMCLK_FREQ 4000000

void initClock()
{
  while(BAKCTL & LOCKIO) //瑙ｉ攣XT1寮曡剼鎿嶄綔
BAKCTL &= ~(LOCKIO);
UCSCTL6 &= ~XT1OFF; //鍚姩XT1锛岄�鎷╁唴閮ㄦ椂閽熸簮
    P7SEL |= BIT2 + BIT3; //XT2寮曡剼鍔熻兘閫夋嫨
UCSCTL6 &= ~XT2OFF; //鍚姩XT2
while (SFRIFG1 & OFIFG) //绛夊緟XT1銆乆T2涓嶥CO绋冲畾
{
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
    }
  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //閬垮厤DCO璋冩暣涓窇椋�  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = MCLK_FREQ / (XT2_FREQ / 16); //XT2棰戠巼杈冮珮锛屽垎棰戝悗浣滀负鍩哄噯鍙幏寰楁洿楂樼殑绮惧害
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2杩涜16鍒嗛鍚庝綔涓哄熀鍑�  while (SFRIFG1 & OFIFG) //绛夊緟XT1銆乆T2涓嶥CO绋冲畾
{
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG);
    SFRIFG1 &= ~OFIFG;
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //璁惧畾鍑犱釜CLK鐨勫垎棰�  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__DCOCLK; //璁惧畾鍑犱釜CLK鐨勬椂閽熸簮
}

void main(void)
{
	unsigned char i,j;
	int32_t num2;
    WDTCTL = WDTPW | WDTHOLD;	// 鍋滄鐪嬮棬鐙�    initClock();             //閰嶇疆绯荤粺鏃堕挓
    initLcdSeg();           //鍒濆鍖栨寮忔恫鏅�    while(1)               //杩涘叆绋嬪簭涓诲惊鐜�    {
     	for(i=0;i<6;i++) {//鏄剧ず24.0305
     		LCDSEG_DisplayNumber(240305,3);}
    	for(i=0;i<6;i++){
    		LCDSEG_SetDigit(i,-1);     //娈靛紡娑叉櫠娓呭睆
      	__delay_cycles(MCLK_FREQ); //寤舵椂1000ms
      	//鏄剧ずSJTU
    	}

      //for(i=0;i<6;i++) {//鏄剧ずSJTU
     	LCDSEG_SetLetter(1,'S');
        LCDSEG_SetLetter(2,'J');
        LCDSEG_SetLetter(3,'T');
        LCDSEG_SetLetter(4,'U');
        __delay_cycles(MCLK_FREQ); //寤舵椂1000ms
        
    	for(i=0;i<6;i++){
    		LCDSEG_SetDigit(i,-1);     //娈靛紡娑叉櫠娓呭睆
      	    __delay_cycles(MCLK_FREQ); //寤舵椂1000ms
    	}
      	//1:bcdef 
      	//2:abc
      	//3:bcde
      	//4:acdfg
      	// C1:0510
      	// C2:0BB0
      	// C3:06A0
      	// C4:0C90
      	//鍏蜂綋涓嶅お鐭ラ亾鎬庝箞鍐欏叆杩欏嚑涓瘎瀛樺櫒锛�    }
}

