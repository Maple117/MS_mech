
/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: zenner.C
Author: LBJ
Description: zenner�������ļ�
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
u32 XL1_MSVtime;//��¼����msv�����ʱ��
u32 DL1_MSVtime;//��¼����msv�����ʱ��
float ZennerADC1,ZennerADC2,vcc1,vcc2;
sZenner Zenner;
/* USER CODE END V */


/*----------------------------------------------------------
ModuleName: Zenner_Data_Init
Description: zenner���ݱ�ʶ��ʼ��
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Data_Init(void)
{
  //�洢������Чλ-��ʼλ
  Zenner.SaveStartFlag = 0xaa;          
  /*------------------------------��©���ݱ�ʶ-------------------------------*/
  //������-����1-������־
  Zenner.DLFlag1 = 0;           
  //������-����1-�������ʱ
  Zenner.DLSecSet1 = 22;         
  //������-����1-����������
  Zenner.DLTimesSet1 = 190;       
  //������-����2-������־
  Zenner.DLFlag2 = 0;           
  //������-����2-�������ʱ
  Zenner.DLSecSet2 = 10;         
  //������-����2-����������
  Zenner.DLTimesSet2 = 6;       
  //������-����1����
  Zenner.DL_Pulse = 1;  
  //������-����2����
  Zenner.DL_Pulse2 = 1;  
  
  /*------------------------------С©���ݱ�ʶ-------------------------------*/
  //��������-����1-������־
  Zenner.XLFlag1 = 0;           
  //��������-����1-���������(60*60s)
  Zenner.XLSecSet1 = 3600;        
  //��������-����1-����������
  Zenner.XLTimesSet1 = 24;       
  //��������-����2-������־
  Zenner.XLFlag2 = 0;           
  //��������-����2-��������ã�180*60s��
  Zenner.XLSecSet2 = 10800;        
  //��������-����2-����������
  Zenner.XLTimesSet2 = 24;       
  //��������-�����ط�ʹ������
  Zenner.XLCloseEnableFlag = 1; 
  
  /*------------------------------�������ݱ�ʶ-------------------------------*/
  //����-������־
  Zenner.TimeFlag = 0;                  
  //����-ʣ��ʱ�䣨75*30*24*3600S��
  Zenner.TimeShengYuSec = 194400000;           
  //����-����ʱ�䣨6*30*24*3600S��
  Zenner.TimeBaoJingSec = 15552000;           
  //����-ѭ����������ʱ������
  Zenner.TimeFlagSendTimeSet = 3600;      
  //����-ѭ���������ʹ�������  ȡ��
  Zenner.TimeFlagSendTimesSet = 72;           
  
  /*------------------------------�������ݱ�ʶ-------------------------------*/
  //������-������־
  Zenner.QMFlag = 0;                    
  //������-ѭ����������ʱ������
  Zenner.QMFlagSendTimeSet = 3600;        
  //������-ѭ���������ʹ�������  ȡ��
  Zenner.QMFlagSendTimesSet = 72;       
  
  /*------------------------------�������ݱ�ʶ-------------------------------*/
  //����״̬-0������ 1���ر�
  Zenner.ValveStatus = 0;       
  //���Ź���״̬-0������ 1����ת 2���Ͽ�
  Zenner.ValveErrorStatus = 0;  
  
  Zenner.ValveData1 = 0;
  Zenner.ValveData2 = 0;
  
  /*------------------------------���ݼ�¼��ʶ-------------------------------*/
  Zenner.Reserved1 = 0;
  Zenner.Reserved2 = 0;
  
  /*------------------------------¯�߱�ʶ-------------------------------*/
  Zenner.MeterFlag = 0;         
  
}
/*----------------------------------------------------------
ModuleName: Zenner_Work_State_Init
Description: 
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Zenner_Work_State_Init(void)//��־λ���
{
  /*-----------------��©����״̬��ʶ��ʼ��-----------------*/
  //������-����1-������־
  Zenner.DLFlag1 = 0;           
  //������-����2-������־
  Zenner.DLFlag2 = 0;  
  //������-����1-��⿪ʼ��־
  Zenner.DLStartFlag1 = 0;
  //������-����1-�������ʱ
  Zenner.DLSecConut1 = 0;
  //������-����1-����������
  Zenner.DLTimesCount1 = 0;
  //������-����1-�������������
  Zenner.DLPulseCount1 = 0;
  //������-����1-��ǰ��������־
  Zenner.DLChangeFlag1 = 0;
  //������-����1-��ǰ��糬ʱ��ʱ
  Zenner.DLChangeTimeCount1 = 0;
  //������-����2-��⿪ʼ��־
  Zenner.DLStartFlag2 = 0;
  //������-����2-�������ʱ
  Zenner.DLSecConut2 = 0;
  //������-����2-����������
  Zenner.DLTimesCount2 = 0;
  //������-����2-�������������
  Zenner.DLPulseCount2 = 0;
  //������-����2-��ǰ��������־
  Zenner.DLChangeFlag2 = 0;
  //������-����2-��ǰ��糬ʱ��ʱ
  Zenner.DLChangeTimeCount2 = 0;
  
  /*-----------------С©����״̬��ʶ��ʼ��-----------------*/
  //��������-����1-������־
  Zenner.XLFlag1 = 0;           
  //��������-����2-������־
  Zenner.XLFlag2 = 0;    
  //��������-����1-�������ʱ
  Zenner.XLSecConut1 = 0;
  //��������-����1-����������
  Zenner.XLTimesCount1 = 0;
  //��������-����1-���������־
  Zenner.XLStartFlag1 = 0;
  //��������-����1-�������������
  Zenner.XLPulseCount1 = 0;
  //��������-����2-�������ʱ
  Zenner.XLSecConut2 = 0;
  //��������-����2-����������
  Zenner.XLTimesCount2 = 0;
  //��������-����2-���������־
  Zenner.XLStartFlag2 = 0;
  //��������-����2-�������������
  Zenner.XLPulseCount2 = 0;
  
  /*-----------------���칤��״̬��ʶ��ʼ��-----------------*/
  //����-������־
  Zenner.TimeFlag = 0;         
  //����-ѭ����������ʱ���ʱ
  Zenner.TimeFlagSendTimeCount = 0;
  //����-ѭ���������ʹ�������
  Zenner.TimeFlagSendTimesCount = 0;
  //����-������Сʱ��ʱ
  Zenner.TimeQuanTime = 0;
  //����-���ѱ�־
  Zenner.TimeBaoJingFlag = 0;
  
  /*-----------------�����Թ���״̬��ʶ��ʼ��-----------------*/
  //������-������־
  Zenner.QMFlag = 0;          
  //������-ѭ����������ʱ���ʱ
  Zenner.QMFlagSendTimeCount = 0;
  //������-ѭ���������ʹ�������
  Zenner.QMFlagSendTimesCount = 0;
  
  /*-----------------���Ź���״̬��ʶ��ʼ��-----------------*/  
  //���Ź���״̬-0������ 1����ת 2���Ͽ�
  Zenner.ValveErrorStatus = 0;  
  //����ÿ�ռ��״̬-0������ 2���Ͽ�
  Zenner.ValveCheckErrorStatus = 0;   
  //����ÿ�ռ��״̬-0������ 2���Ͽ�
  Zenner.ValveCheckErrorStatus = 0;     
  //�ȼ�3������־
  Zenner.ErrorLevel3Flag = 0;
  
}


