/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : TM52F82XX_Eeprom.c
  Version       : Initial Draft
  Author        : Dragon
  Created       : 2021/7/6
  Last Modified :
  Description   : �����Զ����䱣���ڷ��������,�û������޸�
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
    Author      : Dragon
    Modification: Created file

******************************************************************************/



#include "TM52F82XX_Eeprom.h"

volatile unsigned char xdata EepromAddress[ChipEepromRamCP] _at_ EepromStartAddr;

volatile struct eepromUser*   EepromUser;

/*****************************************************************************
 Prototype    : EepromSetup
 Description  : ��ʼ���ڴ沢������û�����ӿ�
 Input        : struct eepromUser* ee
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/6
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void  EepromSetup ( struct eepromUser* ee )
{

	ee->EepromOnRamAddress=EepromAddress;
	EepromUser=ee;

}


/*****************************************************************************
 Prototype    : RWEepromStart
 Description  : Eeprom��ʼд��Ĵ�������
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void  RWEepromStart()
{
	EA = 0;
	IAPTE_22_MS;
	IAPWE_SFR=0XE2;    //ʹ��дEEPROM
}



/*****************************************************************************
 Prototype    : RWEepromEnd
 Description  : Eepromд������Ĵ�������
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void RWEepromEnd()
{
	IAPWE_SFR=0x00;    //�ر�дEEPROM
	IAPTE_DISABLE;
	EA = 1;
	Delay700us();   
}

/*****************************************************************************
 Prototype    : FindOnWhichArea
 Description  : �ҵ���ǰ������������
 Input        : struct findOnArea* foa
 Output       : �ṹ�巵������0-N���Ƿ����������ѵ������д�����EepromMaxAreaCount
 Return Value : ���������е����ݱ���״̬
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void FindOnWhichArea ( struct findOnArea* foa )
{
	unsigned char area, dat;
	/*******************��ʵ��ŵĵ�ַ****************************
	**********(EepromStrogeValue*2*OnArea_N)+(addr*2)**************
	����������1��FD_TimeAddr�ĵ�ַ��(15*2*1)+(2*2)=34
	**************************************************************/
#if TestEeprom==TRUE
	RWEepromStart();
	for ( area=0; area<EepromMaxArea; area++ )
	{
		dat=* ( &EepromAddress+ ( EepromStrogeValue*2*area ) ); //�������ڵ�ַ
		EepromUser->EepromTestArea[area]=dat;
	}
	RWEepromEnd();
#endif
	RWEepromStart();
	for ( area=0; area<EepromMaxArea; area++ )
	{
		//dat=* ( &EepromAddress+ ((EepromStrogeValue*2*area)+(AreaAddr*2)) );
		dat=* ( &EepromAddress+ ( EepromStrogeValue*2*area ) ); //�������ڵ�ַ

		if ( dat<EepromMaxAreaCount ) //�κ�һ����������д�����ÿ�� ��������
		{
			foa->whichhArea=area;
			foa->AreaCount=dat;
			RWEepromEnd();
			return;
		}
	}
	RWEepromEnd();

	//���������������
	{
		foa->whichhArea= OutOfTheArea;
		return ;
	}

}

