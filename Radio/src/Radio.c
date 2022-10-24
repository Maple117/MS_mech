/*-----------------------------------------------------------
Copyright (C), 2022, Ningbo Sentient Electronics Co.Ltd.
FileName: Radio.c
Author: LBJ
Description: AX5043的应用程序文件
Version: v0.0.1 
History: 
Date          Version         Author          Notes
2022/8/24      v0.0.1            LBJ            first version
-----------------------------------------------------------*/

/* Includes ---------------------------------------------*/

#include "spi.h"
#include "ax5043def.h"
#include "uart.h"
#include "stdio.h"
#include "sysdata.h"
#include "Radio.h"
#include "gpio.h"
#include "time.h"

/*       
AX5043发射功率设置:
13dbm   TXPWRCOEFFB1 0x0C  TXPWRCOEFFB0 0x00
12dbm   TXPWRCOEFFB1 0x09  TXPWRCOEFFB0 0xD4
10dbm   TXPWRCOEFFB1 0x07  TXPWRCOEFFB0 0x00
8.5dbm  TXPWRCOEFFB1 0x05  TXPWRCOEFFB0 0xBC
0dbm    TXPWRCOEFFB1 0x02  TXPWRCOEFFB0 0x07
*/

/* Macro ------------------------------------------------------------*/
/* USER CODE BEGIN M */

#define SYNCWORD 0xAACCAACC
#define PREAMBLE_LEN_BYTES 4
#define PREAMBLE_CHAR 0x55
#define PREAMBLE_FLAGS 0x38
#define SYNCWORD_FLAGS 0x18

/* USER CODE END M */

/* Variables ---------------------------------------------*/
/* USER CODE BEGIN V */

uint8_t GCV_PllRange;
uint8_t GCV_PllVcoi;

/* USER CODE END V */



/*----------------------------------------------------------
ModuleName: AX5043_Init
Description: 5043初始化
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void AX5043_Init(void)
{
  
  GPIO_WriteBit(TCXO_EN_GPIO_Port, TCXO_EN_Pin, SET);
  Spi_Init();           //SPI初始化
  Delay_5us(30*200);
  SpiWriteSingleAddressRegister(REG_AX5043_SCRATCH, 0x01);
  SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, 0x00);
  GPIO_WriteBit(TCXO_EN_GPIO_Port, TCXO_EN_Pin, RESET);
  Delay_5us(2*200);
  SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, 0x01);
  
}

/*----------------------------------------------------------
ModuleName: AX5043_Registers
Description: AX5043寄存器初始化设置
Params: null
Return: null
Notice: 
-----------------------------------------------------------*/
void AX5043_Registers(void)
{
  SpiWriteAutoAddressRegister(REG_AX5043_MODULATION           , 0x08);
  SpiWriteAutoAddressRegister(REG_AX5043_ENCODING             , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_FRAMING              , 0x06);
  SpiWriteAutoAddressRegister(REG_AX5043_PINFUNCSYSCLK        , 0x01);  
  SpiWriteAutoAddressRegister(REG_AX5043_MODCFGF              , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV2              , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV1              , 0x0a);
  SpiWriteAutoAddressRegister(REG_AX5043_FSKDEV0              , 0xb6);
  //REG_AX5043_MODCFGA  05是差分模式，06是单路模式
  SpiWriteAutoAddressRegister(REG_AX5043_MODCFGA              , 0x05);
  SpiWriteAutoAddressRegister(REG_AX5043_TXRATE2              , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_TXRATE1              , 0x01);
  SpiWriteAutoAddressRegister(REG_AX5043_TXRATE0              , 0x80);
  SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFA1         , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFA0         , 0x00);
  //发射功率配置 8.5dbm  TXPWRCOEFFB1 0x05  TXPWRCOEFFB0 0xBC
  SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB1         , 0x05);
  SpiWriteAutoAddressRegister(REG_AX5043_TXPWRCOEFFB0         , 0xBC);
  SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI              , 0x80);
  SpiWriteAutoAddressRegister(REG_AX5043_PLLRNGCLK            , 0x04);        
  SpiWriteAutoAddressRegister(REG_AX5043_PKTADDRCFG           , 0x80);	
  SpiWriteAutoAddressRegister(REG_AX5043_TMGTXBOOST           , 0x3e);
  SpiWriteAutoAddressRegister(REG_AX5043_TMGTXSETTLE          , 0x31);
  SpiWriteAutoAddressRegister(REG_AX5043_REF                  , 0x03);
  SpiWriteAutoAddressRegister(REG_AX5043_XTALOSC              , 0x04);          
  SpiWriteAutoAddressRegister(REG_AX5043_XTALAMPL             , 0x00);            
  SpiWriteAutoAddressRegister(REG_AX5043_PLLLOOP              , 0x0A);
  SpiWriteAutoAddressRegister(REG_AX5043_PLLCPI               , 0x10);
  SpiWriteAutoAddressRegister(REG_AX5043_PLLVCODIV            , 0x35);
  SpiWriteAutoAddressRegister(REG_AX5043_XTALCAP              , 0x00);
  SpiWriteAutoAddressRegister(REG_AX5043_0xF00                , 0x0F);
  SpiWriteAutoAddressRegister(REG_AX5043_0xF18                , 0x06);
  
  //AX5043频率设置  Frequency =40680000/26000000*2^24+0.5=0x1908AA2
  uint32_t LLC_Frequency  = 0x1908AA2;
  SpiWriteAutoAddressRegister(REG_AX5043_FREQA0,LLC_Frequency & 0xff);
  SpiWriteAutoAddressRegister(REG_AX5043_FREQA1,LLC_Frequency >> 8);
  SpiWriteAutoAddressRegister(REG_AX5043_FREQA2,LLC_Frequency >> 16);
  SpiWriteAutoAddressRegister(REG_AX5043_FREQA3,LLC_Frequency >> 24);
  
  
}

