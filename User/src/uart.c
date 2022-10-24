/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: uart.c
Author: LBJ
Description: 串口操作程序文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/9/9      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

/* Includes ---------------------------------------------*/
#include "stm8l15x.h"
#include "MS.h"
#include "uart.h"
#include "stdio.h"
#include "sysdata.h"
#include "time.h"
#include "record.h"
#include "string.h"
#include "gpio.h"
/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */

UART_STRUCT uart;
uint32_t Uart_lData;
uint8_t Uart_VErrorFlag;
uint8_t Uart_RFlag;
uint8_t Uart_ret;
/* USER CODE END V */


/*----------------------------------------------------------
ModuleName: fputc
Description: 重新定义串口1输出
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
int fputc(int ch, FILE *f)
{
  USART_SendData8(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
  return ch;
}

/*----------------------------------------------------------
ModuleName: Uart_Init
Description: 串口初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Uart_Init(void)
{
  //串口1 RX口初始化  浮空输入无中断
  GPIO_Init(RXD_GPIO_Port,RXD_Pin,GPIO_Mode_In_FL_No_IT);
  //串口1 TX口初始化  低速推挽输出高电平
  GPIO_Init(TXD_GPIO_Port,TXD_Pin,GPIO_Mode_Out_PP_High_Slow);
  //开启USART1时钟
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);  
  //端口重映射，去掉注释之后USART1为PA2-TX、PA3-RX；
  //注释之后USART1为TX-PC5、RX-PC6；
  //复位之后USART会自动恢复至PC5、PC6
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);  
  //设置USART参数600，8N1，接收/发送
  USART_Init(USART1,600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
  //使能接收中断
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  //使能USART
  USART_Cmd (USART1,ENABLE);
  //清中断标志
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  //开启串口1接收
  USART_ReceiveData8(USART1);
  uart.AIFlag = 1;
  uart.ReceiveLen = 0;
  enableInterrupts(); 
}

/*----------------------------------------------------------
ModuleName: Uart_DeInit
Description: 串口初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Uart_DeInit(void)
{
  //关闭USART1
  USART_Cmd (USART1,DISABLE);
  //关闭USART1时钟
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,DISABLE);
  //串口1 RX口初始化  浮空输入带中断
  GPIO_Init(RXD_GPIO_Port,RXD_Pin,GPIO_Mode_In_FL_IT);
  //串口1 TX口初始化  低速推挽输出低电平
  GPIO_Init(TXD_GPIO_Port,TXD_Pin,GPIO_Mode_Out_PP_Low_Slow);
}

/*----------------------------------------------------------
ModuleName: UartSendStr
Description: 字符串打印
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void UartSendStr(uint8_t * buf,uint8_t len)
{
  for(uint8_t i = 0 ; i < len ; i ++)
  {
    USART_SendData8 (USART1,buf[i]);
    //等待发送完毕
    while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));
  }
}

/*----------------------------------------------------------
ModuleName: GetCRC
Description: 获取检验码
Params: buf数组地址，len数组长度
Return: 将数组和的最后两位，转为两个字节的返回值
Notice: null
-----------------------------------------------------------*/
uint16_t GetCRC(uint8_t * buf,uint8_t len)
{
  uint16_t ChackCode = 0;
  uint8_t Sum = 0;
  uint8_t Str = 0;
  uint8_t i;
  for(i = 0 ; i < len ; i ++) 
    Sum += buf[i];  
  Str = Sum >> 4;
  if(Str < 10) 
    Str += '0';
  else 
    Str += 'A' - 10;
  ChackCode = Str << 8;
  Str = Sum & 0xf;
  if(Str < 10) 
    Str += '0';
  else 
    Str += 'A' - 10;
  ChackCode |= Str;
  return ChackCode;
}

/*----------------------------------------------------------
ModuleName: WriteCRC
Description: 写入校验码
Params: null
Return: null
Notice: 将数组和的最后两位，拆分为两个字节填入数组中
-----------------------------------------------------------*/
void WriteCRC(uint8_t * buf,uint8_t len)  
{
  uint16_t ChackCode = GetCRC(buf+1,len-1);
  buf[len] = ChackCode >> 8;
  buf[len + 1] = ChackCode & 0xff;
  buf[len + 2] = '*';
  buf[len + 3] ='\r';
}

