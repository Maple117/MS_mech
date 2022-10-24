/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: MS.h
Author: LBJ
Description: MS应用程序相关声明
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/8      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
#ifndef __MS_H_
#define __MS_H_

/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */
typedef struct
{ 
  //存储数据有效位-起始位
  u8 SaveFlag;         
   //MS版本号
  u8 Versions;         
   //MSID
  u32 MSID;   
  //MS值
  u32 MSV; 
  //MS时间
  u32 MST;              
  //存储数据校验位-结束位
  u8 SaveCheck;         
   //MS干扰触发标志
  u8 Tamper;        
  //MS低电量报警标志
  u8 Battery;        
  //MS超低低电量报警标志
  u8 LBattery;          
}sMS;
extern sMS MS;
/* USER CODE END V */




/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */
void MS_Data_Save(void);
void MS_Data_Read(void);
void MS_Zenner_Pulse_Logic(void);
void MSSend_And_Save(void);
void MSSend_Continuous(void);
void Get_Interference_Flag(void);
void Get_Signal(void);
void Get_Interference(void);
/* USER CODE END FP */




#endif












