/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: main.c
Author: LBJ
Description: 可控阀门-机械阀门主程序
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/25      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

/* Includes ---------------------------------------------*/
#include "stm8l15x.h"
#include "sysdata.h"
#include "uart.h"
#include "Radio.h"
#include "record.h"
#include "main.h"
#include "gpio.h"
#include "time.h"
#include "MS.h"


/* Functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN FP */
void IWDG_Init(void);
/* USER CODE END FP */





int main(void)
{
  //延时，等待系统电源稳定
  Delay_5us(30*200);         
  //内部高速时钟初始化
  CLK_Init();          
  //电池低电压检测
  Battery_Check_2V45(); 
  //从eeprom读取，或设置默认值              
  MS_Data_Read();
  Zenner_Data_Read();
  //外部32.768K始终初始化 + RTC实时时钟配置
  RTC_Config();         
  //设置RTC闹钟时间，就是发送时间，1024个为一组，
  //每个设置发送间隔时间为84秒，一共约1天  
  Set_RTC_Alarm(MS.MSID % 1024 * 84);        
  //低功耗设置 + IO口配置
  MX_GPIO_Init();              
  //5043初始化，进入低功耗
  AX5043_Init();
  //关电量检测
  Battery_Check_End();             
  //RTC定时唤醒，2048 = 1秒唤醒一次
  Set_RTC_WakeUp(2047);         
  
  EX_EEPROM_Init();
  //看门狗初始化
  IWDG_Init();         
  //发射一次数据
  sysflag.send = 1;
  while (1)
  {
    //喂狗
    IWDG_ReloadCounter();  
    //串口，发送一次，连续发送，都不进低功耗            
    if(sysflag.uart == 0 && sysflag.send == 0 && sysflag.send2min == 0)       
    {
      //快速低功耗函数
      FastLowPowerInit();               
      do
      { 
        //快速睡眠标志清零
        sysflag.FastSleep = 1;         
        //进入低功耗睡眠  
        halt();         
        //唤醒后喂狗
        IWDG_ReloadCounter();
        
        Fast_Low_Power();
        //计数40000次约11小时6分钟40秒，保存数据一次。
        sysdata.SaveNum ++;             
        if(sysdata.SaveNum >= 40000)
        {
          sysdata.SaveNum = 0;
          //保存数据
          MS_Data_Save();
        }
        
      }
      //快速睡眠标志，如果不需要处理别的事情唤醒后直接睡眠，flag = 1；      
      while(sysflag.FastSleep);         
      //开启内部高速时钟
      CLK_Init();               
    }
    //需要执行发送一次服务
    else if(sysflag.send)               
    {
      
      Zenner_Valve_State();

      MSSend_And_Save();
      
      Zenner_Data_Save();
      //清发送标志
      sysflag.send = 0;        
    }
    else if(sysflag.uart)               //需要执行串口服务
    {
      Fast_Low_Power();
      if(UartWork()) sysflag.uart = 0;          //执行串口服务，返回1表示，执行完成，清标志位
      Delay_5us(30*200);
    }
    else if(sysflag.send2min)           //如果需要执行连续发送服务
    {
      sysflag.send2min = 0;             //清标志位
      
      MSSend_Continuous();          //执行连续发送服务函数
    }
  }
}



/*----------------------------------------------------------
ModuleName: IWDG_Init
Description: 看门狗初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void IWDG_Init(void)
{
  
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(255);
  IWDG_ReloadCounter();
  
}

/*----------------------------------------------------------
ModuleName: Battery_Check_2V45
Description: 2.45基准电池低电压检测（低电压检测）
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_2V45(void)
{
    
  sysdata.PVDBatteryFlag = 0;
  sysdata.PVDLBatteryFlag1 = 0;
  //PVD检测关闭
  PWR_PVDCmd(DISABLE); 
  //PVD检测配置2.45V基准电压
  PWR_PVDLevelConfig(PWR_PVDLevel_2V45); 
  //PVD检测使能
  PWR_PVDCmd(ENABLE); 
  //PVD检测中断标志位清楚
  PWR_PVDClearITPendingBit();
  //PVD检测中断使能
  PWR_PVDITConfig(ENABLE);                                
}

/*----------------------------------------------------------
ModuleName: Battery_Check_2V05
Description: 2.05基准电池低电压检测（超低电压检测）
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_2V05(void)   //阀门低电压
{ 
  sysdata.PVDLBatteryFlag2 = 0;
  //PVD检测关闭
  PWR_PVDCmd(DISABLE); 
  //PVD检测配置2.05V基准电压
  PWR_PVDLevelConfig(PWR_PVDLevel_2V05);                  // pvd修改为2.05V
  //PVD检测使能  
  PWR_PVDCmd(ENABLE);                                     //PVD使能
  //PVD检测中断标志位清楚
  PWR_PVDClearITPendingBit();
  //PVD检测中断使能
  PWR_PVDITConfig(ENABLE);                                //PVD中断使能
}

/*----------------------------------------------------------
ModuleName: Battery_Check_End
Description: 关闭电池低电压检测
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_End(void)
{
  //PVD关闭
  PWR_PVDCmd(DISABLE); 
  //PVD中断关闭
  PWR_PVDITConfig(DISABLE);
  //清除中断标志
  PWR_PVDClearITPendingBit();

}