/*****************************************************************************
 Prototype    : EepromRead
 Description  : Eeprom���ݶ�ȡ
 Input        : ��ȡ���ݵ�ַ
 Output       : None
 Return Value : ��ַ��Ӧ������
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
unsigned char EepromRead ( unsigned int addr )
{
	unsigned char  dat=0;
	struct findOnArea fOA;
	FindOnWhichArea ( &fOA );
	if ( fOA.whichhArea ==OutOfTheArea )
	{
		//�������򶼳������д����� ����
		//ֻ�������оƬ��һ�����е�ʱ����Ϊ��ʱ���EEPROM����ʱ��ȷ����
		//�����һ�����г���û�н���˴��������ڳ�ʼ����ʱ��Ҫ����ȡ������
		fOA.whichhArea=0;
		EepromSaveAllData ( fOA.whichhArea,FALSE ); //�����������0

	}

	RWEepromStart();
	dat=* ( &EepromAddress+ ( EepromStrogeValue*2* fOA.whichhArea )+ ( addr*2 ) ) ;
	RWEepromEnd();
	return dat;
}




/*****************************************************************************
 Prototype    : EepromWriteByte
 Description  : Eepromֱ��д��һ���ֽ� û����ȷ��д�������鲻Ҫ�������
 Input        : addr: д����������ڵ�ַ
                dat ��д�������
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void EepromWriteByte ( unsigned int addr,unsigned char dat )
{
	unsigned char edat = 0;

#if TestEeprom!=TRUE  //����ʱȫд
	RWEepromStart();
	edat=* ( &EepromAddress+addr );
	RWEepromEnd();
	if ( edat!=dat )
#endif
	{
		RWEepromStart();
		* ( &EepromAddress+addr ) =dat;
		RWEepromEnd();
	}

}

/*****************************************************************************
 Prototype    : EepromSaveAllData
 Description  : ���桢������������
 Input        : writeOnWhichArea:�������ĸ�����
                isSaveAllOnThisArea:�Ƿ񱣴浽��ǰ����

 Output       : None
 Return Value : None
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void  EepromSaveAllData ( unsigned char writeOnWhichArea,unsigned char isSaveAllOnThisArea )
{
	//ֻ�ڵ�ǰ��������������ݺ���
	if ( isSaveAllOnThisArea!=TRUE )
	{

		EepromWriteByte ( EepromStrogeValue*2*writeOnWhichArea,1 ); //��ǰ��д���������Ϊ1

		//��һ��д�������־Ϊд�� =EepromMaxAreaCount
		//�����ǰ�����ǵ�һ��0������һ��ΪEepromLastArea�������
		EepromWriteByte ( EepromStrogeValue*2* ( writeOnWhichArea!=0?writeOnWhichArea-1:EepromLastArea ), EepromMaxAreaCount );
#if TestEeprom==TRUE
		EepromUser->EepromTestArea[writeOnWhichArea]=1;
		EepromUser->EepromTestArea[writeOnWhichArea!=0?writeOnWhichArea-1:EepromLastArea]=EepromMaxAreaCount;
#endif
	}
	EepromUser->SaveAllDataOnUserHandle ( writeOnWhichArea );

}


/*****************************************************************************
 Prototype    : EepromWrite
 Description  : Eepromд���ݲ���
 Input        : unsigned int  addr 
                unsigned char dat  
                unsigned char isSaveAll 
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2021/7/4
    Author       : Dragon
    Modification : Created function

*****************************************************************************/
void EepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll )
{
	struct findOnArea fOA;
	unsigned char isN;

	FindOnWhichArea ( &fOA );
	if ( fOA.whichhArea<EepromMaxArea )
	{
		if ( fOA.AreaCount>EepromMaxAreaCount-2 )
		{
			if ( fOA.whichhArea==EepromLastArea ) //���һ�������Ѿ�д�������ˣ���ת����һ������
			{
				fOA.whichhArea=0;
			}
			else
			{
				fOA.whichhArea++;    //�������һ��������ת���¸�����
			}
			isN=TRUE; //����д�� ʵ��д�����EepromMaxAreaCount-1�� ���ݸ��µ��¸�����
		}
		else
		{
			isN=FALSE;//����ַ����д������
		}

	}
	else
	{
		fOA.whichhArea=0; //�����������ݸ��µ�����0
		isN=TRUE;
	}

	if ( isN==TRUE )  //��ǰ������д�����������������㣬��Ҫһ��������������
	{
		
#if TestEeprom==TRUE
		EepromUser->EepromTestWriteCount++;
#endif
         EepromSaveAllData ( fOA.whichhArea,FALSE );
	}
	else
	{
#if TestEeprom==TRUE
		EepromUser->EepromTestWriteCount++;
#endif	
	  //д�ڵ�ǰ����
	  
		/*************************��ʵ��ŵĵ�ַ*******************
		 *   (EepromStrogeValue*2*OnArea_N)+(addr*2)              *
		 *  ����������1��FD_TimeAddr�ĵ�ַ��(15*2*1)+(2*2)=34              *
		 *  оƬEeprom��ʼ��ַ�Ѿ�������д���ݺ���                                *
		 **********************************************************/
		EepromWriteByte ( EepromStrogeValue*2*fOA.whichhArea+AreaAddr*2,++fOA.AreaCount ); //����������д�����Ӵ���

		if ( isSaveAll ) //�ڵ�ǰ��������������
		{

			EepromSaveAllData ( fOA.whichhArea,TRUE );
		}

		//�ڵ�ǰ�����¸�������
		else
		{
			EepromWriteByte ( EepromStrogeValue*2*fOA.whichhArea+addr*2,dat );
		}


	}

}



