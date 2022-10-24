/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: MS.c
Author: LBJ
Description: MSӦ�ó���
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/5      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
/* Includes ---------------------------------------------*/

#include "spi.h"
#include "sysdata.h"
#include "Radio.h"
#include "main.h"
#include "time.h"
#include "zenner.h"
#include "gpio.h"
#include "record.h"
#include "MS.h"
/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */
#define MS_DATA_ADDR1 0X00
#define MS_DATA_ADDR2 0X40
#define MS_DATA_SIZE    14
#define DATA_START      10
#define THSTART         0
#define THVERSIONS_LEN  2
#define THTAMPER_LEN    1
#define THBATTERY_LEN   1
#define THMSID_LEN      20
#define THMSV_LEN       20
/* USER CODE END M */

/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */
sMS MS;
uint8_t send_buf[32]=
{
  0xe1,0x1A,0x03,
  0X55,0X55,0X55,0X55,0X55,0X55,0X5A,
};
uint8_t data_buf[9] = 
{
  0,
};
/* USER CODE END V */





/*----------------------------------------------------------
ModuleName: MSDataInit
Description: MS�ĳ�ʼֵ�趨
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MS_Data_Init(void)
{
  //�洢������Чλ-��ʼλ
  MS.SaveFlag = 0xaa;
  //MS�汾��
  MS.Versions = 0;  
  //MSID
  MS.MSID = 20221915;     
  //MSȦ��
  MS.MSV = 0;
  //MSʱ��
  MS.MST = 666666;        
}

/*----------------------------------------------------------
ModuleName: MS_Data_Save
Description: MS���ݴ洢
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MS_Data_Save(void)
{
 Write_Data_And_Check_Backup(MS_DATA_ADDR1,MS_DATA_ADDR2,&(MS.SaveFlag),MS_DATA_SIZE);
}

/*----------------------------------------------------------
ModuleName: MS_Data_Read
Description: MS��ȡ����
Params: null
Return: null
Notice: ��ȡ����������ݴ��������Ϊ��ʼֵ
-----------------------------------------------------------*/
void MS_Data_Read(void)
{
 if(Read_Data_Or_Backup(MS_DATA_ADDR1,MS_DATA_ADDR2,&(MS.SaveFlag),MS_DATA_SIZE))
  {
    sysdata.SyadataError = 1;   
    MS_Data_Init();
    EEPROM_Write(MS_DATA_ADDR1,&(MS.SaveFlag),MS_DATA_SIZE);
    EEPROM_Write(MS_DATA_ADDR2,&(MS.SaveFlag),MS_DATA_SIZE);
    MS_Data_Save();
  }
}

