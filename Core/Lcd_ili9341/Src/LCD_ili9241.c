#include "stm32f4xx.h"
#include "LCD_ili9341.h"


uint16_t X_SIZE = 0;
uint16_t Y_SIZE = 0;
char array [100]; 
/*******************************************************************/

void LCD_SendCommand(uint16_t com)
{
	CMD_ADR = com;
}
/*******************************************************************/

void LCD_SendData(uint16_t data)
{   
	
    DAT_ADR= data; 	
}


void LCD_ili9341_Clear( uint16_t color)
{	     
	LCD_SetCursorPosition(0, 0,  LCD_WIDTH - 1, LCD_HEIGHT - 1);
	  uint32_t n = LCD_PIXEL_COUNT;
		while (n) 
			{
				n--;
        DAT_ADR= color; 	
	  	}     
} 


 


void LCD_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) 
{
	LCD_SendCommand(ILI9341_COLUMN_ADDR); // ��� ��������� ��.������
	LCD_SendData (y1 >> 8);                // "�����" ��� ����� "�������" � ���������� �� ������
	LCD_SendData(y1 & 0xFF);
	LCD_SendData (y2 >> 8);
	LCD_SendData(y2 & 0xFF);


	LCD_SendCommand(ILI9341_PAGE_ADDR); // ��� ��������� ��.������
  LCD_SendData(x1 >> 8);
	LCD_SendData(x1 & 0xFF);
	LCD_SendData(x2 >> 8);
	LCD_SendData(x2 & 0xFF);

  LCD_SendCommand(ILI9341_GRAM); // ���������� � ������ LCD
}




