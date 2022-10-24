/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Spi.c
Author: LBJ
Description: AX5043的SPI底层驱动文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/24      v0.0.1            LBJ            first version
-----------------------------------------------------------*/


/* Includes ---------------------------------------------*/


#include "stm8l15x.h"
#include "spi.h"
#include "gpio.h"

/*----------------------------------------------------------
ModuleName: Spi_Init
Description: SPI的IO口初始化
Params: void
Return: null
Notice: null
-----------------------------------------------------------*/
void Spi_Init(void)
{
  GPIO_Init(SPINSS_GPIO_Port, SPINSS_Pin, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(SPICLK_GPIO_Port, SPICLK_Pin, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(SPIMOSI_GPIO_Port, SPIMOSI_Pin, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(SPIMISO_GPIO_Port, SPIMISO_Pin, GPIO_Mode_In_FL_No_IT);
}

/*----------------------------------------------------------
ModuleName: SpiWriteReadByte
Description: 读写单字节
Params: unsigned char ubByte
Return: unsigned char redata
Notice: 
-----------------------------------------------------------*/
unsigned char SpiWriteReadByte(unsigned char ubByte)
{
    unsigned char readByte;
    SCK(0);
    for (unsigned char i = 0; i < 8; i++)
    {
        readByte <<= 1;
        if ((ubByte&0x80) == 0x80)
          SDO(1);
        else
          SDO(0);
        SCK(1);
        ubByte = ubByte << 1;
        if (SDI)
            readByte |= 0x01;
        SCK(0);
    }
    asm("NOP");
    SDO(0);
    return readByte;
}

/*----------------------------------------------------------
ModuleName: SpiWriteSingleAddressRegister
Description: 在单字节地址中写入单字节数据
Params: unsigned char Addr  unsigned char Data
Return: null
Notice: null
-----------------------------------------------------------*/
void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data)
{
  Addr = Addr|0x80 ;  
  NSS(0);
  SpiWriteReadByte(Addr);
  SpiWriteReadByte(Data);
  NSS(1);
}

/*----------------------------------------------------------
ModuleName: SpiWriteLongAddressRegister
Description: 在双字节的地址中写入单字节的数据
Params: unsigned int Addr  unsigned char Data
Return: null
Notice: null
-----------------------------------------------------------*/
void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data)
{
  Addr = Addr|0xF000; 
  NSS(0);
  SpiWriteReadByte(Addr >> 8);
  SpiWriteReadByte(Addr & 0xff);
  SpiWriteReadByte(Data);
  NSS(1);
}

/*----------------------------------------------------------
ModuleName: SpiReadSingleAddressRegister
Description: 读取单字节地址上一个字节的数据
Params: unsigned char Addr
Return: unsigned char Data
Notice: null
-----------------------------------------------------------*/
unsigned char SpiReadSingleAddressRegister(unsigned char Addr)
{
  unsigned char Data;
  Addr = Addr&0x7F ;
  NSS(0);
  SpiWriteReadByte(Addr);
  Data = SpiWriteReadByte(0);
  NSS(1);
  return Data;
}

/*----------------------------------------------------------
ModuleName: SpiReadLongAddressRegister
Description: 读取双字节地址上一个字节的数据
Params: unsigned int Addr
Return: unsigned char Data
Notice: null
-----------------------------------------------------------*/
unsigned char SpiReadLongAddressRegister(unsigned int Addr)
{
  unsigned char Data;
  Addr = Addr&0x7FFF; 
  Addr = Addr|0x7000; 
  NSS(0);
  SpiWriteReadByte(Addr >> 8);
  SpiWriteReadByte(Addr & 0xff);
  Data = SpiWriteReadByte(0);
  NSS(1);
  return Data;
}

/*----------------------------------------------------------
ModuleName: SpiReadAutoAddressRegister
Description: 自适应读取地址数据
Params: unsigned int Addr
Return: unsigned char Data
Notice: null
-----------------------------------------------------------*/
unsigned char SpiReadAutoAddressRegister(unsigned int Addr)
{
  if(Addr < 0x70) return SpiReadSingleAddressRegister(Addr);
  else return SpiReadLongAddressRegister(Addr);
}

/*----------------------------------------------------------
ModuleName: SpiWriteAutoAddressRegister
Description: 自适应写入地址数据
Params: unsigned int Addr
Return: unsigned char Data
Notice: null
-----------------------------------------------------------*/
void SpiWriteAutoAddressRegister(unsigned int Addr,unsigned char Data)
{
  if(Addr < 0x70) SpiWriteSingleAddressRegister(Addr,Data);
  else SpiWriteLongAddressRegister(Addr,Data);
}




