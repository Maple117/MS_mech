/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: gpio.c
Author: LBJ
Description: GPIO���ܶ����ļ�
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/25      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

/* Includes ---------------------------------------------*/
#include "gpio.h"
#include "stm8l15x.h"

/*----------------------------------------------------------
ModuleName: GPIO_Init
Description: IO�ڹ��ܶ���
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_GPIO_Init(void)
{
  
  //���ٻ���ʹ��
  PWR_FastWakeUpCmd(ENABLE);  
  //����IO����͵�ƽ
  GPIO_Init(GPIOA, GPIO_Pin_All & ~(0x08), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_All & ~(0x07), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_All & ~(0x10), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_All & ~(0x8F), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_All & ~(0x30), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_All , GPIO_Mode_Out_PP_Low_Slow);
  
  //ADC��ʼ��  ��������  ���ж�
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //motoʹ�ܿ�1��ʼ��   ���ٸߵ�ƽ  �������
  GPIO_Init(MOTO_EN1_GPIO_Port, MOTO_EN1_Pin, GPIO_Mode_Out_PP_High_Slow);
  //motoʹ�ܿ�2��ʼ��   ���ٸߵ�ƽ  �������
  GPIO_Init(MOTO_EN2_GPIO_Port, MOTO_EN2_Pin, GPIO_Mode_Out_PP_High_Slow);
  //moto����2��ʼ��     ���ٵ͵�ƽ  �������
  GPIO_Init(MOTO_IN2_GPIO_Port, MOTO_IN2_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //moto����1��ʼ��     ���ٵ͵�ƽ  �������
  GPIO_Init(MOTO_IN1_GPIO_Port, MOTO_IN1_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //����ʹ�ܳ�ʼ��      ���ٸߵ�ƽ  �������
  GPIO_Init(TCXO_EN_GPIO_Port, TCXO_EN_Pin, GPIO_Mode_Out_PP_High_Slow);
  
  //��ʼ���������룬�������룬���ж�  
  GPIO_Init(RXD_GPIO_Port, RXD_Pin, GPIO_Mode_In_FL_IT);
  //��ʼ���������������룬���ж�
  GPIO_Init(KEY_GPIO_Port, KEY_Pin, GPIO_Mode_In_PU_IT);
  //��ʼ��������1���������룬���ж�
  GPIO_Init(SENSOR_INTERFERE_GPIO_Port, SENSOR_INTERFERE_Pin, GPIO_Mode_In_FL_IT);
  //��ʼ��������2���������룬���ж�
  GPIO_Init(SENSOR_SIGNAL_GPIO_Port, SENSOR_SIGNAL_Pin, GPIO_Mode_In_FL_IT);
  //�ָ��жϵ��������� 
  EXTI_DeInit ();
  //�ⲿ�ж�0�������½��ش�����������8
  EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising_Falling);
  //�ⲿ�ж�1���½��ش�����������9
  EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);
  //�ⲿ�ж�2���½��ش�����������10
  EXTI_SetPinSensitivity (EXTI_Pin_2,EXTI_Trigger_Falling);
  //�ⲿ�ж�3�������ش�����������11       
  EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Rising);
  //���͹���
  PWR_UltraLowPowerCmd(ENABLE);
  //����жϱ�־
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);
  //����жϱ�־
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);
  //ʹ���ж�
  enableInterrupts();
}

/*----------------------------------------------------------
ModuleName: FastLowPowerInit
Description: ���ٽ���͹��ĳ�ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void FastLowPowerInit(void)
{
  
  PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
  //����IO��
  GPIO_Init(SPINSS_GPIO_Port, SPINSS_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPICLK_GPIO_Port, SPICLK_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPIMOSI_GPIO_Port, SPIMOSI_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPIMISO_GPIO_Port, SPIMISO_Pin, GPIO_Mode_Out_PP_Low_Slow);
  PWR_UltraLowPowerCmd(ENABLE);//���͹���
  
}