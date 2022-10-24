/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: main.c
Author: LBJ
Description: �ɿط���-��е����������
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
  //��ʱ���ȴ�ϵͳ��Դ�ȶ�
  Delay_5us(30*200);         
  //�ڲ�����ʱ�ӳ�ʼ��
  CLK_Init();          
  //��ص͵�ѹ���
  Battery_Check_2V45(); 
  //��eeprom��ȡ��������Ĭ��ֵ              
  MS_Data_Read();
  Zenner_Data_Read();
  //�ⲿ32.768Kʼ�ճ�ʼ�� + RTCʵʱʱ������
  RTC_Config();         
  //����RTC����ʱ�䣬���Ƿ���ʱ�䣬1024��Ϊһ�飬
  //ÿ�����÷��ͼ��ʱ��Ϊ84�룬һ��Լ1��  
  Set_RTC_Alarm(MS.MSID % 1024 * 84);        
  //�͹������� + IO������
  MX_GPIO_Init();              
  //5043��ʼ��������͹���
  AX5043_Init();
  //�ص������
  Battery_Check_End();             
  //RTC��ʱ���ѣ�2048 = 1�뻽��һ��
  Set_RTC_WakeUp(2047);         
  
  EX_EEPROM_Init();
  //���Ź���ʼ��
  IWDG_Init();         
  //����һ������
  sysflag.send = 1;
  while (1)
  {
    //ι��
    IWDG_ReloadCounter();  
    //���ڣ�����һ�Σ��������ͣ��������͹���            
    if(sysflag.uart == 0 && sysflag.send == 0 && sysflag.send2min == 0)       
    {
      //���ٵ͹��ĺ���
      FastLowPowerInit();               
      do
      { 
        //����˯�߱�־����
        sysflag.FastSleep = 1;         
        //����͹���˯��  
        halt();         
        //���Ѻ�ι��
        IWDG_ReloadCounter();
        
        Fast_Low_Power();
        //����40000��Լ11Сʱ6����40�룬��������һ�Ρ�
        sysdata.SaveNum ++;             
        if(sysdata.SaveNum >= 40000)
        {
          sysdata.SaveNum = 0;
          //��������
          MS_Data_Save();
        }
        
      }
      //����˯�߱�־���������Ҫ���������黽�Ѻ�ֱ��˯�ߣ�flag = 1��      
      while(sysflag.FastSleep);         
      //�����ڲ�����ʱ��
      CLK_Init();               
    }
    //��Ҫִ�з���һ�η���
    else if(sysflag.send)               
    {
      
      Zenner_Valve_State();

      MSSend_And_Save();
      
      Zenner_Data_Save();
      //�巢�ͱ�־
      sysflag.send = 0;        
    }
    else if(sysflag.uart)               //��Ҫִ�д��ڷ���
    {
      Fast_Low_Power();
      if(UartWork()) sysflag.uart = 0;          //ִ�д��ڷ��񣬷���1��ʾ��ִ����ɣ����־λ
      Delay_5us(30*200);
    }
    else if(sysflag.send2min)           //�����Ҫִ���������ͷ���
    {
      sysflag.send2min = 0;             //���־λ
      
      MSSend_Continuous();          //ִ���������ͷ�����
    }
  }
}



/*----------------------------------------------------------
ModuleName: IWDG_Init
Description: ���Ź���ʼ��
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
Description: 2.45��׼��ص͵�ѹ��⣨�͵�ѹ��⣩
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_2V45(void)
{
    
  sysdata.PVDBatteryFlag = 0;
  sysdata.PVDLBatteryFlag1 = 0;
  //PVD���ر�
  PWR_PVDCmd(DISABLE); 
  //PVD�������2.45V��׼��ѹ
  PWR_PVDLevelConfig(PWR_PVDLevel_2V45); 
  //PVD���ʹ��
  PWR_PVDCmd(ENABLE); 
  //PVD����жϱ�־λ���
  PWR_PVDClearITPendingBit();
  //PVD����ж�ʹ��
  PWR_PVDITConfig(ENABLE);                                
}

/*----------------------------------------------------------
ModuleName: Battery_Check_2V05
Description: 2.05��׼��ص͵�ѹ��⣨���͵�ѹ��⣩
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_2V05(void)   //���ŵ͵�ѹ
{ 
  sysdata.PVDLBatteryFlag2 = 0;
  //PVD���ر�
  PWR_PVDCmd(DISABLE); 
  //PVD�������2.05V��׼��ѹ
  PWR_PVDLevelConfig(PWR_PVDLevel_2V05);                  // pvd�޸�Ϊ2.05V
  //PVD���ʹ��  
  PWR_PVDCmd(ENABLE);                                     //PVDʹ��
  //PVD����жϱ�־λ���
  PWR_PVDClearITPendingBit();
  //PVD����ж�ʹ��
  PWR_PVDITConfig(ENABLE);                                //PVD�ж�ʹ��
}

/*----------------------------------------------------------
ModuleName: Battery_Check_End
Description: �رյ�ص͵�ѹ���
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Battery_Check_End(void)
{
  //PVD�ر�
  PWR_PVDCmd(DISABLE); 
  //PVD�жϹر�
  PWR_PVDITConfig(DISABLE);
  //����жϱ�־
  PWR_PVDClearITPendingBit();

}







