
#ifndef __SYSDATA_H_
#define __SYSDATA_H_

#include "stm8l15x.h"

#define EEPROM_ADDR 0x1030
#define EEPROM_ADDR_BANK1 EEPROM_ADDR
#define EEPROM_ADDR_BANK2 EEPROM_ADDR + 50

#define EEPROM_DATA_LEN 41

#define TIME_MIFENG 187200
#define TIME_2HOURS 7200

#define ZENNER_TIME_2HOURS 7200
#define ZENNER_CHANGE_TIME (12 + 1)


#define ZENNER_OPEN 0
#define ZENNER_CLOSE 1




uint8_t Zenner_Valve_State(void);
uint8_t Zenner_Valve_Work(uint8_t da);
void Fast_Low_Power(void);
void Zenner_Work_State_Init(void);
void Zenner_Data_Save(void);
void Zenner_Data_Read(void);










typedef struct
{
  /**************************************************************/
  u8 SaveStartFlag;          //存储数据有效位-起始位
  
  u8 DLFlag1;           //超流量-条件1-触发标志
  u16 DLSecSet1;        //超流量-条件1-检测间隔计时
  u16 DLTimesSet1;      //超流量-条件1-检测次数设置
  u8 DLFlag2;           //超流量-条件2-触发标志
  u16 DLSecSet2;        //超流量-条件2-检测间隔计时
  u16 DLTimesSet2;      //超流量-条件2-检测次数设置
  u8 DL_Pulse;
  u8 DL_Pulse2;
  
  
  
  u8 XLFlag1;           //连续流量-条件1-触发标志
  u32 XLSecSet1;        //连续流量-条件1-检测间隔设置
  u8 XLTimesSet1;       //连续流量-条件1-检测次数设置
  u8 XLFlag2;           //连续流量-条件2-触发标志
  u32 XLSecSet2;        //连续流量-条件2-检测间隔设置
  u8 XLTimesSet2;       //连续流量-条件2-检测次数设置
  u8 XLCloseEnableFlag; //连续流量-报警关阀使能设置
  
  u8 TimeFlag;                  //安检-触发标志
  u32 TimeShengYuSec;           //安检-剩余时间
  u32 TimeBaoJingSec;           //安检-提醒时间
  u32 TimeFlagSendTimeSet;      //安检-循环报警发送时间设置
  u8 TimeFlagSendTimesSet;      //安检-循环报警发送次数设置
  
  u8 QMFlag;                    //气密性-触发标志
  u32 QMFlagSendTimeSet;        //气密性-循环报警发送时间设置
  u8 QMFlagSendTimesSet;        //气密性-循环报警发送次数设置
  
  u8 ValveStatus;               //阀门状态-0：开启 1：关闭
  u8 ValveErrorStatus;          //阀门故障状态-0：正常 1：堵转 2：断开
  
  u16 ValveData1;
  u16 ValveData2;
  
  u8 Reserved1;
  u8 Reserved2;
  
  u8 MeterFlag;         //炉具标识
  
  u8 SaveCheck;         //存储数据校验位-结束位

  /**************************************************************/
  
  /**************************************************************/
  u8 DLStartFlag1;       //超流量-条件1-检测开始标志
  u8 DLSecConut1;        //超流量-条件1-检测间隔计时
  u8 DLTimesCount1;      //超流量-条件1-检测次数计数
  u8 DLPulseCount1;      //超流量-条件1-单周期脉冲计数
  u8 DLChangeFlag1;      //超流量-条件1-提前启动充电标志
  u8 DLChangeTimeCount1; //超流量-条件1-提前充电超时计时
  
  u8 DLStartFlag2;        //超流量-条件2-检测开始标志
  u8 DLSecConut2;         //超流量-条件2-检测间隔计时
  u8 DLTimesCount2;       //超流量-条件2-检测次数计数
  u8 DLPulseCount2;       //超流量-条件2-单周期脉冲计数
  u8 DLChangeFlag2;       //超流量-条件2-提前启动充电标志
  u8 DLChangeTimeCount2;  //超流量-条件2-提前充电超时计时
  /**************************************************************/

  /**************************************************************/
  u32 XLSecConut1;        //连续流量-条件1-检测间隔计时
  u8 XLTimesCount1;       //连续流量-条件1-检测次数计数
  u8 XLStartFlag1;        //连续流量-条件1-检测启动标志
  u8 XLPulseCount1;       //连续流量-条件1-单周期脉冲计数
  
  u32 XLSecConut2;        //连续流量-条件2-检测间隔计时
  u8 XLTimesCount2;       //连续流量-条件2-检测次数计数
  u8 XLStartFlag2;        //连续流量-条件2-检测启动标志
  u8 XLPulseCount2;       //连续流量-条件2-单周期脉冲计数
  /**************************************************************/ 
  
  /**************************************************************/
  u32 TimeFlagSendTimeCount;//安检-循环报警发送时间计时
  u8 TimeFlagSendTimesCount;//安检-循环报警发送次数计数
  u16 TimeQuanTime;         //安检-空闲两小时计时
  u8 TimeBaoJingFlag;       //安检-提醒标志
  /**************************************************************/
  
  /**************************************************************/
  u32 QMFlagSendTimeCount;  //气密性-循环报警发送时间计时
  u8 QMFlagSendTimesCount;  //气密性-循环报警发送次数计数
  /**************************************************************/
  
  /**************************************************************/
  u8 ErrorLevel3Flag;       //等级3报警标志
  u8 ValveCheckErrorStatus; //阀门每日检测状态-0：正常 2：断开
  /**************************************************************/
  
}sZenner;
extern sZenner Zenner;






