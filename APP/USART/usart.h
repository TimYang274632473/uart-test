#ifndef __USART_H
#define __USART_H

#include "config.h"

typedef enum 
{
	TX,
	RX,
	BOTH_TX_RX
}TX_RX_SELECT;

#define UART_RING_BUFF_LEN 200
typedef struct
{
	u16 Head;
	u16 Tail;
	u16 Lenght;
	u8  Ring_Buff[UART_RING_BUFF_LEN];
}RingBuff_st;

#define USART1_RX_LEN  		90  	//定义最大接收字节数 200
#define USART1_TX_LEN  		90  	//定义最大接收字节数 200
#define USART3_RX_LEN  		8	  	//定义最大接收字节数 200
#define USART3_TX_LEN  		8  		//定义最大接收字节数 200

extern DMA_InitTypeDef 			DMA_InitStructure;

extern vu8  USART1_RX_BUF[USART1_RX_LEN]; 
extern vu8  USART1_TX_BUF[USART1_TX_LEN];
extern vu8  USART3_RX_BUF[USART3_RX_LEN]; 
extern vu8  USART3_TX_BUF[USART3_TX_LEN];

extern 	RingBuff_st Uart1_Buff_st,Uart3_Buff_st;
extern vb vb_usart1_dma_rx_flag,vb_usart3_dma_rx_flag;


//如果想串口中断接收，请不要注释以下宏定义
void Uart1_Init(u32 bound);
void Uart3_Init(u32 bound);
void MYDMA_Config(void);
void RingBuff_Init(RingBuff_st *ringbuff);
bool Write_RingBuff(RingBuff_st *ringbuff,u8 data);
bool Read_RingBuff(RingBuff_st *ringbuff,u8 *rData);
void Uartx_Dma_Init(USART_TypeDef* USARTx, TX_RX_SELECT tx_or_rx,u8 preepriority,u8 subpriority,u32 *buffer, u16 buffer_lenth);
void Usart_Dma_Tx_Enable(USART_TypeDef* USARTx,DMA_Channel_TypeDef*DMA_CHx,vb enable_flag,u16 buffer_lenth);
void uart_init(u32 bound);
#endif


