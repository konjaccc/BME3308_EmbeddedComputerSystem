/*
 * dr_lcdseg.h
 *
 *  Created on: 2013-5-28
 *      Author: DengPihui
 */

#ifndef DR_LCDSEG_H_
#define DR_LCDSEG_H_

void initLcdSeg();	//��ʾƵ��ʼ��
void LCDSEG_SetDigit(int pos, int value); //��pos(0<=pos<=5)λ��д��һ������value��-1��=value��=16��0��=pos��=5����value=16��ʾд�븺�ţ�value=-1��ʾ�����λ
void LCDSEG_SetSpecSymbol(int pos);//��posλ�����С���㣨3<=pos<=5��
void LCDSEG_ResetSpecSymbol(int pos);//��posλ��дС���㣨3<=pos<=5��
void LCDSEG_DisplayNumber(int32_t num, int dppos);//�ö�ʽҺ����ʾnum�������������dpposλ�ô����С����(��0��dppos��=3)


#endif /* DR_LCDSEG_H_ */
