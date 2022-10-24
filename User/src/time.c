/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: time.c
Author: LBJ
Description: 时钟相关程序文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/1      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
/* Includes ---------------------------------------------*/
#include "stm8l15x.h"
#include "time.h"

/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */

/* USER CODE END V */


/*----------------------------------------------------------
ModuleName: Set_RTC_WakeUp
Description: 设置RTC唤醒时间周期
Params: uint32_t time
Return: null
Notice: null
-----------------------------------------------------------*/
void Set_RTC_WakeUp(uint32_t time)
{
  RTC_WakeUpCmd(DISABLE);
  //32.768K/16=2.048k=0.488ms  
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  //开启中断
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //2040/60=34Min
  RTC_SetWakeUpCounter(time);
  //唤醒定时器中断使能
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //RTC唤醒使能
  RTC_WakeUpCmd(ENABLE);
}

/*----------------------------------------------------------
ModuleName: Set_RTC_Alarm
Description: 设置RTC闹钟时间
Params: uint32_t time
Return: null
Notice: null
-----------------------------------------------------------*/
void Set_RTC_Alarm(uint32_t time)
{
  
  RTC_AlarmCmd(DISABLE);
  RTC_AlarmTypeDef RTC_AlarmType;
  RTC_AlarmStructInit(&RTC_AlarmType);
  RTC_AlarmType.RTC_AlarmTime.RTC_Hours=time/ 3600;
  RTC_AlarmType.RTC_AlarmTime.RTC_Minutes=time/ 60 % 60;
  RTC_AlarmType.RTC_AlarmTime.RTC_Seconds=time% 60;
  RTC_AlarmType.RTC_AlarmTime.RTC_H12=RTC_H12_AM;
  RTC_AlarmType.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay;
  RTC_AlarmType.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmType.RTC_AlarmDateWeekDay=2;
  RTC_SetAlarm(RTC_Format_BIN,&RTC_AlarmType);
  RTC_AlarmCmd(ENABLE);
}

/*----------------------------------------------------------
ModuleName: Set_RTC_Time
Description: 设置RTC时间
Params: uint32_t time
Return: null
Notice: null
-----------------------------------------------------------*/
void Set_RTC_Time(uint32_t time)
{
  RTC_TimeTypeDef RTC_TimeType;
  RTC_TimeStructInit(&RTC_TimeType);
  RTC_TimeType.RTC_Hours=time/3600;
  RTC_TimeType.RTC_Minutes=time/60%60;
  RTC_TimeType.RTC_Seconds=time%60;
  RTC_TimeType.RTC_H12=RTC_H12_AM;
  RTC_SetTime(RTC_Format_BIN,&RTC_TimeType);
}

/*----------------------------------------------------------
ModuleName: Read_RTC_Time
Description: 设置RTC时间
Params: uint32_t time
Return: null
Notice: null
-----------------------------------------------------------*/
uint32_t Read_RTC_Time(void)
{
  uint32_t time = 0;
  RTC_TimeTypeDef RTC_TimeRead;
  RTC_GetTime(RTC_Format_BIN,&RTC_TimeRead);
  time = RTC_TimeRead.RTC_Hours;
  time *= 3600;
  time += RTC_TimeRead.RTC_Minutes * 60;
  time += RTC_TimeRead.RTC_Seconds;
  return time;
}

/*----------------------------------------------------------
ModuleName: RTC_Config
Description: RTC配置及初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void RTC_Config(void)
{
  //使能LSE
  CLK_LSEConfig(CLK_LSE_ON);
  //RTC时钟源LSE，1分频=32.768K
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  //等待LSE就绪
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  //RTC时钟使能
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  //等待RTC同步
  RTC_WaitForSynchro();
  //清除闹钟标志
  RTC_ClearITPendingBit(RTC_IT_ALRA);
  //开启中断
  RTC_ITConfig(RTC_IT_ALRA,ENABLE);
  
}

/*----------------------------------------------------------
ModuleName: CLK_Init
Description: 内部晶振初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void CLK_Init(void)
{
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  // 16M内部RC经8分频后系统时钟为2M 
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);         
}

/*----------------------------------------------------------
ModuleName: Delay_5us
Description: 微秒级延时
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Delay_5us(uint16_t nCount)
{
  while (nCount--);
}

/*----------------------------------------------------------
ModuleName: Delay_ms
Description: 毫秒级延时
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Delay_ms(uint16_t nCount)
{
  while (nCount--)
  {
    Delay_5us(200);
  }
}
