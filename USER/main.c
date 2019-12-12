
#include "config.h"



extern st_TICK_COUNT_FOR_TASK st_tick_for_task; 

int main(void)
	{		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		Systick_Init(1000);	    	  																					//1k HzÐÄÌø		
		Uart1_Init(115200);	 
		Uart3_Init(230000);	
//		Uartx_Dma_Init(USART1,RX,0,0,(u32 *)USART1_RX_BUF,USART1_RX_LEN);
//		Uartx_Dma_Init(USART1,TX,0,1,(u32 *)USART1_TX_BUF,USART1_TX_LEN);
		Uartx_Dma_Init(USART3,RX,3,0,(u32 *)USART3_RX_BUF,USART3_RX_LEN);
		Uartx_Dma_Init(USART3,TX,3,1,(u32 *)(*p_u8_can_rx),USART3_TX_LEN);
		
		CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal);
	
		TICK_Task_fpConfig(&st_tick_for_task);
	 
//		RingBuff_Init(&Uart1_Buff_st);
//		RingBuff_Init(&Uart3_Buff_st);
	

	 
 	while(1)
	{
		if(vu16_now_systick_count > vu16_last_systick_count)
		{
			Mark_Task_Flag(&vu16_now_systick_count , &st_tick_for_task);					//flag mark
			
			if(st_tick_for_task.bl_arrive_1ms_flag == true)												//task exe
			{
//				st_tick_for_task.fp_systick_1ms();
				st_tick_for_task.bl_arrive_1ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_5ms_flag == true)
			{
//				st_tick_for_task.fp_systick_5ms();
				st_tick_for_task.bl_arrive_5ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_10ms_flag == true)
			{
				st_tick_for_task.fp_systick_10ms();
				st_tick_for_task.bl_arrive_10ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_50ms_flag == true)
			{
				st_tick_for_task.fp_systick_50ms();
				st_tick_for_task.bl_arrive_50ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_100ms_flag == true)
			{
				st_tick_for_task.fp_systick_100ms();
				st_tick_for_task.bl_arrive_100ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_500ms_flag == true)
			{
				st_tick_for_task.fp_systick_500ms();
				st_tick_for_task.bl_arrive_500ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_1000ms_flag == true)
			{
				st_tick_for_task.fp_systick_1000ms();
				st_tick_for_task.bl_arrive_1000ms_flag = false;
			}
			if(st_tick_for_task.bl_arrive_5000ms_flag == true)
			{
				st_tick_for_task.fp_systick_5000ms();
				st_tick_for_task.bl_arrive_5000ms_flag = false;
			}
			
			if(vu16_now_systick_count >= ARRIVE_5000MS)
			{
				vu16_now_systick_count = 1;
				vu16_last_systick_count = 0;
			}
			else
			{
				vu16_last_systick_count = vu16_now_systick_count;
			}

		}

	}	 
	
 }
 
#ifdef USE_FULL_ASSERT 
void assert_failed(u8* file, u32 line) { while (1) { } } 
#endif 
