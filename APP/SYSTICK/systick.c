#include "config.h"

vu16 vu16_last_systick_count=0,vu16_now_systick_count=0;
st_TICK_COUNT_FOR_TASK st_tick_for_task;

static u32 can_tx_count = 0;

void SysTick_Handler(void)
{
	vu16_now_systick_count++;
}

void Systick_Init(u16 frequence)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	
	SysTick->LOAD  = (SystemCoreClock / frequence)/8 - 1; 						  		 
	SysTick->VAL   = 0x00;        					
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk |SysTick_CLKSource_HCLK_Div8 ;	
}	

void Mark_Task_Flag(vu16 *p_systick_count ,st_TICK_COUNT_FOR_TASK *st_task_flag)
{
		if((*p_systick_count % ARRIVE_1MS) == 0)
		{
			st_task_flag->bl_arrive_1ms_flag = true;
		}
		if((*p_systick_count % ARRIVE_5MS) == 0)
		{
			st_task_flag->bl_arrive_5ms_flag = true;
		}
		if((*p_systick_count % ARRIVE_10MS) == 0)
		{
			st_task_flag->bl_arrive_10ms_flag = true;
		}
		if((*p_systick_count % ARRIVE_50MS) == 0)
		{
			st_task_flag->bl_arrive_50ms_flag = true;
		}				
		if((*p_systick_count % ARRIVE_100MS) == 0)
		{
			st_task_flag->bl_arrive_100ms_flag = true;
		}
		if((*p_systick_count % ARRIVE_500MS) == 0)
		{
			st_task_flag->bl_arrive_500ms_flag = true;
		}			
		if((*p_systick_count % ARRIVE_1000MS) == 0)
		{
			st_task_flag->bl_arrive_1000ms_flag = true;
		}
		if((*p_systick_count % ARRIVE_5000MS) == 0)
		{
			st_task_flag->bl_arrive_5000ms_flag = true;
		}
}
void TICK_Task_fpConfig(st_TICK_COUNT_FOR_TASK *st_task_flag)
{
	st_task_flag->fp_systick_5000ms = Task_5000MS;
	st_task_flag->fp_systick_500ms  = Task_500MS;
	st_task_flag->fp_systick_50ms 	= Task_50MS;
	st_task_flag->fp_systick_5ms 	  = Task_5MS;
	st_task_flag->fp_systick_1000ms = Task_1000MS;
	st_task_flag->fp_systick_100ms  = Task_100MS;
	st_task_flag->fp_systick_10ms 	= Task_10MS;
	st_task_flag->fp_systick_1ms 	  = Task_1MS;
	
}

void Task_1000MS(void)
{

}
void Task_100MS(void)
{	
#if SUB_BOARD1
	u32 temp = 0;
	
	temp = can_tx_count++;
	for(u8 i=0;i<4;i++)
	{
		CAN_SUB1_TX_BUFF[i] = (u8)(temp >> 8*i);
	}
	if(Can_Send_Msg(CAN_SUB1_TX_BUFF,CAN_RTX_LEN))
	{
		printf("can send fail \r\n");
	}
#endif
#if SUB_BOARD2 
	
	u32 temp = 0;
	
	temp = can_tx_count++;
	for(u8 i=0;i<4;i++)
	{
		CAN_SUB2_TX_BUFF[i] = (u8)(temp >> 8*i);
	}
	if(Can_Send_Msg(CAN_SUB2_TX_BUFF,CAN_RTX_LEN))
	{
//		printf("can send fail \r\n");
	}
#endif
//	printf("100\r\n");	
}
void Task_10MS(void)
{
#if REMOTE_FRAME_TEST
		static u16 remote_req_count = 0;
	if(remote_req_count % 2 == 0)
	{
		
	}
	else
	{
	
	}
	remote_req_count++;

	if(Can_Send_Msg(*p_u8_can_rx,CAN_RTX_LEN,REMOTE_FRAME))
	{
		//send error
	}
		
#endif
//	if(can_rx_flag)
//	{		
//		DMA_SetCurrDataCounter(DMA1_Channel2,CAN_RTX_LEN);
//		Usart_Dma_Tx_Enable(USART1,DMA1_Channel2,ENABLE,CAN_RTX_LEN);		
//		can_rx_flag = false;
//	}
	//	printf("10\r\n");
}
//CAN数据接收检测保存
void Task_1MS(void)
{
	#if MAIN_BOARD	
		if(can_rx_flag)
		{
			if(RxMessage.StdId == CAN_SUB1_STDID)
			{
				p_u8_can_rx = &CAN_SUB1_RX_BUFF;
				can_sub1_rx_flag = true;
			}
			else if(RxMessage.StdId == CAN_SUB2_STDID)
			{
				p_u8_can_rx = &CAN_SUB2_RX_BUFF;
				can_sub2_rx_flag = true;
			}
			for(u8 i=0;i<8;i++)
			{
				(*p_u8_can_rx)[7-i] = RxMessage.Data[i];
			}
			can_rx_flag = false;
		}
	#endif		
	
//	printf("1\r\n");
}
void Task_5000MS(void)
{

//	printf("5000\r\n");
}
void Task_500MS(void)
{
	
//	printf("500\r\n");
}
void Task_50MS(void)
{
//	printf("50\r\n");
}
//CAN数据uart打印和发送
void Task_5MS(void)
{
#if MAIN_BOARD
if(can_sub1_rx_flag)
	{
		if(Can_Send_Msg(*p_u8_can_rx,CAN_RTX_LEN,DATA_FRAME))
		{
			//send error
		}
		
		//打印can接收数据
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(*p_u8_can_rx); 
		DMA_Init(DMA1_Channel2, &DMA_InitStructure); 
		
		DMA_SetCurrDataCounter(DMA1_Channel2,CAN_RTX_LEN);
		Usart_Dma_Tx_Enable(USART3,DMA1_Channel2,ENABLE,CAN_RTX_LEN);	
		can_sub1_rx_flag = false;
	}
if(can_sub2_rx_flag)
	{
		if(Can_Send_Msg(*p_u8_can_rx,CAN_RTX_LEN,DATA_FRAME))
		{
			//send error
		}
		
		//打印can接收数据
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(*p_u8_can_rx); 
		DMA_Init(DMA1_Channel2, &DMA_InitStructure); 
		
		DMA_SetCurrDataCounter(DMA1_Channel2,CAN_RTX_LEN);
		Usart_Dma_Tx_Enable(USART3,DMA1_Channel2,ENABLE,CAN_RTX_LEN);	
		can_sub2_rx_flag = false;
	}	
	
#endif
//	if(vb_usart1_dma_rx_flag == true)
//	{
//		DMA_SetCurrDataCounter(DMA1_Channel4,USART1_TX_LEN);
//		Usart_Dma_Tx_Enable(USART1,DMA1_Channel4,ENABLE,USART1_TX_LEN);
//		vb_usart1_dma_rx_flag = false;
//	}
//	
//	if(vb_usart3_dma_rx_flag == true)
//	{
//		DMA_SetCurrDataCounter(DMA1_Channel2,USART3_TX_LEN);
//		Usart_Dma_Tx_Enable(USART3,DMA1_Channel2,ENABLE,USART3_TX_LEN);
//		vb_usart3_dma_rx_flag = false;
//	}
}
