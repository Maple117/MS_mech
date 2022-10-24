#ifndef _24C08_H_
#define _24C08_H_

#include "stm8l15x.h"

void EEPROM_Power_ON(void);
void EEPROM_Power_OFF(void);

uint32_t Read_EEPROM(uint16_t addr,uint8_t *p,uint32_t len);
uint32_t Write_EEPROM(uint16_t addr,uint8_t *p,uint32_t len);

#endif


