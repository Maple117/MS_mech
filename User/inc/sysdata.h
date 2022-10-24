
#ifndef __SYSDATA_H_
#define __SYSDATA_H_

#include "stm8l15x.h"

#define EEPROM_ADDR 0x1030
#define EEPROM_ADDR_BANK1 EEPROM_ADDR
#define EEPROM_ADDR_BANK2 EEPROM_ADDR + 50

#define EEPROM_DATA_LEN 41

#define TIME_MIFENG 187200
#define TIME_2HOURS 7200

#define ZENNER_TIME_2HOURS 7200
#define ZENNER_CHANGE_TIME (12 + 1)


#define ZENNER_OPEN 0
#define ZENNER_CLOSE 1




uint8_t Zenner_Valve_State(void);
uint8_t Zenner_Valve_Work(uint8_t da);
void Fast_Low_Power(void);
void Zenner_Work_State_Init(void);
void Zenner_Data_Save(void);
void Zenner_Data_Read(void);










typedef struct
{
  /**************************************************************/
  u8 SaveStartFlag;          //�洢������Чλ-��ʼλ
  
  u8 DLFlag1;           //������-����1-������־
  u16 DLSecSet1;        //������-����1-�������ʱ
  u16 DLTimesSet1;      //������-����1-����������
  u8 DLFlag2;           //������-����2-������־
  u16 DLSecSet2;        //������-����2-�������ʱ
  u16 DLTimesSet2;      //������-����2-����������
  u8 DL_Pulse;
  u8 DL_Pulse2;
  
  
  
  u8 XLFlag1;           //��������-����1-������־
  u32 XLSecSet1;        //��������-����1-���������
  u8 XLTimesSet1;       //��������-����1-����������
  u8 XLFlag2;           //��������-����2-������־
  u32 XLSecSet2;        //��������-����2-���������
  u8 XLTimesSet2;       //��������-����2-����������
  u8 XLCloseEnableFlag; //��������-�����ط�ʹ������
  
  u8 TimeFlag;                  //����-������־
  u32 TimeShengYuSec;           //����-ʣ��ʱ��
  u32 TimeBaoJingSec;           //����-����ʱ��
  u32 TimeFlagSendTimeSet;      //����-ѭ����������ʱ������
  u8 TimeFlagSendTimesSet;      //����-ѭ���������ʹ�������
  
  u8 QMFlag;                    //������-������־
  u32 QMFlagSendTimeSet;        //������-ѭ����������ʱ������
  u8 QMFlagSendTimesSet;        //������-ѭ���������ʹ�������
  
  u8 ValveStatus;               //����״̬-0������ 1���ر�
  u8 ValveErrorStatus;          //���Ź���״̬-0������ 1����ת 2���Ͽ�
  
  u16 ValveData1;
  u16 ValveData2;
  
  u8 Reserved1;
  u8 Reserved2;
  
  u8 MeterFlag;         //¯�߱�ʶ
  
  u8 SaveCheck;         //�洢����У��λ-����λ

  /**************************************************************/
  
  /**************************************************************/
  u8 DLStartFlag1;       //������-����1-��⿪ʼ��־
  u8 DLSecConut1;        //������-����1-�������ʱ
  u8 DLTimesCount1;      //������-����1-����������
  u8 DLPulseCount1;      //������-����1-�������������
  u8 DLChangeFlag1;      //������-����1-��ǰ��������־
  u8 DLChangeTimeCount1; //������-����1-��ǰ��糬ʱ��ʱ
  
  u8 DLStartFlag2;        //������-����2-��⿪ʼ��־
  u8 DLSecConut2;         //������-����2-�������ʱ
  u8 DLTimesCount2;       //������-����2-����������
  u8 DLPulseCount2;       //������-����2-�������������
  u8 DLChangeFlag2;       //������-����2-��ǰ��������־
  u8 DLChangeTimeCount2;  //������-����2-��ǰ��糬ʱ��ʱ
  /**************************************************************/

  /**************************************************************/
  u32 XLSecConut1;        //��������-����1-�������ʱ
  u8 XLTimesCount1;       //��������-����1-����������
  u8 XLStartFlag1;        //��������-����1-���������־
  u8 XLPulseCount1;       //��������-����1-�������������
  
  u32 XLSecConut2;        //��������-����2-�������ʱ
  u8 XLTimesCount2;       //��������-����2-����������
  u8 XLStartFlag2;        //��������-����2-���������־
  u8 XLPulseCount2;       //��������-����2-�������������
  /**************************************************************/ 
  
  /**************************************************************/
  u32 TimeFlagSendTimeCount;//����-ѭ����������ʱ���ʱ
  u8 TimeFlagSendTimesCount;//����-ѭ���������ʹ�������
  u16 TimeQuanTime;         //����-������Сʱ��ʱ
  u8 TimeBaoJingFlag;       //����-���ѱ�־
  /**************************************************************/
  
  /**************************************************************/
  u32 QMFlagSendTimeCount;  //������-ѭ����������ʱ���ʱ
  u8 QMFlagSendTimesCount;  //������-ѭ���������ʹ�������
  /**************************************************************/
  
  /**************************************************************/
  u8 ErrorLevel3Flag;       //�ȼ�3������־
  u8 ValveCheckErrorStatus; //����ÿ�ռ��״̬-0������ 2���Ͽ�
  /**************************************************************/
  
}sZenner;
extern sZenner Zenner;






