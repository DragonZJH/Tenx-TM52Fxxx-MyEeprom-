

#include "Ds18b20.h"


unsigned char TempSensor_DS_ErrorFlag=FALSE;





unsigned char DS18B20RWFlag=FALSE;


//#if UseMax31865!=TRUE
#if DS18B20==TRUE


/*------------------------------------------------
                    18b20初始化
------------------------------------------------*/

bit Init_DS18B20 ( void )
{
	bit dat=0;
	unsigned char t;
	Ds18b20_Data_Port_OUTPUT;
	DQ = 1;    //DQ复位

	Delay5us();
	Delay5us();
	DS18B20RWFlag=TRUE;
	DQ = 0;         //单片机将DQ拉低
	Delay700us(); //精确延时 大于 480us 小于960us
	DQ = 1;        //拉高总线
	//15~60us 后 接收60-240us的存在脉冲
	Delay30us();
	Delay50us();
	dat=1;
	for ( t=0; t<10; t++ )
	{
         
		if (!DQ)
		{
			dat=0;    //如果x=0则初始化成功, x=1则初始化失败
		    DS18B20RWFlag=FALSE;
		}
		
		
		Delay50us();
	}

	return dat;
}

/*------------------------------------------------
                    读取一个字节
------------------------------------------------*/
unsigned char ReadOneChar ( void )
{
	unsigned char i=0;
	unsigned char dat = 0;
	DS18B20RWFlag=TRUE;
	for ( i=8; i>0; i-- )
	{
		DQ = 0; // 给脉冲信号
		Delay5us();
		Delay5us();
		dat>>=1;
		DQ = 1; // 给脉冲信号
		nop;
		if ( DQ )
		{
			dat|=0x80;
		}

		Delay50us();
	}
	DS18B20RWFlag=FALSE;
	return dat ;
}
/*------------------------------------------------
                    写入一个字节
------------------------------------------------*/
void WriteOneChar ( unsigned char dat )
{
	unsigned char i=0;
	DS18B20RWFlag=TRUE;
	for ( i=8; i>0; i-- )
	{
		DQ = 0;
		Delay5us();
		Delay5us();
		DQ = dat&0x01;
		Delay50us();
		DQ = 1;
		dat>>=1;
	}
	DS18B20RWFlag=FALSE;
	Delay50us();
}

/*------------------------------------------------
                    读取温度
 isReadRightNow:为1表示马上读取当前温度有延时750ms
                为0表示下次读取当前温度没有延时750ms，意味着下次读取时间必须大于750ms
------------------------------------------------*/
unsigned int ReadTemperature ( unsigned char isReadRightNow )
{
	unsigned char a=0;
	unsigned int b=0;
	unsigned int t=0;
	//立刻读取当前温度 需要等待750Ms
	if ( isReadRightNow )
	{
		Init_DS18B20();
		WriteOneChar ( 0xCC ); // 跳过读序号列号的操作
		WriteOneChar ( 0x44 ); // 启动温度转换
		for ( a=0; a<75; a++ )
		{
			Delay10Ms();
		}
	}

	t=Init_DS18B20();
	if ( t==1 ) //传感器初始失败
	{
		return 0xffff;
	}
	WriteOneChar ( 0xCC ); //跳过读序号列号的操作
	WriteOneChar ( 0xBE ); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度


	a=ReadOneChar();   //低位
	b=ReadOneChar();   //高位
	

	b<<=8;
	t=a+b;

	//温度保存到下次来读取
	//读取温度周期必须大于750Ms
	if ( !isReadRightNow )
	{
		Init_DS18B20();
		WriteOneChar ( 0xCC ); // 跳过读序号列号的操作
		WriteOneChar ( 0x44 ); // 启动温度转换
	}

	return t ;
}




void SetTempSensor_DS_Status ( unsigned char status )
{
	static unsigned char xdata sensorError,cb;
	if ( status ==FALSE )
	{
		sensorError=0;
		if ( TempSensor_DS_ErrorFlag==TRUE )
		{
			cb++;
			if ( cb>5 )
			{
				TempSensor_DS_ErrorFlag=FALSE;
				RunErrorStatus.Status.TempSensorErrorFlag=FALSE;
				cb=0;
			}
		}
	}
	else
	{
		sensorError++;
	}
	if ( sensorError>5 )
	{
	    sensorError=6;
		TempSensor_DS_ErrorFlag=TRUE;
		RunErrorStatus.Status.TempSensorErrorFlag=TRUE;
		cb=0;
	}
}

#endif

