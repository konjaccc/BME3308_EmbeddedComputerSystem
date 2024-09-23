/*
 * dr_lcdseg.h
 *
 *  Created on: 2013-5-28
 *      Author: DengPihui
 */

#ifndef DR_LCDSEG_H_
#define DR_LCDSEG_H_

void initLcdSeg();	//嚙踝蕭尨嚙踝蕭宎嚙踝蕭
void LCDSEG_SetDigit(int pos, int value); //嚙踝蕭pos(0<=pos<=5)弇嚙踝蕭迡嚙踝蕭珨嚙踝蕭嚙踝蕭嚙踝蕭value嚙踝蕭-1嚙踝蕭=value嚙踝蕭=16嚙踝蕭0嚙踝蕭=pos嚙踝蕭=5嚙踝蕭嚙踝蕭value=16嚙踝蕭尨迡嚙趟蛹嚙褐�嚙緞alue=-1嚙踝蕭尨嚙踝蕭嚙踝蕭嚙諄�
void LCDSEG_SetSpecSymbol(int pos);//嚙踝蕭pos弇嚙踝蕭嚙踝蕭嚙請∴蕭嚙踝蕭聜�3<=pos<=5嚙踝蕭
void LCDSEG_ResetSpecSymbol(int pos);//嚙踝蕭pos弇嚙踝蕭迡苤嚙踝蕭嚙賬ㄗ3<=pos<=5嚙踝蕭
void LCDSEG_DisplayNumber(int32_t num, int dppos);//嚙衛塚蕭宒珘嚙踝蕭嚙踝蕭尨num嚙踝蕭嚙踝蕭嚙踝蕭嚙踝蕭嚙踝蕭嚙踝蕭嚙範ppos弇嚙衛湛蕭嚙踝蕭嚙踝蕭苤嚙踝蕭嚙踝蕭(嚙踝蕭0嚙踝蕭dppos嚙踝蕭=3)


#endif /* DR_LCDSEG_H_ */
