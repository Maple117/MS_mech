/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: gpio.h
Author: LBJ
Description: GPIO口功能声明文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/25      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__


/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */

#define ADC_Pin GPIO_Pin_7
#define ADC_GPIO_Port GPIOD
#define MOTO_EN1_Pin GPIO_Pin_4
#define MOTO_EN1_GPIO_Port GPIOE
#define MOTO_EN2_Pin GPIO_Pin_0
#define MOTO_EN2_GPIO_Port GPIOD
#define MOTO_IN1_Pin GPIO_Pin_3
#define MOTO_IN1_GPIO_Port GPIOD
#define MOTO_IN2_Pin GPIO_Pin_2
#define MOTO_IN2_GPIO_Port GPIOD
#define MOTO_CHECK_Pin GPIO_Pin_3
#define MOTO_CHECK_GPIO_Port GPIOE
#define MOTO_POWER_Pin GPIO_Pin_2
#define MOTO_POWER_GPIO_Port GPIOE
#define TCXO_EN_Pin  GPIO_Pin_4
#define TCXO_EN_GPIO_Port GPIOC
#define TXD_Pin  GPIO_Pin_2
#define TXD_GPIO_Port GPIOA
#define RXD_Pin  GPIO_Pin_3
#define RXD_GPIO_Port GPIOA
#define KEY_Pin  GPIO_Pin_0
#define KEY_GPIO_Port GPIOB
#define SENSOR_INTERFERE_Pin  GPIO_Pin_1
#define SENSOR_INTERFERE_GPIO_Port GPIOB
#define SENSOR_SIGNAL_Pin  GPIO_Pin_2
#define SENSOR_SIGNAL_GPIO_Port GPIOB
#define SPINSS_Pin  GPIO_Pin_3
#define SPINSS_GPIO_Port GPIOB
#define SPICLK_Pin  GPIO_Pin_5
#define SPICLK_GPIO_Port GPIOB
#define SPIMOSI_Pin  GPIO_Pin_6
#define SPIMOSI_GPIO_Port GPIOB
#define SPIMISO_Pin  GPIO_Pin_7
#define SPIMISO_GPIO_Port GPIOB
#define IIC_EN_Pin  GPIO_Pin_2
#define IIC_EN_GPIO_Port GPIOC
#define IIC_SCL_Pin  GPIO_Pin_1
#define IIC_SCL_GPIO_Port GPIOC
#define IIC_SDA_Pin  GPIO_Pin_0
#define IIC_SDA_GPIO_Port GPIOC


#define TCXO_EN(n)   GPIO_WriteBit(TCXO_EN_GPIO_Port, TCXO_EN_Pin, (BitAction)n)
#define MOTO_EN1(n)  GPIO_WriteBit(MOTO_EN1_GPIO_Port,MOTO_EN1_Pin,(BitAction)n)
#define MOTO_EN2(n)  GPIO_WriteBit(MOTO_EN2_GPIO_Port,MOTO_EN2_Pin,(BitAction)n)
#define MOTO_IN1(n)  GPIO_WriteBit(MOTO_IN1_GPIO_Port,MOTO_IN1_Pin,(BitAction)n)
#define MOTO_IN2(n)  GPIO_WriteBit(MOTO_IN2_GPIO_Port,MOTO_IN2_Pin,(BitAction)n)
#define SDI          GPIO_ReadInputDataBit(SPIMISO_GPIO_Port, SPIMISO_Pin)
#define SDO(n)       GPIO_WriteBit(SPIMOSI_GPIO_Port,SPIMOSI_Pin,(BitAction)n)
#define SCK(n)       GPIO_WriteBit(SPICLK_GPIO_Port,SPICLK_Pin,(BitAction)n)
#define NSS(n)       GPIO_WriteBit(SPINSS_GPIO_Port,SPINSS_Pin,(BitAction)n)
#define IIC_SCL(n)   GPIO_WriteBit(IIC_SCL_GPIO_Port,IIC_SCL_Pin,(BitAction)n)
#define IIC_SDA(n)   GPIO_WriteBit(IIC_SDA_GPIO_Port,IIC_SDA_Pin,(BitAction)n)
#define IIC_EN(n)    GPIO_WriteBit(IIC_EN_GPIO_Port,IIC_EN_Pin,(BitAction)n)
#define IIC_Read_SDA GPIO_ReadInputDataBit(IIC_SDA_GPIO_Port,IIC_SDA_Pin)
#define SIGNAL       GPIO_ReadInputDataBit(SENSOR_SIGNAL_GPIO_Port,SENSOR_SIGNAL_Pin)
#define INTERFERE    GPIO_ReadInputDataBit(SENSOR_INTERFERE_GPIO_Port,SENSOR_INTERFERE_Pin)
#define RXD          GPIO_ReadInputDataBit(RXD_GPIO_Port, RXD_Pin)
#define MOTO_POWER(n)GPIO_WriteBit(MOTO_POWER_GPIO_Port,MOTO_POWER_Pin,(BitAction)n)
/* USER CODE END M */

/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */

void MX_GPIO_Init(void);
void FastLowPowerInit(void);

/* USER CODE END FP */













#endif /*__ GPIO_H__ */

