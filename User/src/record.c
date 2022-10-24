/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Record.c
Author: LBJ
Description: EERPOM的应用程序
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/2      v0.0.1            LBJ            first version
-----------------------------------------------------------*/


/* Includes ---------------------------------------------*/
#include "record.h"
#include "sysdata.h"
#include "24c08.h"
#include "uart.h"
#include "Radio.h"
#include "MS.h"

#define RESTATEARR 0
#define REDATAARR 32

#define EEPNUM 1000

void IWDG_ReloadCounter(void);

uint8_t RecorderRBuf[16];
uint8_t RecorderWBuf[16];

uint8_t RecorderDays;
uint16_t RecorderNumber;

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Init
Description: 外部EEPROM初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Init(void)
{
  EEPROM_Power_ON();
  
  Read_EEPROM(RESTATEARR,RecorderRBuf,4);
  if(RecorderRBuf[0] != 0xa5)
  {
    RecorderRBuf[0] = 0xa5;
    RecorderRBuf[1] = 0;
    RecorderRBuf[2] = 0;
    RecorderRBuf[3] = 0;
    RecorderNumber = 0;
    Write_EEPROM(RESTATEARR,RecorderRBuf,4);
  }
  else
  {
    RecorderNumber = RecorderRBuf[2] << 8 | RecorderRBuf[3];
  }
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Clear
Description: 外部EEPROM清除数据
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Clear(void)
{
  EEPROM_Power_ON();
  uint8_t CleanBuf[16] = {0};
  uint16_t i;
  //EEPROM全部清除写0
  for(i = 0 ; i < EEPNUM ; i ++)
  {
    Write_EEPROM(REDATAARR + (i << 3),CleanBuf,8);   
    IWDG_ReloadCounter();               //喂狗
  }
  //改写地址
  RecorderNumber = 0;
  RecorderWBuf[0] = RecorderNumber >> 8;
  RecorderWBuf[1] = RecorderNumber &0xff;
  Write_EEPROM(RESTATEARR + 2,RecorderWBuf,2);
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Save
Description: 存储MS的参数
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Save(void)
{
  u8 da;
  EEPROM_Power_ON();
  
  *(u32 *)RecorderWBuf = MS.MSV;
  da = 0;
  if(MS.Tamper) da |= 0x80;
  if(MS.Battery) da |= 0x40;
  if(MS.LBattery) da |= 0x20;
  if(Zenner.ValveStatus) da |= 0x10;
  if(Zenner.DLFlag1 || Zenner.DLFlag2) da |= 0x08;
  if(Zenner.XLFlag1 || Zenner.XLFlag2) da |= 0x04;
  if(Zenner.TimeFlag) da |= 0x02;
  if(Zenner.ValveErrorStatus == 2 || Zenner.ValveCheckErrorStatus == 2) da |= 0x01;
  RecorderWBuf[4] = da;
  da = 0;
  if(Zenner.ValveErrorStatus == 1) da |= 0x80;
  if(Zenner.QMFlag) da |= 0x40;
  da |= RecorderDays & 0x3f;
  RecorderWBuf[5] = da;
  RecorderWBuf[6] = GCV_PllVcoi;
  RecorderWBuf[7] = GCV_PllRange;
  Write_EEPROM(REDATAARR + ((RecorderNumber%EEPNUM) << 3),RecorderWBuf,8);
  RecorderNumber++;
  RecorderWBuf[0] = RecorderNumber >> 8;
  RecorderWBuf[1] = RecorderNumber &0xff;
  Write_EEPROM(RESTATEARR + 2,RecorderWBuf,2);
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Print
Description: 存储MS的数据打印_6位
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Print(void)
{
  u16 i;
  EEPROM_Power_ON();
  if(RecorderNumber>EEPNUM)
  {
    for(i = RecorderNumber%EEPNUM ; i < EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,6);
      IWDG_ReloadCounter();               //喂狗
    }
    for(i = 0 ; i < RecorderNumber%EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,6);
      IWDG_ReloadCounter();               //喂狗
    }  
  }
  else
  {
    for(i = 0 ; i < RecorderNumber ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,6);
      IWDG_ReloadCounter();               //喂狗
    }     
  }
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Print_More
Description: 存储MS的数据打印_8位
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Print_More(void)
{
  u16 i;
  EEPROM_Power_ON();
  if(RecorderNumber>EEPNUM)
  {
    for(i = RecorderNumber%EEPNUM ; i < EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,8);
      IWDG_ReloadCounter();               //喂狗
    }
    for(i = 0 ; i < RecorderNumber%EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,8);
      IWDG_ReloadCounter();               //喂狗
    }
  }
  else
  {
    for(i = 0 ; i < RecorderNumber ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,8);
      IWDG_ReloadCounter();               //喂狗
    }
  }
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EEPROM_Write
Description: 写入数据到内部EEPROM
Params: null
Return: 返回0，写入成功;1,写入失败
Notice: null
-----------------------------------------------------------*/
uint8_t EEPROM_Write(uint32_t address,uint8_t * data,uint8_t size)
{
  uint8_t Check = 0;
  uint8_t i;
  uint16_t Count=0;
  address += 0x1000;
  if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
  {
    FLASH_Unlock(FLASH_MemType_Data);
    for(i = 0; i < size ; i ++)
    {
      FLASH_ProgramByte(address + i,data[i]);
      Check += data[i];
      while(!(FLASH->IAPSR & FLASH_IAPSR_EOP))
      {
        Count++;
        if(Count==6000)
        {
          return 1;
        }
      }
    }
    FLASH_ProgramByte(address + i,Check);
    while(!(FLASH->IAPSR & FLASH_IAPSR_EOP))
    {
      Count++;
      if(Count==6000)
      {
        return 1;
      }
    }
    FLASH_Lock(FLASH_MemType_Data);
  }
  return 0;
}

