


/**************************** MyEeprom ģ�����**********************************
*********************************************************************************
    TM52Fxxϵ��оƬ �ڲ�EEPROM��д����Ϊ��С����5���,��������ַ�Ĳ�д���������޵�
 ���Ų�д����������ÿ�β�д��ʱ��������ӳ�����Ȼ����һ�㲻��Ҫ��������EEPROM��Ӧ
 ��5-10��Ĳ�д������ȫ������������оƬʹ�����ڣ�����ЩӦ����Ҫ����Ƶ���ı���/��ȡ
 ��������ҪоƬ�ܹ��ṩ�㹻��Ĳ�д��������DEMO��ҪΪ����������Ҫ���������ṩһ����
 �õĽ������.
    DEMO�Ĵ���ԭ�������оƬ�ṩ������EEPROM�ռ��ڣ������û���Ҫ�������ݵĴ�С,�Զ�
 ����N������,��������д��Ĵ�����̬�ذ��û����ݱ�������Щ�����У���оƬʹ��������ȷ
 ������EEPROM�ռ�ʹ���ʵõ��ܺõ�ƽ�⡣�����Ͽ��Ծʹﵽ���ӳ�оƬEEPROM��ʹ��������
 Ŀ�ģ�ʹ���������ӳ���ԭ����xN(����)������ѹ������ÿ���д20-30�α���15�ֽ����ݲ�д
 �����Ѿ�����18��ζ࣬����û�з����쳣.

 ģ������ʹ�ò���----MyEeprom.hͷ�ļ�
     1��������ʾ�޸����º궨��     
        #define ChipEepromRamCP  256	   //оƬEepromRam��������С
        #define EepromStartAddr  0XEE00	  //���оƬEEPROM�����ڴ�����ʼ��ַ
        #define TM52FE8276_TotalEeprom  128  //���оƬ�ܹ�����ʹ�õ�EEPROM�ֽڣ�TM52f82xxоƬż��ַ��Ч

     2����ö����@AllEepromAddr��������Ҫ�������ݵĵ�ַ����
        ע��ö����ǰ��������ܸ���

     3�������Ҫ���Ի��������ԣ����ú궨�� TestEeprom  ΪTRUE  
     
 ģ������ʹ�ò���----MyEeprom.c�ļ�
     1���ڹ��̳�ʼ�����ú��� MyEepromInit()

     2��������ʾ�޸Ĵ˽ӿں����ڲ�MyEepromSaveAllDataUserHandle (  )

     3����������д����--MyEepromWrite ( unsigned int,unsigned char ,unsigned char )
        �������ݶ�����--unsigned char MyEepromRead(unsigned char)

        ����ʱ�ӿ�--    MyEepromTestHandle ( char* from,unsigned char flag )


 ��������ʱֻ������ʮ��TM52FE8273оƬ�����������ֲ�����TM52FEXXϵ��Ӧ��Ҳ�ǿ���ͨ�õ�
       ��ֲ��������ֻ������޸Ĳ��ִ��룬������ʱ���������
 
******************************************************************************

******************************************************************************/



#include "TM52F82XX_Eeprom.h"
#include "MyEeprom.h"

#include "DEFINE.H"


#include "Uart.h"

#include "Delay.h"
#include "LED.h"


#include	<intrins.h>

#include <REGtenxTM52FE8276.h>




void		 Timer0_Init();
void         SetLEDDisplay();



/* ��ʱ��ϵͳ��ƵΪ 14.7456M
 */
void bsp_clock_init()
{
	SELFCK = 0;	     //�л�����ʱ��
	SYSCLOCK_DIV_1;   //div 1ʱ�ӷ�Ƶ
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

	#if TestEeprom==TRUE
  MyEeprom.EepromTestWriteCount=0;
		#endif
	ReadSameDataFromEeprom();

	
	WDTPSC0;//WDT 480MS
	WDT_RUN_FAST_DIS_ILDE;

	EA=1;
}

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


#if TestEeprom==TRUE
//��Ҫ�û����м���LED��ʾģ��
void SetLEDDisplay()
{

    //eeprom������
	if ( RunData.WorkCount_60s<20 )
	{
		MyEepromTestHandle  ( ( char* ) LEDShow,'C' ); //LED��ʾ��Ӧ����
	}
	else
	{
		MyEepromTestHandle ( ( char* ) LEDShow,'D' );//LED��ʾ��д�����
	}
}

#endif

#if TestEeprom
unsigned char GetHex(unsigned char dat )
{
  if(dat==0)
    return 0x30;
 if(dat<10)
    return dat+0x30;
  else
    return dat+0x37;
    
}

