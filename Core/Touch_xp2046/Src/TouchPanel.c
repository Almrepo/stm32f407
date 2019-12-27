/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_hal.h"
#include "main.h"
//#include "TouchPanel.h"


//#include "spi.h"
/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;
extern uint8_t INT_Touch_value;
/* DisplaySample */
Coordinate ScreenSample[3];
/* LCD??�???�? */
Coordinate DisplaySample[3] =   {
                      { 25, 25 }, // ������ ����� ����������
											{ 300 , 100}, // ������
                      { 150, 200}
	                            } ;

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 300   // ������ (�����������)
extern hspi1;

/*******************************************************************************
* Function Name  : RD_AD
* Description    : �???ADC?�
* Input          : None
* Output         : None
* Return         : ADS7843���?�?????????
* Attention		 : None
*******************************************************************************/




void TP_GetAdXY(int *x,int *y)  
{ 
	unsigned int buff = 0;
	unsigned char cmd, temp;

  cmd = CHX;

HAL_SPI_TransmitReceive(&hspi1, &cmd, (uint8_t *) &temp, 1, 3000); // ���������� ������� �� X
//HAL_Delay(1);

cmd  = 0;
temp = 0;
	
HAL_SPI_TransmitReceive(&hspi1, &cmd, (uint8_t *) &temp, 1, 3000);
  buff = temp<<8; 
	cmd = CHY;
HAL_SPI_TransmitReceive(&hspi1, &cmd, (uint8_t *) &temp, 1, 3000); // �������� ������� ���� � ������������ ���������� ������� �� Y
//HAL_Delay(1);
  buff |= temp; 
  *x= ((buff >>3) & 0x0FFF) ; //��������� �
	
cmd = 0;
HAL_SPI_TransmitReceive(&hspi1, &cmd, (uint8_t *) &temp, 1, 3000);
  buff = temp<<8; 
HAL_SPI_TransmitReceive(&hspi1, &cmd, (uint8_t *) &temp, 1, 3000);
  buff |= temp; 
  *y= ((buff >> 3) & 0x0FFF) ; //��������� Y
}


/*******************************************************************************
* Function Name  : DrawCross
* Description    : ???????????
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos) // ������ �������
{
  LCD_ili9341_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,0xffff);
  LCD_ili9341_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,0xffff);
  LCD_ili9341_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,0xffff);
  LCD_ili9341_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,0xffff);
}	

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : ?????? ?????????? Ads7846
* Input          : None
* Output         : None
* Return         : Coordinate X Y
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};  
  
  do	
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }
  while(!TP_INT_IN&& count<9); /* ���� ���� ������� �� ��� ��� ���� ��������� 9 ����������� ��� */
  if(count==9) /* ���� �������� ��� 9 ����������� ��� */ 
  {      
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3; // ������ ���������� 1:3
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3; // ������
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3; //������

	m0=temp[0]-temp[1];   // ���������� ������� � �����������
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];

	m0=m0>0?m0:(-m0);    // �������� �� ��������������� ���������� � ���������� � "���������������"
    m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
		/* �������� ������� � �����������, ���� ������ ������ (THRESHOLD) �� �����  */
	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
	   
	if(m0<m1) // ��������� ������� ��������� � ����������
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

		/*** ��-�� ����� ��� Y ***/
    temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3; 
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3; 
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3; 
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);	
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;
	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}


/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    :  K A B C D E F
* Input          : None
* Output         : None
* Return         : retTHRESHOLD 1 0
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{


  FunctionalState retTHRESHOLD = ENABLE ;
  /* K=(X0-X2) (Y1-Y2)-(X1-X2) (Y0-Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A=((XD0-XD2) (Y1-Y2)-(XD1-XD2) (Y0-Y2))/K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B=((X0-X2) (XD1-XD2)-(XD0-XD2) (X1-X2))/K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C=(Y0(X2XD1-X1XD2)+Y1(X0XD2-X2XD0)+Y2(X1XD0-X0XD1))/K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D=((YD0-YD2) (Y1-Y2)-(YD1-YD2) (Y0-Y2))/K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E=((X0-X2) (YD1-YD2)-(YD0-YD2) (X1-X2))/K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F=(Y0(X2YD1-X1YD2)+Y1(X0YD2-X2YD0)+Y2(X1YD0-X0YD1))/K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}


/*******************************************************************************
* Function Name  : getDisplayPoint // ��������� ����� �������, ��������� � �������� ��������.
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr,Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  static GUI_PID_STATE TS_State;


  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
   // displayPtr->x=320-displayPtr->x;
   // displayPtr->y=240-displayPtr->y;
    if (TP_INT_IN==0)
    {
    	EXTI->PR = (1<<2);
    	INT_Touch_value=1;
    	TS_State.Pressed = INT_Touch_value;
    	TS_State.x = 320-displayPtr->x;
    	TS_State.y = 240-displayPtr->y;
    	GUI_PID_StoreState(&TS_State);


    }
    else
    {
    	if(TP_INT_IN==1)
    	{
    		INT_Touch_value=0;
    		EXTI->PR = (1<<2);
    		TS_State.Pressed = INT_Touch_value;
    		GUI_PID_StoreState(&TS_State);



    	}

    }
   // GUI_TOUCH_StoreState(displayPtr->x,displayPtr->y);
   // GUI_CURSOR_SetPosition(displayPtr->x,displayPtr->y);
   // INT_Touch_value=0;
  }
  else
  {
	     retTHRESHOLD = DISABLE;
  }

  return(retTHRESHOLD);
} 

void GetPoint_TS  (uint16_t *x, uint16_t *y)
{
	Matrix * matrixPtr;
	Coordinate * displayPtr;
	Coordinate * screenPtr;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) +
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) +
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;

  }

}



/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : ?????
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
  uint8_t i;
  Coordinate * Ptr;

	

  for(i=0;i<3;i++)
  {
		LCD_ili9341_Clear(BLACK);
	 sprintf( array,"Touch to calibrate");
	 WriteString( 10, 50, array , WHITE);


	
		DrawCross(DisplaySample[i].x,DisplaySample[i].y);
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;		
  	HAL_Delay(500);
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
LCD_ili9341_Clear(BLACK);


} 

void Touch_Cal_Read (Matrix * matrixPtr) // ����� ����� ������ ���������� ������ �������, ��� � ������� ������� ����������� ����������
{																				// ��������� ����� ���������� � ��������� (���������)
	matrixPtr->An = 2575;//-4400;
	matrixPtr->Bn = 474000;//329175;
	matrixPtr->Cn = -190988525;//-107729200;
	matrixPtr->Dn = -430500;//-429775;//-310500;
	matrixPtr->En = -21500;//-21725;
	matrixPtr->Fn = 1578894425;//1579784425;//1157034400;
	matrixPtr->Divider =5255690;//3720161;

}	





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
