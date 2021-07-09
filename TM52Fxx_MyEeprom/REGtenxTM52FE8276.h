/*
******************************************************************************************************
Name		:TM52FE8276.h
Version		:V1.0
Author		:MFTu
Date		:2020.01.21
******************************************************************************************************
*/
#ifndef __TM52FE8276_H__
#define __TM52FE8276_H__
/*
******************************************************************************************************
BYTE Registers
******************************************************************************************************
*/
sfr 	P0    		= 0x80;			//Port 0 data
	//--------------------------------------------------------------------------------------------
	sbit 		P0_7		= P0^7;
	sbit 		P0_6		= P0^6;
	sbit 		P0_5		= P0^5;
	sbit 		P0_4		= P0^4;
	sbit 		P0_3		= P0^3;
	sbit 		P0_2		= P0^2;
	sbit 		P0_1		= P0^1;
	sbit 		P0_0		= P0^0;
/*
******************************************************************************************************
*/
sfr 	SP   		= 0x81;			//Stack Point
/*
******************************************************************************************************
*/
sfr 	DPL   		= 0x82;			//Data Point Low byte
sfr 	DPH   		= 0x83;   		//Data Point High byte
/*
******************************************************************************************************
*/
sfr 	PCON  		= 0x87;			//不支持位操作,用读改写方式
	//--------------------------------------------------------------------------------------------
	#define 	SMOD	0x80		//PCON Bit[7],1=波特率倍增
	//--------------------------------------------------------------------------------------------
	#define 	GF1		0x08		//PCON Bit[3],用户可以随便使用
	#define 	GF0		0x04		//PCON Bit[2],用户可以随便使用
	//--------------------------------------------------------------------------------------------
	#define 	PD		0x02		//PCON Bit[1],此位置1,芯片进入stop模式
	//--------------------------------------------------------------------------------------------
	#define 	IDL		0x01		//PCON Bit[0],此位置1,芯片进入IDL模式
	//---------------------------------------------------------------------------------------------
	#define		ENTER_IDLE_MODE		PCON=(PCON&0xfe)|IDL
	#define		LEAVE_IDLE_MODE		PCON=(PCON&0xfe)
	#define		ENTER_STOP_MODE		PCON=(PCON&0xfd)|PD
	#define		LEAVE_STOP_MODE		PCON=(PCON&0xfd)
	
/*
******************************************************************************************************
*/
sfr 	TCON  		= 0x88;
	//--------------------------------------------------------------------------------------------
	sbit 		TF1 		= TCON^7;	//定时器T1溢出标志,可由程序查询和清零,TF1也是中断请求源,当CPU响应T1中断时由硬件清零
	sbit 		TR1 		= TCON^6;	//T1充许计数控制位,为1时充许T1计数
	//--------------------------------------------------------------------------------------------
	sbit 		TF0 		= TCON^5;	//定时器T0溢出标志,可由程序查询和清零,TF0也是中断请求源,当CPU响应T0中断时由硬件清零
	sbit 		TR0			= TCON^4;	//T0充许计数控制位,为1时充许T0计数
	//--------------------------------------------------------------------------------------------
	sbit 		IE1 		= TCON^3;	//外部中断1请示源(INT1,P3.3)标志;IE1也是中断请求源,当CPU响应该中断时由硬件清IE1(边沿触发方式)
	sbit 		IT1 		= TCON^2;	//外部中断源1触发方式控制位,IT1＝0,外部中断1程控为电平触发方式;当INT1（P3.3）输入低电平时,置位IE1
	//--------------------------------------------------------------------------------------------
	sbit 		IE0 		= TCON^1;	//外部中断0请示源(INT1,P3.2)标志;IE0也是中断请求源,当CPU响应该中断时由硬件清IE0(边沿触发方式)
	sbit 		IT0 		= TCON^0;	//外部中断源0触发方式控制位,IT0＝0,外部中断0程控为电平触发方式;当INT0（P3.2）输入低电平时,置位IE0
/*
******************************************************************************************************
*/
sfr 	TMOD  		= 0x89;				//不支持位操作,用读改写方式
	//--------------------------------------------------------------------------------------------
	#define 	T1_Mask		~0xf0		//T1工作模式选择Mask
	//--------------------------------------------------------------------------------------------
	#define 	GATE1		0x80		//定时器1操作开关控制位
					 	 				//当GATE1=1时,INT1引脚为高电平,同时TCON中的TR1控制位为1时,计时/计数器1才开始工作
					 	 				//当GATE1=0时,则只要将TR1控制位设为1，计时/计数器1就开始工作
	#define 	CT1			0x40		//定时/计数器1功能的选择位
					 	 				//CT1=1为计数器,通过外部引脚T1输入计数脉冲
					 	 				//CT1=0为定时器,由内部系统时钟提供计时工作脉冲
	//--------------------------------------------------------------------------------------------
	#define 	T1_Mode0		0x00		//T1为13位计时/计数器
	#define 	T1_Mode1		0x10		//T1为16位计时/计数器
	#define 	T1_Mode2		0x20		//T1为8位自动加载计数/计时器 
	#define 	T1_Mode3		0x30		//timer stop
	//--------------------------------------------------------------------------------------------
	#define 	T0_Mask			~0x0f		//T0工作模式选择Mask
	//--------------------------------------------------------------------------------------------
	#define 	GATE0			0x08		//定时器0操作开关控制位
					 	 				//当GATE0=1时,INT0引脚为高电平,同时TCON中的TR0控制位为1时,计时/计数器0才开始工作
					 	 				//当GATE0=0时,则只要将TR0控制位设为1，计时/计数器0就开始工作
	#define 	CT0				0x04		//定时/计数器0功能的选择位
					 	 				//CT0=1为计数器,通过外部引脚T0输入计数脉冲
					 	 				//CT0=0为定时器,由内部系统时钟提供计时工作脉冲
	//--------------------------------------------------------------------------------------------
	#define 	T0_Mode0			0x00		//T0为13位计时/计数器 
	#define 	T0_Mode1			0x01		//T0为16位计时/计数器 
	#define 	T0_Mode2			0x02		//T0为8位自动加载计数/计时器
	#define 	T0_Mode3			0x03		//仅适用于T0,T0分为两个独立的8位定时器/计数器TH0及TL0,在此模式下T1停止工作
/*
******************************************************************************************************
*/	
sfr 	TL0   		= 0x8A;				//Timer 0 data low byte
sfr 	TH0   		= 0x8C;				//Timer 0 data high byte
/*
******************************************************************************************************
*/
sfr 	TL1   		= 0x8B;				//Timer 1 data low byte
sfr 	TH1   		= 0x8D;			  //Timer 1 data high byte
/*
******************************************************************************************************
*/
sfr 	SCON2  		= 0x8E;			  //
	#define		SM				0x80
	#define 	UART2_9Bits		SCON2=(SCON2&0x7f)|SM
	#define 	UART2_8Bits		SCON2=(SCON2&0x7f)

	#define		REN2			0x10
	#define 	UART2_RECEPTION_ENABLE		SCON2=(SCON2&0xef)|REN2
	#define 	UART2_RECEPTION_DISABLE		SCON2=(SCON2&0xef)

	#define		TB82		0x08	//Uart2 transmit Bit 8
	#define		RB82		0x04	//Uart2 Receive Bit 8
	#define		TI2			0x02	//Uart2 Transmit interrupt flag
	#define		RI2			0x01	//Uart2  receive interrupt flag