typedef struct
{
  unsigned char State;
  unsigned char Versions;
  unsigned char Tamper;
  unsigned char Battery;
  uint32_t MSID;
  uint32_t MSV;
  uint32_t MST;
  
  unsigned char DL_Sec;         //16
  unsigned char DL_Times;
  unsigned char DL_Times2;

  unsigned char XL_Enable;
  unsigned char XL_Times;
  unsigned char XL_Times2;
  unsigned int XL_Sec;
  unsigned int XL_Sec2;
  
  unsigned long TSY_Sec;      //28
  unsigned long TBJ_Sec;
  
  unsigned int Meter_Flag;      //36
  
  unsigned char aaa1;           //38
  
  unsigned char T_Month;
  unsigned char T_Day;
  unsigned char T_XyMonth;
  unsigned char T_Hour;
  
  uint16_t T_Sec;
  uint32_t TXL_Sec;
  uint32_t TXL_Sec2;
  
  unsigned char cDL_Times;
  unsigned char cXL_Times;
  unsigned char cXL_Times2;
  unsigned char cJiQuan;
  unsigned char cJiQuan2;
  unsigned char cMiFeng;
  
  uint32_t cQuanTime;
  
  uint32_t S1TIME;
  
  uint32_t SaveNum;
  
  unsigned char PVDBatteryFlag;
  unsigned char PVDLBatteryFlag1;
  unsigned char PVDLBatteryFlag2;
  unsigned char SendMode;
  
  unsigned char VTTime;
  unsigned char VTFlag;
  unsigned char TTTime;
  unsigned char TTFlag;
  
  unsigned char KeyTime;
  
  unsigned char cZlDelayTime;
  unsigned char cChargeDelayTime;
  unsigned char cCloseDelayTime;
  
  unsigned char cXL_StartFlag;
  unsigned char cXL_StartFlag2;
  
  unsigned char DLCloseFlag;
  unsigned char XLCloseFlag;
  unsigned char TIMECloseFlag;
  unsigned char cChangeDelayTime;
  
  unsigned char cDL_IDLETime;
  
  unsigned char cQuanTimeBufCount;
  unsigned char cQuanTimeBuf[10];
  
  unsigned char cQuanTimeBufI;
  unsigned int cQuanTimeBufSum;
  
  unsigned char TestOpenFlag;
  
  unsigned char SyadataError;
  
}SYSDATA_STRUCT;

typedef struct
{
  unsigned char send;
  unsigned char uart;
  
  unsigned char FastSleep;
  
  unsigned char send2min;
  
  uint32_t WhileTime;
  uint32_t WhileNum;
  
  unsigned char XLFlag;
  unsigned char DLFlag;
  unsigned char TimeFlag;
  unsigned char MFFlag;
  unsigned char MFFirstFlag;
  unsigned char CloseFlag;
  unsigned char ZlErrorFlag;
  
  unsigned char ErrorClose;
  unsigned char ErrorShort;
  
}SYSFLAG_STRUCT;

extern SYSDATA_STRUCT sysdata;
extern SYSFLAG_STRUCT sysflag;






#endif




