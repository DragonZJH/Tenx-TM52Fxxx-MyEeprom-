

/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : MyEeprom.c
  Version       : Initial Draft
  Author        : Dragon
  Created       : 2021/7/6
  Last Modified :
  Description   : �û��ӿ��ļ�
                  �����û��������Ҫ�Զ����޸ģ�
                  ������Ҫ���Ա������ݵ������
                  ע�⣺��������ݽӿں���@EepromSaveAllDataUserHandle()
                        ��������ݱ������������ݣ���Ϊ������������ݱ���
                        ��һ������������
  Function List :
              EepromSaveAllDataUserHandle
              MyEepromInit
              EepromTestHandle
  History       :
  1.Date        : 2021/7/6
    Author      : Dragon
    Modification: Created file

******************************************************************************/




#include "MyEeprom.h"
#include "TM52f82XX_Eeprom.h"
#include "LED.h"
#include "DEFINE.H"

void EepromSaveAllDataUserHandle ( unsigned char writeOnWhichArea );



volatile struct eepromUser xdata  MyEeprom;



/*****************************************************************************
 Prototype    : MyEepromInit
 Description  : �û�Eeprom��ʼ��
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/

void MyEepromInit()
{

	MyEeprom.SaveAllDataOnUserHandle= ( eepromSaveData* ) EepromSaveAllDataUserHandle;
	EepromSetup ( &MyEeprom );
}


/*****************************************************************************
 Prototype    : MyEepromWrite
 Description  : �û�����Eepromд���ݲ���
                ע��д���Զ����䵽�µ������  �ڵ�ǰ����ȡ����
                    ��ǰ�������д�����
 Input        : unsigned int  addr д�����ݵĵ�ַ@AllEepromAddr
                unsigned char dat  д������
                unsigned char isSaveAll  �Ƿ������������
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void MyEepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll )
{

	EepromWrite (  addr, dat,  isSaveAll );
}



#if TestEeprom==TRUE
/*****************************************************************************
 Prototype    : EepromTestHandle
 Description  : �������ã��û������Զ����޸ģ�
 Input        : char* from �û��Զ���ָ�� ���������ʾ�����������ڴ�ӡ���������
                unsigned int  dat
                unsigned char flag  �û��Զ����������
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void EepromTestHandle ( char* from,unsigned char flag )
{
	unsigned char idata t;
	unsigned char idata area=0;
	switch ( flag )
	{

		case 'A':
#if TestEeprom==TRUE
			//����ʱ��ȡÿ������д�������
			for ( t=0; t<EepromMaxArea; t++ )
			{
				RWEepromStart();
				MyEeprom.EepromTestArea[t]= * ( MyEeprom.EepromOnRamAddress+ ( EepromStrogeValue*2*t ) );
				RWEepromEnd();
			}
			for ( t=0; t<EepromMaxArea; t++ )
			{
				if ( MyEeprom.EepromTestArea[t]<EepromMaxAreaCount )
				{
					area=t;
					break;
				}


			}
			RWEepromStart();
			MyEeprom.EepromTestWriteCount = ( unsigned long ) ( * ( MyEeprom.EepromOnRamAddress+ ( EepromStrogeValue*2* area )+CS_CountAddr_L *2  ) ) ;
			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  ( * ( MyEeprom.EepromOnRamAddress+ ( EepromStrogeValue*2* area )+ CS_CountAddr_H *2      ) ) ) <<8 );
			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  ( * ( MyEeprom.EepromOnRamAddress+ ( EepromStrogeValue*2* area )+ SetStableDukAddr_L *2  ) ) ) <<16 );
			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  ( * ( MyEeprom.EepromOnRamAddress+ ( EepromStrogeValue*2* area )+ SetStableDukAddr_H *2  ) ) ) <<24 );
			RWEepromEnd();
#endif



//			MyEeprom.EepromTestWriteCount = ( unsigned long ) EepromRead( CS_CountAddr_L  ) ;
//			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  EepromRead ( CS_CountAddr_H) ) <<8 );
//			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  EepromRead( SetStableDukAddr_L ) )  <<16 );
//			MyEeprom.EepromTestWriteCount|= ( ( ( unsigned long )  EepromRead(  SetStableDukAddr_H  ) ) <<24 );
//

			break;
		case 'B':
			//�ⲿ�����ı�����ַ ���ݴ�С��Ϊд����ٶ�
			//�ò���EEprom��д���޻��д����
			if ( * ( ( unsigned int* ) from ) >=500 ) //=500Ϊ5��дһ�� ��10=100ms
			{
				* ( ( unsigned int* ) from ) =0;
				//������������
				MyEepromWrite ( FALSE,FALSE,TRUE );

			}

			break;
		case 'C':
#if TestEeprom==TRUE
			//LED��ʾ������д���ض�����
			//��ʾ��ǰд������� ����ʾÿ������ǰд��Ĵ���
			from[LED_SetTemp_COM1]=Number[MyEeprom.EepromTestArea[0]/100]; //0
			from[LED_SetTemp_COM2]=Number[MyEeprom.EepromTestArea[0]%100/10]&SEG_DPOFF;
			from[LED_SetTemp_COM3]=Number[MyEeprom.EepromTestArea[0]%10];

			from[LED_RealTemp_COM1]=ALL_SEG_OFF;
			from[LED_RealTemp_COM2]=Number[MyEeprom.EepromTestArea[1]/100]&SEG_DPOFF; //1
			from[LED_RealTemp_COM3]=Number[MyEeprom.EepromTestArea[1]%100/10];
			from[LED_RealTemp_COM4]=Number[MyEeprom.EepromTestArea[1]%10];


			from[LED_SetHumidity_COM1]=ALL_SEG_OFF;
			from[LED_SetHumidity_COM2]=Number[MyEeprom.EepromTestArea[2]/100];
			from[LED_RealHumidity_COM1]=Number[MyEeprom.EepromTestArea[2]%100/10];
			from[LED_RealHumidity_COM2]=Number[MyEeprom.EepromTestArea[2]%10];

			from[LED_FDTimeLeft_COM1]=Number[MyEeprom.EepromTestArea[3]/100]; //3
			from[LED_FDTimeLeft_COM2]=Number[MyEeprom.EepromTestArea[3]%100/10];
			from[LED_FDTimeLeft_COM3]=Number[MyEeprom.EepromTestArea[3]%10];
#endif
			break;
		case 'D':
#if TestEeprom==TRUE
			//��ʾ�ܹ�д���˶��ٴ�
			from[LED_RealTemp_COM1]=Number[MyEeprom.EepromTestWriteCount/1000000];
			from[LED_RealTemp_COM2]=Number[MyEeprom.EepromTestWriteCount%1000000/100000]&SEG_DPOFF; //1
			from[LED_RealTemp_COM3]=Number[MyEeprom.EepromTestWriteCount%100000/10000];
			from[LED_RealTemp_COM4]=Number[MyEeprom.EepromTestWriteCount%10000/1000];


			from[LED_RealHumidity_COM1]=Number[MyEeprom.EepromTestWriteCount%1000/100];
			from[LED_RealHumidity_COM2]=Number[MyEeprom.EepromTestWriteCount%100/10];

			from[LED_FDTimeLeft_COM1]=Number[MyEeprom.EepromTestWriteCount%10];
			from[LED_FDTimeLeft_COM2]=ALL_SEG_OFF;
			from[LED_FDTimeLeft_COM3]=ALL_SEG_OFF;
#endif
			break;
		case 'E': //������
			ReadSameDataFromEeprom();
			break;
		default:
			break;
	}
}

#endif



/*****************************************************************************
 Prototype    : EepromSaveAllDataUserHandle
 Description  : �û��ӿ�,�����Զ�ѡ��ĵ�ǰ�����б����û������е�����
 Input        : unsigned char writeOnWhichArea ��ǰ�������ݵ�����
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void EepromSaveAllDataUserHandle ( unsigned char writeOnWhichArea )
{
	unsigned int xdata setTemp;
	unsigned char xdata setHumidity, fd_Time;



	//�û��Զ���

	//ֻ����DIYģʽ�� �¶� ʪ�� ����ʱ�����Ч
	//����ģʽ��Ҫ������д��,�����´�����DIYģʽʱ��������
	if ( SetData.FH_Mode != 9 )
	{
		setTemp = SetData.SetTemp; //���ݵ�ǰ����
		setHumidity = SetData.SetHumidity;
		fd_Time = SetData.FD_Time;
		SetData.SetHumidity = SetData.SetHumidityBuf; //����ԭʼ���ݣ�
		SetData.SetTemp = SetData.SetTempBuf;
		SetData.FD_Time = SetData.FD_TimeBuf;

		RunData.WorkTimeBuf = RunData.WorkTime_Second;
	}
	else
	{
		//��DIY ��Ҫ���±������ݣ������´��л�����ģʽʱ���Ǳ���ɵ�����
		SetData.FD_TimeBuf = SetData.FD_Time;
		SetData.SetTempBuf = SetData.SetTemp;
		SetData.SetHumidityBuf = SetData.SetHumidity;

		RunData.WorkTimeBuf = RunData.WorkTime_Second;
	}



	CLR_WDT;

	//����Ϊ�������������ڳ���ѡ��Ķ�Ӧ������
	/*************************��ʵ��ŵĵ�ַ*******************
	 *   (EepromStrogeValue*2*OnArea_N)+(addr*2)              *
	 *  ����������1��FD_TimeAddr�ĵ�ַ��(15*2*1)+(2*2)=34              *
	 *  оƬEeprom��ʼ��ַ�Ѿ�������д���ݺ���                                *
	 **********************************************************/
	EepromWriteByte ( EepromStrogeValue*2* writeOnWhichArea +FH_ModeAddr*2, SetData.FH_Mode );
	//д�������¶�
	EepromWriteByte ( EepromStrogeValue*2* writeOnWhichArea +SetTempAddr_L*2,SetData.SetTemp );
	EepromWriteByte ( EepromStrogeValue* 2* writeOnWhichArea +SetTempAddr_H*2, SetData.SetTemp >> 8 );

	EepromWriteByte ( EepromStrogeValue* 2* writeOnWhichArea + SetHumidityAddr*2, SetData.SetHumidity );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +FD_TimeAddr*2, SetData.FD_Time );

	CLR_WDT;
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +SystemStartFlagAddr*2, ( unsigned char ) SystemStart_Flag );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +WorkTimeAddr_L*2, RunData.WorkTime_Second & 0x00ff );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +WorkTimeAddr_H*2, RunData.WorkTime_Second >> 8 );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +FD_CountAdd_L*2, RunData.FD_Count & 0x00ff );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +FD_CountAdd_H*2, RunData.FD_Count >> 8 );