void WriteString(unsigned char x0,unsigned int y0,  char *s,unsigned int color)//, unsigned int back_color) ��������, ���� ���� �������� ���������� ���� ����
{

		
  	unsigned char z, y, nb, he, wi, d, j, q, st1, en1,y2;
	  unsigned int x, i, k;
	  y2 = y0;            // �������� ���������� ������� ��� ������ �������� ������� ������
		d = FONT_info [0];  // ������ �������, ����, ������� �� ����� ������
		wi=d*8;             // ����������� ������ ������� , ��� 
	  he = FONT_info [1]; // ������ �������, �����, ������� �� ����� ������
		nb = (he*d);        //��������� ���-�� ���� �� ���� ������

  for(z = 0;s[z]!='\0';z++) // ���������� ��� ������� � ������, ���� �� ������ �� ��������
    {
		if(s [z] < 128) 										// ���� ������ ���������, ��..
    {i = (s [z]-32)*nb;}                //������������ ������ �� ������ 32 �������
    else if (s [z] > 191)  							// ���� ������ �������, ��..
		{i =(s [z]-32-(FONT_info [2]))*nb;} //������� ���-�� ����., ����� 127-� � 192-� �� ANSI		(��.���� ������ � �����)
		else 																// ���� ����� ���. � ����, �.�. ����� 127 � 192, ��	
		{ 																	// ���� �������������� � ���� correct_ANSI.c � ������������ � ������ ������������ 
			i = (corr_ANSI ((unsigned char*)s, z))*nb;				// , �.�. ������ ������� �� �������� �� ������ ������ �������� - ���������
		}
																     // ������ �������� �������� ������ �������� �������, �.�. ��������� ������ ������� ����� � ������ �� �������
																     // ����� �������� ������� ������� ����, ������, �� ��������� ����� (PS. ������ ������� �� ��������)
		x = i;                         // �������� ����� ������� ����� ������� � ������� (��������� ����, � �������� ���������� ������ � �������)
		st1=0;											   // ������ �������, �� ����������� ����������� �������
		en1=0;											   // -*-*-
		for (q = 0; q < he; q++)       // ���������� (��������) "������" �������
		{				
			for(j = 0, k = 0; j < d; j++)// ���������� ��� ����� ������, ����� ���� ������� ��� ����� ������� ������, 
			{ 													 // PS. "�" - ������� ���� � ���� ������, ����� ���������� ����� ����� �������
				 	y = 8;      						 // ������� ��� ������ ����� ������
          while (y--) 						 // ���������� � ��������� ��� �� �����
            {
							k++;					       // ���������� ������� ���� � ������							
		      		if (((FONT[x]&(1<<y)) && (st1 == 0))||((FONT[x]&(1<<y)) && (k < st1))) {st1 = k;} // ���� ����� ���� ����� ����� ����� ��� =1							
	      			if  ((FONT[x]&(1<<y)) && (en1 < k))  	{en1 = k;}							// ���� ����� ���� ����� ����� ������ ��� =1
		      	}
						x++; 									// ��������� ���� ����� �������
			}	
		}	
		
		if (st1 > 0) {st1--;} 				// ������� ���������, ����� ������� �� "���������"
		if (en1 < 2){en1 = wi/3;}				// ���� ������ ������, �� ��� ������, ������ ��� ������ 1/3 �� ������ �������
		else if (en1 < wi){en1 = (en1 - st1 + indent);} // ����������� �������� ������ � ���������� ������ (��. font.h), ���� ���������� ���������� ����� ����.

		j=0;      // �������� "����������-���������" ��� ��� ������ ���� � ������
		k = nb+i; // �������� ����� ���������� (� �������) �����  ����� �������
		q = en1;  // �������� ������ �������, ��� , en1 �������� "��������������" ������ ����� �������, ���� ��������� "������"
		
		
		if ((y0+en1) > LCD_HEIGHT) {y0=y2; x0++;}  // ���� ��������� ������ �� ���������, �� ��������� �� ��������� ������ (LCD_HEIGHT - ��.������)
																							 // ���� �� ����� �������, �� ���������� ������� ������� "�����" ����� � ����� ����� ����� ������
		LCD_SetCursorPosition(x0*he, y0, (x0*he)+(he-1), (y0)+(en1-1)); // �������� ���� � ������ LCD, � - ���������, � - �����������
		y0=y0+en1; 																											// ��������� �0, ��� � ������ ����� ������ ���������� �������
		
		
     for(x = i; x < k; x++) // �������� (� �������) �� ������� ��� ����� �������� �������
    {
					if (j++ == 0) {y = 10-st1;} // ���� ��� ������ ���� ������ (j=0), �� �������� ������ ���� ������ ������, �� ���������� ���������� 2 pt
					else {y = 8;}               // ������ ��� �� ������ ���� ������
       while (y--)                    // ������� ���� ������, �.�. ��������� ��� ���� (?=0 ��� ?=1) ����� �����
        {									
       if((FONT[x]&(1<<y))!=0)        // ��� =1 ? ��� =0 ?
       { LCD_SendData(color);}     // ���� �� (=1) ����� ����  ������� (16-bit SPI)
			 		 
       else
				 { LCD_SendData (BLACK);}     // ���� ��� (=0) ����� ����	����  (16-bit SPI) //BLACK <- back_color);} ��������, ���� ���� �������� ���������� ���� ����

				if(!--q)                      // ������� ������� �������������� ��� ������ ������, ���� ��� ��� (������ ����� ������)
					{
						if (j != d){x = x+(d-j);} // ��.. ��������, ����� ����������� ������ ������ ��������� ������ ��� �� ����, ���� ���, �� ��������� �������� 
						                          // (������ ����� � �������) (��������: ���������� ����� = 3���, � �� 1 ������ �������� 3 ����� = 24���, 2 ���� "������")
					  y = 0; j = 0; q = en1;    // �� � ����� ������ �������� ������� ���, ���� ������ � ������� ������ ������
					} 			
        }
     }  
    }		
}

void LCD_ili9341_DrawPoint(uint16_t ysta, uint16_t xsta, uint16_t color)
{
	LCD_SetCursorPosition(xsta, ysta, xsta, ysta);
	LCD_SendData(color); 
}



void LCD_ili9341_DrawHLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t color)
{
	unsigned int y;

	LCD_SetCursorPosition(xsta, ysta, xend, ysta);  		
	y =  ((xend-xsta)+1);

	while(y--)
	{
		DAT_ADR= color; 	
	}
}



