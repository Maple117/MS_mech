
/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: zenner.C
Author: LBJ
Description: zenner功能性文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/30      v0.0.1            LBJ            first version
-----------------------------------------------------------*/
/* Includes ---------------------------------------------*/

#include "main.h"
#include "sysdata.h"
#include "gpio.h"
#include "time.h"
#include "adc.h"
#include "zenner.h"
#include "record.h"
/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */
#define ZENNER_OPEN 0
#define ZENNER_CLOSE 1

#define ZENNER_DATA_ADDR1 0X80
#define ZENNER_DATA_ADDR2 0XC0
#define ZENNER_DATA_SIZE 54+4


/* USER CODE END M */


/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */
u32 XL1_MSVtime;//记录相邻msv间隔的时间
u32 DL1_MSVtime;//记录相邻msv间隔的时间
float ZennerADC1,ZennerADC2,vcc1,vcc2;
sZenner Zenner;
/* USER CODE END V */


/*----------------------------------------------------------
ModuleName: Zenner_Data_Init
Description: zenner数据标识初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Data_Init(void)
{
  //存储数据有效位-起始位
  Zenner.SaveStartFlag = 0xaa;          
  /*------------------------------大漏数据标识-------------------------------*/
  //超流量-条件1-触发标志
  Zenner.DLFlag1 = 0;           
  //超流量-条件1-检测间隔计时
  Zenner.DLSecSet1 = 22;         
  //超流量-条件1-检测次数设置
  Zenner.DLTimesSet1 = 190;       
  //超流量-条件2-触发标志
  Zenner.DLFlag2 = 0;           
  //超流量-条件2-检测间隔计时
  Zenner.DLSecSet2 = 10;         
  //超流量-条件2-检测次数设置
  Zenner.DLTimesSet2 = 6;       
  //超流量-条件1脉冲
  Zenner.DL_Pulse = 1;  
  //超流量-条件2脉冲
  Zenner.DL_Pulse2 = 1;  
  
  /*------------------------------小漏数据标识-------------------------------*/
  //连续流量-条件1-触发标志
  Zenner.XLFlag1 = 0;           
  //连续流量-条件1-检测间隔设置(60*60s)
  Zenner.XLSecSet1 = 3600;        
  //连续流量-条件1-检测次数设置
  Zenner.XLTimesSet1 = 24;       
  //连续流量-条件2-触发标志
  Zenner.XLFlag2 = 0;           
  //连续流量-条件2-检测间隔设置（180*60s）
  Zenner.XLSecSet2 = 10800;        
  //连续流量-条件2-检测次数设置
  Zenner.XLTimesSet2 = 24;       
  //连续流量-报警关阀使能设置
  Zenner.XLCloseEnableFlag = 1; 
  
  /*------------------------------安检数据标识-------------------------------*/
  //安检-触发标志
  Zenner.TimeFlag = 0;                  
  //安检-剩余时间（75*30*24*3600S）
  Zenner.TimeShengYuSec = 194400000;           
  //安检-提醒时间（6*30*24*3600S）
  Zenner.TimeBaoJingSec = 15552000;           
  //安检-循环报警发送时间设置
  Zenner.TimeFlagSendTimeSet = 3600;      
  //安检-循环报警发送次数设置  取消
  Zenner.TimeFlagSendTimesSet = 72;           
  
  /*------------------------------气密数据标识-------------------------------*/
  //气密性-触发标志
  Zenner.QMFlag = 0;                    
  //气密性-循环报警发送时间设置
  Zenner.QMFlagSendTimeSet = 3600;        
  //气密性-循环报警发送次数设置  取消
  Zenner.QMFlagSendTimesSet = 72;       
  
  /*------------------------------阀门数据标识-------------------------------*/
  //阀门状态-0：开启 1：关闭
  Zenner.ValveStatus = 0;       
  //阀门故障状态-0：正常 1：堵转 2：断开
  Zenner.ValveErrorStatus = 0;  
  
  Zenner.ValveData1 = 0;
  Zenner.ValveData2 = 0;
  
  /*------------------------------数据记录标识-------------------------------*/
  Zenner.Reserved1 = 0;
  Zenner.Reserved2 = 0;
  
  /*------------------------------炉具标识-------------------------------*/
  Zenner.MeterFlag = 0;         
  
}
/*----------------------------------------------------------
ModuleName: Zenner_Work_State_Init
Description: 
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Work_State_Init(void)//标志位清除
{
  /*-----------------大漏工作状态标识初始化-----------------*/
  //超流量-条件1-触发标志
  Zenner.DLFlag1 = 0;           
  //超流量-条件2-触发标志
  Zenner.DLFlag2 = 0;  
  //超流量-条件1-检测开始标志
  Zenner.DLStartFlag1 = 0;
  //超流量-条件1-检测间隔计时
  Zenner.DLSecConut1 = 0;
  //超流量-条件1-检测次数计数
  Zenner.DLTimesCount1 = 0;
  //超流量-条件1-单周期脉冲计数
  Zenner.DLPulseCount1 = 0;
  //超流量-条件1-提前启动充电标志
  Zenner.DLChangeFlag1 = 0;
  //超流量-条件1-提前充电超时计时
  Zenner.DLChangeTimeCount1 = 0;
  //超流量-条件2-检测开始标志
  Zenner.DLStartFlag2 = 0;
  //超流量-条件2-检测间隔计时
  Zenner.DLSecConut2 = 0;
  //超流量-条件2-检测次数计数
  Zenner.DLTimesCount2 = 0;
  //超流量-条件2-单周期脉冲计数
  Zenner.DLPulseCount2 = 0;
  //超流量-条件2-提前启动充电标志
  Zenner.DLChangeFlag2 = 0;
  //超流量-条件2-提前充电超时计时
  Zenner.DLChangeTimeCount2 = 0;
  
  /*-----------------小漏工作状态标识初始化-----------------*/
  //连续流量-条件1-触发标志
  Zenner.XLFlag1 = 0;           
  //连续流量-条件2-触发标志
  Zenner.XLFlag2 = 0;    
  //连续流量-条件1-检测间隔计时
  Zenner.XLSecConut1 = 0;
  //连续流量-条件1-检测次数计数
  Zenner.XLTimesCount1 = 0;
  //连续流量-条件1-检测启动标志
  Zenner.XLStartFlag1 = 0;
  //连续流量-条件1-单周期脉冲计数
  Zenner.XLPulseCount1 = 0;
  //连续流量-条件2-检测间隔计时
  Zenner.XLSecConut2 = 0;
  //连续流量-条件2-检测次数计数
  Zenner.XLTimesCount2 = 0;
  //连续流量-条件2-检测启动标志
  Zenner.XLStartFlag2 = 0;
  //连续流量-条件2-单周期脉冲计数
  Zenner.XLPulseCount2 = 0;
  
  /*-----------------安检工作状态标识初始化-----------------*/
  //安检-触发标志
  Zenner.TimeFlag = 0;         
  //安检-循环报警发送时间计时
  Zenner.TimeFlagSendTimeCount = 0;
  //安检-循环报警发送次数计数
  Zenner.TimeFlagSendTimesCount = 0;
  //安检-空闲两小时计时
  Zenner.TimeQuanTime = 0;
  //安检-提醒标志
  Zenner.TimeBaoJingFlag = 0;
  
  /*-----------------气密性工作状态标识初始化-----------------*/
  //气密性-触发标志
  Zenner.QMFlag = 0;          
  //气密性-循环报警发送时间计时
  Zenner.QMFlagSendTimeCount = 0;
  //气密性-循环报警发送次数计数
  Zenner.QMFlagSendTimesCount = 0;
  
  /*-----------------阀门工作状态标识初始化-----------------*/  
  //阀门故障状态-0：正常 1：堵转 2：断开
  Zenner.ValveErrorStatus = 0;  
  //阀门每日检测状态-0：正常 2：断开
  Zenner.ValveCheckErrorStatus = 0;   
  //阀门每日检测状态-0：正常 2：断开
  Zenner.ValveCheckErrorStatus = 0;     
  //等级3报警标志
  Zenner.ErrorLevel3Flag = 0;
  
}


