
#ifndef  __LED_H__
#define  __LED_H__
#include "DEFINE.H"



#define  sega	  0x01
#define  segb	  0x02
#define  segc	  0x04
#define  segd	  0x08
#define  sege	  0x10
#define  segf	  0x20
#define  segg	  0x40
#define  segdp	  0x80

#define  pwmL	  0x80
#define  fdLL	  0x80
#define  fdRL	  0x80



#define     ALL_SEG_OFF      0x00  
#define     SEG_ON           1
#define     SEG_OFF			 0
#define     SEG_DPON         segdp
#define     SEG_DPOFF        (~segdp)



//使用(TM1629C 16位8段) COM脚对应的地址
//设置温度
#define     LED_SetTemp_COM1     0
#define     LED_SetTemp_COM2     2
#define     LED_SetTemp_COM3     4

//实时温度
#define     LED_RealTemp_COM1    6
#define     LED_RealTemp_COM2    8
#define     LED_RealTemp_COM3    10
#define     LED_RealTemp_COM4    12

//设置周期
#define     LED_SetFDTime_COM1       14


//设置湿度
#define     LED_SetHumidity_COM1       1
#define     LED_SetHumidity_COM2       3


//实时湿度
#define     LED_RealHumidity_COM1  5
#define     LED_RealHumidity_COM2  7

//剩余时间
#define     LED_FDTimeLeft_COM1   9
#define     LED_FDTimeLeft_COM2   11
#define     LED_FDTimeLeft_COM3   13

//模式LED灯
#define     LED_ModeLightShow_All     15




extern unsigned char code Number[];

extern unsigned char idata LEDShow[16];


#endif
