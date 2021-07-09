

#include "TM52F82XX_Eeprom.h"
#include "MyEeprom.h"

#include "DEFINE.H"


#include "Uart.h"

#include "Delay.h"
#include "LED.h"


#include	"setting.h"
#include	<intrins.h>

#include <REGtenxTM52FE8276.h>





void		 Timer0_Init();
void         Timer2_Init();
void         SetLEDDisplay();



/* ��ʱ��ϵͳ��ƵΪ 14.7456M/2 = 7.3728M
 */
void bsp_clock_init()
{
	SELFCK = 0;	     //�л�����ʱ��
	SYSCLOCK_DIV_1;   //div 1	 ʱ�ӷ�Ƶ
	Delay30us();
	SELFCK = 1;			//�л�����ʱ��


}




void SystemInit()
{
	unsigned char t;

	for ( t=0; t<10; t++ )
	{
		Delay10Ms();
	}
	bsp_clock_init();
	MyEepromInit();
	UartInit();
	Timer0_Init();

	ReadSameDataFromEeprom();
	EA=1;


}




//=============================================================================
// Function:	void ClearXRAM(void)
// Purpose:	initial ram
// Input:	none
// OutPut:	none
// Author:
//=============================================================================
void ClearXRAM ( void )
{
	unsigned char xdata* data l_p_ucXdata;
	unsigned int idata i;
	l_p_ucXdata = 0xfd00;
	for ( i = 0; i < 768; i++ )
	{
		* ( l_p_ucXdata++ ) = 0x00;
	}
}

void SetLEDDisplay()
{

//eeprom������
#if TestEeprom==TRUE
	if ( RunData.WorkCount_60s<20 )
	{
		EepromTestHandle  ( ( char* ) LEDShow,'C' ); //LED��ʾ��Ӧ����
	}
	else
	{
		EepromTestHandle ( ( char* ) LEDShow,'D' );//LED��ʾ��д�����
	}

#endif
}


void SendToUart()
{

	if ( ( RunData.SendDataToUartCount >=3 )  ) //ÿ3S����һ��
	{

#if TestEeprom==TRUE

		unsigned char xdata send[80];
		unsigned char  t=0;

		EepromTestHandle ( FALSE,'A' );

		ReadSameDataFromEeprom();

        send[t++]= 'T' ;
		send[t++]=  'e' ;
		send[t++]=  'S' ;
		send[t++]=  't' ;
		send[t++]=  'M' ;
		send[t++]=  'y' ;
		send[t++]= 'E' ;
		send[t++]=  'e' ;
		send[t++]=  'p' ;
		send[t++]=  'r' ;
		send[t++]= 'o' ;
		send[t++]=  'm' ;
		send[t++]=  ':' ;
		send[t++]=  '\r' ;
		send[t++]=  '\n' ;

		send[t++]=  '[' ;
		send[t++]= ( RunData.WorkTime_Second/10000+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%10000/1000+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%1000/100+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%100/10+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%10+0x30 );
		send[t++]=  ']' ;
		send[t++]=  '-' ;

		send[t++]=  '[' ;
		send[t++]=  ( RunData.CS_Count/10000+0x30 );
		send[t++]=  ( RunData.CS_Count%10000/1000+0x30 );
		send[t++]=  ( RunData.CS_Count%1000/100+0x30 );
		send[t++]=  ( RunData.CS_Count%100/10+0x30 );
		send[t++]=  ( RunData.CS_Count%10+0x30 );
		send[t++]=  ']' ;
		send[t++]=  '-' ;

		send[t++]=  '[' ;
		send[t++]=  ( RunData.FD_Count/10000+0x30 );
		send[t++]=  ( RunData.FD_Count  %10000/1000+0x30 );
		send[t++]=  ( RunData.FD_Count%1000/100+0x30 );
		send[t++]=  ( RunData.FD_Count%100/10+0x30 );
		send[t++]=  ( RunData.FD_Count%10+0x30 );
		send[t++]=   ']' ;
		send[t++]=  '-' ;

		send[t++]= '[' ;
		send[t++]=  ( SetData.SetTemp/10000+0x30 );
		send[t++]=  ( SetData.SetTemp%10000/1000+0x30 );
		send[t++]=  ( SetData.SetTemp%1000/100+0x30 );
		send[t++]=  ( SetData.SetTemp%100/10+0x30 );
		send[t++]=  ( SetData.SetTemp%10+0x30 );
		send[t++]=  ']' ;
		send[t++]=  '-' ;

		send[t++]= '[' ;
		send[t++]=  ( SetData.SetHumidity /10000+0x30 );
		send[t++]=  ( SetData.SetHumidity%10000/1000+0x30 );
		send[t++]=  ( SetData.SetHumidity%1000/100+0x30 );
		send[t++]=  ( SetData.SetHumidity%100/10+0x30 );
		send[t++]=  ( SetData.SetHumidity%10+0x30 );
		send[t++]= ']' ;
		send[t++]=  '-' ;


		send[t++]=  '[' ;
		send[t++]=  ( MyEeprom.EepromTestWriteCount  /1000000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %1000000/100000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %100000/10000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %10000/1000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %1000/100+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %100/10+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %10+0x30 );
		send[t++]=  ']' ;
		

		send[t++]= '\r' ;
		send[t++]=  '\n' ;
		UartSendArray ( send, t );
#else
		Uart_Send_Byte ( '-' );
#endif

		RunData.SendDataToUartCount=0;


	}


}