/*********************************************************************************************************
** Functoin name:       LCD_DrawVLine
*********************************************************************************************************/

void LCD_ili9341_DrawVLine(uint16_t xsta, uint16_t ysta, uint16_t yend, uint16_t color)
{
	unsigned int y;

	LCD_SetCursorPosition(xsta, ysta, xsta, yend);  		
	y =  ((yend-ysta)+1);
	while(y--)
	{
		DAT_ADR= color; 	
	}
}



void LCD_ili9341_DrawLine(uint16_t ysta, uint16_t xsta, uint16_t yend, uint16_t xend, uint16_t color)
{
    uint16_t x, y, t;	 
	if((xsta==xend)&&(ysta==yend))
		LCD_ili9341_DrawPoint(xsta, ysta, color);
	else if(xsta==xend)
	{
		LCD_ili9341_DrawVLine(xsta,ysta,yend,color);
	}
	else if(ysta==yend)
	{	
		LCD_ili9341_DrawHLine(xsta,ysta,xend,color);
	}
	else{ 
		if(abs(yend-ysta)>abs(xend-xsta))
		{
			if(ysta>yend) 
			{
				t=ysta;
				ysta=yend;
				yend=t; 
				t=xsta;
				xsta=xend;
				xend=t; 
			}
			for(y=ysta;y<yend;y++)
			{
				x = (uint32_t)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
				LCD_ili9341_DrawPoint(x, y, color);  
			}
		}
		else
		{
			if(xsta>xend)
			{
				t=ysta;
				ysta=yend;
				yend=t;
				t=xsta;
				xsta=xend;
				xend=t;
			}   
			for(x=xsta;x<=xend;x++)
			{
				y = (uint32_t)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
				LCD_ili9341_DrawPoint(x, y, color); 
			}
		}
	} 
}

void bright_PWM_ssd1963(uint8_t bright)
{
LCD_SendCommand(0xBE);    // PWM configuration 
LCD_SendData(0x08);     // set PWM signal frequency to 170Hz when PLL frequency is 100MHz 
LCD_SendData(bright);     // PWM duty cycle  
	LCD_SendData(0x01);     //
}


void LCD_ili9341_SetRotation(uint8_t r)
{
	LCD_SendCommand(0x36);
	switch(r)
	{
		case 0:
			LCD_SendData(0x48);
			X_SIZE = 240;
			Y_SIZE = 320;
			break;
		case 1:
			LCD_SendData(0x28);
			X_SIZE = 320;
			Y_SIZE = 240;
			break;
		case 2:
			LCD_SendData(0x88);
			X_SIZE = 240;
			Y_SIZE = 320;
			break;
		case 3:
			LCD_SendData(0xE8);
			X_SIZE = 320;
			Y_SIZE = 240;
			break;
	}
}