/*
******************************************************************************************************
*/
sfr 	SBUF2  		= 0x8F;			//Uart2 transmit and receive data

/*
******************************************************************************************************
*/
sfr 	P1    		= 0x90;				//Port 1 data
	//--------------------------------------------------------------------------------------------
	sbit 		P1_7		= P1^7;
	sbit 		P1_6		= P1^6;	
	sbit 		P1_5		= P1^5;	
	sbit 		P1_4		= P1^4;	
	sbit 		P1_3		= P1^3;	
	sbit 		P1_2		= P1^2;	
	sbit 		P1_1		= P1^1;	
	sbit 		P1_0		= P1^0;	
/*
******************************************************************************************************
*/
sfr		P0OE		= 0x91;				//P0 CMOS Push-Pull Output Enable (1=Enable) 
/*
******************************************************************************************************
*/
sfr		P0LOE		= 0x92;				//P0 LCD 1/2 bais Output Enable (1=Enable) 
/*
******************************************************************************************************
*/
sfr		P2MOD		= 0x93;				//P2 CMOS Push-Pull Output Enable (1=Enable)
	//--------------------------------------------------------------------------------------------
	//P2MOD1:P2MOD Bit[3:2],P2.1 Pin Mode Control
	#define		P21MOD_Mask		~0x0c
	#define		P21MOD_0		0x00
	#define		P21MOD_1		0x04
	#define		P21MOD_2		0x08
	#define		P21MOD_3		0x0C
	//--------------------------------------------------------------------------------------------
	//P2MOD0:P2MOD Bit[1:0],P2.0 Pin Mode Control
	#define		P20MOD_Mask		~0x03
	#define		P20MOD_0		0x00
	#define		P20MOD_1		0x01
	#define		P20MOD_2		0x02
	#define		P20MOD_3		0x03
/*
******************************************************************************************************
*/
sfr 	OPTION    	= 0x94;
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	//UARTIW:OPTION Bit[7],P3.1单线UART,1=One wire UART, both TXD/RXD use P3.1 pin
	//--------------------------------------------------------------------------------------------
	#define 	UART1W		0x80		//P3.1单线UART

	//--------------------------------------------------------------------------------------------
	//WDT:OPTION Bit[5:4],WDT Prescale.  
	#define		WDTPSC_1	  0x00	  //Watch Dog Timer Prescale 3=Fastest   
	#define		WDTPSC_2		0x10		//Watch Dog Timer Prescale 3=Fastest   
	#define		WDTPSC_3 		0x20		//Watch Dog Timer Prescale 3=Fastest   
	#define		WDTPSC_4 		0x30		//Watch Dog Timer Prescale 3=Fastest   
	
	#define		WDTPSC_00		0x00	  	//Watch Dog Timer Prescale 0  
	#define		WDTPSC_01		0x10		//Watch Dog Timer Prescale 1   
	#define		WDTPSC_02 		0x20		//Watch Dog Timer Prescale 2   
	#define		WDTPSC_03		0x30		//Watch Dog Timer Prescale 3
		
	#define  	WDTPSC0    OPTION=OPTION&0xcf			//Watch Dog Timer Prescale 3=Fastest   
	#define  	WDTPSC1    OPTION=(OPTION&0xcf)|0x10	//Watch Dog Timer Prescale 3=Fastest   
	#define  	WDTPSC2    OPTION=(OPTION&0xcf)|0x20	//Watch Dog Timer Prescale 3=Fastest   
	#define  	WDTPSC3    OPTION=(OPTION&0xcf)|0x30	//Watch Dog Timer Prescale 3=Fastest   
	
	//--------------------------------------------------------------------------------------------
	//ADCKS:OPTION Bit[3:2],ADC Clock Rate 
	#define		ADCKS_Mask			b'00001100'		//ADC Clock Rate Mask
	#define		ADCKS_sys_32	  0x00	    //Interrupt rate is 32768 system clock cycle
	#define		ADCKS_sys_16		0x04		//Interrupt rate is 16384 system clock cycle
	#define		ADCKS_sys_8 		0x08		// Interrupt rate is 8192 system clock cycle
	#define		ADCKS_sys_4 		0x0c		//Interrupt rate is 128 system clock cycle

/*
******************************************************************************************************
*/
sfr 	INTFLG    	= 0x95;

	//--------------------------------------------------------------------------------------------
	#define		TKIF	0x20		//Touch Key 中断标志，HW clear while TKSOC set
	//--------------------------------------------------------------------------------------------
	#define		ADIF	0x10		//ADC 中断标志，HW clear while ADSOC set
	//--------------------------------------------------------------------------------------------
	#define		IE2		0x04		//INT2 中断标志，1=外部中断2中断
	//--------------------------------------------------------------------------------------------
	#define 	P1IF	0x02		//P1端口中断标志，1=P1端口中断
	//--------------------------------------------------------------------------------------------
	#define 	TF3		0x01		//TM3中断标志,1=TM3中断
/*
******************************************************************************************************
*/
sfr		P1WKUP		= 0x96;						//P1端口中断唤醒设置，Bit=1有效
	#define		P1WKUP_Mask			~0xff	   	//P1端口中断唤醒设置Mask
	#define		P1WKUP_AllEnable	0xff		//P1所有端口中断唤醒启用
	#define		P1WKUP_P10Enable	0x01		//P10端口中断唤醒启用
	#define		P1WKUP_P11Enable	0x02		//P11端口中断唤醒启用
	#define		P1WKUP_P12Enable	0x04		//P12端口中断唤醒启用
	#define		P1WKUP_P13Enable	0x08		//P13端口中断唤醒启用
	#define		P1WKUP_P14Enable	0x10		//P14端口中断唤醒启用
	#define		P1WKUP_P15Enable	0x20		//P15端口中断唤醒启用
	#define		P1WKUP_P16Enable	0x40		//P16端口中断唤醒启用
	#define		P1WKUP_P17Enable	0x80		//P17端口中断唤醒启用
/*
******************************************************************************************************
*/
sfr		SWCMD		= 0x97;			//Write 56h to this register will generate SW Reset 
									//Write 65h to this register to enable IAP all area, other value to disable IAP all

	#define		WDTO	0x02		//WatchDog Time-Out flag
	#define		IAPALL	0x01		//IAP Enable Control
