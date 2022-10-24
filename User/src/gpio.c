/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: gpio.c
Author: LBJ
Description: GPIO功能定义文件
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
Description: IO口功能定义
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_GPIO_Init(void)
{
  
  //快速唤醒使能
  PWR_FastWakeUpCmd(ENABLE);  
  //所有IO输出低电平
  GPIO_Init(GPIOA, GPIO_Pin_All & ~(0x08), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_All & ~(0x07), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_All & ~(0x10), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_All & ~(0x8F), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_All & ~(0x30), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_All , GPIO_Mode_Out_PP_Low_Slow);
  
  //ADC初始化  浮空输入  无中断
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //moto使能口1初始化   低速高电平  推挽输出
  GPIO_Init(MOTO_EN1_GPIO_Port, MOTO_EN1_Pin, GPIO_Mode_Out_PP_High_Slow);
  //moto使能口2初始化   低速高电平  推挽输出
  GPIO_Init(MOTO_EN2_GPIO_Port, MOTO_EN2_Pin, GPIO_Mode_Out_PP_High_Slow);
  //moto输入2初始化     低速低电平  推挽输出
  GPIO_Init(MOTO_IN2_GPIO_Port, MOTO_IN2_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //moto输入1初始化     低速低电平  推挽输出
  GPIO_Init(MOTO_IN1_GPIO_Port, MOTO_IN1_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //晶振使能初始化      低速高电平  推挽输出
  GPIO_Init(TCXO_EN_GPIO_Port, TCXO_EN_Pin, GPIO_Mode_Out_PP_High_Slow);
  
  //初始化串口输入，浮空输入，带中断  
  GPIO_Init(RXD_GPIO_Port, RXD_Pin, GPIO_Mode_In_FL_IT);
  //初始化按键，上拉输入，带中断
  GPIO_Init(KEY_GPIO_Port, KEY_Pin, GPIO_Mode_In_PU_IT);
  //初始化传感器1，浮空输入，带中断
  GPIO_Init(SENSOR_INTERFERE_GPIO_Port, SENSOR_INTERFERE_Pin, GPIO_Mode_In_FL_IT);
  //初始化传感器2，浮空输入，带中断
  GPIO_Init(SENSOR_SIGNAL_GPIO_Port, SENSOR_SIGNAL_Pin, GPIO_Mode_In_FL_IT);
  //恢复中断的所有设置 
  EXTI_DeInit ();
  //外部中断0，上升下降沿触发，向量号8
  EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising_Falling);
  //外部中断1，下降沿触发，向量号9
  EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);
  //外部中断2，下降沿触发，向量号10
  EXTI_SetPinSensitivity (EXTI_Pin_2,EXTI_Trigger_Falling);
  //外部中断3，上升沿触发，向量号11       
  EXTI_SetPinSensitivity (EXTI_Pin_3,EXTI_Trigger_Rising);
  //超低功耗
  PWR_UltraLowPowerCmd(ENABLE);
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);
  //使能中断
  enableInterrupts();
}

/*----------------------------------------------------------
ModuleName: FastLowPowerInit
Description: 快速进入低功耗初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void FastLowPowerInit(void)
{
  
  PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能
  //拉低IO口
  GPIO_Init(SPINSS_GPIO_Port, SPINSS_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPICLK_GPIO_Port, SPICLK_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPIMOSI_GPIO_Port, SPIMOSI_Pin, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(SPIMISO_GPIO_Port, SPIMISO_Pin, GPIO_Mode_Out_PP_Low_Slow);
  PWR_UltraLowPowerCmd(ENABLE);//超低功耗
  
}