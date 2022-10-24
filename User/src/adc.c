/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: adc.c
Author: LBJ
Description: ADC采集应用程序文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/1      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

#include "stm8l15x.h"
#include "gpio.h"

/*----------------------------------------------------------
ModuleName: MX_ADC_DeInit
Description: ADC反初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_ADC_DeInit(void)
{
  //ADC采集口设置  低速低电平 推挽输出
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //内部参考电压关闭
  ADC_VrefintCmd(DISABLE); 
  //ADC不使能
  ADC_Cmd(ADC1,DISABLE);
  //关闭ADC1时钟
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
}

/*----------------------------------------------------------
ModuleName: MX_ADC_Init
Description: ADC初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_ADC_Init(void)
{ 
  //开启ADC1时钟
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  //ADC采集IO初始化  浮空输入 无中断
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //使能内部参考电压
  ADC_VrefintCmd(ENABLE); 
  //ADC通道配置
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);//连续转换，12位，转换时钟1分频
  //使能ADC通道
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint,ENABLE );
  //ADC使能
  ADC_Cmd(ADC1,ENABLE);
}

/*----------------------------------------------------------
ModuleName: ADC_Read_Internal_Voltage
Description: ADC采集内部电压
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint16_t ADC_Read_Internal_Voltage(void)
{
  int ADCValue;
  //ADC通道采集使能关闭
  ADC_Cmd(ADC1,DISABLE);
  //使能内部参考电压
  ADC_VrefintCmd(ENABLE);
  //连续转换，12位，转换时钟1分频
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  //使能内部参考电压通道
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);
  //ADC通道采集使能
  ADC_Cmd(ADC1,ENABLE);
  //开启软件转换
  ADC_SoftwareStartConv(ADC1); 
  //等待转换结束
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
  //清除对应标志
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  //获取转换值
  ADCValue = ADC_GetConversionValue(ADC1); 
  //关闭通道电压
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,DISABLE);
  return ADCValue;
}

/*----------------------------------------------------------
ModuleName: ADC_Read_VFB
Description: ADC采集反馈电压
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint16_t ADC_Read_VFB(void)
{
  int ADCValue;
  //关闭内部参考电压
  ADC_VrefintCmd(DISABLE);
  //ADC通道采集使能关闭
  ADC_Cmd(ADC1,DISABLE);
   //ADC采集IO初始化  浮空输入 无中断
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //连续转换，12位，转换时钟1分频
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  //使能通道7电压
  ADC_ChannelCmd(ADC1,ADC_Channel_7,ENABLE);
  //ADC使能
  ADC_Cmd(ADC1,ENABLE);
  //开启软件转换
  ADC_SoftwareStartConv(ADC1); 
  //等待转换结束
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
  //清除对应标志
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  //获取转换值
  ADCValue = ADC_GetConversionValue(ADC1); 
  //关闭通道7电压
  ADC_ChannelCmd(ADC1,ADC_Channel_7,DISABLE);
  return ADCValue;
}