/*
******************************************************************************************************
*/
sfr 	SCON  		= 0x98;
	//--------------------------------------------------------------------------------------------
	sbit 		SM0 		= SCON^7;	//串行口工作方式控制位 Bit[0]
	sbit 		SM1 		= SCON^6;	//串行口工作方式控制位 Bit[1]
	#define		Uart_ModeMask	~0xC0	//串口模式Mask
	#define		Uart_Mode0		0x00	//00 方式0－波特率由振荡器频率所定:振荡器频率/12
	#define		Uart_Mode1		0x40	//01 方式1－波特率由定时器T1或T2的溢出率和SMOD所定:2SMOD*(T1溢出率)/32
	#define		Uart_Mode2		0x80	//10 方式2－波特率由振荡器频率和SMOD所定:2SMOD*振荡器频率/64
	#define		Uart_Mode3		0xc0	//11 方式3－波特率由定时器T1或T2的溢出率和SMOD所定:2SMOD*(T1溢出率)/32
	
	//--------------------------------------------------------------------------------------------
	sbit 		SM2 		= SCON^5;	//多机通信控制位*/
										//多机通信是工作于方式2和方式3，SM2位主要用于方式2和方式3接收状态
					 	 				//当串行口工作于方式2或3:
					 	 				//当SM2=1时,只有当接收到第9位数据(RB8)为1时,才把接收到的前8位数据送入SBUF,且置位RI发出中断申请,否则会将接收到的数据放弃
					 	 				//当SM2=0时,就不管第9位数据是0还是1,都得数据送入SBUF,并发出中断申请,工作于方式0时，SM2必须为0
	//--------------------------------------------------------------------------------------------
	sbit 		REN 		= SCON^4;	//允许接收位:
					 	 				//REN用于控制数据接收的允许和禁止,REN=1时,允许接收,REN=0时,禁止接收
	//--------------------------------------------------------------------------------------------
	sbit 		TB8 		= SCON^3;	//发送数据第8位
	sbit 		RB8 		= SCON^2;	//接收数据第8位
	//--------------------------------------------------------------------------------------------
	sbit 		TI 			= SCON^1;	//发送中断标志位：
					 	 				//方式0时,发送完第8位数据后,由硬件置位
					 	 				//其它方式下,由硬件置位,TI=1表示帧发送结束,TI可由软件清零
	//--------------------------------------------------------------------------------------------
	sbit 		RI 			= SCON^0;	//接收中断标志位：
					 	 				//方式0时,接收完第8位数据后,由硬件置位
					 	 				//其它方式下,由硬件置位,RI=1表示帧接收完成
/*
******************************************************************************************************
*/
sfr 	SBUF  		= 0x99;				//串口缓存寄存器,UART transmit and receive data
/*
******************************************************************************************************
*/
sfr		PWM0PRD		= 0x9A;	//PWM0 Period
/*
******************************************************************************************************
*/
sfr		PWM0DH		= 0x9B;	//PWM0 Duty 9~2
/*
******************************************************************************************************
*/
sfr		PWM1PRD		= 0x9C;	//PWM1 Period
/*
******************************************************************************************************
*/
sfr		PWM1DH		= 0x9D;	//PWM1 Duty 9~2
//****************************************************************************************************
sfr		PWM2PRD		= 0x9E;	//PWM2 Period
//****************************************************************************************************
sfr		PWM2DH		= 0x9F;	//PWM2 Duty 9~2
/*
******************************************************************************************************
*/
sfr 	P2    		= 0xA0;			//Port 2 data
	//--------------------------------------------------------------------------------------------
	sbit 		P2_5		= P2^5;	
	sbit 		P2_4		= P2^4;	
	sbit 		P2_3		= P2^3;	
	sbit 		P2_2		= P2^2;	
	sbit 		P2_1		= P2^1;	
	sbit 		P2_0		= P2^0;							
/*
******************************************************************************************************
*/
sfr		PWMCON		= 0xA1;
	#define		PWM1CKS_Mask		~0xc0
	#define		PWM1CKS_1   		 0x00// PWM1 Clock source sysclk div4
	#define		PWM1CKS_2   		 0x40// PWM1 Clock source sysclk div2
	#define		PWM1CKS_3   		 0x80// PWM1 Clock source sysclk div1
	#define		PWM1CKS_4   		 0xc0// PWM1 IRC16M
	#define		PWM1DL_Mask		  ~0x30
	#define		PWM0CKS_Mask		~0x0C
	#define		PWM0CKS_1   		 0x00// PWM0 Clock source sysclk div4
	#define		PWM0CKS_2   		 0x04// PWM0 Clock source sysclk div2
	#define		PWM0CKS_3   		 0x08// PWM0 Clock source sysclk div1
	#define		PWM0CKS_4   		 0x0C// PWM0 IRC16M
	#define		PWM0DL_Mask		  ~0x03
/*
******************************************************************************************************
*/
sfr		P1MODL		= 0xA2;
	//--------------------------------------------------------------------------------------------
	//P1MOD3:P1MODL Bit[7:6],P1.3 Pin Mode Control
	#define		P13MOD_Mask		~0xc0
	#define		P13MOD_0		0x00
	#define		P13MOD_1		0x40
	#define		P13MOD_2		0x80
	#define		P13MOD_3		0xC0
	//--------------------------------------------------------------------------------------------
	//P1MOD2:P1MODL Bit[5:4],P1.2 Pin Mode Control
	#define		P12MOD_Mask		~0x30
	#define		P12MOD_0		0x00
	#define		P12MOD_1		0x10
	#define		P12MOD_2		0x20
	#define		P12MOD_3		0x30
	//--------------------------------------------------------------------------------------------
	//P1MOD1:P1MODL Bit[3:2],P1.1 Pin Mode Control
	#define		P11MOD_Mask		~0x0C
	#define		P11MOD_0		0x00
	#define		P11MOD_1		0x04
	#define		P11MOD_2		0x08
	#define		P11MOD_3		0x0C
	//--------------------------------------------------------------------------------------------
	//P1MOD0:P1MODL Bit[1:0],P1.0 Pin Mode Control
	#define		P10MOD_Mask		~0x03
	#define		P10MOD_0		0x00
	#define		P10MOD_1		0x01
	#define		P10MOD_2		0x02
	#define		P10MOD_3		0x03
/*
******************************************************************************************************
*/
sfr		P1MODH		= 0xA3;
	//--------------------------------------------------------------------------------------------
	//P1MOD7:P1MODH Bit[7:6],P1.7 Pin Mode Control
	#define		P17MOD_Mask		~0xc0
	#define		P17MOD_0		0x00
	#define		P17MOD_1		0x40
	#define		P17MOD_2		0x80
	#define		P17MOD_3		0xc0
	//--------------------------------------------------------------------------------------------
	//P1MOD6:P1MODH Bit[5:4],P1.6 Pin Mode Control
	#define		P16MOD_Mask		~0x30
	#define		P16MOD_0		0x00
	#define		P16MOD_1		0x10
	#define		P16MOD_2		0x20
	#define		P16MOD_3		0x30
	//--------------------------------------------------------------------------------------------
	//P1MOD5:P1MODH Bit[3:2],P1.5 Pin Mode Control
	#define		P15MOD_Mask		~0x0c
	#define		P15MOD_0		0x00
	#define		P15MOD_1		0x04
	#define		P15MOD_2		0x08
	#define		P15MOD_3		0x0c
	//--------------------------------------------------------------------------------------------
	//P1MOD4:P1MODH Bit[1:0],P1.4 Pin Mode Control
	#define		P14MOD_Mask		~0x03
	#define		P14MOD_0		0x00
	#define		P14MOD_1		0x01
	#define		P14MOD_2		0x02
	#define		P14MOD_3		0x03
