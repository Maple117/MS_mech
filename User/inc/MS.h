/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: MS.h
Author: LBJ
Description: MSӦ�ó����������
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
  //�洢������Чλ-��ʼλ
  u8 SaveFlag;         
   //MS�汾��
  u8 Versions;         
   //MSID
  u32 MSID;   
  //MSֵ
  u32 MSV; 
  //MSʱ��
  u32 MST;              
  //�洢����У��λ-����λ
  u8 SaveCheck;         
   //MS���Ŵ�����־
  u8 Tamper;        
  //MS�͵���������־
  u8 Battery;        
  //MS���͵͵���������־
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