#if TestEeprom==TRUE
	{
		//����ʱ�⼸����ַ����Ϊ������д����� ,Ҳ�����Զ���������ַ
		//MyEeprom.EepromTestWriteCount=tt;
		EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +CS_CountAddr_L*2,  MyEeprom.EepromTestWriteCount & 0x000000ff );
		EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +CS_CountAddr_H*2, ( MyEeprom.EepromTestWriteCount>>8 ) & 0x000000ff );
		CLR_WDT;
		EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +SetStableDukAddr_L*2, ( MyEeprom.EepromTestWriteCount>>16 ) & 0x000000ff );
		EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +SetStableDukAddr_H*2, ( MyEeprom.EepromTestWriteCount>>24 ) );

	}
#else

	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +CS_CountAddr_L*2, RunData.CS_Count & 0x00ff );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +CS_CountAddr_H*2, RunData.CS_Count >> 8 );
	CLR_WDT;
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +SetStableDukAddr_L*2, RunData.DukBuf_H & 0x00ff );
	EepromWriteByte (  EepromStrogeValue* 2* writeOnWhichArea +SetStableDukAddr_H*2, RunData.DukBuf_H >> 8 );
#endif

	//�û��Զ������ݻָ�
	if ( SetData.FH_Mode != 9 )
	{
		SetData.SetHumidity = setHumidity; //�ָ�
		SetData.SetTemp = setTemp;
		SetData.FD_Time = fd_Time;
	}

}