void main ()
{

	bsp_clock_init();  //��ʱ��ϵͳ��ƵΪ 14.7456M/2 = 7.3728M
	ClearXRAM();
	SystemInit();


	WDTPSC0;//WDT 480MS
	WDT_RUN_FAST_DIS_ILDE;

	P0OE|= ( 1<<7 );
#if TestEeprom==TRUE
	EepromTestHandle (  FALSE,'A' );
#endif
    P0_7=1;
	while ( 1 )
	{
        if(RunData.WorkCount_60s%5==0)
         P0_7=1;
         else if(RunData.WorkCount_60s%7==0)
         P0_7=0;
#if TestEeprom==TRUE
		{
		 
		EepromTestHandle ((char*)(&RunData.WorkCount_10ms) ,'B' ); //д�����
		SetLEDDisplay(); //д��LED����������Ҫ�û��Լ�����LED��ʾģ��
		}
#endif

		SendToUart();//���ݷ��͵�����


	}
}






void ISR_TIMER0() interrupt 1  //2ms

{


	static unsigned char Count_10Ms;

	TH0 = 0xc6;	// timer0 ���ֽ�����
	TL0 = 0x66+16;	// timer0 ���ֽ�����

	CLR_WDT;

	//Uart_SetTimeOut();

	Count_10Ms++;
	if ( Count_10Ms>=5 )
	{
		Count_10Ms=0;
		RunData.WorkCount_10ms++;
		RunData.WorkCount_1s++;
		if ( RunData.WorkCount_1s>=100 )
		{
			RunData.SendDataToUartCount++;
			RunData.WorkCount_1s=0;
			RunData.WorkCount_60s++;
			RunData.WorkTime_Second++;
			if ( RunData.WorkCount_60s>=60 )
			{
				RunData.WorkCount_60s=0;
			}
		}

	}


}






void Timer0_Init()
{

	TMOD = ( ( TMOD&~0X03 ) |T0_Mode1 ); //ģʽ1 16λ��ʱ��/������(TMOD &~ TMOD_TMOD0_MASK)|1<<TMOD_TMOD0_POS;

	TH0 = 0xc6;    // timer0 ���ֽ�����
	TL0 = 0x66;    // timer0 ���ֽ�����
	ET0 = 1;
	TF0 = 0;
	TR0 =1 ;
}

