typedef struct
{
  unsigned char State;
  unsigned char Versions;
  unsigned char Tamper;
  unsigned char Battery;
  uint32_t MSID;
  uint32_t MSV;
  uint32_t MST;
  
  unsigned char DL_Sec;         //16
  unsigned char DL_Times;
  unsigned char DL_Times2;

  unsigned char XL_Enable;
  unsigned char XL_Times;
  unsigned char XL_Times2;
  unsigned int XL_Sec;
  unsigned int XL_Sec2;
  
  unsigned long TSY_Sec;      //28
  unsigned long TBJ_Sec;
  
  unsigned int Meter_Flag;      //36
  
  unsigned char aaa1;           //38
  
  unsigned char T_Month;
  unsigned char T_Day;
  unsigned char T_XyMonth;
  unsigned char T_Hour;
  
  uint16_t T_Sec;
  uint32_t TXL_Sec;
  uint32_t TXL_Sec2;
  
  unsigned char cDL_Times;
  unsigned char cXL_Times;
  unsigned char cXL_Times2;
  unsigned char cJiQuan;
  unsigned char cJiQuan2;
  unsigned char cMiFeng;
  
  uint32_t cQuanTime;
  
  uint32_t S1TIME;
  
  uint32_t SaveNum;
  
  unsigned char PVDBatteryFlag;
  unsigned char PVDLBatteryFlag1;
  unsigned char PVDLBatteryFlag2;
  unsigned char SendMode;
  
  unsigned char VTTime;
  unsigned char VTFlag;
  unsigned char TTTime;
  unsigned char TTFlag;
  
  unsigned char KeyTime;
  
  unsigned char cZlDelayTime;
  unsigned char cChargeDelayTime;
  unsigned char cCloseDelayTime;
  
  unsigned char cXL_StartFlag;
  unsigned char cXL_StartFlag2;
  
  unsigned char DLCloseFlag;
  unsigned char XLCloseFlag;
  unsigned char TIMECloseFlag;
  unsigned char cChangeDelayTime;
  
  unsigned char cDL_IDLETime;
  
  unsigned char cQuanTimeBufCount;
  unsigned char cQuanTimeBuf[10];
  
  unsigned char cQuanTimeBufI;
  unsigned int cQuanTimeBufSum;
  
  unsigned char TestOpenFlag;
  
  unsigned char SyadataError;
  
}SYSDATA_STRUCT;

typedef struct
{
  unsigned char send;
  unsigned char uart;
  
  unsigned char FastSleep;
  
  unsigned char send2min;
  
  uint32_t WhileTime;
  uint32_t WhileNum;
  
  unsigned char XLFlag;
  unsigned char DLFlag;
  unsigned char TimeFlag;
  unsigned char MFFlag;
  unsigned char MFFirstFlag;
  unsigned char CloseFlag;
  unsigned char ZlErrorFlag;
  
  unsigned char ErrorClose;
  unsigned char ErrorShort;
  
}SYSFLAG_STRUCT;

extern SYSDATA_STRUCT sysdata;
extern SYSFLAG_STRUCT sysflag;






#endif




