/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: record.h
Author: LBJ
Description: 外部EEPROM记录声明
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/8      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
#ifndef __RECORD_H_
#define __RECORD_H_
/* Includes ---------------------------------------------*/
#include "stm8l15x.h"

/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */

extern unsigned char RecorderDays;
/* USER CODE END V */


/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */
void EX_EEPROM_Init(void);
void EX_EEPROM_Clear(void);
void EX_EEPROM_Save(void);
void EX_EEPROM_Print(void);
void EX_EEPROM_Print_More(void);

uint8_t EEPROM_Write(uint32_t address,uint8_t * data,uint8_t size);
uint8_t EEPROM_Read(uint32_t address,uint8_t * data,uint8_t size);
uint8_t Write_Data_And_Check_Backup(uint16_t address1,uint16_t address2,uint8_t * data,uint16_t size);
uint8_t Read_Data_Or_Backup(uint16_t address1,uint16_t address2, uint8_t * data,uint16_t size);


/* USER CODE END FP */




#endif