/*----------------------------------------------------------
ModuleName: Zenner_Data_Save
Description: Zenner数据存储
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Data_Save(void)
{
  Write_Data_And_Check_Backup(ZENNER_DATA_ADDR1,ZENNER_DATA_ADDR2,&(Zenner.SaveStartFlag),ZENNER_DATA_SIZE);
}

/*----------------------------------------------------------
ModuleName: Zenner_Data_Read
Description: Zenner读取数据
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Data_Read(void)
{
  if(Read_Data_Or_Backup(ZENNER_DATA_ADDR1,ZENNER_DATA_ADDR2,&(Zenner.SaveStartFlag),ZENNER_DATA_SIZE))
  {
    sysdata.SyadataError = 1;
    Zenner_Data_Init();
    EEPROM_Write(ZENNER_DATA_ADDR1,&(Zenner.SaveStartFlag),ZENNER_DATA_SIZE);
    EEPROM_Write(ZENNER_DATA_ADDR2,&(Zenner.SaveStartFlag),ZENNER_DATA_SIZE);
    Zenner_Data_Save();
  }
}

/*----------------------------------------------------------
ModuleName:  Zenner_Valve_State
Description: 电机通断检测
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
uint8_t Zenner_Valve_State(void)
{
  MOTO_POWER(1);
  GPIO_Init(MOTO_CHECK_GPIO_Port, MOTO_CHECK_Pin, GPIO_Mode_Out_PP_High_Slow);
  Delay_5us(100);
  GPIO_Init(MOTO_CHECK_GPIO_Port, MOTO_CHECK_Pin, GPIO_Mode_In_PU_No_IT);
  Delay_5us(200);
  if(!GPIO_ReadInputDataBit(MOTO_CHECK_GPIO_Port, MOTO_CHECK_Pin))
  {
    Zenner.ValveCheckErrorStatus = 0;
  }
  else 
  {
    Zenner.ValveCheckErrorStatus = 2;
  }
  Delay_5us(200);
  MOTO_POWER(0);
  GPIO_Init(MOTO_CHECK_GPIO_Port, MOTO_CHECK_Pin, GPIO_Mode_Out_PP_Low_Slow);
  return 0;
}

/*----------------------------------------------------------
ModuleName:  Zenner_Valve_Work
Description: 电机检测
Params: null
Return: 返回0，正常；返回1，堵转；返回2，断开
Notice: null
-----------------------------------------------------------*/
uint8_t Zenner_Valve_Work(uint8_t state)
{
  //记录高低电压次数1.2-1.8
  uint8_t count1=0;  
  //记录高低电压次数2.0-2.8
  uint8_t count2=0;   
  //记录高低电压次数0-1.0
  uint8_t count3=0;      
  int adc_value=0,adc_value2=0;
  //阀门超低电压检测 
  Battery_Check_2V05();
  MX_ADC_Init();
  MOTO_EN1(1);
  MOTO_EN2(1);
  Delay_5us(5*200);
  //开阀
  if(state == ZENNER_OPEN) 
  {
    MOTO_IN1(0);
    MOTO_IN2(1);
  }
  //关阀
  else        
  {
    MOTO_IN1(1);
    MOTO_IN2(0);    
  }
  Delay_ms(900);
  for(uint8_t i = 0;i<21;i++)
  {
    //喂狗
    IWDG_ReloadCounter();       
    //vcc对应寄存器
    adc_value = ADC_Read_Internal_Voltage(); 
    vcc1 = 1.225*4096/adc_value;
    //当时的电压寄存器值
    adc_value2 = ADC_Read_VFB();  
    //反馈电压
    ZennerADC1 = vcc1 * adc_value2/4096 ;  
    if((ZennerADC1>=1.2&&ZennerADC1<=1.8))
    {
      count1++;
    }
    if((ZennerADC1>=2.0&&ZennerADC1<=2.8))
    {
      count2++;
    }
    if((ZennerADC1<=1))
    {
      count3++;
    }
    Delay_ms(150); 
  }
  //动作结束
  MX_ADC_DeInit(); 
  MOTO_IN1(0);
  MOTO_IN2(0);  
  MOTO_EN1(0);
  MOTO_EN2(0);
  //阀门电压检测关
  Battery_Check_End();
  
  if((count1>20)||(count2>20))
  {
    //堵转
    return 1;
  }
  else if(count3>=20)
  {
    //断开
    return 2;
  }
  else
  {
    //正常
    return 0;
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerErrorLevel3
Description: 报警三级事件处理
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerErrorLevel3(void)
{
  if(Zenner.ErrorLevel3Flag == 0)
  {
    Zenner.ErrorLevel3Flag = 1;
    Zenner.TimeFlagSendTimeCount = 0;
    Zenner.TimeFlagSendTimesCount = 0;
    sysflag.send = 1;
    sysflag.FastSleep = 0;
    if(Zenner.TimeFlagSendTimesSet && Zenner.TimeFlagSendTimeSet)
    {
      Zenner.QMFlagSendTimesCount = Zenner.QMFlagSendTimesSet;
    }
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerPulseLogic
Description: Zenner脉冲逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerPulseLogic(void)
{
  //脉冲到来，时间清除
  XL1_MSVtime = 0;
  //脉冲到来，时间清除
  DL1_MSVtime = 0;
  //强制关阀pulse计时复位
  Zenner.TimeQuanTime = 0;
  //气密性报警
  if(Zenner.ValveStatus == ZENNER_CLOSE)
  {
    if(Zenner.QMFlag == 0)
    {
      Zenner.QMFlag = 1;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
  //连续流量-小漏-条件1检测启动
  if(Zenner.XLTimesSet1)
  {
    if(Zenner.XLPulseCount1 < 255)
    {
      if(Zenner.XLStartFlag1 == 0) 
      {
        Zenner.XLStartFlag1 = 1;
      }
      else 
      {
        Zenner.XLPulseCount1 ++;
      }
    }
  }
  //连续流量-小漏-条件2检测启动
  if(Zenner.XLTimesSet2)
  {
    if(Zenner.XLPulseCount2 < 255)
    {
      if(Zenner.XLStartFlag2 == 0) 
      {
        Zenner.XLStartFlag2 = 1;
      }
      else 
      {
        Zenner.XLPulseCount2 ++;
      }
    }
  }
  //超流量-大漏-条件1检测启动
  if(Zenner.DLTimesSet1&&Zenner.DLSecSet1&&Zenner.DL_Pulse2)
  {
    if(Zenner.DLPulseCount1 < 255)
    {
      if(Zenner.DLStartFlag1 == 0)
      {
        Zenner.DLStartFlag1 = 1;
      }
      else 
      {
        Zenner.DLPulseCount1 ++;
      }
    }
  }
  //超流量-大漏-条件2检测启动
  if(Zenner.DLTimesSet2&&Zenner.DLSecSet2&&Zenner.DL_Pulse)
  {
    if(Zenner.DLStartFlag2 == 0)
    {
      Zenner.DLStartFlag2 = 1;
    }
    else
    {
      if(Zenner.DLSecConut2 <= Zenner.DLSecSet2)
      {
        Zenner.DLSecConut2 = 0;
        Zenner.DLTimesCount2 ++;
        if(Zenner.DLTimesCount2 >= Zenner.DLTimesSet2)
        {
          Zenner.DLFlag2 = 1;
          if(Zenner.QMFlag)
          {
            ZennerErrorLevel3();
          }
          else 
          {
            Zenner.DLTimesCount2 = 0;
          }
        }
        if(Zenner.DLTimesCount2 + 2 >= Zenner.DLTimesSet2)
          Zenner.DLChangeFlag2 = 1;
      }
      else
      {
        Zenner.DLSecConut2 = 0;
        Zenner.DLTimesCount2 = 0;
        Zenner.DLStartFlag2 = 0;
        Zenner.DLChangeFlag2 = 0;
      }
    }
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerDLLogicWork
Description: 超流量-大漏工作逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerDLLogicWork(void)
{
  //超流量-大漏-条件1检测
  if(Zenner.DLStartFlag1)
  {
    Zenner.DLSecConut1 ++;
  }
  else 
  {
    Zenner.DLSecConut1 = 0;
  }
  //超出时间，认为没有触发DL条件
  if(DL1_MSVtime>Zenner.DLSecSet1)
  {
    //大周期成功计数
    Zenner.DLTimesCount1 = 0;
    Zenner.DLStartFlag1 = 0;
    //周期内脉冲计数
    Zenner.DLPulseCount1 = 0;
    Zenner.DLSecConut1 = 0;
  }
  //超流量-大漏-条件1触发 检测次数大于22 （一级事件）
  if(Zenner.DLSecConut1 >= Zenner.DLSecSet1)
  {
    Zenner.DLSecConut1 = 0;
    if(Zenner.DLPulseCount1)
    {
      Zenner.DLPulseCount1 = 0;       
      Zenner.DLTimesCount1 ++;
      //DL条件1触发  每次脉冲次数大于190（二级事件）
      if(Zenner.DLTimesCount1 >= Zenner.DLTimesSet1)
      {
        Zenner.DLFlag1 = 1;
        //DL条件1触发  气密性故障（三级事件）
        if(Zenner.QMFlag)
        {
          ZennerErrorLevel3();
        }
        else 
        {
          Zenner.DLTimesCount1 = 0;
        }
      }
    }
    else
    {
      Zenner.DLTimesCount1 = 0;
      Zenner.DLStartFlag1 = 0;
      Zenner.DLChangeFlag1 = 0;
    }
    Zenner.DLPulseCount1 = 0;
  }
  //超流量-大漏-条件2-
  if(Zenner.DLTimesSet2 && Zenner.DLSecSet2)
  {
    //超流量-大漏充电超时2
    if(Zenner.DLChangeFlag2)
    {
      if(Zenner.DLChangeTimeCount2 < 60) 
        Zenner.DLChangeTimeCount2 ++;
      if(Zenner.DLChangeTimeCount2 >= 40)
      {
        Zenner.DLChangeTimeCount2 = 0;
        Zenner.DLChangeFlag2 = 0;
      }
    }
    else 
    {
      Zenner.DLChangeTimeCount2 = 0;
    }
    //超流量-大漏2
    if(Zenner.DLStartFlag2)
    {
      if(Zenner.DLSecConut2 < 255) 
        Zenner.DLSecConut2 ++;
    }
    else 
    {
      Zenner.DLSecConut2 = 0;
    }
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerXLLogicWork
Description: 连续流量-小漏工作逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerXLLogicWork(void)
{
  //连续流量-小漏条件1检测
  if(Zenner.XLSecSet1)
  {
    if(Zenner.XLStartFlag1) 
    {
      Zenner.XLSecConut1 ++;
    }
    else 
    {
      //时间间隔实际值清除
      Zenner.XLSecConut1 = 0;
    }
    //判断XL1_MSVtime是否超出时间
    if(XL1_MSVtime>=Zenner.XLSecSet1)
    {
      //超出时间，认为没有触发XL条件
      Zenner.XLTimesCount1 = 0;
      Zenner.XLStartFlag1 = 0;
      Zenner.XLPulseCount1 = 0;  
    }
    //连续流量-小漏-条件1触发 检测时间大于3600S  （一级事件）
    if(Zenner.XLSecConut1 >= Zenner.XLSecSet1)
    {
      Zenner.XLSecConut1 = 0;
      if(Zenner.XLPulseCount1)
      {
        Zenner.XLTimesCount1 ++;
        //连续流量-小漏-条件1触发 检测次数大于24  （二级事件）
        if(Zenner.XLTimesCount1 >= Zenner.XLTimesSet1 && Zenner.XLTimesSet1)
        {
          Zenner.XLFlag1 = 1;
          //气密性故障  （三级事件）
          if(Zenner.QMFlag)
          {
            ZennerErrorLevel3();
          }
          else 
          {
            Zenner.XLTimesCount1 = 0;
          }
        }
      }
      else
      {
        Zenner.XLTimesCount1 = 0;
        Zenner.XLStartFlag1 = 0;
      }
      Zenner.XLPulseCount1 = 0;
    }
  } 
   //连续流量-小漏条件2检测
  if(Zenner.XLSecSet2)
  {
    if(Zenner.XLStartFlag2) 
    {
      Zenner.XLSecConut2 ++;
    }
    else 
    {
      Zenner.XLSecConut2 = 0;
    }
    if(XL1_MSVtime>=Zenner.XLSecSet2)
    {
      //超出时间，认为没有触发XL条件
      Zenner.XLTimesCount2 = 0;
      Zenner.XLStartFlag2 = 0;
      Zenner.XLPulseCount2 = 0;
    }
    //连续流量-小漏-条件2触发 检测时间大于10800S  （一级事件）    
    if(Zenner.XLSecConut2 >= Zenner.XLSecSet2)
    {
      Zenner.XLSecConut2 = 0;
      if(Zenner.XLPulseCount2)
      {
        Zenner.XLTimesCount2 ++;
        //连续流量-小漏-条件2触发 检测次数大于24  （二级事件）        
        if(Zenner.XLTimesCount2 >= Zenner.XLTimesSet2 && Zenner.XLTimesSet2)
        {
          Zenner.XLFlag2 = 1;
          //气密性故障  （三级事件）          
          if(Zenner.QMFlag)
          {
            ZennerErrorLevel3();
          }
          else 
          {
            Zenner.XLTimesCount2 = 0;
          }
        }
      }
      else
      {
        Zenner.XLTimesCount2 = 0;
        Zenner.XLStartFlag2 = 0;
      }
      Zenner.XLPulseCount2 = 0;
    }
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerTimeLogicWork
Description: Zenner安检工作逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerTimeLogicWork(void)
{
  //强制关阀
  if(Zenner.TimeShengYuSec)
  {
    Zenner.TimeShengYuSec --;
    //安检剩余时间达到报警时间点（一级事件）
    if(Zenner.TimeShengYuSec == Zenner.TimeBaoJingSec)
    {
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
    if(Zenner.TimeShengYuSec == 0)
    {
      Zenner.TimeQuanTime = 0;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
   //安检剩余时间耗尽，创造2小时的安全时间（一级事件）
  if(Zenner.TimeShengYuSec == 0 && Zenner.TimeQuanTime >= ZENNER_TIME_2HOURS && Zenner.TimeFlag == 0 && Zenner.TimeBaoJingSec) 
  {
    Zenner.TimeFlag = 1;
  }
  //安检剩余时间小于报警时间，强制关阀报警
  if(Zenner.TimeShengYuSec <= Zenner.TimeBaoJingSec && Zenner.TimeBaoJingSec) 
  {
    Zenner.TimeBaoJingFlag = 1;
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerAlarmLogicWork
Description: Zenner报警工作逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerAlarmLogicWork(void)
{
  //阀门故障循环报警
  if(Zenner.ValveStatus&&(Zenner.DLFlag1||Zenner.DLFlag2||Zenner.XLFlag1||Zenner.XLFlag2||Zenner.TimeFlag||Zenner.ValveErrorStatus))
  {
    Zenner.TimeFlagSendTimeCount ++;
    if(Zenner.TimeFlagSendTimeCount >= Zenner.TimeFlagSendTimeSet && Zenner.TimeFlagSendTimesCount < Zenner.TimeFlagSendTimesSet && Zenner.TimeFlagSendTimeSet && Zenner.TimeFlagSendTimesSet && (Zenner.QMFlag == 0 || Zenner.ErrorLevel3Flag))
    {
      Zenner.TimeFlagSendTimeCount = 0;
      Zenner.TimeFlagSendTimesCount ++;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
  else if(Zenner.ValveErrorStatus == 0)
  {
    Zenner.TimeFlagSendTimeCount = 0;
    Zenner.TimeFlagSendTimesCount = 0;
  }
  //气密性故障循环报警
  if(Zenner.QMFlag)
  {
    Zenner.QMFlagSendTimeCount ++;
    if(Zenner.QMFlagSendTimeCount >= Zenner.QMFlagSendTimeSet && Zenner.QMFlagSendTimesCount < Zenner.QMFlagSendTimesSet && Zenner.QMFlagSendTimeSet && Zenner.QMFlagSendTimesSet)
    {
      //取消阀门故障循环报警
      Zenner.TimeFlagSendTimesCount = Zenner.TimeFlagSendTimesSet;
      Zenner.QMFlagSendTimeCount = 0;
      Zenner.QMFlagSendTimesCount ++;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
  else if(Zenner.QMFlag == 0)
  {
    Zenner.QMFlagSendTimeCount = 0;
    Zenner.QMFlagSendTimesCount = 0;
  }
}

/*----------------------------------------------------------
ModuleName: ZennerLogicWork
Description: Zenner工作逻辑
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerLogicWork(void)
{
  XL1_MSVtime++;
  DL1_MSVtime++;
  //强制关阀pulse计时
  if(Zenner.TimeQuanTime < 10000) 
  {
    Zenner.TimeQuanTime ++;
  }
  //超流量-大漏-逻辑服务函数
  ZennerDLLogicWork();
  //连续流量-小漏-逻辑服务函数
  ZennerXLLogicWork();
  //安检-逻辑服务函数
  ZennerTimeLogicWork();
  //循环报警-逻辑服务函数
  ZennerAlarmLogicWork();
}

/*----------------------------------------------------------
ModuleName: Fast_Low_Power
Description: 电机检测
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Fast_Low_Power(void)
{
  //关阀标志
  if((Zenner.TimeFlag || Zenner.DLFlag1 || Zenner.DLFlag2 || ((Zenner.XLFlag1 || Zenner.XLFlag2) && Zenner.XLCloseEnableFlag)))
  {
    //充电完成
    if(Zenner.ValveStatus == 0)         
    {      
      Zenner.ValveErrorStatus = Zenner_Valve_Work(ZENNER_CLOSE); 
      Zenner.ValveStatus = 1;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
}


