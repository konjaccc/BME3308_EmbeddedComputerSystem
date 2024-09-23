/*
 * dr_lcdseg.c
 *
 *  Created on: 2013-5-28
 *      Author: DengPihui
 */

#include <msp430.h>
#include <stdint.h>
#include "dr_lcdseg.h"

const uint8_t SEG_CTRL_BIN[17] =
{
  0x3F,	//display 0
  0x06,	//display 1
  0x5B,	//display 2
  0x4F,	//display 3
  0x66,	//display 4
  0x6D,	//display 5
  0x7D,	//display 6
  0x07,	//display 7
  0x7F,	//display 8
  0x6F,	//display 9
  0x77,	//display A
  0x7C,	//display b
  0x39,	//display C
  0x5E,	//display d
  0x79, //display E
  0x71, //display F
  0x40, //display -
};
//display mem: a-d, e-h, MSB starts from h and d, LSB ends with a, e, use a hex to define

void initLcdSeg()
{
  //嚙誼選蕭嚙質隅
  P5SEL |= BIT3 + BIT4 + BIT5; //P5.3 .4 .5嚙踝蕭峈LCD嚙踝蕭COM
  LCDBPCTL0 = 0x0FFF; //S0~S11嚙踝蕭嚙誹傷選蕭嚙踝蕭峈LCD嚙衝塚蕭恁
  //嚙踝蕭嚙踝蕭嚙踝蕭嚙質隅
  LCDBCTL0 = LCDDIV_21 + LCDPRE__4 + LCD4MUX; //ACLK, 21*4嚙踝蕭嚙踝蕭嚙踝蕭48.76Hz
  LCDBMEMCTL |= LCDCLRM; //嚙踝蕭嚙盤CD嚙賣揣嚙踝蕭
  LCDBCTL0 |= LCDSON + LCDON; //嚙踝蕭嚙踝蕭LCD耀嚙踝蕭
}

void LCDSEG_SetDigit(int pos, int value) //value嚙踝蕭嚙踝蕭0~16奀洠嚙踝蕭,16嚙踝蕭尨-
{
  if(pos < 0 || pos > 6)
    return;
  pos = 5 - pos;

  uint8_t temp, mem;
  if(value < 0 || value > 16)
    temp = 0x00;
  else
    temp = SEG_CTRL_BIN[value];

 const static uint8_t map[7] ={ BIT7, BIT6, BIT5, BIT0, BIT1, BIT3, BIT2 };

  mem = LCDMEM[pos];
  mem &= 0x10; //嚙踝蕭梲嚙踝蕭嚙踝蕭嚙踝蕭硍庰嚙諄�
  int i;
  for(i=0;i<7;++i)
  {
    if(temp & (1 << i))
      mem |= map[i];
  }
  LCDMEM[pos] = mem;
}

void LCDSEG_SetSpecSymbol(int pos)
{
  LCDMEM[pos] |= 0x10;
}

void LCDSEG_ResetSpecSymbol(int pos)
{
  LCDMEM[pos] &= ~0x10;
}

void LCDSEG_DisplayNumber(int32_t num, int dppos)
{
  int curpos = 0, isneg = 0;

  if(num < 0)
  {
    isneg = 1;
    num = -num;
  }

  while(1)
  {
    int digit = num % 10;
    num /= 10;
    LCDSEG_SetDigit(curpos++, digit);
    if(num == 0)
      break;
  }

  if(isneg)
    LCDSEG_SetDigit(curpos++, 16); //嚙諉賂蕭嚙踝蕭

  while(curpos < 6)
    LCDSEG_SetDigit(curpos++, -1); //嚙踝蕭嚙踝蕭嚙踝蕭弇嚙踝蕭嚙�

  int i;
  for(i=3;i<=5;++i)
    LCDSEG_ResetSpecSymbol(i);

  if(dppos > 0 && dppos <= 3)
    LCDSEG_SetSpecSymbol(6 - dppos);
}


