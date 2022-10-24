/**
******************************************************************************
* @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
* @author  MCD Application Team
* @version V1.6.1
* @date    30-September-2014
* @brief   Main Interrupt Service Routines.
*          This file provides template for all peripherals interrupt service routine.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "uart.h"
#include "sysdata.h"
#include "record.h"
#include "zenner.h"
#include "MS.h"
/** @addtogroup STM8L15x_StdPeriph_Template
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
* @brief Dummy interrupt routine
* @par Parameters:
* None
* @retval 
* None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
* @brief TRAP interrupt routine
* @par Parameters:
* None
* @retval 
* None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief FLASH Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief DMA1 channel0 and channel1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief DMA1 channel2 and channel3 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief RTC / CSS_LSE Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_WUT);	
    MS_Zenner_Pulse_Logic();
    //长按连续发送
    if(sysdata.KeyTime != 0 && sysdata.KeyTime < 10)
    {
      sysdata.KeyTime ++;
      if(sysdata.KeyTime >= 10)
      {
        sysflag.send2min = 1;
        sysflag.FastSleep = 0;
      }
    }  
    //Zenner时基服务函数
   ZennerLogicWork();
  }
  
  //闹钟
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    sysflag.send = 1;
    sysflag.FastSleep = 0;
    RecorderDays ++;
  }
}
/**
* @brief External IT PORTE/F and PVD Interrupt routine.
* @param  None
* @retval None
*/
#include "uart.h"
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  if(PWR_GetFlagStatus(PWR_FLAG_PVDIF) != RESET)
  {
    PWR_PVDClearITPendingBit(); 
    sysdata.PVDBatteryFlag = 1;
    if((PWR->CSR1 & 0x0E) == PWR_PVDLevel_2V45)
    {
      PWR_PVDCmd(DISABLE); 
      PWR_PVDLevelConfig(PWR_PVDLevel_2V26);
      PWR_PVDCmd(ENABLE);
    }
    else if((PWR->CSR1 & 0x0E) == PWR_PVDLevel_2V26)
    {
      sysdata.PVDLBatteryFlag1 = 1;
    }
    else if((PWR->CSR1 & 0x0E) == PWR_PVDLevel_2V05)
    {
      sysdata.PVDLBatteryFlag2 = 1;
    }
    
  }
}

/**
* @brief External IT PORTB / PORTG Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PORTD /PORTH Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN0 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin0);
  
  
  if((GPIOB->IDR & 0x01) == 0)
  {
    //if(sysdata.KeyTime == 0)
    sysdata.KeyTime = 1;
  }
  else
  {
    if(sysdata.KeyTime <= 4)      //s + 2
    {
      sysflag.send = 1;
      sysflag.FastSleep = 0;
    }
    sysdata.KeyTime = 0;
  }
  
}

/**
* @brief External IT PIN1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)           //ganrao
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);
  //获取干扰
  Get_Interference();
}

/**
* @brief External IT PIN2 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin2);
  //获取信号
  Get_Signal();
  
}

/**
* @brief External IT PIN3 Interrupt routine.
* @param  None
* @retval None
*/
#include "uart.h"
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  //清除中断标志
  EXTI_ClearITPendingBit (EXTI_IT_Pin3);
  Uart_Init();
  sysflag.uart = 1;
  uart.Start = 1;
  sysflag.FastSleep = 0;
  
}

/**
* @brief External IT PIN4 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)   
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  EXTI_ClearITPendingBit (EXTI_IT_Pin4);
}

/**
* @brief External IT PIN5 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)   
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  EXTI_ClearITPendingBit (EXTI_IT_Pin5);
}

/**
* @brief External IT PIN6 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN7 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief LCD /AES Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief CLK switch/CSS/TIM1 break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief ADC1/Comparator Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}


/**
* @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
* @param  None
* @retval None
*/


INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief TIM1 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief TIM4 Update/Overflow/Trigger Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief SPI1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */		
}

/**
* @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/

INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
  
  unsigned char data = USART_ReceiveData8 (USART1);
  
  if(uart.ReceiveLen==0)
  {
    if(data == 0x40)
    {
      uart.ReceiveBuf[uart.ReceiveLen++] = data;
    }
  }
  else
  {
    if(uart.ReceiveLen < 76)
    {
      uart.ReceiveBuf[uart.ReceiveLen++] = data;
    }
    else uart.ReceiveLen = 0;
  }
  //将接收到的数据发送出去
  //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));
}

/**
* @brief I2C1 / SPI2 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/