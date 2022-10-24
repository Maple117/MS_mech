/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: time.c
Author: LBJ
Description: ʱ����س����ļ�
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
Description: ����RTC����ʱ������
Params: uint32_t time
Return: null
Notice: null
-----------------------------------------------------------*/
void Set_RTC_WakeUp(uint32_t time)
{
  RTC_WakeUpCmd(DISABLE);
  //32.768K/16=2.048k=0.488ms  
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  //�����ж�
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //2040/60=34Min
  RTC_SetWakeUpCounter(time);
  //���Ѷ�ʱ���ж�ʹ��
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  //RTC����ʹ��
  RTC_WakeUpCmd(ENABLE);
}

/*----------------------------------------------------------
ModuleName: Set_RTC_Alarm
Description: ����RTC����ʱ��
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
Description: ����RTCʱ��
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
Description: ����RTCʱ��
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
Description: RTC���ü���ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void RTC_Config(void)
{
  //ʹ��LSE
  CLK_LSEConfig(CLK_LSE_ON);
  //RTCʱ��ԴLSE��1��Ƶ=32.768K
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  //�ȴ�LSE����
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  //RTCʱ��ʹ��
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  //�ȴ�RTCͬ��
  RTC_WaitForSynchro();
  //������ӱ�־
  RTC_ClearITPendingBit(RTC_IT_ALRA);
  //�����ж�
  RTC_ITConfig(RTC_IT_ALRA,ENABLE);
  
}

/*----------------------------------------------------------
ModuleName: CLK_Init
Description: �ڲ������ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void CLK_Init(void)
{
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  // 16M�ڲ�RC��8��Ƶ��ϵͳʱ��Ϊ2M 
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);         
}

/*----------------------------------------------------------
ModuleName: Delay_5us
Description: ΢�뼶��ʱ
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
Description: ���뼶��ʱ
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
