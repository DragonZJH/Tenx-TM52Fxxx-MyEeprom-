



 /**************************** MyEeprom 模块概述**********************************
*********************************************************************************
    TM52Fxx系列芯片 内部EEPROM擦写次数为最小大于5万次,即单个地址的擦写次数是有限的
 随着擦写次数的增加每次擦写的时间会有所延长，虽然对于一般不需要经常操作EEPROM的应
 用5-10万的擦写次数完全可以满足整个芯片使用周期，但有些应用需要更加频繁的保存/读取
 数据则需要芯片能够提供足够多的擦写次数。本DEMO主要为了有以上需要的朋友们提供一个很
 好的解决方案.
    DEMO的大致原理就是在芯片提供的整个EEPROM空间内，根据用户需要保存数据的大小,自动
 分配N个区域,根据数据写入的次数动态地把用户数据保存在这些区域中，在芯片使用周期内确
 保整个EEPROM空间使用率得到很好的平衡。理论上可以就达到了延长芯片EEPROM的使用寿命的
 目的，使用寿命将延长至原来的xN(区域)倍。经压力测试每秒擦写20-30次保存15字节数据擦写
 次数已经超过18万次多，数据没有发现异常.

 模块配置使用步骤----MyEeprom.h头文件
     1：根据提示修改以下宏定义     
        #define ChipEepromRamCP  256	   //芯片EepromRam的容量大小
        #define EepromStartAddr  0XEE00	  //这个芯片EEPROM物理内存中起始地址
        #define TM52FE8276_TotalEeprom  128  //这个芯片总共可以使用的EEPROM字节，TM52f82xx芯片偶地址有效

     2：在枚举项@AllEepromAddr加入你需要保存数据的地址名称
        注意枚举里的前后两个项不能更改

     3：如果需要调试或其他测试，设置宏定义 TestEeprom  为TRUE  
     
 模块配置使用步骤----MyEeprom.c文件
     1：在工程初始化调用函数-->MyEepromInit()

     2：根据提示修改此接口函数内部-->MyEepromSaveAllDataUserHandle (  )

     3：单个数据写调用-->MyEepromWrite ( unsigned int,unsigned char ,unsigned char )
        单个数据读调用-->MyEepromRead(unsigned char)

        测试时接口--    >MyEepromTestHandle ( char* from,unsigned char flag )
        

  其他：暂时只测试了十速TM52FE8273芯片，根据数据手册其他TM52FEXX系列应该也是可以通用的
       移植到到其他只需需该修改部分代码，后续有时间继续更新 
       
 感兴趣的朋友可以关注我github中的此开源项目提供宝贵意见
 仓库地址：https://github.com/DragonZJH/Tenx-TM52Fxxx-MyEeprom-       

 ******************************************************************************
  File Name     : MyEeprom.h
  Version       : Initial Draft
  Author        : Dragon8814
  Created       : 2021/7/6
  Last Modified :
  Description   : 用户接口头文件
                  根据芯片，用户需要保存的数据，修改相应的参数
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



//需要用户定义修改
#define ChipEepromRamCP  256      //芯片EepromRam的容量大小
#define EepromStartAddr  0XEE00      //这个芯片EEPROM物理内存中起始地址
#define TM52FE8276_TotalEeprom  128  //这个芯片总共可以使用的EEPROM字节，TM52f82xx芯片偶地址有效

//用户初期调试测试查看数据用   //测试Eeprom时置TRUE --- 后期设为FALSE即可
#define TestEeprom  TRUE           


//用户需要设置保存数据的地址
enum AllEepromAddr  {

  AreaAddr=0,//标志区域的字节，放在第一位不能改变位置

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
  

  LastItemAddr,//最后一个数据地址 这个项必须放最后不能改变位置
  
  /*************************真实存放的地址*********************************
   ****   (EepromStartAddr+EepromStrogeValue*2*OnArea_N)+(addr*2)   *******
   ****如存放在区域1中FD_TimeAddr的地址：(0xEE00+15*2*1)+(2*2)=0XEE00+34****
  *************************************************************************/
 
};




//一般用户不用修改
#define EepromMaxAreaCount    100 //每个区最大写入数据次数
#define EepromStrogeValue    LastItemAddr // Eeprom 128字节中每组数据个数 
#define EepromMaxArea         (unsigned char)(TM52FE8276_TotalEeprom/(EepromStrogeValue*2))// Eeprom 128字节最大分组数 128/15*2 =4
#define EepromLastArea        (EepromMaxArea-1) //Eeprom最后一个分区
#define OutOfTheArea        (EepromMaxArea+2) //超出写区域范围  




extern void MyEepromInit();
extern void MyEepromWrite ( unsigned int  addr,unsigned char dat,unsigned char isSaveAll );
extern unsigned char MyEepromRead(unsigned char addr);
extern void ReadSameDataFromEeprom();
#if TestEeprom==TRUE
extern void MyEepromTestHandle ( char* from,unsigned char flag );
#endif
extern volatile struct eepromUser xdata MyEeprom;


#endif
