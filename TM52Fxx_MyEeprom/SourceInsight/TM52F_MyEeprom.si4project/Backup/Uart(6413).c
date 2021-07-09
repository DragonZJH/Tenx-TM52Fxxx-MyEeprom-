
#include "Uart.h"

uart_status UARTStatus;


//串口初始化
void UartInit ( void ) //24M 9600
{
	
	//第七位为1,实际是设置波特率的双倍  --如设置的是9600则实际上接收和发送波特率为19200
	PCON=(PCON&~0X80)|(0<<7);  //UART1禁止双波特率，第七位为1,实际是设置波特率的双倍  如设置9600则接收和发送实际为19200
	OPTION=OPTION&~0X80; //禁止一线模式
	//SCON=0X50;         //8位数据,可变波特率
	SM0=0;               //模式1 8位数据,可变波特率
	SM1=1;
	SM2=0;

	
	RCLK=0;            //time1溢出作为串口接受 
	TCLK=0;            //time1溢出作为串口发送

	
    TMOD=(TMOD&0XCF|0X20); //8 位自动重载定时器/计数器(TL1),溢出时从 TH1 重新装载
	//波特率=(SMOD+1) x FSYSCLK /(32x2x (256 – TH1))
	TH1=232;           //9600 自动重载

	ET1=0;             //禁止TIMEER1中断
	TR1=1;             //启动定时器1

	REN=1;              //允许接收
	ES=1;              //允许串口中断
	
}

void Uart_Send_Byte ( unsigned char data1 )
{
	
	TI=0;
	SBUF=data1;
    while (  !TI );
	TI=0;

}




void UartSendArray ( unsigned char* array,unsigned char t )
{
	unsigned char i;

  for ( i=0; i<t; i++ )
	{
		Uart_Send_Byte ( array[i] );
	}


}

#if 0

unsigned int StringLen(char *string){
  unsigned int len=0;
   while(*string){
         string++;
         len++;
   }
   return len;
}
void UartSendString (  char* string )
{
	unsigned char i;
	unsigned int t= StringLen(string);
	for ( i=0; i<t; i++ )
	{
		Uart_Send_Byte ( string[i] );
	}

}

unsigned char ChuckUartStatus()
{
	if(PCON&~0X80) return FALSE;
	if(OPTION&~0X80) return FALSE;
	if(SM0|RCLK|TCLK) return FALSE;
	if(!(SM1&REN&ES&TR1)) return FALSE;
   return TRUE;
}

void Uart_SetTimeOut(){

if ( UARTStatus.Run==UART_BUSY )
	{

		if ( UARTStatus.TimeOutCout<UART_TIME_OUT_50MS )

		{
			UARTStatus.TimeOutCout++;
		}

	}


}

void Uart_ReceiveTimeOutChuck()
{
	if ( UARTStatus.TimeOutCout>=UART_TIME_OUT_50MS )
	{
		UARTStatus.Run=UART_IDLE;
		UARTStatus.TimeOutCout=0;
		UARTStatus.CharCount=0;
		// AddArrayList ( &ArrayList_SendData,UartReadData );
	}

}
#endif


//串口中断
void UART_int ( void ) interrupt 4
{
	unsigned char data a=0;
	if ( TI )
	{
		TI=0;
		return;
	}
	if ( RI )
	{
		RI=0;
		SBUF=SBUF;
	}


}




