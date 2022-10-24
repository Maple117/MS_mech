/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Record.c
Author: LBJ
Description: EERPOM��Ӧ�ó���
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
Description: �ⲿEEPROM��ʼ��
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
Description: �ⲿEEPROM�������
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EX_EEPROM_Clear(void)
{
  EEPROM_Power_ON();
  uint8_t CleanBuf[16] = {0};
  uint16_t i;
  //EEPROMȫ�����д0
  for(i = 0 ; i < EEPNUM ; i ++)
  {
    Write_EEPROM(REDATAARR + (i << 3),CleanBuf,8);   
    IWDG_ReloadCounter();               //ι��
  }
  //��д��ַ
  RecorderNumber = 0;
  RecorderWBuf[0] = RecorderNumber >> 8;
  RecorderWBuf[1] = RecorderNumber &0xff;
  Write_EEPROM(RESTATEARR + 2,RecorderWBuf,2);
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Save
Description: �洢MS�Ĳ���
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
Description: �洢MS�����ݴ�ӡ_6λ
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
      IWDG_ReloadCounter();               //ι��
    }
    for(i = 0 ; i < RecorderNumber%EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,6);
      IWDG_ReloadCounter();               //ι��
    }  
  }
  else
  {
    for(i = 0 ; i < RecorderNumber ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,6);
      IWDG_ReloadCounter();               //ι��
    }     
  }
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EX_EEPROM_Print_More
Description: �洢MS�����ݴ�ӡ_8λ
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
      IWDG_ReloadCounter();               //ι��
    }
    for(i = 0 ; i < RecorderNumber%EEPNUM ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,8);
      IWDG_ReloadCounter();               //ι��
    }
  }
  else
  {
    for(i = 0 ; i < RecorderNumber ; i ++)
    {
      Read_EEPROM(REDATAARR + (i << 3),RecorderRBuf,8);
      UartSendStr(RecorderRBuf,8);
      IWDG_ReloadCounter();               //ι��
    }
  }
  EEPROM_Power_OFF();
}

/*----------------------------------------------------------
ModuleName: EEPROM_Write
Description: д�����ݵ��ڲ�EEPROM
Params: null
Return: ����0��д��ɹ�;1,д��ʧ��
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
Description: ��ȡ�ڲ�EEPROM����
Params: null
Return:  ���� 0����ȡ�ɹ�; ����1����ȡʧ��;
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
Description: д���������ݲ���У�鱸������
Params: address�������ݴ洢��ַ,addressb�������ݵ�ַ
Return:  ���� 0��д��ɹ�; ����1��д��ʧ��;
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
  //��ȡ����������ȷ
  if(State == 0)         
  { 
    //��ȡ����������ȷ
    if(StateBackup == 0)       
    { 
      //���ݱȶ�ͬ��
      for(i = 0 ; i < size ; i ++)       
      {
        if(Buf[i] != BufBackup[i])
          break;
      }
    }
    //���ݲ�ͬ����ֱ�Ӹ�����д���µ�����
    if(i != size)         
    {
      EEPROM_Write(addressb,data,size);
    }
    
  } 
  //�������ݺͱ������ݶ���ȡʧ�ܣ��ȴ����Ź���λ
  else if(StateBackup) 
    return 1;       
  
  //д�������ݺͱ������ݣ����������д��͵ȴ����Ź���λ
  if(EEPROM_Write(address,data,size)&&EEPROM_Write(addressb,data,size))
    return 1;
  return 0;
}

/*----------------------------------------------------------
ModuleName: Read_Data_Or_Backup
Description: ��ȡ�������ݻ򱸷�����
Params: address�������ݴ洢��ַ,addressb�������ݵ�ַ
Return:  ���� 0����ȡ�ɹ�; ����1����ȡʧ��;
Notice: null
-----------------------------------------------------------*/
uint8_t Read_Data_Or_Backup(uint16_t address,
                            uint16_t addressb,
                            uint8_t * data,
                            uint16_t size)
{
  
  //��ȡ�������ݣ����ʧ���ٶ�ȡ��������
  if(EEPROM_Read(address,data,size))              
  { 
    //��ȡ�������ݣ����ʧ�ܷ���1
    if(EEPROM_Read(addressb,data,size))
      return 1;              
  }
  return 0;
}







