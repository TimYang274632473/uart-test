#ifndef __CONFIG_H__
#define __CONFIG_H__

#define REMOTE_FRAME_TEST				 1

//#define CAN_FILTER_LIST_MODE		 1										//列表模式

#define MAIN_BOARD   						 1
#define SUB_BOARD1   						 0
#define SUB_BOARD2   						 0

#if MAIN_BOARD
	#define CAN_FILTER_LIST_MODE		 0										//主板屏蔽模式
#else
	#define CAN_FILTER_LIST_MODE		 1										//副板列表模式
#endif


#include "stdio.h"	
#include "stdbool.h"
#include "sys.h"
#include "usart.h"
#include "can.h"
#include "systick.h"





#endif