#endif


void SendToUart()
{
   #if TestEeprom==TRUE
   /***********************************************************
     ÿ1S����һ��
     ע����Ϊ�����д��Ͷ�ȡʱ�䵼����Щ����ʼ��û�б仯������
   ************************************************************/
	if ( ( RunData.SendDataToUartCount >=1)  ) 
	{

		unsigned char xdata send[100];
		unsigned char  t=0;
		MyEepromTestHandle ( FALSE,'A' );

		ReadSameDataFromEeprom();

		send[t++]=  'M' ;
		send[t++]=  'y' ;
		send[t++]= 'E' ;
		send[t++]=  'e' ;
		send[t++]=  'p' ;
		send[t++]=  'r' ;
		send[t++]= 'o' ;
		send[t++]=  'm' ;
		send[t++]= 'T' ;
		send[t++]=  'e' ;
		send[t++]=  's' ;
		send[t++]=  't' ;
		send[t++]=  ':' ;
		send[t++]=  '\r' ;
		send[t++]=  '\n' ;

		send[t++]=  '[' ;
		send[t++]=  'T' ;
		send[t++]=  'i' ;
		send[t++]=  'm' ;
		send[t++]=  'e' ;
		send[t++]=  ':' ;
		send[t++]=  ( RunData.WorkTime_Second/10000+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%10000/1000+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%1000/100+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%100/10+0x30 );
		send[t++]=  ( RunData.WorkTime_Second%10+0x30 );
		send[t++]=  ']' ;
		send[t++]=  '-' ;

		send[t++]=  '[' ;
		send[t++]=  GetHex(RunData.FD_Count>>12);
		send[t++]=  GetHex((RunData.FD_Count>>8)&0x000f);
		send[t++]=  GetHex((RunData.FD_Count>>4)&0x000f);
		send[t++]=  GetHex(RunData.FD_Count&0x000f);
		send[t++]=   ']' ;
		send[t++]=  '-' ;

        send[t++]=  '[' ;
		send[t++]=  GetHex( SetData.FH_Mode>>8 );
		send[t++]=  GetHex( SetData.FH_Mode&0x0f);
		send[t++]=  ']' ;
		send[t++]=  '-' ;
		
		send[t++]= '[' ;
		send[t++]= GetHex( SetData.SetTemp>>12);
		send[t++]= GetHex( (SetData.SetTemp>>8)&0x000f);
		send[t++]= GetHex( (SetData.SetTemp>>4)&0x000f );
		send[t++]= GetHex( SetData.SetTemp&0x000f);
		send[t++]=  ']' ;
		send[t++]=  '-' ;

		send[t++]= '[' ;
		send[t++]= GetHex( SetData.SetHumidity>>8 );
		send[t++]= GetHex( SetData.SetHumidity&0x0f );
		send[t++]= ']' ;
		send[t++]=  '-' ;



		send[t++]=  '[' ;
		send[t++]=  ( MyEeprom.EepromTestWriteCount /10000000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %10000000/1000000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %1000000/100000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %100000/10000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %10000/1000+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %1000/100+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %100/10+0x30 );
		send[t++]=  ( MyEeprom.EepromTestWriteCount %10+0x30 );
		send[t++]=  ']' ;
		

		send[t++]= '\r' ;
		send[t++]=  '\n' ;
		send[t++]= '\r' ;
		send[t++]=  '\n' ;
		UartSendArray ( send, t );

		RunData.SendDataToUartCount=0;
	}

   #else
    UartSendArray ( "---\r\n", 5 );
   #endif
}




void main ()
{

	bsp_clock_init();  //��ʱ��ϵͳ��ƵΪ 14.7456M/2 = 7.3728M
	ClearXRAM();
	SystemInit();


	P0OE|= ( 1<<7 );
    P0_7=1;

	while ( 1 )
	{
	
        if(RunData.WorkCount_60s%5==0)
         P0_7=1;
         else if(RunData.WorkCount_60s%7==0)
         P0_7=0;
         
#if TestEeprom==TRUE
        //EEPROMд�����
		MyEepromTestHandle ((char*)(&RunData.WorkCount_10ms) ,'B' ); 
		//д��LED��������ʾ�鿴
		SetLEDDisplay(); 
#endif		

		//���ݷ��͵����ڲ鿴
		SendToUart();

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
		RunData.WorkTime_Second++;
		if ( RunData.WorkCount_1s>=100 )
		{
			RunData.SendDataToUartCount++;
			RunData.WorkCount_1s=0;
			RunData.WorkCount_60s++;
			//RunData.WorkTime_Second++;
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

















