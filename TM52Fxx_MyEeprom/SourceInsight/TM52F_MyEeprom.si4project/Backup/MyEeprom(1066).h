/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : MyEeprom.h
  Version       : Initial Draft
  Author        : Dragon
  Created       : 2021/7/6
  Last Modified :
  Description   : �û��ӿ�ͷ�ļ�
                  ����оƬ���û���Ҫ��������ݣ��޸���Ӧ�Ĳ���
  Function List :
  History       :
  1.Date        : 2021/7/6
    Author      : Dragon
    Modification: Created file

******************************************************************************/

#ifndef _MYEEPROM__H__
#define _MYEEPROM__H__
#include "Config.h"
#include "REGtenxTM52FE8276.h"
#include "tm52fe8276_bsp.h"
#include "Delay.h"



#define UseIAP413AS TRUE

//��Ҫ�û������޸�
#define ChipEepromRamCP  256      //оƬEepromRam��������С
#define EepromStartAddr  0XEE00      //���оƬEEPROM�����ڴ�����ʼ��ַ
#define TM52FE8276_TotalEeprom  128  //���оƬ�ܹ�����ʹ�õ�EEPROM�ֽ�
#define TestEeprom  TRUE          //����Eepromʱ��TRUE  


//һ���û������޸�
#define EepromMaxAreaCount    100 //ÿ�������д�����ݴ���
#define EepromStrogeValue    LastItemAddr // Eeprom 128�ֽ���ÿ�����ݸ��� 
#define EepromMaxArea         (unsigned char)(TM52FE8276_TotalEeprom/(EepromStrogeValue*2))// Eeprom 128�ֽ��������� 128/15*2 =4
#define EepromLastArea        (EepromMaxArea-1) //Eeprom���һ������
#define OutOfTheArea        (EepromMaxArea+2) //����д����Χ  



enum AllEepromAddr  {

  AreaAddr=0,//��־������ֽڣ����ڵ�һλ

  //�û��Զ������ݱ���ĵ�ַ
  FH_ModeAddr, 
  FD_TimeAddr,     
  SetTempAddr_L,
  SetTempAddr_H,
  SetHumidityAddr,
  SystemStartFlagAddr,
  WorkTimeAddr_L   ,
  WorkTimeAddr_H,
  FD_CountAdd_L   ,
  FD_CountAdd_H,
  CS_CountAddr_L ,
  CS_CountAddr_H,
  SetStableDukAddr_L ,
  SetStableDukAddr_H,
  

  LastItemAddr,//���һ�����ݵ�ַ ������������
  
  /*************************��ʵ��ŵĵ�ַ*********************************
   ****   (EepromStartAddr+EepromStrogeValue*2*OnArea_N)+(addr*2)   *******
   ****����������1��FD_TimeAddr�ĵ�ַ��(0xEE00+15*2*1)+(2*2)=0XEE00+34****
  *************************************************************************/
 
};



extern void MyEepromInit();
extern void MyEepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll );
extern void ReadSameDataFromEeprom();
#if TestEeprom==TRUE
extern void EepromTestHandle ( char* from,unsigned char flag );
extern volatile struct eepromUser xdata MyEeprom;
#endif

#endif
