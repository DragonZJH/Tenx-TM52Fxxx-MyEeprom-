
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
 #if TestEeprom==TRUE //����eeprom��
	unsigned char  EepromTestArea[4];  //��ǰEEPROM�������е�����
	unsigned long    EepromTestWriteCount; //��¼һ��д����ٴΣ�ע���¼���ǵ���д��������
#endif
  unsigned char*  EepromOnRamAddress; //Eeprom��Ram�еĵ�ַ����xdata 0XEE00
   eepromSaveData  *SaveAllDataOnUserHandle; 

 };


extern volatile struct eepromUser*   EepromUser;

extern void  EepromSetup(struct eepromUser* ee );
extern unsigned char EepromRead(unsigned int Adr);
extern unsigned char EepromReadWithArea ( unsigned int addr,unsigned char area );
extern void EepromWrite(unsigned int  addr,unsigned char dat,unsigned char isSaveAll);
extern void EepromWriteByte ( unsigned int addr,unsigned char dat );

#endif