/*****************************************************************************
 Prototype    : ReadSameDataFromEeprom
 Description  : �û��Զ��壬��Eeprom��ȡһЩ����
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/

void ReadSameDataFromEeprom()
{

	SetData.FH_Mode=EepromRead ( FH_ModeAddr );

	SetData.FD_Time=EepromRead (  FD_TimeAddr );

	SetData.SetTemp=EepromRead (  SetTempAddr_L );
	SetData.SetTemp+= ( ( unsigned int ) EepromRead ( SetTempAddr_H ) <<8 );
	SetData.SetHumidity= EepromRead ( SetHumidityAddr );
	SetData.SetHumidityBuf=SetData.SetHumidity;
	SystemStart_Flag= ( bit ) EepromRead ( SystemStartFlagAddr );
	RunData.WorkTime_Second=EepromRead ( WorkTimeAddr_L )+ ( ( unsigned int ) EepromRead ( WorkTimeAddr_H ) <<8 ) ;
	RunData.WorkTimeBuf=RunData.WorkTime_Second;
	RunData.FD_Count=EepromRead (  FD_CountAdd_L );
	RunData.FD_Count+= ( ( unsigned int ) EepromRead ( FD_CountAdd_H ) <<8 );
	RunData.CS_Count=EepromRead ( CS_CountAddr_L )+ ( ( unsigned int ) EepromRead ( CS_CountAddr_H ) <<8 ) ;


}




