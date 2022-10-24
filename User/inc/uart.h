
#ifndef __UART_H_
#define __UART_H_

typedef struct
{
  unsigned char Start;
  unsigned int  Time;
  unsigned char ReceiveBuf[80];
  unsigned char ReceiveLen;
  unsigned char SendBuf[80];
  unsigned char SendLen;
  unsigned char SendFlag;  
  unsigned char AIFlag;
}UART_STRUCT;

extern UART_STRUCT uart;

void Uart_Init(void);
void Uart_DeInit(void);
unsigned char UartWork(void);
void UartSendStr(unsigned char * buf,unsigned char len);
#endif