/*
******************************************************************************************************
*/
sfr		P3MODL		= 0xA4;
	//--------------------------------------------------------------------------------------------
	//P3MOD3:P3MODL Bit[7:6],P3.3 Pin Mode Control
	#define		P33MOD_Mask		~0xc0
	#define		P33MOD_0		0x00
	#define		P33MOD_1		0x40
	#define		P33MOD_2		0x80
	#define		P33MOD_3		0xc0
	//--------------------------------------------------------------------------------------------
	//P3MOD2:P3MODL Bit[5:4],P3.2 Pin Mode Control
	#define		P32MOD_Mask		~0x30
	#define		P32MOD_0		0x00
	#define		P32MOD_1		0x10
	#define		P32MOD_2		0x20
	#define		P32MOD_3		0x30
	//--------------------------------------------------------------------------------------------
	//P3MOD1:P3MODL Bit[3:2],P3.1 Pin Mode Control
	#define		P31MOD_Mask		~0x0c
	#define		P31MOD_0		0x00
	#define		P31MOD_1		0x04
	#define		P31MOD_2		0x08
	#define		P31MOD_3		0x0c
	//--------------------------------------------------------------------------------------------
	//P3MOD0:P3MODL Bit[1:0],P3.0 Pin Mode Control
	#define		P30MOD_Mask		~0x03
	#define		P30MOD_0		0x00
	#define		P30MOD_1		0x01
	#define		P30MOD_2		0x02
	#define		P30MOD_3		0x03
/*
******************************************************************************************************
*/
sfr		P3MODH		= 0xA5;
	//--------------------------------------------------------------------------------------------
	//P3MOD7:P3MODH Bit[7:6],P3.7 Pin Mode Control
	#define		P37MOD_Mask		~0xc0
	#define		P37MOD_0		0x00
	#define		P37MOD_1		0x40
	#define		P37MOD_2		0x80
	#define		P37MOD_3		0xc0
	//--------------------------------------------------------------------------------------------
	//P3MOD6:P3MODH Bit[5:4],P3.6 Pin Mode Control
	#define		P36MOD_Mask		~0x30
	#define		P36MOD_0		0x00
	#define		P36MOD_1		0x10
	#define		P36MOD_2		0x20
	#define		P36MOD_3		0x30
	//--------------------------------------------------------------------------------------------
	//P3MOD5:P3MODH Bit[3:2],P3.5 Pin Mode Control
	#define		P35MOD_Mask		~0x0c
	#define		P35MOD_0		0x00
	#define		P35MOD_1		0x04
	#define		P35MOD_2		0x08
	#define		P35MOD_3		0x0c
	//--------------------------------------------------------------------------------------------
	//P3MOD4:P3MODH Bit[1:0],P3.4 Pin Mode Control
	#define		P34MOD_Mask		~0x03
	#define		P34MOD_0		0x00
	#define		P34MOD_1		0x01
	#define		P34MOD_2		0x02
	#define		P34MOD_3		0x03
/*
******************************************************************************************************
*/
sfr 	PINMOD  		= 0xA6;
	#define		PWM1OE		0x80//Set to enable PWM1A and signal output to P1.3 pin
	#define		PWM0OE		0x40//Set to enable PWM0A and signal output to P1.2 pin
	#define		TCOE  		0x20//Set to output instruction cycle clock signal to P1.4 pin
	#define		T2OE		0x10//Set to output "Timer2 overflow divided by 2" signal to P1.0 pin
	#define		T0OE    	0x01//"Timer0 overflow divided by 64" signal output to P3.4 pin
//--------------------------------------------------------------------------------------------------------

	#define		PWM1OE_ENABLE 	PINMOD=(PINMOD&0x7f)|PWM1OE
	#define		PWM1OE_DISABLE	PINMOD=(PINMOD&0x7f)
	#define		PWM0OE_ENABLE	PINMOD=(PINMOD&0xbf)|PWM0OE
	#define		PWM0OE_DISABLE	PINMOD=(PINMOD&0xbf)
	#define		TCOE_ENABLE		PINMOD=(PINMOD&0xdf)|TCOE
	#define		TCOE_DISABLE	PINMOD=(PINMOD&0xdf)
	#define		T2OE_ENABLE		PINMOD=(PINMOD&0xef)|T2OE
	#define		T2OE_DISABLE	PINMOD=(PINMOD&0xef)
	#define		T0OE_ENABLE		PINMOD=(PINMOD&0xfe)|T0OE
	#define		T0OE_DISABLE	PINMOD=(PINMOD&0xfe)
//------------------------------------------------------------------------------------------------------------
sfr		PWMCON2			=0xA7;
	#define		PWM2OE			0x10
	#define		PWM2CKS_DIV4	0x00
	#define		PWM2CKS_DIV2	0x04
	#define		PWM2CKS_SYSCLK	0x08
	#define		PWM2CKS_FRC		0x0C
	#define		PWM2DL_Mask		~0x03
	
	
	#define 	PWM2OE_ENABLE	PWMCON2=(PWMCON2&0xef)|PWM2OE
	#define 	PWM2OE_DISABLE	PWMCON2=(PWMCON2&0xef) 
	#define		PWM2CLK_DIV4	PWMCON2=(PWMCON2&0xf3)|PWM2CKS_DIV4
	#define		PWM2CLK_DIV2	PWMCON2=(PWMCON2&0xf3)|PWM2CKS_DIV2
	#define		PWM2CLK_SYSCLK	PWMCON2=(PWMCON2&0xf3)|PWM2CKS_SYSCLK
	#define		PWM2CLK_FRC		PWMCON2=(PWMCON2&0xf3)|PWM2CKS_FRC
/*
******************************************************************************************************
*/
sfr 	IE    		= 0xA8;
	//--------------------------------------------------------------------------------------------
	sbit 		EA 			= IE^7;		//EA=0时,所有中断禁止;EA=1时,各中断的产生由个别的允许位决定
	sbit		ET2			= IE^5;		//定时2中断允许
	sbit 		ES 			= IE^4;		//串行口中断充许(ES=1充许,ES=0禁止)
	sbit 		ET1 		= IE^3;		//定时1中断充许
	sbit 		EX1 		= IE^2;		//外中断INT1中断充许
	sbit 		ET0 		= IE^1;		//定时器0中断充许
	sbit 		EX0 		= IE^0;		//外部中断INT0的中断允许
/*
******************************************************************************************************
*/
sfr		INTE1		= 0xA9;
	//--------------------------------------------------------------------------------------------
	#define		ES2			0x20		//Uart2 Interrupt Enable 中断允许
	//--------------------------------------------------------------------------------------------
	#define		SPIE		0x10		//SPI 中断允许
	//--------------------------------------------------------------------------------------------
	#define		ADTKIE 		0x08		//ADC/Touch Key  中断允许
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	#define		EX2			0x04		//INT2 中断允许、Stop Mode 唤醒功能启用
	//--------------------------------------------------------------------------------------------
	#define	 	P1IE		0x02		//P1端口中断允许
	//--------------------------------------------------------------------------------------------
	#define		TM3IE		0x01		//TM3中断允许		
/*
******************************************************************************************************
*/
sfr		ADTKDT		= 0xAA;
	//--------------------------------------------------------------------------------------------
	//ADCDL:ADTKDT Bit[7:4],ADC Data LSB 3~0
	//--------------------------------------------------------------------------------------------
	//TKDH:ADTKDT Bit[3:0],Touch Key Counter Data 11~8
