/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: 24c08.c
Author: LBJ
Description: EEPROM驱动及其应用程序文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/2      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

#include "stm8l15x.h"
#include "24c08.h"
#include "gpio.h"


/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */



#define DEFINE_24C64            1
#define DEVICE_ADDRESS           0XA0


#if DEFINE_24C02
#define	_24CXX_ADDR_LEN         1
#define _24CXX_PAGE_BYTE	8

#elif DEFINE_24C04
#define	_24CXX_ADDR_LEN         1
#define _24CXX_PAGE_BYTE	8

#elif DEFINE_24C08
#define	_24CXX_ADDR_LEN         1
#define _24CXX_PAGE_BYTE	8

#elif DEFINE_24C32
#define	_24CXX_ADDR_LEN         2
#define _24CXX_PAGE_BYTE	8

#elif DEFINE_24C64
#define	_24CXX_ADDR_LEN         2
#define _24CXX_PAGE_BYTE	8

#else
#define	_24CXX_ADDR_LEN         1
#define _24CXX_PAGE_BYTE	8

#endif

typedef enum 
{
  Success=0U,
  Error_ID,
  Error_Address,
  Error_Data,	
}IIC;


/* USER CODE END M */






/*----------------------------------------------------------
ModuleName: IIC_Delay_us
Description: 微秒级延时
Params: uint8_t nCount 
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_Delay_us(uint8_t nCount)
{
  while (nCount--);
}

/*----------------------------------------------------------
ModuleName: IIC_Delay_ms
Description: 毫秒级延时
Params: uint8_t nCount 
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_Delay_ms(uint8_t nCount)
{
  nCount *= 4;
  while (nCount--)
  {
    IIC_Delay_us(250);
    IIC_Delay_us(250);
    IIC_Delay_us(250);
    IIC_Delay_us(250);
  }
}

/*----------------------------------------------------------
ModuleName: EEPROM_Power_ON
Description: 开启EEPROM的电源
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EEPROM_Power_ON(void)
{
  IIC_EN(1);
}

/*----------------------------------------------------------
ModuleName: EEPROM_Power_OFF
Description: 关闭EEPROM的电源并反向初始化IIC口
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void EEPROM_Power_OFF(void)
{
  IIC_EN(0);
  GPIO_Init(IIC_SDA_GPIO_Port,IIC_SDA_Pin, GPIO_Mode_Out_PP_High_Slow);
  IIC_SCL(0);
}

/*----------------------------------------------------------
ModuleName: IIC_SDA_OUT
Description: 设置IIC数据脚为输出
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_SDA_OUT(void)
{
  GPIO_Init(IIC_SDA_GPIO_Port,IIC_SDA_Pin, GPIO_Mode_Out_PP_High_Slow);
}

/*----------------------------------------------------------
ModuleName: IIC_SDA_IN
Description: 设置IIC数据脚为输入
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_SDA_IN(void)
{
  GPIO_Init(IIC_SDA_GPIO_Port,IIC_SDA_Pin, GPIO_Mode_In_PU_No_IT);
}

/*----------------------------------------------------------
ModuleName: IIC_Start
Description: IIC通讯开始起始信号
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_Start(void)
{
  IIC_SDA_OUT();
  IIC_SCL(1);
  IIC_SDA(1);
  IIC_Delay_us(1);
  IIC_SDA(0);
  IIC_Delay_us(1);
  IIC_SCL(0);
}

/*----------------------------------------------------------
ModuleName: IIC_Stop
Description: IIC通讯开始结束信号
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_Stop(void)
{
  IIC_SDA_OUT();
  IIC_SDA(0);
  IIC_SCL(0);
  IIC_Delay_us(1);
  IIC_SCL(1);
  IIC_Delay_us(1);
  IIC_SDA(1);
  IIC_Delay_ms(5);
}

/*----------------------------------------------------------
ModuleName: IIC_Write_Ask
Description: IIC通讯写入应答
Params: ask：1有应答；0：无应答
Return: null
Notice: null
-----------------------------------------------------------*/
void IIC_Write_Ask(uint8_t ask)
{	
  if(ask) IIC_SDA(1);
  else IIC_SDA(0);
  IIC_Delay_us(1);
  IIC_SCL(1);
  IIC_Delay_us(1);
  IIC_SCL(0);
  IIC_Delay_us(1);
}

/*----------------------------------------------------------
ModuleName: IIC_Read_Ask
Description: IIC通讯读取应答
Params: null
Return: ask ：1为应答成功；0为应答失败
Notice: null
-----------------------------------------------------------*/
uint8_t IIC_Read_Ask(void)
{	
  uint8_t ask;
  
  IIC_SDA_IN();
  IIC_Delay_us(1);
  if(IIC_Read_SDA) ask=1;
  else ask=0;
  IIC_SCL(1);
  IIC_Delay_us(1);
  IIC_SCL(0);
  IIC_Delay_us(1);
  IIC_SDA_OUT();
  return(ask);
}

