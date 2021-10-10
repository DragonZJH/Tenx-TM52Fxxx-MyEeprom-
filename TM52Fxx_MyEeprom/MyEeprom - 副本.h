



 /**************************** MyEeprom ģ�����**********************************
*********************************************************************************
    TM52Fxxϵ��оƬ �ڲ�EEPROM��д����Ϊ��С����5���,��������ַ�Ĳ�д���������޵�
 ���Ų�д����������ÿ�β�д��ʱ��������ӳ�����Ȼ����һ�㲻��Ҫ��������EEPROM��Ӧ
 ��5-10��Ĳ�д������ȫ������������оƬʹ�����ڣ�����ЩӦ����Ҫ����Ƶ���ı���/��ȡ
 ��������ҪоƬ�ܹ��ṩ�㹻��Ĳ�д��������DEMO��ҪΪ����������Ҫ���������ṩһ����
 �õĽ������.
    DEMO�Ĵ���ԭ�������оƬ�ṩ������EEPROM�ռ��ڣ������û���Ҫ�������ݵĴ�С,�Զ�
 ����N������,��������д��Ĵ�����̬�ذ��û����ݱ�������Щ�����У���оƬʹ��������ȷ
 ������EEPROM�ռ�ʹ���ʵõ��ܺõ�ƽ�⡣�����Ͽ��Ծʹﵽ���ӳ�оƬEEPROM��ʹ��������
 Ŀ�ģ�ʹ���������ӳ���ԭ����xN(����)������ѹ������ÿ���д20-30�α���15�ֽ����ݲ�д
 �����Ѿ�����18��ζ࣬����û�з����쳣.

 ģ������ʹ�ò���----MyEeprom.hͷ�ļ�
     1��������ʾ�޸����º궨��     
        #define ChipEepromRamCP  256	   //оƬEepromRam��������С
        #define EepromStartAddr  0XEE00	  //���оƬEEPROM�����ڴ�����ʼ��ַ
        #define TM52FE8276_TotalEeprom  128  //���оƬ�ܹ�����ʹ�õ�EEPROM�ֽڣ�TM52f82xxоƬż��ַ��Ч

     2����ö����@AllEepromAddr��������Ҫ�������ݵĵ�ַ����
        ע��ö�����ǰ��������ܸ���

     3�������Ҫ���Ի��������ԣ����ú궨�� TestEeprom  ΪTRUE  
     
 ģ������ʹ�ò���----MyEeprom.c�ļ�
     1���ڹ��̳�ʼ�����ú���-->MyEepromInit()

     2��������ʾ�޸Ĵ˽ӿں����ڲ�-->MyEepromSaveAllDataUserHandle (  )

     3����������д����-->MyEepromWrite ( unsigned int,unsigned char ,unsigned char )
        �������ݶ�����-->MyEepromRead(unsigned char)

        ����ʱ�ӿ�--    >MyEepromTestHandle ( char* from,unsigned char flag )
        

  ��������ʱֻ������ʮ��TM52FE8273оƬ�����������ֲ�����TM52FEXXϵ��Ӧ��Ҳ�ǿ���ͨ�õ�
       ��ֲ��������ֻ������޸Ĳ��ִ��룬������ʱ��������� 
       
 ����Ȥ�����ѿ��Թ�ע��github�еĴ˿�Դ��Ŀ�ṩ�������
 �ֿ��ַ��https://github.com/DragonZJH/Tenx-TM52Fxxx-MyEeprom-       

 ******************************************************************************
  File Name     : MyEeprom.h
  Version       : Initial Draft
  Author        : Dragon8814
  Created       : 2021/7/6
  Last Modified :
  Description   : �û��ӿ�ͷ�ļ�
                  ����оƬ���û���Ҫ��������ݣ��޸���Ӧ�Ĳ���
  Function List :
  History       :
  1.Date        : 2021/7/6
    Author      : Dragon8814
    Modification: Created file

******************************************************************************/

#ifndef _MYEEPROM__H__
#define _MYEEPROM__H__
#include "Config.h"
#include "REGtenxTM52FE8276.h"
//#include "tm52fe8276_bsp.h"
#include "Delay.h"



//��Ҫ�û������޸�
#define ChipEepromRamCP  256      //оƬEepromRam��������С
#define EepromStartAddr  0XEE00      //���оƬEEPROM�����ڴ�����ʼ��ַ
#define TM52FE8276_TotalEeprom  128  //���оƬ�ܹ�����ʹ�õ�EEPROM�ֽڣ�TM52f82xxоƬż��ַ��Ч

//�û����ڵ��Բ��Բ鿴������   //����Eepromʱ��TRUE --- ������ΪFALSE����
#define TestEeprom  TRUE           


//�û���Ҫ���ñ������ݵĵ�ַ
enum AllEepromAddr  {

  AreaAddr=0,//��־������ֽڣ����ڵ�һλ���ܸı�λ��

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
  

  LastItemAddr,//���һ�����ݵ�ַ �������������ܸı�λ��
  
  /*************************��ʵ��ŵĵ�ַ*********************************
   ****   (EepromStartAddr+EepromStrogeValue*2*OnArea_N)+(addr*2)   *******
   ****����������1��FD_TimeAddr�ĵ�ַ��(0xEE00+15*2*1)+(2*2)=0XEE00+34****
  *************************************************************************/
 
};




//һ���û������޸�
#define EepromMaxAreaCount    100 //ÿ�������д�����ݴ���
#define EepromStrogeValue    LastItemAddr // Eeprom 128�ֽ���ÿ�����ݸ��� 
#define EepromMaxArea         (unsigned char)(TM52FE8276_TotalEeprom/(EepromStrogeValue*2))// Eeprom 128�ֽ��������� 128/15*2 =4
#define EepromLastArea        (EepromMaxArea-1) //Eeprom���һ������
#define OutOfTheArea        (EepromMaxArea+2) //����д����Χ  




extern void MyEepromInit();
extern void MyEepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll );
extern unsigned char MyEepromRead(unsigned char addr);
extern void ReadSameDataFromEeprom();
#if TestEeprom==TRUE
extern void MyEepromTestHandle ( char* from,unsigned char flag );
#endif
extern volatile struct eepromUser xdata MyEeprom;


#endif
