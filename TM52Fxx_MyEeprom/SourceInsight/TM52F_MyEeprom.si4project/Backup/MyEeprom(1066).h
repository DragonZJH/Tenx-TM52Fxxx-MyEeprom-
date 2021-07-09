/******************************************************************************

  Copyright (C), 2015-2020, xxx Co., Ltd.

 ******************************************************************************
  File Name     : MyEeprom.h
  Version       : Initial Draft
  Author        : Dragon
  Created       : 2021/7/6
  Last Modified :
  Description   : 用户接口头文件
                  根据芯片，用户需要保存的数据，修改相应的参数
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

//需要用户定义修改
#define ChipEepromRamCP  256      //芯片EepromRam的容量大小
#define EepromStartAddr  0XEE00      //这个芯片EEPROM物理内存中起始地址
#define TM52FE8276_TotalEeprom  128  //这个芯片总共可以使用的EEPROM字节
#define TestEeprom  TRUE          //测试Eeprom时置TRUE  


//一般用户不用修改
#define EepromMaxAreaCount    100 //每个区最大写入数据次数
#define EepromStrogeValue    LastItemAddr // Eeprom 128字节中每组数据个数 
#define EepromMaxArea         (unsigned char)(TM52FE8276_TotalEeprom/(EepromStrogeValue*2))// Eeprom 128字节最大分组数 128/15*2 =4
#define EepromLastArea        (EepromMaxArea-1) //Eeprom最后一个分区
#define OutOfTheArea        (EepromMaxArea+2) //超出写区域范围  



enum AllEepromAddr  {

  AreaAddr=0,//标志区域的字节，放在第一位

  //用户自定义数据保存的地址
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
  

  LastItemAddr,//最后一个数据地址 这个项必须放最后
  
  /*************************真实存放的地址*********************************
   ****   (EepromStartAddr+EepromStrogeValue*2*OnArea_N)+(addr*2)   *******
   ****如存放在区域1中FD_TimeAddr的地址：(0xEE00+15*2*1)+(2*2)=0XEE00+34****
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
