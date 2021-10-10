#ifndef __DEFINE_H__
#define __DEFINE_H__


#define TRUE   1
#define FALSE  0
#define NULL   0
#define LED_OFF    1
#define LED_ON     0
#define LED_Test    P0_7


#define     nop		    _nop_();


struct setData{
  unsigned int   SetTemp;
  unsigned int   SetTempBuf;
  unsigned char  SetHumidity;
  unsigned char  SetHumidityBuf;
  unsigned char  FD_Time;
  unsigned char  FD_TimeBuf;
  unsigned char  FH_Mode;


};

struct runData{
 unsigned int FD_Count; 
 unsigned int CS_Count; 
 unsigned int WorkTime_Second;
 unsigned int WorkTimeBuf;
 unsigned int   DukBuf_H;
  unsigned int SendDataToUartCount;
  unsigned int  WorkCount_10ms;
  unsigned char  WorkCount_1s;
 unsigned char WorkCount_60s;


};

extern struct runData RunData;
extern struct setData SetData;
extern bit SystemStart_Flag;
#endif
