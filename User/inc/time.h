/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: time.h
Author: LBJ
Description: 时钟相关程序声明
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/1      v0.0.1            LBJ            first version
-----------------------------------------------------------*/


#ifndef __TIME_H__
#define __TIME_H__


void Delay_5us(uint16_t nCount);
void Delay_ms(uint16_t nCount);
void Set_RTC_WakeUp(uint32_t time);
void Set_RTC_Alarm(uint32_t time);
void Set_RTC_Time(uint32_t time);
uint32_t Read_RTC_Time(void);
void CLK_Init(void);
void RTC_Config(void);



#endif /*__ TIME_H__ */