/*----------------------------------------------------------
ModuleName: AX5043_Reset
Description: AX5043重置
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void AX5043_Reset(void)
{
  unsigned char ubAddres;
  
  NSS(1);
  NSS(0);
  while (SDI == 0);
  SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, 0x80);
  NSS(1);
  IWDG_ReloadCounter();
  SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, 0x00);       
  SpiWriteSingleAddressRegister(REG_AX5043_SCRATCH, 0x55);
  do
  {
    asm("NOP");
    asm("NOP");
    ubAddres = SpiReadSingleAddressRegister(REG_AX5043_SCRATCH);
  }
  while (ubAddres != 0x55);
  // IRQ Line 1   001 IRQ Output ’1’
  SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x01); 
  asm("NOP");
  asm("NOP");
  //IRQ Line 0  000 IRQ Output ’0’
  SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x00);  
  //011 IRQ Output Interrupt Request
  SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x03); 
}

/*----------------------------------------------------------
ModuleName: Simple_Wait_For_Xtal
Description: 简易晶振起振程序
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
void Simple_Wait_For_Xtal(void)
{
  uint8_t LCV_XtalStaus,LCV_IRQMask;
  LCV_IRQMask=SpiReadAutoAddressRegister(REG_AX5043_IRQMASK1)| 0x01;
  // enable xtal ready interrupt
  SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1,LCV_IRQMask);
  for(;;)
  {
    LCV_XtalStaus = SpiReadAutoAddressRegister(REG_AX5043_XTALSTATUS);
    if(LCV_XtalStaus)
      break;
  }
  SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1,0);
}

/*----------------------------------------------------------
ModuleName: Simple_Autorange_Pll
Description: 简易自动锁频程序
Params: null
Return: null
Notice: null
-----------------------------------------------------------*/
unsigned char Simple_Autorange_Pll(void)
{
  
  uint8_t  LCV_Range = 0x20;//RANGE初始值
  uint8_t  LCV_Vcoi = 0xa2;//VCOI检索值，首个为0xa2
  uint8_t  LCV_VcoiMax =0xa2;//最大可用VCOI值
  uint8_t  LCV_RangeMin=0x0f;
  
  SpiReadAutoAddressRegister(REG_AX5043_PLLLOOP);
  SpiReadAutoAddressRegister(REG_AX5043_PLLCPI);
  // default 2.1kHz loop BW for ranging
  SpiWriteAutoAddressRegister(REG_AX5043_PLLLOOP , 0x09); 
  SpiWriteAutoAddressRegister(REG_AX5043_PLLCPI , 0x08);
  // enable radio interrupt start crystal
  SpiWriteAutoAddressRegister(REG_AX5043_PWRMODE , AX5043_PWRSTATE_XTAL_ON);
  Simple_Wait_For_Xtal();
  IWDG_ReloadCounter();
  
  /*-----------------以下为锁频检索，范围：VcoiF-0xbf-----------------*/
  while(LCV_Vcoi< 0xbf)
  {
    SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1,0x00);
    
    SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI,LCV_Vcoi);  
    // enable pll autoranging done interrupt
    SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1,0x10); 
    // init ranging process starting from range 8
    SpiWriteAutoAddressRegister(REG_AX5043_PLLRANGINGA ,0x18);
    while(!(SpiReadAutoAddressRegister(REG_AX5043_IRQREQUEST1) & 0x10));
    SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1,0x00);
    LCV_Range = SpiReadAutoAddressRegister(REG_AX5043_PLLRANGINGA); 
    
    if(!(LCV_Range & 0x20))
    {
      LCV_Range &= 0x0f;
      if(LCV_RangeMin>=LCV_Range) 
      {
        LCV_RangeMin=LCV_Range;
        LCV_VcoiMax= LCV_Vcoi ;
      }
    }
     LCV_Vcoi ++;
  } 
  /*----------------------锁频检索END-------------------------*/
  
  /*-----选出最小的Rang值中所有的VCOI的值中取最大的VCOI写入----*/
  if(LCV_RangeMin==0)
  {
    SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI,0xbe);
  }
  else
  {
    SpiWriteAutoAddressRegister(REG_AX5043_PLLVCOI,LCV_VcoiMax);
  }
  // enable pll autoranging done interrupt
  SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1 , 0x10); 
  // init ranging process starting from range 8
  SpiWriteAutoAddressRegister(REG_AX5043_PLLRANGINGA , 0x18); 
  while(!(SpiReadAutoAddressRegister(REG_AX5043_IRQREQUEST1) & 0x10));
  SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK1 , 0x00);
  LCV_Range = SpiReadAutoAddressRegister(REG_AX5043_PLLRANGINGA);
  SpiWriteAutoAddressRegister(REG_AX5043_PWRMODE , 0x0c);
  GCV_PllRange = LCV_Range;
  GCV_PllVcoi  = LCV_VcoiMax;
  // ranging ok, keep crystal running
  return 0;
}

