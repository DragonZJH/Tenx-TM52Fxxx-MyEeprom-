/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : TM52F82XX_Eeprom.c
  Version       : Initial Draft
  Author        : Dragon8814
  Created       : 2021/7/6
  Last Modified :
  Description   : 数据自动分配保存在分配的区域,用户无需修改
  Function List :
              EepromRead
              EepromSaveAllData
              EepromSetup
              EepromWrite
              EepromWriteByte
              FindOnWhichArea
              RWEepromEnd
              RWEepromStart
  History       :
  1.Date        : 2021/7/6
    Author      : Dragon8814
    Modification: Created file

******************************************************************************/



#include "TM52F82XX_Eeprom.h"


static void  EepromSaveAllData ( unsigned char writeOnWhichArea,unsigned char saveAllOnThisArea );



volatile struct eepromUser* xdata  EepromUser;

static void Delay700us()		//@7.3728MHz
{
	unsigned char i, j;

	i = 6;
	j = 1;
	do
	{
		while (--j);
	} while (--i);
}


/*****************************************************************************
 Prototype    : EepromSetup
 Description  : 初始化内存并分配给用户定义接口
 Input        : struct eepromUser* ee
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void  EepromSetup ( struct eepromUser* ee )
{


	EepromUser=ee;

}


/*****************************************************************************
 Prototype    : RWEepromStart
 Description  : Eeprom开始写入寄存器操作
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void  RWEepromStart()
{
	EA = 0;
	IAPTE_22_MS;
	IAPWE_SFR=0XE2;    //使能写EEPROM
}



/*****************************************************************************
 Prototype    : RWEepromEnd
 Description  : Eeprom写入结束寄存器操作
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void RWEepromEnd()
{
	IAPWE_SFR=0x00;    //关闭写EEPROM
	IAPTE_DISABLE;
	EA = 1;
	Delay700us();   
}

/*****************************************************************************
 Prototype    : FindOnWhichArea
 Description  : 找到当前数据所在区域
 Input        : struct findOnArea* foa
 Output       : 结构体返回区域0-N或是否所有区域已到达最大写入次数EepromMaxAreaCount
 Return Value : 返回区域中的数据保存状态
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void FindOnWhichArea ( struct findOnArea* foa )
{
	unsigned char area, dat;
	/*******************真实存放的地址****************************
	**********(EepromStrogeValue*2*OnArea_N)+(addr*2)**************
	如存放在区域1中FD_TimeAddr的地址：(15*2*1)+(2*2)=34
	**************************************************************/
#if TestEeprom_==TRUE
	RWEepromStart();
	for ( area=0; area<EepromUser->EepromMaxArea; area++ )
	{
		dat=* ( EepromUser->EepromOnRamAddress + (EepromUser->EepromStrogeValue*2*area ) ); //区域所在地址
		EepromUser->EepromTestArea[area]=dat;
	}
	RWEepromEnd();
#endif
	RWEepromStart();
	for ( area=0; area<EepromUser->EepromMaxArea; area++ )
	{
		//dat=* ( &EepromAddress+ ((EepromStrogeValue*2*area)+(AreaAddr*2)) );
		dat=* ( EepromUser->EepromOnRamAddress + ( EepromUser->EepromStrogeValue*2*area ) ); //区域所在地址

		if ( dat<EepromUser->EepromMaxAreaCount ) //任何一个区域的最大写入次数每超 返回数据
		{
			foa->whichhArea=area;
			foa->AreaCount=dat;
			RWEepromEnd();
			return;
		}
	}
	RWEepromEnd();

	//出错，超出最大区域
	{
		foa->whichhArea= EepromUser->OutOfTheArea;
		return ;
	}

}

