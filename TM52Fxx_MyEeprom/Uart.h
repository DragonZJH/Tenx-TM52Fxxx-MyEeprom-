

#ifndef  __UART_H__
#define  __UART_H__

#include "DEFINE.H"
#include	"REGtenxTM52FE8276.h"


typedef enum
{
	UART_TIME_OUT_50MS = 25,  
	UART_BUSY=12,
	UART_IDLE=16
} UART_PRA;
typedef struct
{
	unsigned int TimeOutCout;
	UART_PRA Run;
	unsigned char CharCount;

} uart_status;



extern void Uart_SetTimeOut();
extern void Uart_ReceiveTimeOutChuck();
extern unsigned char ChuckUartStatus();
extern void UartInit ( void );
extern void Uart_Send_Byte ( unsigned char );
extern void UartSendArray ( unsigned char*,unsigned char );

#endif
