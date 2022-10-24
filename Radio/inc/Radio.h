/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Radio.h
Author: LBJ
Description: AX5043的应用程序声明文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/25      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

#ifndef _RADIO_H_
#define _RADIO_H_


/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */
extern  uint8_t GCV_PllRange;
extern  uint8_t GCV_PllVcoi;

/* USER CODE END V */


/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */
void AX5043_Init(void);
void AX5043_Reset(void);
unsigned char Simple_Autorange_Pll(void);
void AX5043_Registers(void);
void Simple_Transmit(uint8_t *pkt, uint16_t pktlen);
void Send_Continue_For_Tow_Minutes(void);
/* USER CODE END FP */




#endif

/*************************the end of file*******************************/
