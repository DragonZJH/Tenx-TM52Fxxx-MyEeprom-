


/**************************** MyEeprom 模块概述**********************************
*********************************************************************************
    TM52Fxx系列芯片 内部EEPROM擦写次数为最小大于5万次,即单个地址的擦写次数是有限的
 随着擦写次数的增加每次擦写的时间会有所延长，虽然对于一般不需要经常操作EEPROM的应
 用5-10万的擦写次数完全可以满足整个芯片使用周期，但有些应用需要更加频繁的保存/读取
 数据则需要芯片能够提供足够多的擦写次数。本DEMO主要为了有以上需要的朋友们提供一个很
 好的解决方案.
    DEMO的大致原理就是在芯片提供的整个EEPROM空间内，根据用户需要保存数据的大小,自动
 分配N个区域,根据数据写入的次数动态地把用户数据保存在这些区域中，在芯片使用周期内确
 保整个EEPROM空间使用率得到很好的平衡。理论上可以就达到了延长芯片EEPROM的使用寿命的
 目的，使用寿命将延长至原来的xN(区域)倍。经压力测试每秒擦写20-30次保存15字节数据擦写
 次数已经超过18万次多，数据没有发现异常.

 模块配置使用步骤----MyEeprom.h头文件
     1：根据提示修改以下宏定义     
        #define ChipEepromRamCP  256	   //芯片EepromRam的容量大小
        #define EepromStartAddr  0XEE00	  //这个芯片EEPROM物理内存中起始地址
        #define TM52FE8276_TotalEeprom  128  //这个芯片总共可以使用的EEPROM字节，TM52f82xx芯片偶地址有效

     2：在枚举项@AllEepromAddr加入你需要保存数据的地址名称
        注意枚举中前后两个项不能更改

     3：如果需要调试或其他测试，设置宏定义 TestEeprom  为TRUE  
     
 模块配置使用步骤----MyEeprom.c文件
     1：在工程初始化调用函数 MyEepromInit()

     2：根据提示修改此接口函数内部MyEepromSaveAllDataUserHandle (  )

     3：单个数据写调用--MyEepromWrite ( unsigned int,unsigned char ,unsigned char )
        单个数据读调用--unsigned char MyEepromRead(unsigned char)

        测试时接口--    MyEepromTestHandle ( char* from,unsigned char flag )


 其他：暂时只测试了十速TM52FE8273芯片，根据数据手册其他TM52FEXX系列应该也是可以通用的
       移植到到其他只需需该修改部分代码，后续有时间继续更新
 
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



/* 快时钟系统主频为 14.7456M
 */
void bsp_clock_init()
{
	SELFCK = 0;	     //切换到慢时钟
	SYSCLOCK_DIV_1;   //div 1时钟分频
	Delay30us();
	SELFCK = 1;			//切换到快时钟
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
//需要用户自行加入LED显示模块
void SetLEDDisplay()
{

    //eeprom测试用
	if ( RunData.WorkCount_60s<20 )
	{
		MyEepromTestHandle  ( ( char* ) LEDShow,'C' ); //LED显示相应区域
	}
	else
	{
		MyEepromTestHandle ( ( char* ) LEDShow,'D' );//LED显示总写入次数
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
     每1S发送一次
     注意因为过快的写入和读取时间导致有些参数始终没有变化的问题
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

	bsp_clock_init();  //快时钟系统主频为 14.7456M/2 = 7.3728M
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
        //EEPROM写入测试
		MyEepromTestHandle ((char*)(&RunData.WorkCount_10ms) ,'B' ); 
		//写入LED缓冲区显示查看
		SetLEDDisplay(); 
#endif		

		//数据发送到串口查看
		SendToUart();

	}
}






void ISR_TIMER0() interrupt 1  //2ms

{


	static unsigned char Count_10Ms;

	TH0 = 0xc6;	// timer0 高字节数据
	TL0 = 0x66+16;	// timer0 低字节数据

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

	TMOD = ( ( TMOD&~0X03 ) |T0_Mode1 ); //模式1 16位定时器/计数器(TMOD &~ TMOD_TMOD0_MASK)|1<<TMOD_TMOD0_POS;

	TH0 = 0xc6;    // timer0 高字节数据
	TL0 = 0x66;    // timer0 低字节数据
	ET0 = 1;
	TF0 = 0;
	TR0 =1 ;
}

















