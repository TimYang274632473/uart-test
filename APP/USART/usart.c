
#include "config.h"



//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
 
#else
/*使用microLib的方法*/

int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);			//防止首字节丢失
	
	USART_SendData(USART3, (uint8_t) ch);

	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART3->SR & USART_FLAG_RXNE));

    return ((int)(USART3->DR & 0x1FF));
}

#endif

vu8  USART1_RX_BUF[USART1_RX_LEN];
vu8  USART1_TX_BUF[USART1_TX_LEN];

vu8  USART3_RX_BUF[USART3_RX_LEN];
vu8  USART3_TX_BUF[USART3_TX_LEN];

vb vb_usart1_dma_rx_flag = false,vb_usart3_dma_rx_flag = false;

void Uart1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	//USART1_TX GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	//USART1_RX    GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx|USART_DMAReq_Tx,ENABLE);
	USART_Cmd(USART1,ENABLE);
}

void Uart3_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

  USART_Init(USART3, &USART_InitStructure); 
	USART_DMACmd(USART3,USART_DMAReq_Rx|USART_DMAReq_Tx,ENABLE);
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
  USART_Cmd(USART3, ENABLE);                    
}

void USART1_IRQHandler(void)                	
{
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //接收中断
		{
		  USART_ReceiveData(USART1);
			vb_usart1_dma_rx_flag = true;
			DMA_Cmd(DMA1_Channel5, DISABLE);
//		  Write_RingBuff(&Uart1_Buff_st,USART_ReceiveData(USART1));//读取接收到的数据
		}
} 

void USART3_IRQHandler(void)                	
{
		if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) //接收中断
		{
		  USART_ReceiveData(USART3);
//			vb_usart3_dma_rx_flag = true;			
//		  Write_RingBuff(&Uart3_Buff_st,USART_ReceiveData(USART3));//读取接收到的数据
		}
}
	
void RingBuff_Init(RingBuff_st *ringbuff)
{
   //初始化相关信息
	ringbuff->Head=0;
	ringbuff->Lenght=0;
	ringbuff->Tail=0;
}


bool Write_RingBuff(RingBuff_st *ringbuff,u8 data)
{
 if(ringbuff->Lenght >= UART_RING_BUFF_LEN) //判断缓冲区是否已满
 {
  return true;
 }
 ringbuff->Ring_Buff[ ringbuff->Tail]=data;
 ringbuff->Tail = (++ ringbuff->Tail)%UART_RING_BUFF_LEN;//防止越界非法访问
 ringbuff->Lenght++;
 return false;
}


bool Read_RingBuff(RingBuff_st *ringbuff,u8 *rData)
{
	if(ringbuff->Lenght == 0)//判断非空
 {
  return false;
 }
  *rData = ringbuff->Ring_Buff[ringbuff->Head];//先进先出FIFO，从缓冲区头出
 ringbuff->Head = (++ringbuff->Head)%UART_RING_BUFF_LEN;//防止越界非法访问
 ringbuff->Lenght--;
 return true;
}


void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_IT_TC5)==SET)
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC5);		
	}

}
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4)==SET)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}

void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_IT_TC3)==SET)
	{
		DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}

}
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2)==SET)
	{
		DMA_Cmd(DMA1_Channel2, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

DMA_InitTypeDef 			DMA_InitStructure;

void Uartx_Dma_Init(USART_TypeDef* USARTx, TX_RX_SELECT tx_or_rx,u8 preepriority,u8 subpriority,u32 *buffer, u16 buffer_lenth)
{
	
//	DMA_Channel_TypeDef   DMA_ChannelStructure;
	DMA_Channel_TypeDef   *p_DMA_Channel_Str;
	NVIC_InitTypeDef 			NVIC_InitStructure;
		
//	//DMA channel reset
//	DMA_DeInit(p_DMA_Channel_Str); 
	
	//DMAx RCC select enable
	if(USARTx == USART1 || USARTx == USART2 || USARTx == USART3)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	}
	else if(USARTx == USART1)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	}
	//USARTx and tx rx channel corresponding dma channel
	if(USARTx == USART1)
	{
		if(tx_or_rx == TX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
			p_DMA_Channel_Str = DMA1_Channel4;			
		}
		else if(tx_or_rx == RX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;	
			p_DMA_Channel_Str = DMA1_Channel5;
		}
	}
	else if(USARTx == USART2)
	{
		if(tx_or_rx == TX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;	
			p_DMA_Channel_Str = DMA1_Channel7;
		}
		else if(tx_or_rx == RX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
			p_DMA_Channel_Str = DMA1_Channel6;			
		}
	}
	else if(USARTx == USART3)
	{
		if(tx_or_rx == TX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
			p_DMA_Channel_Str = DMA1_Channel2;
		}
		else if(tx_or_rx == RX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
			p_DMA_Channel_Str = DMA1_Channel3;
		}
	}
	else if(USARTx == UART4)
	{
		if(tx_or_rx == TX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
			p_DMA_Channel_Str = DMA2_Channel5;			
		}
		else if(tx_or_rx == RX)
		{
			NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
			p_DMA_Channel_Str = DMA2_Channel3;			
		}
	}
	//DMA channel NVIC config
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preepriority ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpriority;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					
	NVIC_Init(&NVIC_InitStructure);	
	//DMA IT enable
	DMA_ITConfig(p_DMA_Channel_Str,DMA_IT_TC,ENABLE);

	//DMA mode config
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buffer; 				 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 					 
	DMA_InitStructure.DMA_BufferSize = buffer_lenth; 							
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 				
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 								
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 												
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 									
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 														
	if(tx_or_rx == RX)
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //peripheral to memary
	}	
	else
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //memary to peripheral
	}
	DMA_Init(p_DMA_Channel_Str, &DMA_InitStructure); 
	
	//if che dma function for RX can be enable by config
	if(tx_or_rx == RX)
	{
		DMA_Cmd(p_DMA_Channel_Str, ENABLE);
	}
	else
	{
		DMA_Cmd(p_DMA_Channel_Str, DISABLE);
	}
	
}




void Usart_Dma_Tx_Enable(USART_TypeDef* USARTx,DMA_Channel_TypeDef*DMA_CHx,vb enable_flag,u16 buffer_lenth)
{ 	
	if(USARTx == USART1)		//USARTx and tx channel corresponding dma channel
	{
			DMA_CHx = DMA1_Channel5;			
	}
	else if(USARTx == USART2)
	{	
			DMA_CHx = DMA1_Channel7;
	}
	else if(USARTx == USART3)
	{
			DMA_CHx = DMA1_Channel2;
	}
	else if(USARTx == UART4)
	{
			DMA_CHx = DMA2_Channel5;		
	}
	
	
	if(enable_flag)
	{
		DMA_SetCurrDataCounter(DMA_CHx,buffer_lenth);		//从新设置缓冲大小,指向需要发送的数组
		DMA_Cmd(DMA_CHx, ENABLE);
	}
	else
	{
		DMA_Cmd(DMA_CHx, DISABLE);
	}
}  


