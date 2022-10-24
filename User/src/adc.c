/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: adc.c
Author: LBJ
Description: ADC�ɼ�Ӧ�ó����ļ�
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/1      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

#include "stm8l15x.h"
#include "gpio.h"

/*----------------------------------------------------------
ModuleName: MX_ADC_DeInit
Description: ADC����ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_ADC_DeInit(void)
{
  //ADC�ɼ�������  ���ٵ͵�ƽ �������
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_Out_PP_Low_Slow);
  //�ڲ��ο���ѹ�ر�
  ADC_VrefintCmd(DISABLE); 
  //ADC��ʹ��
  ADC_Cmd(ADC1,DISABLE);
  //�ر�ADC1ʱ��
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
}

/*----------------------------------------------------------
ModuleName: MX_ADC_Init
Description: ADC��ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MX_ADC_Init(void)
{ 
  //����ADC1ʱ��
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  //ADC�ɼ�IO��ʼ��  �������� ���ж�
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //ʹ���ڲ��ο���ѹ
  ADC_VrefintCmd(ENABLE); 
  //ADCͨ������
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);//����ת����12λ��ת��ʱ��1��Ƶ
  //ʹ��ADCͨ��
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint,ENABLE );
  //ADCʹ��
  ADC_Cmd(ADC1,ENABLE);
}

/*----------------------------------------------------------
ModuleName: ADC_Read_Internal_Voltage
Description: ADC�ɼ��ڲ���ѹ
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint16_t ADC_Read_Internal_Voltage(void)
{
  int ADCValue;
  //ADCͨ���ɼ�ʹ�ܹر�
  ADC_Cmd(ADC1,DISABLE);
  //ʹ���ڲ��ο���ѹ
  ADC_VrefintCmd(ENABLE);
  //����ת����12λ��ת��ʱ��1��Ƶ
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  //ʹ���ڲ��ο���ѹͨ��
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);
  //ADCͨ���ɼ�ʹ��
  ADC_Cmd(ADC1,ENABLE);
  //�������ת��
  ADC_SoftwareStartConv(ADC1); 
  //�ȴ�ת������
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
  //�����Ӧ��־
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  //��ȡת��ֵ
  ADCValue = ADC_GetConversionValue(ADC1); 
  //�ر�ͨ����ѹ
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,DISABLE);
  return ADCValue;
}

/*----------------------------------------------------------
ModuleName: ADC_Read_VFB
Description: ADC�ɼ�������ѹ
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint16_t ADC_Read_VFB(void)
{
  int ADCValue;
  //�ر��ڲ��ο���ѹ
  ADC_VrefintCmd(DISABLE);
  //ADCͨ���ɼ�ʹ�ܹر�
  ADC_Cmd(ADC1,DISABLE);
   //ADC�ɼ�IO��ʼ��  �������� ���ж�
  GPIO_Init(ADC_GPIO_Port, ADC_Pin, GPIO_Mode_In_FL_No_IT);
  //����ת����12λ��ת��ʱ��1��Ƶ
  ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
  //ʹ��ͨ��7��ѹ
  ADC_ChannelCmd(ADC1,ADC_Channel_7,ENABLE);
  //ADCʹ��
  ADC_Cmd(ADC1,ENABLE);
  //�������ת��
  ADC_SoftwareStartConv(ADC1); 
  //�ȴ�ת������
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
  //�����Ӧ��־
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  //��ȡת��ֵ
  ADCValue = ADC_GetConversionValue(ADC1); 
  //�ر�ͨ��7��ѹ
  ADC_ChannelCmd(ADC1,ADC_Channel_7,DISABLE);
  return ADCValue;
}