/*----------------------------------------------------------
ModuleName: Simple_Transmit
Description: 简易发射程序
Params: uint8_t *pkt  uint16_t pktlen
Return: null
Notice:assumes that the chip is initialized and the PLL has been autoranged
*pkt points to the packet without sync word, but including len byte 
(if desired) CRC (computed over the entire *pkt) is added by the 
hardware automatically if configured in the register settings
-----------------------------------------------------------*/
void Simple_Transmit(uint8_t *pkt, uint16_t pktlen)
{
  
  // clear FIFO
  SpiWriteAutoAddressRegister(REG_AX5043_FIFOSTAT ,0x03); 
  // FULL_TX mode (note: actual packet transmission only starts after commiting the FIFO)
  SpiWriteAutoAddressRegister(REG_AX5043_PWRMODE , AX5043_PWRSTATE_FULL_TX); 
  // wait until VDDMODEM ready so writing the FIFO is safe
  while (!(SpiReadAutoAddressRegister(REG_AX5043_POWSTAT) & 0x08));
  IWDG_ReloadCounter();
  // write preamble to the FIFO using a REPEATDATA chunk
  for(uint16_t len = 0 ; len < pktlen ; len ++)
  {
    SpiWriteAutoAddressRegister(REG_AX5043_FIFODATA , pkt[len]);
  }
  // clear flag
  SpiReadAutoAddressRegister(REG_AX5043_RADIOEVENTREQ0); 
  // wait until cystal oscillator ready
  Simple_Wait_For_Xtal(); 
  // commit the FIFO --> TX starts
  SpiWriteAutoAddressRegister(REG_AX5043_FIFOSTAT ,0x04); 
  // wait until TX is finished
  {
    // TX done
    SpiWriteAutoAddressRegister(REG_AX5043_RADIOEVENTMASK0 , 0x01);
    // radio controller irq
    SpiWriteAutoAddressRegister(REG_AX5043_IRQMASK0 , 0x40); 
    while(!(SpiReadAutoAddressRegister(REG_AX5043_RADIOEVENTREQ0) & 0x01));
  }
  
}