/*----------------------------------------------------------
ModuleName: Zenner_Data_Save
Description: Zenner���ݴ洢
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
Description: Zenner��ȡ����
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
Description: ���ͨ�ϼ��
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
Description: ������
Params: null
Return: ����0������������1����ת������2���Ͽ�
Notice: null
-----------------------------------------------------------*/
uint8_t Zenner_Valve_Work(uint8_t state)
{
  //��¼�ߵ͵�ѹ����1.2-1.8
  uint8_t count1=0;  
  //��¼�ߵ͵�ѹ����2.0-2.8
  uint8_t count2=0;   
  //��¼�ߵ͵�ѹ����0-1.0
  uint8_t count3=0;      
  int adc_value=0,adc_value2=0;
  //���ų��͵�ѹ��� 
  Battery_Check_2V05();
  MX_ADC_Init();
  MOTO_EN1(1);
  MOTO_EN2(1);
  Delay_5us(5*200);
  //����
  if(state == ZENNER_OPEN) 
  {
    MOTO_IN1(0);
    MOTO_IN2(1);
  }
  //�ط�
  else        
  {
    MOTO_IN1(1);
    MOTO_IN2(0);    
  }
  Delay_ms(900);
  for(uint8_t i = 0;i<21;i++)
  {
    //ι��
    IWDG_ReloadCounter();       
    //vcc��Ӧ�Ĵ���
    adc_value = ADC_Read_Internal_Voltage(); 
    vcc1 = 1.225*4096/adc_value;
    //��ʱ�ĵ�ѹ�Ĵ���ֵ
    adc_value2 = ADC_Read_VFB();  
    //������ѹ
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
  //��������
  MX_ADC_DeInit(); 
  MOTO_IN1(0);
  MOTO_IN2(0);  
  MOTO_EN1(0);
  MOTO_EN2(0);
  //���ŵ�ѹ����
  Battery_Check_End();
  
  if((count1>20)||(count2>20))
  {
    //��ת
    return 1;
  }
  else if(count3>=20)
  {
    //�Ͽ�
    return 2;
  }
  else
  {
    //����
    return 0;
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerErrorLevel3
Description: ���������¼�����
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
Description: Zenner�����߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerPulseLogic(void)
{
  //���嵽����ʱ�����
  XL1_MSVtime = 0;
  //���嵽����ʱ�����
  DL1_MSVtime = 0;
  //ǿ�ƹط�pulse��ʱ��λ
  Zenner.TimeQuanTime = 0;
  //�����Ա���
  if(Zenner.ValveStatus == ZENNER_CLOSE)
  {
    if(Zenner.QMFlag == 0)
    {
      Zenner.QMFlag = 1;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
  //��������-С©-����1�������
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
  //��������-С©-����2�������
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
  //������-��©-����1�������
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
  //������-��©-����2�������
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
Description: ������-��©�����߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerDLLogicWork(void)
{
  //������-��©-����1���
  if(Zenner.DLStartFlag1)
  {
    Zenner.DLSecConut1 ++;
  }
  else 
  {
    Zenner.DLSecConut1 = 0;
  }
  //����ʱ�䣬��Ϊû�д���DL����
  if(DL1_MSVtime>Zenner.DLSecSet1)
  {
    //�����ڳɹ�����
    Zenner.DLTimesCount1 = 0;
    Zenner.DLStartFlag1 = 0;
    //�������������
    Zenner.DLPulseCount1 = 0;
    Zenner.DLSecConut1 = 0;
  }
  //������-��©-����1���� ����������22 ��һ���¼���
  if(Zenner.DLSecConut1 >= Zenner.DLSecSet1)
  {
    Zenner.DLSecConut1 = 0;
    if(Zenner.DLPulseCount1)
    {
      Zenner.DLPulseCount1 = 0;       
      Zenner.DLTimesCount1 ++;
      //DL����1����  ÿ�������������190�������¼���
      if(Zenner.DLTimesCount1 >= Zenner.DLTimesSet1)
      {
        Zenner.DLFlag1 = 1;
        //DL����1����  �����Թ��ϣ������¼���
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
  //������-��©-����2-
  if(Zenner.DLTimesSet2 && Zenner.DLSecSet2)
  {
    //������-��©��糬ʱ2
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
    //������-��©2
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
Description: ��������-С©�����߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerXLLogicWork(void)
{
  //��������-С©����1���
  if(Zenner.XLSecSet1)
  {
    if(Zenner.XLStartFlag1) 
    {
      Zenner.XLSecConut1 ++;
    }
    else 
    {
      //ʱ����ʵ��ֵ���
      Zenner.XLSecConut1 = 0;
    }
    //�ж�XL1_MSVtime�Ƿ񳬳�ʱ��
    if(XL1_MSVtime>=Zenner.XLSecSet1)
    {
      //����ʱ�䣬��Ϊû�д���XL����
      Zenner.XLTimesCount1 = 0;
      Zenner.XLStartFlag1 = 0;
      Zenner.XLPulseCount1 = 0;  
    }
    //��������-С©-����1���� ���ʱ�����3600S  ��һ���¼���
    if(Zenner.XLSecConut1 >= Zenner.XLSecSet1)
    {
      Zenner.XLSecConut1 = 0;
      if(Zenner.XLPulseCount1)
      {
        Zenner.XLTimesCount1 ++;
        //��������-С©-����1���� ����������24  �������¼���
        if(Zenner.XLTimesCount1 >= Zenner.XLTimesSet1 && Zenner.XLTimesSet1)
        {
          Zenner.XLFlag1 = 1;
          //�����Թ���  �������¼���
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
   //��������-С©����2���
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
      //����ʱ�䣬��Ϊû�д���XL����
      Zenner.XLTimesCount2 = 0;
      Zenner.XLStartFlag2 = 0;
      Zenner.XLPulseCount2 = 0;
    }
    //��������-С©-����2���� ���ʱ�����10800S  ��һ���¼���    
    if(Zenner.XLSecConut2 >= Zenner.XLSecSet2)
    {
      Zenner.XLSecConut2 = 0;
      if(Zenner.XLPulseCount2)
      {
        Zenner.XLTimesCount2 ++;
        //��������-С©-����2���� ����������24  �������¼���        
        if(Zenner.XLTimesCount2 >= Zenner.XLTimesSet2 && Zenner.XLTimesSet2)
        {
          Zenner.XLFlag2 = 1;
          //�����Թ���  �������¼���          
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
Description: Zenner���칤���߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerTimeLogicWork(void)
{
  //ǿ�ƹط�
  if(Zenner.TimeShengYuSec)
  {
    Zenner.TimeShengYuSec --;
    //����ʣ��ʱ��ﵽ����ʱ��㣨һ���¼���
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
   //����ʣ��ʱ��ľ�������2Сʱ�İ�ȫʱ�䣨һ���¼���
  if(Zenner.TimeShengYuSec == 0 && Zenner.TimeQuanTime >= ZENNER_TIME_2HOURS && Zenner.TimeFlag == 0 && Zenner.TimeBaoJingSec) 
  {
    Zenner.TimeFlag = 1;
  }
  //����ʣ��ʱ��С�ڱ���ʱ�䣬ǿ�ƹط�����
  if(Zenner.TimeShengYuSec <= Zenner.TimeBaoJingSec && Zenner.TimeBaoJingSec) 
  {
    Zenner.TimeBaoJingFlag = 1;
  }
}

/*----------------------------------------------------------
ModuleName:  ZennerAlarmLogicWork
Description: Zenner���������߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerAlarmLogicWork(void)
{
  //���Ź���ѭ������
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
  //�����Թ���ѭ������
  if(Zenner.QMFlag)
  {
    Zenner.QMFlagSendTimeCount ++;
    if(Zenner.QMFlagSendTimeCount >= Zenner.QMFlagSendTimeSet && Zenner.QMFlagSendTimesCount < Zenner.QMFlagSendTimesSet && Zenner.QMFlagSendTimeSet && Zenner.QMFlagSendTimesSet)
    {
      //ȡ�����Ź���ѭ������
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
Description: Zenner�����߼�
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void ZennerLogicWork(void)
{
  XL1_MSVtime++;
  DL1_MSVtime++;
  //ǿ�ƹط�pulse��ʱ
  if(Zenner.TimeQuanTime < 10000) 
  {
    Zenner.TimeQuanTime ++;
  }
  //������-��©-�߼�������
  ZennerDLLogicWork();
  //��������-С©-�߼�������
  ZennerXLLogicWork();
  //����-�߼�������
  ZennerTimeLogicWork();
  //ѭ������-�߼�������
  ZennerAlarmLogicWork();
}

/*----------------------------------------------------------
ModuleName: Fast_Low_Power
Description: ������
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Fast_Low_Power(void)
{
  //�ط���־
  if((Zenner.TimeFlag || Zenner.DLFlag1 || Zenner.DLFlag2 || ((Zenner.XLFlag1 || Zenner.XLFlag2) && Zenner.XLCloseEnableFlag)))
  {
    //������
    if(Zenner.ValveStatus == 0)         
    {      
      Zenner.ValveErrorStatus = Zenner_Valve_Work(ZENNER_CLOSE); 
      Zenner.ValveStatus = 1;
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
  }
}