/*----------------------------------------------------------
ModuleName: MS_Zenner_Pulse_Logic
Description: MS��zenner�������ȡ����
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MS_Zenner_Pulse_Logic(void)
{
  //�ź�λ
  if(sysdata.VTTime)
  {
    sysdata.VTTime --;
    if(sysdata.VTTime == 0)
    {
      if(!sysdata.VTFlag)
      {
        
        MS.MSV == 999999 ?( MS.MSV = 0):(MS.MSV ++);  
        ZennerPulseLogic();//sign                
      }
      sysdata.VTFlag = 0;
    }
  }    
  //����λ
  if(sysdata.TTTime)
  {
    sysdata.TTTime --;
    if(sysdata.TTTime == 0)
    {
      if(!sysdata.TTFlag)
      {
        MS.Tamper = 1;
      }
      sysdata.TTFlag = 0;
    }
  }   
}

/*----------------------------------------------------------
ModuleName: Load_Data_To_SendBuf
Description: װ�����ݵ�����������
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Load_Data_To_SendBuf(void)
{
  uint8_t addr = DATA_START;
  uint8_t addr_i = 0;
  for(uint8_t n = 0 ; n < 9 ; n ++)
  {
    uint8_t data = data_buf[n];
    for(uint8_t i = 0 ; i < 8 ; i ++)
    {
      send_buf[addr] <<= 2;
      if(data & 0x80) send_buf[addr] |= 1;
      else send_buf[addr] |= 2;
      data <<= 1;
      addr_i ++;
      if(addr_i == 4)
      {
        addr_i = 0;
        addr ++;
      }
    }
  }
}

/*----------------------------------------------------------
ModuleName: Data_Check
Description: ����У��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Data_Check(void)
{
  uint8_t wei_data = 0x01;
  for(uint8_t n = 0 ; n < 7 ; n ++)
  {
    uint8_t buf_data = data_buf[n];
    uint8_t data = 0;
    for(uint8_t i = 0 ; i < 7 ; i ++)
    {
      if(buf_data & 0x80) data ++;
      buf_data <<= 1;
    }
    if((data % 2) != 0) data_buf[n] |= 0x01;
  }
  data_buf[7] = 0;
  for(uint8_t n = 0 ; n < 8 ; n ++)
  {
    uint8_t data = 0;
    for(uint8_t i = 0 ; i < 7 ; i ++)
    {
      if(data_buf[i] & wei_data) data ++;
    }
    if((data % 2) != 0) data_buf[7] |= wei_data;
    
    wei_data <<= 1;
  }
  data_buf[8] = data_buf[7];
  Load_Data_To_SendBuf();
}

/*----------------------------------------------------------
ModuleName: Package_Short_Data
Description: ��������ݰ�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Package_Short_Data(void)
{
  int8_t i;
  uint32_t data;
  uint8_t len;
  uint8_t addr = 0;
  uint8_t addr_i = 0;
  for(uint8_t n = 0 ; n < 3 ; n ++)
  {
    if(n == 0)
    {
      data = MS.Versions;
      len = 2;
    }
    else if(n == 1)
    {
      data = MS.MSID;
      len = 27;
    }
    else if(n == 2)
    {
      data = MS.MSV;
      len = 20;
    }
    data <<= (32 - len);
    for(i = 0 ; i < len ; i ++)
    {
      data_buf[addr] <<= 1;
      if(data & 0x80000000)
      {
        data_buf[addr] |= 1;
      }
      data <<= 1;
      addr_i ++;
      if(addr_i == 7)
      {
        data_buf[addr] <<= 1;
        addr_i = 0;
        addr ++;
      }
    }
  }
  Data_Check();
}

/*----------------------------------------------------------
ModuleName: Package_Long_Data
Description: ��������ݰ�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Package_Long_Data(void)
{
  int8_t i;
  uint32_t data;
  uint8_t len;
  uint8_t addr = 0;
  uint8_t addr_i = 0;
  
  for(uint8_t n = 0 ; n < 5 ; n ++)
  {
    if(n == 0)
    {
      data = MS.Versions;
      len = 2;
    }
    else if(n == 1)
    {
      data = MS.MSID;
      len = 27;
    }
    else if(n == 2)
    {
      data = 0x1f;
      len = 5;
    }
    else if(n == 3)
    {
      data = 0x00;
      if(MS.Tamper) data |= 0x200;
      //�͵���
      if(MS.Battery)data|= 0x100; 
      //���͵���
      if(MS.LBattery)data|= 0x080;  
      
      if(Zenner.ValveStatus == 1)
        data|= 0x040;
      if(Zenner.DLFlag1 || Zenner.DLFlag2)
        data|= 0x020;
      if(Zenner.XLFlag1 || Zenner.XLFlag2) 
        data|= 0x010;
      if(Zenner.TimeFlag ||Zenner.TimeBaoJingFlag) 
        data|= 0x008;
      //���ŶϿ�
      if(Zenner.ValveErrorStatus==2||Zenner.ValveCheckErrorStatus==2) 
        data|= 0x004;
      //���Ŷ�ת
      if(Zenner.ValveErrorStatus == 1)
        data|= 0x002;
      if(Zenner.QMFlag) 
        data|= 0x001;
      len = 10;
    }
    else if(n == 4)
    {
      data = 0x00;
      len = 5;
    }
    data <<= (32 - len);
    for(i = 0 ; i < len ; i ++)
    {
      data_buf[addr] <<= 1;
      if(data & 0x80000000)
      {
        data_buf[addr] |= 1;
      }
      data <<= 1;
      addr_i ++;
      if(addr_i == 7)
      {
        data_buf[addr] <<= 1;
        addr_i = 0;
        addr ++;
      }
    }
  }
  Data_Check();
}

/*----------------------------------------------------------
ModuleName: MsSend
Description: �������ݷ���
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MsSend(void)
{
  //SPI��ʼ��
  Spi_Init();       
  //������
  TCXO_EN(1);   
  Delay_ms(30);
  //���÷���оƬ
  AX5043_Registers();           
  //����оƬ�Զ���Ƶ
  Simple_Autorange_Pll();           
  //��������ݰ�
  Package_Short_Data();         
  //װ������ݣ�������
  Simple_Transmit(send_buf,28);             
  Delay_ms(30);
  //��������ݰ�
  Package_Long_Data(); 
  //װ������ݣ�������
  Simple_Transmit(send_buf,28);  
  
  SpiWriteSingleAddressRegister(0x01, 0x01);
  SpiWriteSingleAddressRegister(0x02, 0x00);
  //�ؾ���
  TCXO_EN(0);  
  Delay_ms(2);
  //����оƬ����͹���
  SpiWriteSingleAddressRegister(0x02, 0x01);  
}

/*----------------------------------------------------------
ModuleName: MSSend_And_Save
Description: �������ݷ��Ͳ��洢����
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MSSend_And_Save(void)
{
  //���ݴ����ⲿEEPROM
  EX_EEPROM_Save();
  //ι��
  IWDG_ReloadCounter();  
  //��ص͵�ѹ���
  Battery_Check_2V45();
  //��������
  MsSend();         
  //�رյ�ص�ѹ���
  Battery_Check_End(); 
  //���û�磬����1û���־λ���������0����λû���־λ
  if(sysdata.PVDBatteryFlag) 
    MS.Battery = 1;         
  else
    MS.Battery = 0;         
  //���û�磬����1���͵�����־λ���������0����λ���͵�����־λ
  if(sysdata.PVDLBatteryFlag1 || sysdata.PVDLBatteryFlag2 )
    MS.LBattery = 1;         
  else
    MS.LBattery = 0;  
  //��ȡ����λ
  Get_Interference_Flag();           
  //���ݱ���
  MS_Data_Save();
}

/*----------------------------------------------------------
ModuleName: MSSend_Continuous
Description: ������������
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void MSSend_Continuous(void)
{  
  //��ص͵�ѹ���
  Battery_Check_2V45();
  //������
  TCXO_EN(1);
  //��ʼ��SPI
  Spi_Init();
  Delay_ms(30);
  //����5043
  AX5043_Reset();
  //���÷���оƬ
  AX5043_Registers();
  //����оƬ�Զ���Ƶ
  Simple_Autorange_Pll();
  //������������������
  Send_Continue_For_Tow_Minutes();
  //5043����͹���
  SpiWriteSingleAddressRegister(0x01, 0x01);
  SpiWriteSingleAddressRegister(0x02, 0x00);
  //�رվ���
  TCXO_EN(1);
  Delay_ms(2);
  SpiWriteSingleAddressRegister(0x02, 0x01);
  //�ص������
  Battery_Check_End();              
}

/*----------------------------------------------------------
ModuleName: Get_Interference_Flag
Description: ��ȡMS����λ״̬
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Get_Interference_Flag(void)
{
  if( INTERFERE == 0) 
    MS.Tamper = 1;
  else 
    MS.Tamper = 0;
}

/*----------------------------------------------------------
ModuleName: Get_Signal
Description: MSV�źŴ���
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Get_Signal(void)
{
  if(sysdata.VTTime != 0) 
    sysdata.VTFlag = 1;
  else 
    sysdata.VTTime = 3;
  
}

/*----------------------------------------------------------
ModuleName: Get_Interference
Description: MS���Ŵ���
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Get_Interference(void)
{
  if(sysdata.TTTime != 0) 
    sysdata.TTFlag = 1;
  sysdata.TTTime = 3;
}