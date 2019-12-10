#ifndef __SYSTICK_H
#define __SYSTICK_H 

#include "stm32f10x.h"
#include "core_cm3.h"
#include <stdint.h>

#define ARRIVE_1MS 			1
#define ARRIVE_5MS 			5
#define ARRIVE_10MS 		10
#define ARRIVE_50MS 		50
#define ARRIVE_100MS 		100
#define ARRIVE_500MS 		500
#define ARRIVE_1000MS 	1000
#define ARRIVE_5000MS 	5000

#define USART3_TX_LENGTH 			30
#define USART3_RX_LENGTH 			30

extern vu16 vu16_last_systick_count,vu16_now_systick_count;

typedef struct st_tick_count_for_task
{
	vb bl_arrive_1ms_flag;
	vb bl_arrive_5ms_flag;
	vb bl_arrive_10ms_flag;
	vb bl_arrive_50ms_flag;	
	vb bl_arrive_100ms_flag;
	vb bl_arrive_500ms_flag;
	vb bl_arrive_1000ms_flag;
	vb bl_arrive_5000ms_flag;
	void (*fp_systick_1ms)(void);
	void (*fp_systick_5ms)(void);
	void (*fp_systick_10ms)(void);
	void (*fp_systick_50ms)(void);
	void (*fp_systick_100ms)(void);
	void (*fp_systick_500ms)(void);
	void (*fp_systick_1000ms)(void);
	void (*fp_systick_5000ms)(void);
}st_TICK_COUNT_FOR_TASK;

typedef enum em_task_num
{
	TASK_1MS		=0,
	TASK_5MS			,
	TASK_10MS			,
	TASK_50MS			,
	TASK_100MS		,
	TASK_500MS		,
	TASK_1000MS		,
	TASK_5000MS		,
	INIT_TASK
}TASK_NUM;


void SysTick_Handler(void);
void Systick_Init(u16 hz);
void Mark_Task_Flag(vu16 *p_systick_count ,st_TICK_COUNT_FOR_TASK *st_task_flag);
void TICK_Task_fpConfig(st_TICK_COUNT_FOR_TASK *st_task_flag);
void Task_1000MS(void);
void Task_100MS(void);
void Task_10MS(void);
void Task_1MS(void);
void Task_5000MS(void);
void Task_500MS(void);
void Task_50MS(void);
void Task_5MS(void);

#endif