/*----------------------------------------------------------
ModuleName: CheckDataCRC
Description: 校验数据检验码是否正确
Params: null
Return: 返回1，数据正确；返回0，数据错误
Notice: null
-----------------------------------------------------------*/
uint8_t CheckDataCRC(uint8_t * buf,uint8_t len)
{
  uint16_t Chack = GetCRC(buf + 1,len - 1 - 4);
  if((Chack >> 8) == buf[len - 4]&&(Chack & 0xff) == buf[len - 3])
  {
    return 1;
  }
  return 0;
}

/*----------------------------------------------------------
ModuleName: Data_Convert_BCD
Description: 将data转换成BCD码并存入buf
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Data_Convert_BCD(uint8_t * buf,uint32_t data,uint8_t len)
{
  uint8_t temp;
  for(int8_t i = len - 1 ; i >= 0 ; i--)
  {
    temp = data & 0xf;
    if(temp < 10) 
      buf[i] = temp + '0';
    else 
      buf[i] = temp - 10 + 'A';
    data >>= 4;
  }
}

/*----------------------------------------------------------
ModuleName: Data_Convert_HEX
Description: 将buf中的数据转换成HEX码赋予给data
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Data_Convert_HEX(uint8_t * buf,uint32_t * data,uint8_t len)
{
  uint8_t temp;
  uint32_t hex = 0;
  for(uint8_t i = 0 ; i < len ; i ++)
  {
    hex <<= 4;
    temp = buf[i];
    if(temp < 'A') 
      hex += temp - '0';
    else 
      hex += temp - 'A' + 10;
  }
  *data = hex;
}

/*----------------------------------------------------------
ModuleName: Data_Convert_HEX
Description: 将buf中的数据转换成HEX码赋予给data
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint8_t UartWork(void)
{
  if(!RXD) 
    uart.Time ++;
  else 
    uart.Time = 0;
  if(uart.Start == 0)
  {
    uart.Start = 1;
    Uart_Init();
  }

  if(uart.ReceiveLen > 2)
  { 
    if( uart.ReceiveBuf[2] >= 'A' && uart.ReceiveBuf[2] <= 'Z' )
    {
      if(uart.ReceiveLen == 7 && uart.ReceiveBuf[5] == 0x2a&& uart.ReceiveBuf[6] == 0x0d )
      {
        if(CheckDataCRC(uart.ReceiveBuf,7))
        {
          uart.SendBuf[0] = 0x40;
          uart.SendBuf[1] = uart.ReceiveBuf[1];
          uart.SendBuf[2] = uart.ReceiveBuf[2];
          uart.SendBuf[3] = 0x30;
          uart.SendLen = 4;       
          switch(uart.ReceiveBuf[2])
          {
           
          case 'B':
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],MS.MSID,7);
            uart.SendLen += 7;     
            break;
            
          case 'F':
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],MS.MSV,5);
            uart.SendLen += 5;
            break;
            
          case 'H':
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Read_RTC_Time() * 2,5);
            uart.SendLen += 5;
            break;
            
          case 'I':
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],(MS.Versions << 6) + (MS.Tamper << 5) + (MS.Battery << 4) + (MS.LBattery),2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],((!!(Zenner.ValveStatus)) << 7) + ((Zenner.DLFlag1 | Zenner.DLFlag2) << 6)  + ((Zenner.XLFlag1 | Zenner.XLFlag2) << 5) + ((Zenner.TimeFlag|Zenner.TimeBaoJingFlag) << 4) + (Zenner.QMFlag << 2) + ((Zenner.ValveErrorStatus == 1) << 1) + (Zenner.ValveErrorStatus == 2 || Zenner.ValveCheckErrorStatus == 2),2);
            uart.SendLen += 2;            
            Get_Interference_Flag();             
            break;
            
          case 'J':
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],0,2);
            uart.SendLen += 2;
            break;
            //读取escape参数
          case 'Q': 
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DLSecSet2,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DLTimesSet2,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DL_Pulse,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DLTimesSet1,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DL_Pulse2,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.XLSecSet1,6);
            uart.SendLen += 6;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.XLTimesSet1,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.XLSecSet2,6);
            uart.SendLen += 6;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.XLTimesSet2,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.XLCloseEnableFlag,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.MeterFlag,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.DLSecSet1,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeFlagSendTimeSet,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeFlagSendTimesSet,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.QMFlagSendTimeSet,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.QMFlagSendTimesSet,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.Reserved1,2);
            uart.SendLen += 2;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.Reserved2,2);
            uart.SendLen += 2;
            break;
            //读取阀门状态
          case 'T':           
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeFlag | Zenner.TimeBaoJingFlag,1);
            uart.SendLen += 1;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeShengYuSec,8);
            uart.SendLen += 8;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeBaoJingSec,8);
            uart.SendLen += 8;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.ValveData1,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.ValveData2,4);
            uart.SendLen += 4;
            break;
            
          case 'X':
            EX_EEPROM_Print();
            break;
            
          case 'Y':
            EX_EEPROM_Clear();
            break;
            
          case 'Z':
            EX_EEPROM_Print_More();
            break;
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
            
          }
          WriteCRC(uart.SendBuf,uart.SendLen);
          uart.SendLen += 4;
          IWDG_ReloadCounter();              
          printf("%s",uart.SendBuf);
        }
        uart.SendFlag=1;
      }
      else if(uart.ReceiveLen == 8 && uart.ReceiveBuf[6] == 0x2a && uart.ReceiveBuf[7] == 0x0d)
      {
        if(CheckDataCRC(uart.ReceiveBuf,8))
        {
          uart.SendBuf[uart.SendLen ++] = 0x40;
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[1];
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[2];
          uart.SendBuf[uart.SendLen ++] = 0x30;
          switch(uart.ReceiveBuf[2])
          {
            //发送MS并保存参数
          case 'W':
            MSSend_And_Save();
            return 0;
            break;
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
            
          }
        }
        uart.SendFlag=1;
      }
      else if((uart.ReceiveLen == 12&& uart.ReceiveBuf[10] == 0x2a&& uart.ReceiveBuf[11] == 0x0d)||(uart.ReceiveLen == 11&&uart.ReceiveBuf[10] == 0x2a)) 
      {
        if(CheckDataCRC(uart.ReceiveBuf,12))
        {
          uart.SendBuf[uart.SendLen ++] = 0x40;
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[1];
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[2];
          uart.SendBuf[uart.SendLen ++] = 0x30;
          switch(uart.ReceiveBuf[2])
          {
            //msid-6
          case 'A':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MSID),5);
            if(sysdata.SendMode == 0) 
              Set_RTC_Alarm(MS.MSID % 1024 * 84);             
            MS_Data_Save();
            break;
            //msv
          case 'E':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MSV),5);             
            MS_Data_Save();
            break;
            //mst
          case 'G':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MST),5);
            Set_RTC_Time(MS.MST / 2); 
            MS_Data_Save();
            break;
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
            
          }
          WriteCRC(uart.SendBuf,uart.SendLen);
          uart.SendLen += 4;
          printf("%s",uart.SendBuf);
        }
        uart.SendFlag=1;
      }          
      else if((uart.ReceiveLen == 14 && uart.ReceiveBuf[12] == 0x2a&& uart.ReceiveBuf[13] == 0x0d)||(uart.ReceiveLen == 13 && uart.ReceiveBuf[12] == 0x2a))
      {
        if(CheckDataCRC(uart.ReceiveBuf,14))
        {
          uart.SendBuf[uart.SendLen ++] = 0x40;
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[1];
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[2];
          uart.SendBuf[uart.SendLen ++] = 0x30;
          switch(uart.ReceiveBuf[2])
          {
            //msid-8
          case 'A':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MSID),7);
            if(sysdata.SendMode == 0)
              Set_RTC_Alarm(MS.MSID % 1024 * 84);
            MS_Data_Save();
            break;
            //msv
          case 'E':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MSV),7);             
            MS_Data_Save();
            break;
            //mst
          case 'G':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(MS.MST),7);
            Set_RTC_Time(MS.MST / 2); 
            MS_Data_Save();
            break; 
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
            
          }
          WriteCRC(uart.SendBuf,uart.SendLen);
          uart.SendLen += 4;
          printf("%s",uart.SendBuf);
        }
        uart.SendFlag=1;
      }
      else if(uart.ReceiveLen == 32 && uart.ReceiveBuf[30] == 0x2a )
      {
        if(CheckDataCRC(uart.ReceiveBuf,32))
        {
          uart.SendBuf[uart.SendLen ++] = 0x40;
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[1];
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[2];
          uart.SendBuf[uart.SendLen ++] = 0x30;
          switch(uart.ReceiveBuf[2])
          {
            //写阀状态
          case 'R':
            Data_Convert_HEX(&(uart.ReceiveBuf[4]),&(Uart_lData),8);
            if(Uart_lData != 0xffffffff)
            {
              Uart_VErrorFlag = 3;
              //min:60s  max：120月
              if((Uart_lData >= 60 && Uart_lData <= 311040000) || Uart_lData == 0)
                Zenner.TimeShengYuSec = Uart_lData;           
              else 
                Zenner.TimeShengYuSec = 194400000;
              Zenner.TimeFlag = 0;
            }
            Data_Convert_HEX(&(uart.ReceiveBuf[12]),&(Uart_lData),8);
            if(Uart_lData != 0xffffffff)
            {
              Uart_VErrorFlag = 3;
              //min:60s   max：40月
              if((Uart_lData >= 60 && Uart_lData <= 103680000) || Uart_lData == 0)
                Zenner.TimeBaoJingSec = Uart_lData;
              else 
                Zenner.TimeBaoJingSec = 15552000;
            }
            Data_Convert_HEX(&(uart.ReceiveBuf[20]),&(Uart_lData),4);
            if(Uart_lData != 0xffff) 
              Zenner.ValveData1 = Uart_lData;
            Data_Convert_HEX(&(uart.ReceiveBuf[24]),&(Uart_lData),4);
            if(Uart_lData != 0xffff) 
              Zenner.ValveData2 = Uart_lData;
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(Uart_lData),1);
            if(Uart_lData != 0xf)
            {
              if(Uart_lData == 0)//open
              {
                Uart_ret = Zenner_Valve_Work(ZENNER_OPEN);
                Zenner.ValveStatus = 0;
                Uart_RFlag = 1;
                if(Uart_VErrorFlag == 3 && Uart_ret != 2) 
                  Uart_VErrorFlag = 0;//2-断开
                else if(Uart_VErrorFlag == 1 && Uart_ret == 1)
                  Uart_VErrorFlag = 0;
                else 
                  Uart_VErrorFlag = Uart_ret;
                uart.AIFlag = 0;
                Zenner.QMFlag = 0;
              }
              else if(Uart_lData == 1)//close
              {
                Uart_RFlag = 0;
                Uart_ret = Zenner_Valve_Work(ZENNER_CLOSE);
                Zenner.ValveStatus = 1;          
                if(Uart_VErrorFlag == 1 && Uart_ret == 1)
                  Uart_VErrorFlag = 0;
                else 
                  Uart_VErrorFlag = Uart_ret;
              }
            }
            if(Uart_VErrorFlag == 3)
              Uart_VErrorFlag = 0;
            if(Uart_RFlag == 1)
            {
              Zenner_Work_State_Init();                
              if(sysflag.CloseFlag != 2) 
                sysflag.TimeFlag = 0;
              else 
                sysflag.CloseFlag = 0;                
              sysflag.DLFlag = 0;
              sysflag.XLFlag = 0;
              sysflag.MFFlag = 0;
              sysflag.MFFirstFlag = 0;
              sysflag.ZlErrorFlag = 0;                
              sysdata.cDL_Times = 0;
              sysdata.cQuanTime = 0;
              sysdata.cMiFeng = 0;               
              sysdata.TXL_Sec = 0;
              sysdata.cXL_StartFlag = 0;
              sysdata.cXL_Times = 0;
              sysdata.cJiQuan = 0;
              sysdata.TXL_Sec2 = 0;
              sysdata.cXL_StartFlag2 = 0;
              sysdata.cXL_Times2 = 0;
              sysdata.cJiQuan2 = 0;            
              sysdata.DLCloseFlag = 0;
              sysdata.XLCloseFlag = 0;
              for(sysdata.cQuanTimeBufI = 0 ; sysdata.cQuanTimeBufI < 10 ; sysdata.cQuanTimeBufI ++) 
                sysdata.cQuanTimeBuf[sysdata.cQuanTimeBufI] = 255;
            }
            Zenner_Data_Save(); 
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.ValveStatus,1);
            uart.SendLen += 1;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeShengYuSec,8);
            uart.SendLen += 8;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.TimeBaoJingSec,8);
            uart.SendLen += 8;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.ValveData1,4);
            uart.SendLen += 4;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Zenner.ValveData2,4);
            uart.SendLen += 4;
            if(Uart_VErrorFlag == 1) 
              Uart_ret = 2;
            else if(Uart_VErrorFlag == 2) 
              Uart_ret = 1;
            else 
              Uart_ret = 0;
            Data_Convert_BCD(&uart.SendBuf[uart.SendLen],Uart_ret,1);
            uart.SendLen += 1;
            break;
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
          }
          WriteCRC(uart.SendBuf,uart.SendLen);
          uart.SendLen += 4;
          //喂狗
          IWDG_ReloadCounter();               
          printf("ghghghghghghghghghgh");
          printf("%s",uart.SendBuf);
        }
        uart.SendFlag=1;
      }
      else if(((uart.ReceiveLen == 60+4) && (uart.ReceiveBuf[59+4] == 0x2a)) || ((uart.ReceiveLen == 60) && (uart.ReceiveBuf[59] == 0x2a)))
      {
        if(uart.ReceiveLen == 64) 
          uart.ReceiveLen = 65;
        else 
          uart.ReceiveLen = 61;
        if(CheckDataCRC(uart.ReceiveBuf,uart.ReceiveLen))
        {
          uart.SendBuf[uart.SendLen ++] = 0x40;
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[1];
          uart.SendBuf[uart.SendLen ++] = uart.ReceiveBuf[2];
          uart.SendBuf[uart.SendLen ++] = 0x30;
          switch(uart.ReceiveBuf[2])
          {
            //写escape参数
          case 'P':
            Data_Convert_HEX(&(uart.ReceiveBuf[3]),&(Uart_lData),2);
            if(Uart_lData >= 6 && Uart_lData <= 17|| Uart_lData == 0)
              Zenner.DLSecSet2 = Uart_lData;
            else 
              Zenner.DLSecSet2 = 10;
            Data_Convert_HEX(&(uart.ReceiveBuf[5]),&(Uart_lData),4);
            if(Uart_lData >= 3 && Uart_lData <= 10 || Uart_lData == 0)
              Zenner.DLTimesSet2 = Uart_lData;
            else 
              Zenner.DLTimesSet2 = 6;
            Data_Convert_HEX(&(uart.ReceiveBuf[9]),&(Uart_lData),4);
            if(Uart_lData==0)
              Zenner.DL_Pulse = Uart_lData;
            else
              Zenner.DL_Pulse = 1;
            Data_Convert_HEX(&(uart.ReceiveBuf[13]),&(Uart_lData),4);
            if(Uart_lData >= 3 && Uart_lData <= 255 || Uart_lData == 0)
              Zenner.DLTimesSet1 = Uart_lData;
            else 
              Zenner.DLTimesSet1 = 190;
            Data_Convert_HEX(&(uart.ReceiveBuf[17]),&(Uart_lData),4);
            if(Uart_lData)
              Zenner.DL_Pulse2 = 1;
            else
              Zenner.DL_Pulse2 = 0;       
            Data_Convert_HEX(&(uart.ReceiveBuf[21]),&(Uart_lData),6);
            if(Uart_lData >= 3600 && Uart_lData <= 360000 || Uart_lData == 0)
              Zenner.XLSecSet1 = Uart_lData;
            else 
              Zenner.XLSecSet1 = 3600;
            Data_Convert_HEX(&(uart.ReceiveBuf[27]),&(Uart_lData),2);
            if(Uart_lData >= 2 && Uart_lData <= 200 || Uart_lData == 0)
              Zenner.XLTimesSet1 = Uart_lData;
            else 
              Zenner.XLTimesSet1 = 24;          
            Data_Convert_HEX(&(uart.ReceiveBuf[29]),&(Uart_lData),6);
            if(Uart_lData >= 3600 && Uart_lData <= 360000 || Uart_lData == 0)
              Zenner.XLSecSet2 = Uart_lData;
            else 
              Zenner.XLSecSet2 = 10800;
            Data_Convert_HEX(&(uart.ReceiveBuf[35]),&(Uart_lData),2);
            if(Uart_lData >= 2 && Uart_lData <= 200 || Uart_lData == 0)
              Zenner.XLTimesSet2 = Uart_lData;
            else 
              Zenner.XLTimesSet2 = 24; 
            Data_Convert_HEX(&(uart.ReceiveBuf[37]),&(Uart_lData),2);
            if(Uart_lData <= 1) 
              Zenner.XLCloseEnableFlag = Uart_lData;
            else 
              Zenner.XLCloseEnableFlag = 1;
            Data_Convert_HEX(&(uart.ReceiveBuf[39]),&(Uart_lData),4);
            Zenner.MeterFlag = Uart_lData;
            Data_Convert_HEX(&(uart.ReceiveBuf[43]),&(Uart_lData),2);
            if(Uart_lData >= 6 && Uart_lData <= 55|| Uart_lData == 0)
              Zenner.DLSecSet1 = Uart_lData;
            else 
              Zenner.DLSecSet1 = 22;      
            Data_Convert_HEX(&(uart.ReceiveBuf[45]),&(Uart_lData),4);
            if(Uart_lData >= 60 && Uart_lData <= 90000||Uart_lData==0)
              Zenner.TimeFlagSendTimeSet = Uart_lData;
            else 
              Zenner.TimeFlagSendTimeSet=3600;
            Data_Convert_HEX(&(uart.ReceiveBuf[49]),&(Uart_lData),2);
            Zenner.TimeFlagSendTimesSet = Uart_lData;
            Data_Convert_HEX(&(uart.ReceiveBuf[51]),&(Uart_lData),4);
            if(Uart_lData >= 60 && Uart_lData <= 90000||Uart_lData==0)
              Zenner.QMFlagSendTimeSet = Uart_lData;
            else 
              Zenner.QMFlagSendTimeSet = 3600;
            Data_Convert_HEX(&(uart.ReceiveBuf[55]),&(Uart_lData),2);
            Zenner.QMFlagSendTimesSet = Uart_lData;
            if(uart.ReceiveLen == 65)
            {
              Data_Convert_HEX(&(uart.ReceiveBuf[57]),&(Uart_lData),2);
              Zenner.Reserved1 = Uart_lData;
              Data_Convert_HEX(&(uart.ReceiveBuf[59]),&(Uart_lData),2);
              Zenner.Reserved2 = Uart_lData;
            }
            uart.AIFlag = 0;
            Zenner_Work_State_Init();
            Zenner_Data_Save();
            break;
            
          default:
            uart.SendBuf[uart.SendLen - 1] = 0x31;
            break;
          }
          WriteCRC(uart.SendBuf,uart.SendLen);
          uart.SendLen += 4;
          //喂狗
          IWDG_ReloadCounter();               
          printf("%s",uart.SendBuf);
          for(;uart.SendLen > 0 ; uart.SendLen --) 
            uart.SendBuf[uart.SendLen] = 0;
        }  
        uart.SendFlag=1;

      }
    }
    else 
      uart.ReceiveLen = 0;

  }
  if(uart.SendFlag)
  {
    uart.ReceiveLen = 0;
    uart.SendLen = 0;
    uart.SendFlag=0;
    uart.Time = 0;
    memset(uart.SendBuf,0,70);
    memset(uart.ReceiveBuf,0,70);
  }
  if(uart.Time == 200)
  {
    uart.Time = 0;
    uart.Start = 0;
    memset(uart.SendBuf,0,70);
    memset(uart.ReceiveBuf,0,70);
    Uart_DeInit();   
    return 1;
  }  
  return 0;
}




