#ifndef __CAN_H
#define __CAN_H	 
    
#include "config.h"

typedef enum 
{
	REMOTE_FRAME=0,
	DATA_FRAME
}FRAME_TYPE;

#define CAN_FILTER_LIST_MODE		 0										//是否使用列表模式

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE			 1										//0,不使能;1,使能.		
#define CAN_RTX_LEN							 8										//CAN数据长度
#define CAN_MAIN_STDID           (u16)0x101
#define CAN_SUB1_STDID           (u16)0x201
#define CAN_SUB2_STDID           (u16)0x301
#define CAN_RTX_EXTEID					 (u32)0x18FFA2F4


#define CAN_FILTER_STDID				 (((CAN_SUB1_STDID<<5)|CAN_Id_Standard|CAN_RTR_Data)&((CAN_SUB2_STDID<<5)|CAN_Id_Standard|CAN_RTR_Data))   
#define CAN_FILTER_MASK_STDID    (u16)~(((CAN_SUB1_STDID<<5)|CAN_Id_Standard|CAN_RTR_Data)^((CAN_SUB2_STDID<<5)|CAN_Id_Standard|CAN_RTR_Data))



extern CanRxMsg	RxMessage;
extern CanTxMsg	TxMessage;
extern vb can_tx_flag;
extern vb can_rx_flag;
extern u8 (*p_u8_can_rx)[CAN_RTX_LEN];

extern u8 CAN_MAIN_RX_BUFF[CAN_RTX_LEN];
extern u8 CAN_MAIN_TX_BUFF[CAN_RTX_LEN];
extern u8 CAN_SUB1_RX_BUFF[CAN_RTX_LEN];
extern u8 CAN_SUB1_TX_BUFF[CAN_RTX_LEN];
extern u8 CAN_SUB2_RX_BUFF[CAN_RTX_LEN];
extern u8 CAN_SUB2_TX_BUFF[CAN_RTX_LEN];
extern vb can_main_rx_flag;
extern vb can_main_tx_flag;
extern vb can_sub1_rx_flag;
extern vb can_sub1_tx_flag;
extern vb can_sub2_rx_flag;
extern vb can_sub2_tx_flag;
#if MAIN_BOARD 


#endif

#if SUB1_BOARD 


#endif

#if SUB1_BOARD 


#endif



u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 Can_Send_Msg(u8* msg,u8 len,FRAME_TYPE frame_type);						
u8 Can_Receive_Msg(u8 *buf);							
#endif