/*----------------------------------------------------------
ModuleName: EEPROM_Read
Description: 读取内部EEPROM数据
Params: null
Return:  返回 0，读取成功; 返回1，读取失败;
Notice: null
-----------------------------------------------------------*/
uint8_t EEPROM_Read(uint32_t address,uint8_t * data,uint8_t size)
{
  uint8_t check = 0;
  uint8_t i;
  address += 0x1000;
  if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
  {
    for(i = 0; i < size ; i ++)
    {
      data[i] = FLASH_ReadByte(address + i);
      check += data[i];
    }
    
    if(check == FLASH_ReadByte(address + i) && data[0] == 0xaa)
      return 0;
    else 
      return 1;
  }
  return 1;
}

/*----------------------------------------------------------
ModuleName: Write_Data_And_Check_Backup
Description: 写入运行数据并且校验备份数据
Params: address运行数据存储地址,addressb备份数据地址
Return:  返回 0，写入成功; 返回1，写入失败;
Notice: null
-----------------------------------------------------------*/
uint8_t Write_Data_And_Check_Backup(uint16_t address,
                                    uint16_t addressb,
                                    uint8_t * data,
                                    uint16_t size)
{
  uint8_t Buf[64];
  uint8_t BufBackup[64];
  uint8_t State;
  uint8_t StateBackup;
  uint8_t i = 0;
  
  State = EEPROM_Read(address,Buf,size);
  StateBackup = EEPROM_Read(addressb,BufBackup,size);
  //读取运行数据正确
  if(State == 0)         
  { 
    //读取备份数据正确
    if(StateBackup == 0)       
    { 
      //数据比对同步
      for(i = 0 ; i < size ; i ++)       
      {
        if(Buf[i] != BufBackup[i])
          break;
      }
    }
    //数据不同步，直接给备份写最新的数据
    if(i != size)         
    {
      EEPROM_Write(addressb,data,size);
    }
    
  } 
  //运行数据和备份数据都读取失败，等待看门狗复位
  else if(StateBackup) 
    return 1;       
  
  //写运行数据和备份数据，如果都不能写入就等待看门狗复位
  if(EEPROM_Write(address,data,size)&&EEPROM_Write(addressb,data,size))
    return 1;
  return 0;
}

/*----------------------------------------------------------
ModuleName: Read_Data_Or_Backup
Description: 读取运行数据或备份数据
Params: address运行数据存储地址,addressb备份数据地址
Return:  返回 0，读取成功; 返回1，读取失败;
Notice: null
-----------------------------------------------------------*/
uint8_t Read_Data_Or_Backup(uint16_t address,
                            uint16_t addressb,
                            uint8_t * data,
                            uint16_t size)
{
  
  //读取运行数据，如果失败再读取备份数据
  if(EEPROM_Read(address,data,size))              
  { 
    //读取备份数据，如果失败返回1
    if(EEPROM_Read(addressb,data,size))
      return 1;              
  }
  return 0;
}







