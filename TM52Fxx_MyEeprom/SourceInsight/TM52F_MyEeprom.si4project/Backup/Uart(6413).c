
#include "Uart.h"

uart_status UARTStatus;


//���ڳ�ʼ��
void UartInit ( void ) //24M 9600
{
	
	//����λΪ1,ʵ�������ò����ʵ�˫��  --�����õ���9600��ʵ���Ͻ��պͷ��Ͳ�����Ϊ19200
	PCON=(PCON&~0X80)|(0<<7);  //UART1��ֹ˫�����ʣ�����λΪ1,ʵ�������ò����ʵ�˫��  ������9600����պͷ���ʵ��Ϊ19200
	OPTION=OPTION&~0X80; //��ֹһ��ģʽ
	//SCON=0X50;         //8λ����,�ɱ䲨����
	SM0=0;               //ģʽ1 8λ����,�ɱ䲨����
	SM1=1;
	SM2=0;

	
	RCLK=0;            //time1�����Ϊ���ڽ��� 
	TCLK=0;            //time1�����Ϊ���ڷ���

	
    TMOD=(TMOD&0XCF|0X20); //8 λ�Զ����ض�ʱ��/������(TL1),���ʱ�� TH1 ����װ��
	//������=(SMOD+1) x FSYSCLK /(32x2x (256 �C TH1))
	TH1=232;           //9600 �Զ�����

	ET1=0;             //��ֹTIMEER1�ж�
	TR1=1;             //������ʱ��1

	REN=1;              //�������
	ES=1;              //�������ж�
	
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


//�����ж�
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




