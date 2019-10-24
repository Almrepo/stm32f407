#ifndef FONT_16_PTC_H
#define FONT_16_PTC_H
//#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
//#define FONT      cousine_18ptcBitmaps // ��������� ������ �����
//#define FONT_info cousine_18ptcFontInfo // �������� ������, ������ � �.�.

#define FONT      cousine_16ptcBitmaps  // ��������� ������ �����
#define FONT_info cousine_16ptcFontInfo // �������� ������, ������ � �.�. ��.� ����� ����� ������
#define indent 5   // ����� (px)        // ������� ����� �������� ���� ����� ������� (������ ������ �� ������� �� ���������� �������)

//extern const unsigned char cousine_18ptcBitmaps[];
//extern const unsigned char cousine_18ptcFontInfo[];	

extern const unsigned char cousine_16ptcBitmaps[];
extern const unsigned char cousine_16ptcFontInfo[];



/*  
// wi ������ �������, ��� */
// he ���������� �����  */
// skip ������� ���������� ����., ����� 127-� � 192-� �� ANSI */


#endif /* FONT_16_PTC_H */