//******************************************************************************
//***       ������������� �������
//******************************************************************************
void LCD_ili9341_ini (void)
	{
	//1. Power up the system platform and assert the RESET# signal (�L� state) for a minimum of 100us to reset the controller. 
		TFT_RST_SET
    HAL_Delay (100);
    TFT_RST_RESET
    HAL_Delay (120);
    TFT_RST_SET
    HAL_Delay (120);

                LCD_SendCommand(0xE2);  //��������� �������
    	        LCD_SendCommand(0x01);//Software Reset
    			//DelayMicro(1);
    			LCD_SendCommand(0xCB);//Power Control A
    			LCD_SendData(0x39);
    			LCD_SendData(0x2C);
    			LCD_SendData(0x00);
    			LCD_SendData(0x34);
    			LCD_SendData(0x02);
    			//DelayMicro(1);
    			LCD_SendCommand(0xCF);//Power Control B
    			LCD_SendData(0x00);
    			LCD_SendData(0xC1);
    			LCD_SendData(0x30);
    			//DelayMicro(1);
    			LCD_SendCommand(0xE8);//Driver timing control A
    			LCD_SendData(0x85);
    			LCD_SendData(0x00);
    			LCD_SendData(0x78);
    			//DelayMicro(1);
    			LCD_SendCommand(0xEA);//Driver timing control B
    			LCD_SendData(0x00);
    			LCD_SendData(0x00);
    			//DelayMicro(1);
    			LCD_SendCommand(0xED);//Power on Sequence control
    			LCD_SendData(0x64);
    			LCD_SendData(0x03);
    			LCD_SendData(0x12);
    			LCD_SendData(0x81);
    			//DelayMicro(1);
    			LCD_SendCommand(0xF7);//Pump ratio control
    			LCD_SendData(0x20);
    			//DelayMicro(1);
    			LCD_SendCommand(0xC0);//Power Control 1
    			LCD_SendData(0x10);
    			//DelayMicro(1);
    			LCD_SendCommand(0xC1);//Power Control 2
    			LCD_SendData(0x10);
    			//DelayMicro(1);
    			LCD_SendCommand(0xC5);//VCOM Control 1
    			LCD_SendData(0x3E);
    			LCD_SendData(0x28);
    			//DelayMicro(1);
    			LCD_SendCommand(0xC7);//VCOM Control 2
    			LCD_SendData(0x86);
    			//DelayMicro(1);
    			LCD_ili9341_SetRotation(3);
    			//DelayMicro(1);
    			LCD_SendCommand(0x3A);//Pixel Format Set
    			LCD_SendData(0x55);//16bit
    			//DelayMicro(1);
    			LCD_SendCommand(0xB1);
    			LCD_SendData(0x00);
    			LCD_SendData(0x18);// ×àñòîòà êàäðîâ 79 Ãö
    			//DelayMicro(1);
    			LCD_SendCommand(0xB6);//Display Function Control
    			LCD_SendData(0x08);
    			LCD_SendData(0x82);
    			LCD_SendData(0x27);//320 ñòðîê
    			//DelayMicro(1);
    			LCD_SendCommand(0xF2);//Enable 3G (ïîêà íå çíàþ ÷òî ýòî çà ðåæèì)
    			LCD_SendData(0x00);//íå âêëþ÷àåì
    			//DelayMicro(1);
    			LCD_SendCommand(0x26);//Gamma set
    			LCD_SendData(0x01);//Gamma Curve (G2.2) (Êðèâàÿ öâåòîâîé ãàììû)
    			//DelayMicro(1);
    			LCD_SendCommand(0xE0);//Positive Gamma  Correction
    			LCD_SendData(0x0F);
    			LCD_SendData(0x31);
    			LCD_SendData(0x2B);
    			LCD_SendData(0x0C);
    			LCD_SendData(0x0E);
    			LCD_SendData(0x08);
    			LCD_SendData(0x4E);
    			LCD_SendData(0xF1);
    			LCD_SendData(0x37);
    			LCD_SendData(0x07);
    			LCD_SendData(0x10);
    			LCD_SendData(0x03);
    			LCD_SendData(0x0E);
    			LCD_SendData(0x09);
    			LCD_SendData(0x00);
    			//DelayMicro(1);
    			LCD_SendCommand(0xE1);//Negative Gamma  Correction
    			LCD_SendData(0x00);
    			LCD_SendData(0x0E);
    			LCD_SendData(0x14);
    			LCD_SendData(0x03);
    			LCD_SendData(0x11);
    			LCD_SendData(0x07);
    			LCD_SendData(0x31);
    			LCD_SendData(0xC1);
    			LCD_SendData(0x48);
    			LCD_SendData(0x08);
    			LCD_SendData(0x0F);
    			LCD_SendData(0x0C);
    			LCD_SendData(0x31);
    			LCD_SendData(0x36);
    			LCD_SendData(0x0F);
    			//DelayMicro(1);
    			LCD_SendCommand(0x11);//Âûéäåì èç ñïÿùåãî ðåæèì
    			HAL_Delay(150);
    			LCD_SendCommand(0x29);//Âêëþ÷åíèå äèñïëåÿ
    			LCD_SendData(0x2C);
    			HAL_Delay(150);
}
