

/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : MyEeprom.c
  Version       : Initial Draft
  Author        : Dragon8814
  Created       : 2021/7/6
  Last Modified :
  Description   : 用户接口文件
                  根据用户程序的需要自定义修改，
                  比如需要测试保存数据等情况，
                  注意：保存的数据接口函数@MyEepromSaveAllDataUserHandle()
                        保存的数据必须是所有数据，因为它会把所有数据保存
                        在一个最新区域内
  Function List :
              MyEepromSaveAllDataUserHandle
              MyEepromInit
              MyEepromTestHandle
  History       :
  1.Date        : 2021/7/6
    Author      : Dragon8814
    Modification: Created file

******************************************************************************/




#include "MyEeprom.h"
#include "TM52f82XX_Eeprom.h"
#include "LED.h"
#include "PID.h"


void MyEepromSaveAllDataUserHandle ( unsigned char writeOnWhichArea );



volatile struct eepromUser xdata  MyEeprom;



/*****************************************************************************
 Prototype    : MyEepromInit
 Description  : 用户Eeprom初始化调用
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/

void MyEepromInit()
{

	MyEeprom.SaveAllDataOnUserHandle= ( eepromSaveData* ) MyEepromSaveAllDataUserHandle;
	EepromSetup ( &MyEeprom );
}


/*****************************************************************************
 Prototype    : MyEepromWrite
 Description  : 用户Eeprom写数据操作调用
                注意写入自动分配到新的区域或  在当前区域，取决于
                    当前的区域的写入情况
 Input        : unsigned int  addr 写入数据的地址@AllEepromAddr
                unsigned char dat  写入数据
                unsigned char isSaveAll  是否更新所有数据
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void MyEepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll )
{

	EepromWrite (  addr, dat,  isSaveAll );
}





#if TestEeprom==TRUE
/*****************************************************************************
 Prototype    : MyEepromTestHandle
 Description  : 供测试调用，用户可以自定义修改，
 Input        : char* from 用户自定义指针 如数码管显示缓冲区，串口打印数据数组等
                unsigned int  dat
                unsigned char flag  用户自定义操作命令
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void MyEepromTestHandle ( char* from,unsigned char flag )
{
	unsigned char idata t;
	unsigned char idata area=0;
	switch ( flag )
	{

		case 'A':
#if TestEeprom==TRUE
			//启动时读取每个区域写入次数的
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
			//外部给定的变量地址 数据大小即为写入的速度
			//用测试EEprom擦写极限或擦写寿命
			if ( * ( ( unsigned int* ) from ) >=500 ) //=500为5秒写一次 ，10=100ms
			{
				* ( ( unsigned int* ) from ) =0;
				//更新所有数据
				MyEepromWrite ( FALSE,FALSE,TRUE );

			}

			break;
		case 'C':
#if TestEeprom==TRUE
			//LED显示缓冲区写入特定数据
			//显示当前写入的区域 并显示每个区域当前写入的次数
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
			//显示总共写入了多少次
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
		case 'E': //读数据
			ReadSameDataFromEeprom();
			break;
		default:
			break;
	}
}

#endif




/*****************************************************************************
 Prototype    : MyEepromSaveAllDataUserHandle
 Description  : 用户接口,程序自动选择的当前区域中保存用户的所有的数据
                注意:此接口必须写入所有需要保存的数据
 Input        : unsigned char writeOnWhichArea 当前保存数据的区域
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

****************************************************************************/

void MyEepromSaveAllDataUserHandle ( unsigned char writeOnWhichArea )
{

	/***************************用户自定义段没有需要不添加*************************/
	  	unsigned int xdata setTemp;
	    unsigned char xdata setHumidity, fd_Time;
	//只有在DIY模式下需要备份 
	//其他模式需要备份再写入,避免下次运行DIY模式时参数出错
	if ( SetData.FH_Mode != 9 )
	{
		setTemp = SetData.SetTemp; //备份当前运行
		setHumidity = SetData.SetHumidity;
		fd_Time = SetData.FD_Time;
		SetData.SetHumidity = SetData.SetHumidityBuf; //保存原始数据，
		SetData.SetTemp = SetData.SetTempBuf;
		SetData.FD_Time = SetData.FD_TimeBuf;

		RunData.WorkTimeBuf = RunData.WorkTime_Second;
	}
	else
	{
		//是DIY 需要更新本次数据，避免下次切换其他模式时还是保存旧的数据
		SetData.FD_TimeBuf = SetData.FD_Time;
		SetData.SetTempBuf = SetData.SetTemp;
		SetData.SetHumidityBuf = SetData.SetHumidity;

		RunData.WorkTimeBuf = RunData.WorkTime_Second;
	}
	/*********************************END********************************************/






	CLR_WDT;
    /*****************************用户必须添加段，必须加入所有需要保存的数据*************************/
	EepromWriteByte ( EepromStrogeValue*2* writeOnWhichArea +FH_ModeAddr*2, SetData.FH_Mode );
	//写入设置温度
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

#if TestEeprom==TRUE //测试用
	{
		//测试时这几个地址更改为保存总写入次数 ,也可以自定义其他地址
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
   /*********************************END********************************************/







   /***************************用户自定义段没有需要不添加*************************/
	//用户自定义数据恢复
	if ( SetData.FH_Mode != 9 )
	{
		SetData.SetHumidity = setHumidity; //恢复
		SetData.SetTemp = setTemp;
		SetData.FD_Time = fd_Time;
	}
	/***************************用户自定义段没有需要不添加*************************/

}




/*****************************************************************************
 Prototype    : MyEepromRead
 Description  : 根据地址读取单个数据 用户调用
 Input        : unsigned char addr  
 Output       : None
 Return Value : unsigned
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
unsigned char MyEepromRead(unsigned char addr)
{

   return EepromRead ( addr );
}






/*****************************************************************************
 Prototype    : ReadSameDataFromEeprom
 Description  : 用户自定义，在Eeprom读取一些数据
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/

void ReadSameDataFromEeprom()
{

	SetData.FH_Mode=MyEepromRead ( FH_ModeAddr );

	SetData.FD_Time=MyEepromRead (  FD_TimeAddr );

	SetData.SetTemp=MyEepromRead (  SetTempAddr_L );
	SetData.SetTemp+= ( ( unsigned int ) MyEepromRead ( SetTempAddr_H ) <<8 );
	SetData.SetHumidity= MyEepromRead ( SetHumidityAddr );
	SetData.SetHumidityBuf=SetData.SetHumidity;
	SystemStart_Flag= ( bit ) MyEepromRead ( SystemStartFlagAddr );
	RunData.WorkTime_Second=MyEepromRead ( WorkTimeAddr_L )+ ( ( unsigned int ) MyEepromRead ( WorkTimeAddr_H ) <<8 ) ;
	RunData.WorkTimeBuf=RunData.WorkTime_Second;
	RunData.FD_Count=MyEepromRead (  FD_CountAdd_L );
	RunData.FD_Count+= ( ( unsigned int ) MyEepromRead ( FD_CountAdd_H ) <<8 );
	RunData.CS_Count=MyEepromRead ( CS_CountAddr_L )+ ( ( unsigned int ) MyEepromRead ( CS_CountAddr_H ) <<8 ) ;


}