/*
******************************************************************************************************
*/
sfr		ADCDH		= 0xAB;//ADC Data 11~4
/*
******************************************************************************************************
*/
sfr		TKDL		= 0xAC;//Touch Key Counter Data 7~0
/*
******************************************************************************************************
*/
sfr		TKFREQ		= 0xAD;//Bit[6:0],TK oscillation cap. adjustment 
/*
******************************************************************************************************
*/
sfr		TKCON		= 0xAE;
	//--------------------------------------------------------------------------------------------
	#define		TKPD		0x80		//Touch Key Power Down
	#define		TKEOC		0x40		//Touch Key End of Conversion

	//--------------------------------------------------------------------------------------------
	//TKCHS:TKCON Bit[3:0],
	//"TK Channel Select
	//0000: TK0 (P3.3)
	//0001: TK1 (P3.2)
	//0010: TK2 (P3.1)
	//0011: TK3 (P3.0)
	//0100: TK4 (P1.0)
	//0101: TK5 (P1.1)
	//0110: TK6 (P1.2)
	//0111: TK7 (P1.3)
	//1000: TK8 (P1.4)
	//1001: TK9 (P1.6)
	//1010: TK10 (P0.4)
	//1011: TK11 (P0.5)
	//1100: TK12 (P0.6)
	//1101: TK13 (P3.4)
	//1110: TK14 (P1.5)
	//1111: Ref Key"

/*
******************************************************************************************************
*/
sfr		P0ADIE		= 0xAF;
	//--------------------------------------------------------------------------------------------
	//P0ADIE:P0ADIE Bit[7:4],AD10. AD14~12 (P0.7~P0.4 ) channel input  Enable (1=Enable, 0=I/O digital input)
	
/*
******************************************************************************************************
*/
sfr 	P3    		= 0xB0;				//Port 3 data
	//--------------------------------------------------------------------------------------------
	sbit 		P3_7		= P3^7;
	sbit 		P3_6		= P3^6;
	sbit 		P3_5		= P3^5;
	sbit 		P3_4		= P3^4;
	sbit 		P3_3		= P3^3;
	sbit 		P3_2		= P3^2;
	sbit 		P3_1		= P3^1;
	sbit 		P3_0		= P3^0;
/*
******************************************************************************************************
*/
sfr		LEDCON		= 0xB1;
	
	#define	LEDEN_DIV8		0x40
	#define	LEDEN_DIV9		0x80
	#define	LEDEN_DIV10		0xC0
	
	#define LEDEN_DUTY_DIV8		LEDCON=(LEDCON&0x3f)|LEDEN_DIV8
	#define LEDEN_DUTY_DIV9		LEDCON=(LEDCON&0x3f)|LEDEN_DIV9
	#define LEDEN_DUTY_DIV10	LEDCON=(LEDCON&0x3f)|LEDEN_DIV10
	#define LEDEN_DISABLE		LEDCON=(LEDCON&0x3f)
	
	//LEDPSC:LEDCON Bit[5:4],
	//LED Clock divided by, 0: 64, 1: 32, 2: 16  3: 8  ( LEDCLK  > 100K )
	#define	LEDPSC_64	0x00
	#define	LEDPSC_32	0x10
	#define	LEDPSC_16	0x20
	#define	LEDPSC_8	0x30

	//LED hold , 1:hold LED scan, all LED pin state are Hi-Z, 0: release to run LED scan
	#define LEDHOLD		0x08
	#define	LEDHOLD_HOLD	LEDCON=(LEDCON&0xf7)|LEDHOLD
	#define	LEDHOLD_RELEASE	LEDCON=(LEDCON&0xf7)

	//LEDBRIT:LEDCON Bit[2:0]
	//LED SEG0-~3- & COM0-~3- Brightness. 000: Most darkness, 111: Most brightness
	#define	LCDBRIT_0	0x00	// most darkness
	#define	LCDBRIT_1	0x01	// 
	#define	LCDBRIT_2	0x02	// 
	#define	LCDBRIT_3	0x03	// 
	#define	LCDBRIT_4	0x04	// 
	#define	LCDBRIT_5	0x05	// 
	#define	LCDBRIT_6	0x06	// 
	#define	LCDBRIT_7	0x07	// Most brightness
/*
******************************************************************************************************
*/
sfr		LEDCON2		= 0xB2;

	//"LED Brightness Smooth control
	//0: Brightness Smooth Enable
	//1: Brightness Smooth Disable"
	#define LEDSMDIS 0x80
	#define LEDSMDIS_ENABLE LEDCON2=(LEDCON2&0x7f)				//1: Brightness Smooth Disable		
	#define LEDSMDIS_DISABLE LEDCON2=(LEDCON2&0x7f)|LEDSMDIS	//0: Brightness Smooth Enable

	//LEDBRIT2:LEDCON2 Bit[6:4],LED SEG5- Brightness. 000: Most darkness, 111: Most brightness
	#define	LCDBRIT2_0	0x00	// most darkness
	#define	LCDBRIT2_1	0x10	// 
	#define	LCDBRIT2_2	0x20	// 
	#define	LCDBRIT2_3	0x30	// 
	#define	LCDBRIT2_4	0x40	// 
	#define	LCDBRIT2_5	0x50	// 
	#define	LCDBRIT2_6	0x60	// 
	#define	LCDBRIT2_7	0x70	// Most brightness
	
	//LEDBRIT1:LEDCON2 Bit[2:0],LED SEG4- Brightness. 000: Most darkness, 111: Most brightness
	#define	LCDBRIT1_0	0x00	// most darkness
	#define	LCDBRIT1_1	0x01	// 
	#define	LCDBRIT1_2	0x02	// 
	#define	LCDBRIT1_3	0x03	// 
	#define	LCDBRIT1_4	0x04	// 
	#define	LCDBRIT1_5	0x05	// 
	#define	LCDBRIT1_6	0x06	// 
	#define	LCDBRIT1_7	0x07	// Most brightness	
/*
******************************************************************************************************
*/
sfr		TKTMRL		= 0xB4;	//Bit[7:0],STK Module0 Scan length 7~0 adjustment, 000=shortest, FFF=longest

/*
******************************************************************************************************
*/
sfr		TKCON2		= 0xB5;	
	#define	TKFJMP	0x80	//TK Clock frequency auto-change selection, 0=fix frequency, 1=auto-change

	//JMPVAL:TKCON2,Bit[6:4],
	//only available in TKFJMP=0
	//TK Clock frequency fine tune , 0=frequency slowest, 7=frequency  fastest"
	
	//TKTMRH:TKCON2,Bit[3:0]
	//TK Module0 Scan length 11~8 adjustment

/*
******************************************************************************************************
*/
sfr		ADCHS		= 0xB6;
	//ADCHS:ADCHS,Bit[3:0]
	//"ADC Channel Select
	//0000: AD0 (P3.3)
	//0001: AD1 (P3.2)
	//0010: AD2 (P3.1)
	//0011: AD3 (P3.0)
	//0100: AD4 (P1.0)
	//0101: AD5 (P1.1)
	//0110: AD6 (P1.2)
	//0111: AD7 (P1.3)
	//1000: AD8 (P1.4)
	//1001: AD9 (P1.5)
	//1010: AD10 (P0.7)
	//1011: VBG (Internal Bandgap Reference Voltage)
	//1100: AD12 (P0.4)
	//1101: AD13 (P0.5)
	//1110: AD14 (P0.6)
	//1111: 1/4VCC"

