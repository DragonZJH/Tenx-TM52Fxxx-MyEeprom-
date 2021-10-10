
#ifndef __TM52F82XX_Eeprom__H__
#define __TM52F82XX_Eeprom__H__
#include "config.h"
#include "MyEeprom.h"
#include "REGtenxTM52FE8276.h"
//#include "tm52fe8276_bsp.h"
//#include "DEFINE.H"

#include "Delay.h"

typedef void  eepromSaveData(unsigned char);

struct findOnArea     
{
   unsigned char whichhArea; 
   unsigned char AreaCount;
};



 struct eepromUser{
 #if TestEeprom==TRUE //测试eeprom用
	unsigned char  EepromTestArea[4];  //当前EEPROM操作运行的区域
	unsigned long    EepromTestWriteCount; //记录一共写入多少次，注意记录的是调用写函数次数
#endif
  unsigned char*  EepromOnRamAddress; //Eeprom在Ram中的地址如在xdata 0XEE00
   eepromSaveData  *SaveAllDataOnUserHandle; 

 };


extern volatile struct eepromUser*   EepromUser;

extern void  EepromSetup(struct eepromUser* ee );
extern unsigned char EepromRead(unsigned int Adr);
extern unsigned char EepromReadWithArea ( unsigned int addr,unsigned char area );
extern void EepromWrite(unsigned int  addr,unsigned char dat,unsigned char isSaveAll);
extern void EepromWriteByte ( unsigned int addr,unsigned char dat );

#endif
