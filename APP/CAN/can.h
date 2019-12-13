#ifndef __CAN_H
#define __CAN_H	 
    
#include "config.h"

typedef enum 
{
	REMOTE_FRAME=0,
	DATA_FRAME
}FRAME_TYPE;



//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE			 1										//0,不使能;1,使能.		
#define CAN_RTX_LEN							 8										//CAN数据长度
#define CAN_MAIN_STDID           (u16)0x101
#define CAN_SUB1_STDID           (u16)0x201
#define CAN_SUB2_STDID           (u16)0x301
#define CAN_RTX_EXTEID					 (u32)0x18FFA2F4


//RTR:0(数据)  1(远程)
//IDE:0(标准)  1(扩展)
#define CAN_RTR_16BIT_REMOTE    (1<<4)
#define CAN_RTR_16BIT_DATA      (0<<4)
#define CAN_IDE_16BIT_EXI    		(1<<3)
#define CAN_IDE_16BIT_STD      	(0<<3)

#define CAN_FILTER_STD_DATA_DOUBLE				 	(((CAN_SUB1_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)&((CAN_SUB2_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)) 															
#define CAN_FILTER_MASK_STD_DATA_DOUBLE    	~(((CAN_SUB1_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)^((CAN_SUB2_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA))
																										
#define CAN_FILTER_STD_DATA_MAIN				 		((CAN_MAIN_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)  
#define CAN_FILTER_STD_DATA_SUB1				 		((CAN_SUB1_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)   
#define CAN_FILTER_STD_DATA_SUB2				 		((CAN_SUB2_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_DATA)

#define CAN_FILTER_STD_REMOTE_SUB1				 	((CAN_SUB1_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_REMOTE)   
#define CAN_FILTER_STD_REMOTE_SUB2				 	((CAN_SUB2_STDID<<5)|CAN_IDE_16BIT_STD|CAN_RTR_16BIT_REMOTE)



extern CanRxMsg	RxMessage;
extern CanTxMsg	TxMessage;
extern FRAME_TYPE frame_type;
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