/*
******************************************************************************************************
*/
sfr 	IP    		= 0xB8;
	//--------------------------------------------------------------------------------------------
	sbit		PT2			  = IP^5;		//定时2中断优先级低位
	sbit 		PS			  = IP^4;		//串行口中断优先级低位
	sbit 		PT1	    	= IP^3;		//定时1中断优先级低位
	sbit 		PX1	    	= IP^2;		//外中断INT1中断优先级低位
	sbit 		PT0	    	= IP^1;		//定时0中断优先级低位
	sbit 		PX0	    	= IP^0;		//外中断INT0中断优先级低位
/*
******************************************************************************************************
*/
sfr 	IPH    		= 0xB9;
	//--------------------------------------------------------------------------------------------
//	sbit		PT2H		  = IP^5;		//定时2中断优先级高位
//	sbit 		PSH			  = IP^4;		//串行口中断优先级高位
//	sbit 		PT1H	    = IP^3;		//定时1中断优先级高位
//	sbit 		PX1H	    = IP^2;		//外中断INT1中断优先级高位
//	sbit 		PT0H	    = IP^1;		//定时0中断优先级高位
//	sbit 		PX0H	    = IP^0;		//外中断INT0中断优先级高位/*
	#define		IPH_Mask	  ~0x3F		//
	#define		IPH_PT2H    0x20		//定时2中断优先级高位       
	#define		IPH_PSH     0x10 	  //串行口中断优先级高位      
	#define		IPH_PT1H    0x08	  //定时1中断优先级高位       
	#define		IPH_PX1H    0x04		//外中断INT1中断优先级高位  
	#define		IPH_PT0H    0x02		//定时0中断优先级高位       
	#define		IPH_PX0H    0x01 	  //外中断INT0中断优先级高位/*
/*
******************************************************************************************************
*/	
sfr 	IP1  		= 0xBA;
	//--------------------------------------------------------------------------------------------
	#define		PS2			0x20		//UART2中断优先级低位
	#define		PSPI		0x10		//SPI中断优先级低位
	#define 	PADTKI		0x08;		//ADC/Touch Key 中断优先级低位
	#define		PX2			0x04		//XIN T2中断优先级低位
	#define 	PP1			0x02;		//P1中断优先级低位
	#define		PT3			0x01;		//定时3中断优先级低位
/*
******************************************************************************************************
*/
sfr 	IP1H   		= 0xBB;
	//--------------------------------------------------------------------------------------------
	#define		PS2H 		0x20		//UART2中断优先级高位
	#define		PSPIH 		0x10		//SPI中断优先级高位
	#define 	PADTKIH		0x08;		//ADC/Touch Key 中断优先级高位
	#define		PX2H		0x04		//XIN T2中断优先级高位
	#define 	PP1H		0x02;		//P1中断优先级高位
	#define 	PT3H    	0x01;		//定时3中断优先级高位
/*
******************************************************************************************************
*/
sfr   SPCON     = 0xBC;
	#define		SPCON_Mask	~0xFF		//
	#define		SPEN    0x80		//SPI Enable.0: SPI Enable;1: SPI Disable
	#define		MSTR    0x40 	  //Master Mode Enable.0: Slave Mode;1: Master Mode
	#define		CPOL    0x20	  //SPI Clock Polarity.0: SCK is low in idle state;1: SCK is high in idle state
	#define		CPHA    0x10		//SPI Clock Phase.0: Data sampled on first edge of SCK period;1: Data sampled on second edge of SCK period
	#define		SSDIS   0x08		//Pin Disable.0: Enable   pin;1: Disable   pin
	#define		LSBF    0x04 	  //LSB First.0: MSB first;1: LSB first
	//--------------------------------------------------------------------------------------------
	//SPCR:SPCON Bit[1:0], SPI Clock Rate.
	#define		SPCR_2  0x00	  //FSYSCLK/2
	#define		SPCR_4  0x01	  //FSYSCLK/4
	#define		SPCR_8  0x02	  //FSYSCLK/8
	#define		SPCR_16 0x03	  //FSYSCLK/16
/*
******************************************************************************************************
*/
sfr   SPSTA     = 0xBD;
	#define		SPSTA_Mask	~0xFC		//
	#define		SPIF    0x80		//SPI Interrupt Flag,This bit is set by H/W at the end of a data transfer,Cleared by H/W when interrupt is vectored into
	#define		WCOL    0x40 	  //Write Collision Interrupt Flag,This bit is set by H/W if write data to SPDAT when SPBSY is set.Write 0 to this bit or rewrite data to SPDAT when SPBYS is cleared will clear this flag.
	#define		MODF    0x20	  //Mode Fault Interrupt Flag,This bit is set by H/W when SSDIS is cleared and NSS pin is pulled low in Master mode.Write 0 to this bit will clear this flag.When this bit is set, the SPEN and MSTR in SPCON will be cleared by hardware
	#define		RCVOVF  0x10		//Receive Buffer Overrun Flag,This bit is set by H/W at the end of a data transfer and RCVBF is set.Write 0 to this bit or read SPDAT register will clear this flag.
	#define		RCVBF   0x08		//Receive Buffer Full Flag,This bit is set by H/W at the end of a data transfer.Write 0 to this bit or read SPDAT register will clear this flag.
	#define		SPBSY   0x04 	  //SPI Busy Flag (Read Only),This bit is set by H/W when a SPI transfer is progress.
/*
******************************************************************************************************
*/
sfr   SPDAT     = 0xBE;	//Bit[7:0],SPI Transmit and Receive Data
/*
******************************************************************************************************
*/

/*
******************************************************************************************************
*/

sfr 	T2CON  		= 0xC8;
	//--------------------------------------------------------------------------------------------
	sbit 		TF2  		  = T2CON^7;		//定时器T2溢出中断标志。TF2必须由用户程序清“0”。当T2作为串口波特率发生器时，TF2不会被置“1”。
	sbit 		EXF2    	= T2CON^6;		//外部中断2标志。EXEN2为1时，当T2EX（P1.1）发生负跳变时置1中断标志DXF2，EXF2必须由用户程序清“0”。
	sbit 		RCLK    	= T2CON^5;		//串行接口的接收时钟选择标志。TCLK=1时，T2工作于mode 1 or 3。
	sbit 		TCLK 		  = T2CON^4;		//串行接口的发送时钟选择标志位。RCLK＝1时，T2工作于mode 1 or 3。
	sbit 		EXEN2   	= T2CON^3;		//外部中断2充许标志。
	sbit 		TR2     	= T2CON^2;		//外部计数器/定时器选择位。C/T2=1时，T2为外部事件计数器，计数脉冲来自T2（P1.0）；C/T2=0时，T2为定时器，振荡脉冲的十二分频信号作为计数信号。
	sbit 		CT2     	= T2CON^1;		//T2计数/定时控制位。TR1为1时充许计数，为0时禁止计数。
	sbit 		CPRL2   	= T2CON^0;		//捕捉和常数自动再装入方式选择位。为1时工作于捕捉方式，为0时T2工作于常数自动再装入方式。当TCLK或RCLK为1时，CP/RL2被忽略，T2总是工作于常数自动再装入方式。