/*----------------------------------------------------------
ModuleName: IIC_Write_Byte
Description: IIC通讯写入单字节
Params:  uint8_t da
Return: 写入成功为1；失败为0
Notice: null
-----------------------------------------------------------*/
uint8_t IIC_Write_Byte(uint8_t da)
{
  uint8_t i;
  
  for(i=0;i<8;i++)
  {
    if(da&0x80) IIC_SDA(1);
    else IIC_SDA(0);
    da<<=1;
    IIC_Delay_us(1);
    IIC_SCL(1);
    IIC_Delay_us(1);
    IIC_SCL(0);
    IIC_Delay_us(1);
  }
  if(IIC_Read_Ask())
  {
    IIC_Stop();
    return(1);
  }
  else return(0);
}

/*----------------------------------------------------------
ModuleName: IIC_Write_Byte
Description: IIC通讯写入单字节
Params:  uint8_t da
Return: 写入成功为1；失败为0
Notice: null
-----------------------------------------------------------*/
uint8_t IIC_Read_Byte(uint8_t ask)
{
  uint8_t i;
  uint8_t da;
  
  IIC_SDA_IN();
  for(i=0;i<8;i++)
  {
    IIC_Delay_us(1);
    da<<=1;
    if(IIC_Read_SDA) da++;
    IIC_SCL(1);
    IIC_Delay_us(1);
    IIC_SCL(0);
    IIC_Delay_us(1);
  }
  IIC_SDA_OUT();
  IIC_Write_Ask(ask);
  return(da);
}

/*----------------------------------------------------------
ModuleName: Read24cxx
Description: IIC通讯读取指定地址上的数据
Params:  uint8_t id,uint32_t addr,uint8_t *p,uint32_t len
id为24Cxx的写入指令，addr为读取地址,p为读出的数据指针，len为数据长度
Return: 
Notice: null
-----------------------------------------------------------*/
uint32_t Read24cxx(uint8_t id,uint32_t addr,uint8_t *p,uint32_t len)
{
  uint32_t i;
  IIC_Start();
  if(IIC_Write_Byte(id)) return(Error_ID);
#if _24CXX_ADDR_LEN > 1
  if(IIC_Write_Byte(addr>>8)) return(Error_Address);
#endif
  if(IIC_Write_Byte(addr&0xff)) return(Error_Address);
  IIC_Start();
  if(IIC_Write_Byte(id+1)) return(Error_Data);
  for(i=0;i<len-1;i++) p[i]=IIC_Read_Byte(0);
  p[i]=IIC_Read_Byte(1);
  IIC_Stop();
  return(Success);
}

/*----------------------------------------------------------
ModuleName: Write24cxx
Description: IIC通讯在指定地址上写入数据
Params:  uint8_t id,uint32_t addr,uint8_t *p,uint32_t len
id为24Cxx的写入指令，addr为读写入地址,p为写入的数据指针，len为数据长度
Return: 
Notice: null
-----------------------------------------------------------*/
uint32_t Write24cxx(uint8_t id,uint32_t addr,uint8_t *p,uint32_t len)
{
  uint32_t i;
  for(uint32_t j = 0 ; j < len ; )
  {
    IIC_Start();
    if(IIC_Write_Byte(id)) return(Error_ID);
#if _24CXX_ADDR_LEN > 1
    if(IIC_Write_Byte((addr + j)>>8)) return(Error_Address);
#endif
    if(IIC_Write_Byte((addr + j)&0xff)) return(Error_Address);
    for(i = 0 ; i < _24CXX_PAGE_BYTE && j < len ; i ++) if(IIC_Write_Byte(p[j ++])) break;
    IIC_Stop();
    if(i != _24CXX_PAGE_BYTE) return(Error_Data);
  }
  return(Success);
}

/*----------------------------------------------------------
ModuleName: Read_EEPROM
Description: IIC通讯在指定地址上写入数据
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint32_t Read_EEPROM(uint16_t addr,uint8_t *p,uint32_t len)
{
  return Read24cxx(DEVICE_ADDRESS,addr,p,len);
}

/*----------------------------------------------------------
ModuleName: Write_EEPROM
Description: IIC通讯在指定地址上写入数据
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint32_t Write_EEPROM(uint16_t addr,uint8_t *p,uint32_t len)
{
  return Write24cxx(DEVICE_ADDRESS,addr,p,len);
}