/*****************************************************************************
 Prototype    : EepromRead
 Description  : Eeprom数据读取
 Input        : 读取数据地址
 Output       : None
 Return Value : 地址对应的数据
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
unsigned char EepromRead ( unsigned int addr )
{
	unsigned char  dat=0;
	struct findOnArea fOA;
	FindOnWhichArea ( &fOA );
	if ( fOA.whichhArea ==EepromUser->OutOfTheArea )
	{
		//所有区域都超过最大写入次数 出错
		//只会出现在芯片第一次运行的时候，因为那时候的EEPROM数据时不确定的
		//如果第一次运行程序没有进入此处，程序在初始化的时候要做读取错误处理
		fOA.whichhArea=0;
		EepromSaveAllData ( fOA.whichhArea,FALSE ); //出错更新区域0

	}

	
	dat=* ( EepromUser->EepromOnRamAddress + ( EepromUser->EepromStrogeValue*2* fOA.whichhArea )+ ( addr*2 ) ) ;

	return dat;
}


/*****************************************************************************
 Prototype    : EepromReadWithArea
 Description  : Eeprom数据根据提供区域读取
 Input        : addr:数据地址，area:数据所在区域
 Output       : None
 Return Value : 区域内地址对应的数据
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
unsigned char EepromReadWithArea ( unsigned int addr,unsigned char area )
{
    unsigned char dat;
	
	dat=* ( (EepromUser->EepromOnRamAddress) + ( EepromUser->EepromStrogeValue*2* area )+ ( addr*2 ) ) ;
	
	return dat;
}





/*****************************************************************************
 Prototype    : EepromWriteByte
 Description  : Eeprom直接写入一个字节 没有明确的写入区域建议不要随意调用
 Input        : addr: 写入的数据所在地址
                dat ：写入的数据
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/

void EepromWriteByte ( unsigned int  addr,unsigned char  dat )
{
	unsigned char edat = 0;

#if TestEeprom_!=TRUE  //测试时全写
	edat=* (EepromUser->EepromOnRamAddress +addr );
	//edat=EepromAddress[addr];
	if ( edat!=dat )
#endif
	{
	
		RWEepromStart();
		*(EepromUser->EepromOnRamAddress+addr)=dat;
		RWEepromEnd();	
	}

}

/*****************************************************************************
 Prototype    : EepromSaveAllData
 Description  : 保存、更新所有数据
 Input        : writeOnWhichArea:保存在哪个区域
                isSaveAllOnThisArea:是否保存到当前区域，

 Output       : None
 Return Value : None
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
static void  EepromSaveAllData ( unsigned char writeOnWhichArea,unsigned char isSaveAllOnThisArea )
{
	//只在当前区域跟新所有数据忽略
	if ( isSaveAllOnThisArea!=TRUE )
	{

		EepromWriteByte ( EepromUser->EepromStrogeValue*2*writeOnWhichArea,1 ); //当前区写入次数更改为1

		//上一区写入次数标志为写满 =EepromMaxAreaCount
		//如果当前区域是第一区0，则上一区为EepromLastArea最后区域
		EepromWriteByte ( EepromUser->EepromStrogeValue*2* ( writeOnWhichArea!=0?writeOnWhichArea-1:EepromUser->EepromLastArea ), EepromUser->EepromMaxAreaCount );
#if TestEeprom_==TRUE
		EepromUser->EepromTestArea[writeOnWhichArea]=1;
		EepromUser->EepromTestArea[writeOnWhichArea!=0?writeOnWhichArea-1:EepromUser->EepromLastArea]=EepromUser->EepromMaxAreaCount;
#endif
	}
	EepromUser->SaveAllDataOnUserHandle ( writeOnWhichArea );

}


/*****************************************************************************
 Prototype    : EepromWrite
 Description  : Eeprom写数据操作
 Input        : unsigned int  addr 
                unsigned char dat  
                unsigned char isSaveAll 
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon8814
    Modification : Created function

*****************************************************************************/
void EepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll )
{
	struct findOnArea fOA;
	unsigned char isN;

	FindOnWhichArea ( &fOA );
	if ( fOA.whichhArea<EepromUser->EepromMaxArea )
	{
		if ( fOA.AreaCount>EepromUser->EepromMaxAreaCount-2 )
		{
			if ( fOA.whichhArea==EepromUser->EepromLastArea ) //最后一个区域已经写满次数了，则转到第一个区域
			{
				fOA.whichhArea=0;
			}
			else
			{
				fOA.whichhArea++;    //不是最后一个区域，则转到下个区域
			}
			isN=TRUE; //区域写满 实际写入次数EepromMaxAreaCount-1个 数据更新到下个区域
		}
		else
		{
			isN=FALSE;//按地址正常写入数据
		}

	}
	else
	{
		fOA.whichhArea=0; //出错所有数据更新到区域0
		isN=TRUE;
	}

	if ( isN==TRUE )  //当前的区域写入新数据条件不满足，需要一并更新所有数据
	{
		
#if TestEeprom_==TRUE
		EepromUser->EepromTestWriteCount++;
#endif
         EepromSaveAllData ( fOA.whichhArea,FALSE );
	}
	else
	{
#if TestEeprom_==TRUE
		EepromUser->EepromTestWriteCount++;
#endif	
	  //写在当前区域
	  
		/*************************真实存放的地址*******************
		 *   (EepromStrogeValue*2*OnArea_N)+(addr*2)              *
		 *  如存放在区域1中FD_TimeAddr的地址：(15*2*1)+(2*2)=34              *
		 *  芯片Eeprom起始地址已经增加在写数据函数                                *
		 **********************************************************/
		
		EepromWriteByte ( EepromUser->EepromStrogeValue*2*fOA.whichhArea+0*2,++fOA.AreaCount ); //在所在区域写入增加次数

		if ( isSaveAll ) //在当前区更新所有数据
		{

			EepromSaveAllData ( fOA.whichhArea,TRUE );
		}

		//在当前区更新个别数据
		else
		{
             
			EepromWriteByte ( EepromUser->EepromStrogeValue*2*fOA.whichhArea+addr*2,dat );
		}


	}

}