/*
******************************************************************************************************
*/
sfr		IAPWE_SFR	= 0xC9;	//Write 47h to enable main ROM IAP write, Write E2h to enable EEPROM  write, other value to disable IAP write
							
	#define	IAPWE	0x80	//Write: Write 47h to enable main ROM IAP write, Write E2h to enable EEPROM  write, 
							//		 other value to disable IAP write"
							//Read : IAPWE Enable Flag
							
	#define	IAPTO	0x40	//IAP Time-Out flag,? H/W auto clear when IAPWE =0
	#define	EEPWE	0x20	//EEPWE Enable Flag

/*
******************************************************************************************************
*/

sfr 	RCP2L  	 = 0xCA;				//外部输入（P1.1）计数器/自动再装入模式时初值寄存器低八位
sfr 	RCP2H  	 = 0xCB;				//外部输入（P1.1）计数器/自动再装入模式时初值寄存器高八位
/*
******************************************************************************************************
*/
sfr 	TL2   		= 0xCC;			  	//Timer2 data low byte
sfr 	TH2   		= 0xCD;			    //Timer2 data high byte
/*
******************************************************************************************************
*/
sfr 	PSW   		= 0xD0;
	//--------------------------------------------------------------------------------------------
	sbit 		CY 		= PSW^7;		//进位标志,有进或借位,CY＝1;无进或借位,CY＝0
	sbit 		AC 		= PSW^6;		//半进位标志
	sbit 		F0 		= PSW^5;		//用户可以随便使用
	sbit 		RS1 	= PSW^4;		//工作寄存器区选择[RS1:RS0]
	sbit 		RS0 	= PSW^3;		//00--0区,01--1区,10--2区,11--3区
	sbit 		OV 		= PSW^2;		//运算结果按补码运算,有溢出,OV=1;无溢出,OV＝0
	sbit 		F1 		= PSW^1;		//用户自定义标志
	sbit 		P 		= PSW^0;		//用来表示ALU运算结果中二进制数位1的个数的奇偶性,若为奇数,则P=1,否则为0
	
/*
******************************************************************************************************
*/
sfr		P1LOE		= 0xD1;
	//Bit[7:0],P1 LCD 1/2 bais Output Enable (1=Enable) 

/*
******************************************************************************************************
*/
sfr		P2LOE		= 0xD2;
	//P2LOE:P2LOE,Bit[1:0],P2 LCD 1/2 bais Output Enable (1=Enable) 

/*
******************************************************************************************************
*/
sfr		P3LOE		= 0xD3;
	//P3LOE:P3LOE,Bit[1:0],P3 LCD 1/2 bais Output Enable (1=Enable) 

/*
******************************************************************************************************
*/
sfr		CLKCON		= 0xD8;
	sbit 		SCKTYPE = CLKCON^7;		//慢时钟类型选择，Slow Clock Type. 1=SXT, 0=SRC
	sbit 		FCKTYPE = CLKCON^6;		//Fast clock Type. 0=FRC, 1=FXT.
	sbit 		STPSCK 	= CLKCON^5;		//Stop Slow Clock in PDOWN
	sbit 		STPPCK 	= CLKCON^4;		//1=进入IDL模式，Stop UART/Timer0/Timer1/Timer2 Clock in Idle mode
	sbit 		STPFCK 	= CLKCON^3;		//1=停止快时钟，0=快时钟工作
	sbit 		SELFCK 	= CLKCON^2;		//1=选择快时钟为系统时钟源，0=慢时钟为系统时钟源
	sbit 		CLKPSC_H 	= CLKCON^1;	
	sbit 		CLKPSC_L	= CLKCON^0;	
	//********************************************************************************************************
	#define  	SCKTYPE_SRC    	CLKCON=(CLKCON&0x7f)
	#define  	SCKTYPE_SXT    	CLKCON=(CLKCON&0x7f)|0x80
	#define		FCKTYPE_FRC		CLKCON=(CLKCON&0x9f)
	#define		FCKTYPE_FXT		CLKCON=(CLKCON&0x9f)|0x40
	#define		STPPCK_OPEN		CLKCON=(CLKCON&0xef)|0x10
	#define		STPPCK_OFF		CLKCON=(CLKCON&0xef)
	#define		STPFCK_OPEN		CLKCON=(CLKCON&0xf7)|0x08
	#define		STPFCK_OFF		CLKCON=(CLKCON&0xf7)
	#define		SELFCK_SLOW		CLKCON=(CLKCON&0xfb)
	#define		SELFCK_FAST		CLKCON=(CLKCON&0xfb)|0x04
	//--------------------------------------------------------------------------------------------
	//CLKPSC:CLKCON Bit[1:0],系统时钟预分频，SYSCLK Prescaler, 0:div16, 1:div4, 2:div2, 3:div1
	#define		CLKPSC_Mask							~0x03
	#define		CLKPSC_Div16		0x00
	#define		CLKPSC_Div4			0x01
	#define		CLKPSC_Div2			0x02
	#define		CLKPSC_Div1			0x03							
	#define		SYSCLOCK_DIV_16		CLKCON=(CLKCON&0xfc)
	#define		SYSCLOCK_DIV_4		CLKCON=(CLKCON&0xfc)|CLKPSC_Div4
	#define		SYSCLOCK_DIV_2		CLKCON=(CLKCON&0xfc)|CLKPSC_Div2
	#define		SYSCLOCK_DIV_1		CLKCON=(CLKCON&0xfc)|CLKPSC_Div1	
/*
******************************************************************************************************
*/
sfr 	ACC   		= 0xE0;		//Accumulator

/*
******************************************************************************************************
*/
sfr 	PCL   		= 0xED;		//Will not display on datasheet, TEST ONLY
	//PCL:PCL,Bit[7:0],PC counter LSB 7~0

sfr 	PCH   		= 0xEE;		//Will not display on datasheet, TEST ONLY
	//PCH:PCH,Bit[7:0],PC counter MSB 7~0
/*
******************************************************************************************************
*/
sfr 	AUX3   		= 0xEF;		

	//TM3PSC:AUX3,Bit[5:3]
	//Timer3 Int Prescale. 
	//000=32768*Clk, 001=16384*Clk, 010=8192*Clk, 011=4096*Clk
	//100=2048*Clk, 101=1024*Clk, 110=512*Clk, 111=256*Clk

	#define	VBGEN	0x04
	#define VBGEN_ENABLE	AUX3=(AUX3&0xfb)|VBGEN
	#define VBGEN_DISABLE	AUX3=(AUX3&0xfb)

	#define	ADVREFS_00	0x00	//VCC
	#define	ADVREFS_01	0x01	//2.5V
	#define	ADVREFS_02	0x02	//3V
	#define	ADVREFS_03	0x03	//4V
	#define	SET_ADVREFS_00	AUX3=(AUX3&0xfc)
	#define	SET_ADVREFS_01	AUX3=(AUX3&0xfc)|ADVREFS_01
	#define	SET_ADVREFS_02	AUX3=(AUX3&0xfc)|ADVREFS_02
	#define	SET_ADVREFS_03	AUX3=(AUX3&0xfc)|ADVREFS_03
