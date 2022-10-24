/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: adc.h
Author: LBJ
Description: ADC功能声明文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/2      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__


/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */

void MX_ADC_Init(void);
void MX_ADC_DeInit(void);
uint16_t ADC_Read_Internal_Voltage(void);
uint16_t ADC_Read_VFB(void);

/* USER CODE END FP */
















#endif /*__ ADC_H__ */




