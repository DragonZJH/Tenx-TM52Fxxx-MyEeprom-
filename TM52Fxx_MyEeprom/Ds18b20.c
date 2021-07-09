

#include "Ds18b20.h"


unsigned char TempSensor_DS_ErrorFlag=FALSE;





unsigned char DS18B20RWFlag=FALSE;


//#if UseMax31865!=TRUE
#if DS18B20==TRUE


/*------------------------------------------------
                    18b20��ʼ��
------------------------------------------------*/

bit Init_DS18B20 ( void )
{
	bit dat=0;
	unsigned char t;
	Ds18b20_Data_Port_OUTPUT;
	DQ = 1;    //DQ��λ

	Delay5us();
	Delay5us();
	DS18B20RWFlag=TRUE;
	DQ = 0;         //��Ƭ����DQ����
	Delay700us(); //��ȷ��ʱ ���� 480us С��960us
	DQ = 1;        //��������
	//15~60us �� ����60-240us�Ĵ�������
	Delay30us();
	Delay50us();
	dat=1;
	for ( t=0; t<10; t++ )
	{
         
		if (!DQ)
		{
			dat=0;    //���x=0���ʼ���ɹ�, x=1���ʼ��ʧ��
		    DS18B20RWFlag=FALSE;
		}
		
		
		Delay50us();
	}

	return dat;
}

/*------------------------------------------------
                    ��ȡһ���ֽ�
------------------------------------------------*/
unsigned char ReadOneChar ( void )
{
	unsigned char i=0;
	unsigned char dat = 0;
	DS18B20RWFlag=TRUE;
	for ( i=8; i>0; i-- )
	{
		DQ = 0; // �������ź�
		Delay5us();
		Delay5us();
		dat>>=1;
		DQ = 1; // �������ź�
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
                    д��һ���ֽ�
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
                    ��ȡ�¶�
 isReadRightNow:Ϊ1��ʾ���϶�ȡ��ǰ�¶�����ʱ750ms
                Ϊ0��ʾ�´ζ�ȡ��ǰ�¶�û����ʱ750ms����ζ���´ζ�ȡʱ��������750ms
------------------------------------------------*/
unsigned int ReadTemperature ( unsigned char isReadRightNow )
{
	unsigned char a=0;
	unsigned int b=0;
	unsigned int t=0;
	//���̶�ȡ��ǰ�¶� ��Ҫ�ȴ�750Ms
	if ( isReadRightNow )
	{
		Init_DS18B20();
		WriteOneChar ( 0xCC ); // ����������кŵĲ���
		WriteOneChar ( 0x44 ); // �����¶�ת��
		for ( a=0; a<75; a++ )
		{
			Delay10Ms();
		}
	}

	t=Init_DS18B20();
	if ( t==1 ) //��������ʼʧ��
	{
		return 0xffff;
	}
	WriteOneChar ( 0xCC ); //����������кŵĲ���
	WriteOneChar ( 0xBE ); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�


	a=ReadOneChar();   //��λ
	b=ReadOneChar();   //��λ
	

	b<<=8;
	t=a+b;

	//�¶ȱ��浽�´�����ȡ
	//��ȡ�¶����ڱ������750Ms
	if ( !isReadRightNow )
	{
		Init_DS18B20();
		WriteOneChar ( 0xCC ); // ����������кŵĲ���
		WriteOneChar ( 0x44 ); // �����¶�ת��
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