//******************************************************************************************************
sfr 	B   		= 0xF0;		//B register
//******************************************************************************************************
sfr 	CRCDL   		= 0xF1;
	//--------------------------------------------------------------------------------------------
	//CRCDL:CRCDL Bit[7:0],CRC16 Data 7~0
//******************************************************************************************************
sfr 	CRCDH   		= 0xF2;
	//--------------------------------------------------------------------------------------------
	//CRCDH:CRCDH Bit[7:0],CRC16 Data 15~8
//******************************************************************************************************
sfr 	CRCIN   		= 0xF3;
	//--------------------------------------------------------------------------------------------
	//CRCIN:CRCIN Bit[7:0],CRC16 input
	
/*
******************************************************************************************************
*/	
sfr 	TESTMODE   		= 0xF4;	//Will not display on datasheet, TEST ONLY
	#define	TSTREG_4	0x10	
	//0:  LEDDRV is controled by LEDEN
	//1:  force LEDDRV=1"
	
	#define	TSTREG_3	0x08
	//0: EEPROM Read, wait 5 clock
	//1: disable EEPROM read delay (for test EEPROM access time)"

	#define	TSTREG_2	0x04
	//0: POR always enable
	//1:  disable POR, force PORPD=1

	#define	TSTREG_1	0x02
	//0:Disable RCK test mode
	//1: RCK output to P17 "

	#define	TSTREG_0	0x01
	//0: RCK select div 64
	//1: RCK select div 1 &  TCK output to P16"

/*
******************************************************************************************************
*/	
sfr 	CFGBG   		= 0xF5;
	//--------------------------------------------------------------------------------------------
	//BGTRIM:CFGBG Bit[3:0],VBG trimming value
sfr 	CFGWL   		= 0xF6;
	//--------------------------------------------------------------------------------------------
	//FRCF:CFGWL Bit[6:0],FRC freq
	
/*
******************************************************************************************************
*/	
sfr 	AUX2		= 0xF7;
	//--------------------------------------------------------------------------------------------
	//WDTE:AUX2 Bit[7:6],WDT Control
	//#define 	WDT_EN		         	0xC0		
	//#define 	WDT_ENFS_DISILDE		0x80		
	//#define 	WDT_DIS         		0x00		
	
	#define  	WDT_IS_OPEN                         AUX2=(AUX2&0x3f)|0xC0	//11：WDT Always Enable
	#define   	WDT_RUN_FAST_DIS_ILDE               AUX2=(AUX2&0x3f)|0x80	//10: WDT Enable in Fast/Slow, Disable in Idle/Stop mode
	#define  	WDT_IS_STOP                         AUX2=AUX2&0x3f  		//0X: WDT Disable

	//1: LVR disable at IDLE/PDOWN, 0: LVR enable at IDLE/PDOWN
	#define		PWRSAV		0x20
	#define		PDOWN_LVD_ENABLE	AUX2=(AUX2&0xdf)
	#define		PDOWN_LVD_DISABLE	AUX2=(AUX2&0xdf)|PWRSAV
	
	//
	//1: VBG output to P3.2,   0: P32 as normal IO
	//(extra condition: VBGEN =1 or ADCHS=1011b or ADVREFS≠00b)"
	#define		VBGOUT		0x10	//1: VBG output to P3.2,   0: P32 as normal IO
	#define		VBG_OUTPUT	AUX2=(AUX2&0xef)|VBGOUT
	#define		VBG_NORMAL	AUX2=(AUX2&0xef)
	
	#define		TKCLDS		0x08	//Touch Key XCLD selection. 0: disable XCLD, 1: enable XCLD
	
	//
	//IAPTE:AUX2 Bit[2:1],IAP TIMEOUT Enable  00:Disable  01: 2.8mS   10:  11mS  11:22mS
	#define		IAPTE_2P8_MS	AUX2=(AUX2&0xf9)|0x02
	#define		IAPTE_11_MS		AUX2=(AUX2&0xf9)|0x04
	#define		IAPTE_22_MS		AUX2=(AUX2&0xf9)|0x06
	#define		IAPTE_DISABLE	AUX2=(AUX2&0xf9)
	
	#define		LVRPD		0x01	
	//1: LVR disable, 0: LVR enable

/*
******************************************************************************************************
*/	
sfr 	AUX1   		= 0xF8;
	//--------------------------------------------------------------------------------------------
    sbit 		CLRWDT		= AUX1^7;	//Clear WatchDog, HW auto clear
    #define     CLR_WDT		CLRWDT=1                   //设置以清除看门狗定时器,H/W自动在一个时钟周期清除它     	
	sbit 		CLRTM3		= AUX1^6;//清零TM3也是启动TM3
    sbit 		TKSOC		= AUX1^5;//触摸键开始，结束时自动清零
    sbit 		ADSOC		= AUX1^4;//ADC开始，结束时硬件自动清零
    sbit 		CLRPWM0		= AUX1^3;
    sbit 		T2SEL		= AUX1^2;
    sbit 		T1SEL		= AUX1^1;
    sbit 		DPSEL		= AUX1^0;//0表选择DPTR;1表选择DPTR1

/*
******************************************************************************************************
*/	
sfr 	ICECON 		= 0xF9;
	#define 	HWBRK		      0x80		//H/W Break
	#define 	RETROREQ          0x40		//Retro-OPCode Request,HW set at SWBRK,clear at NMI return
	#define 	TXRDY		      0x20		//Transmitt Ready
	#define 	CMDRDY	          0x10		//Ready for Command
	#define 	XFIRCK	          0x08		//Swich to FIRC 4MHZ
	#define 	WRFAIL	          0x01		//EEPROM Write Fail
/*
******************************************************************************************************
*/	
sfr 	RETROCODE 		= 0xFA;	//ICE Mode Retro-OPCode
/*
******************************************************************************************************
*/	
sfr 	ICERCD 		= 0xFB;	//ICE Mode Receive Data
/*
******************************************************************************************************
*/	
sfr 	ICECMDL		= 0xFC;	//ICE Mode Command Low Byte
/*
******************************************************************************************************
*/	
sfr 	ICECMDH		= 0xFD;	//Bit[3:0],ICE Mode Command High Byte
/*
******************************************************************************************************
*/	
sfr 	ICETXD		= 0xFE;	//ICE Mode Transmit Data
/*
******************************************************************************************************
*/	
sfr 	INDSFR		= 0xFF;	//SFR direct Adr replaced by ICECMDL in NMI "mov dir,a" & "mov a,dir"
/*
******************************************************************************************************
*/	

#define	 All_Interrupt_Enable()		EA = 1		//全局中断使能
#define	 All_Interrupt_Disable()	EA = 0		//全局中断关闭
/*
******************************************************************************************************
*/
#define	 PowerDown()			PCON = PCON | PD	//进入掉电模式
#define	 PowerIdel()			PCON = PCON | IDL	//进入IDEL模式
/*
******************************************************************************************************
*/
#endif
/*
******************************************************************************************************
*/
