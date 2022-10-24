/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Spi.h
Author: LBJ
Description: AX5043的驱动文件声明
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/24      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

#ifndef _SPI_H_
#define _SPI_H_

/* Includes ---------------------------------------------*/


#include "stm8l15x.h"


/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */



/* USER CODE END M */


/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */

void Spi_Init(void);
void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data);
void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data);
unsigned char SpiReadSingleAddressRegister(unsigned char Addr);  
unsigned char SpiReadLongAddressRegister(unsigned int Addr);
unsigned char SpiReadAutoAddressRegister(unsigned int Addr);
void SpiWriteAutoAddressRegister(unsigned int Addr,unsigned char Data);

/* USER CODE END FP */

#endif