/*----------------------------------------------------------
ModuleName: Send_Continue_For_Tow_Minutes
Description: 连续发射2分钟
Params: null
Return: null
Notice:
1. clear FIF0
Spi_Write_Radio(AX5043_FIFOSTAT, 3);
2. Change to FULL_TX mode
Spi_Write_Radio(AX5043_PWRMODE, AX5043_PWRSTATE_FULL_TX);
3. wait untill VDDMODEM ready 
wait until VDDMODEM ready so writing the FIFO is safe
while (!(Spi_Read_Radio(AX5043_POWSTAT) & 0x08));
AX5043_ENCODING |=4;  (no need) 
-----------------------------------------------------------*/
void Send_Continue_For_Tow_Minutes(void)
{
  SpiWriteAutoAddressRegister(AX5043_FSKDEV2,0x00);
  SpiWriteAutoAddressRegister(AX5043_FSKDEV1,0x00);
  SpiWriteAutoAddressRegister(AX5043_FSKDEV0,0x00);
  SpiWriteAutoAddressRegister(AX5043_TXRATE2,0x00);
  SpiWriteAutoAddressRegister(AX5043_TXRATE1,0x00);
  SpiWriteAutoAddressRegister(AX5043_TXRATE0,0x01);
  
  SpiWriteAutoAddressRegister(REG_AX5043_FIFOSTAT,0x03);
  SpiWriteAutoAddressRegister(REG_AX5043_PWRMODE,AX5043_PWRSTATE_FULL_TX);
  while (!(SpiReadAutoAddressRegister(REG_AX5043_POWSTAT) & 0x08));
  
  /*连续发射*/
  for(uint16_t i = 0 ; i <3 ; i ++)//i=3时，约为7S  
  {
    SpiWriteAutoAddressRegister(REG_AX5043_FIFODATA , 0x62);//0x61
    SpiWriteAutoAddressRegister(REG_AX5043_FIFODATA , 0x03);//150
    SpiWriteAutoAddressRegister(REG_AX5043_FIFODATA , 0xff);
    SpiWriteAutoAddressRegister(REG_AX5043_FIFODATA , 0xaa);
    IWDG_ReloadCounter();
  }
  Simple_Wait_For_Xtal();
  SpiWriteAutoAddressRegister(REG_AX5043_FIFOSTAT, 0x04);
  while(( SpiReadAutoAddressRegister(REG_AX5043_FIFOFREE1)<<8 
         + SpiReadAutoAddressRegister(REG_AX5043_FIFOFREE0)) < 255)
  {
    IWDG_ReloadCounter();
    Delay_5us(30*200);
  }
  
}











