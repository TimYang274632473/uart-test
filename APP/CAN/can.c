#include "config.h"

CanRxMsg	RxMessage;
CanTxMsg	TxMessage;

#if REMOTE_FRAME_TEST
	#if MAIN_BOARD
		FRAME_TYPE frame_type = REMOTE_FRAME;
	#else
		FRAME_TYPE frame_type = DATA_FRAME;
	#endif
#else
FRAME_TYPE frame_type = DATA_FRAME;
#endif

vb can_tx_flag;			
vb can_rx_flag;													//����֡��־

u8 (*p_u8_can_rx)[CAN_RTX_LEN] = &CAN_MAIN_RX_BUFF;	//ָ�����ID����Ҫ�������

u8 CAN_MAIN_RX_BUFF[CAN_RTX_LEN]={0};
u8 CAN_MAIN_TX_BUFF[CAN_RTX_LEN]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 CAN_SUB1_RX_BUFF[CAN_RTX_LEN]={0};
u8 CAN_SUB1_TX_BUFF[CAN_RTX_LEN]={0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11};
u8 CAN_SUB2_RX_BUFF[CAN_RTX_LEN]={0};
u8 CAN_SUB2_TX_BUFF[CAN_RTX_LEN]={0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x22};

vb can_main_rx_flag = false;
vb can_main_tx_flag = false;
vb can_sub1_rx_flag = false;
vb can_sub1_tx_flag = false;
vb can_sub2_rx_flag = false;
vb can_sub2_tx_flag = false;
#if MAIN_BOARD 

#endif

#if SUB1_BOARD 

#endif

#if SUB2_BOARD 

#endif

//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal);
//������Ϊ:36M/((8+9+1)*8)=250Kbps
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
	GPIO_InitTypeDef 				GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  			NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);			

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);			

	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE;			
	CAN_InitStructure.CAN_ABOM=DISABLE;			
	CAN_InitStructure.CAN_AWUM=DISABLE;			
	CAN_InitStructure.CAN_NART=ENABLE;			
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	
	CAN_InitStructure.CAN_TXFP=DISABLE;			
	CAN_InitStructure.CAN_Mode= mode;	      
	//���ò�����
	CAN_InitStructure.CAN_SJW=tsjw;				
	CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
	CAN_Init(CAN1, &CAN_InitStructure);   

	CAN_FilterInitStructure.CAN_FilterNumber=0;												//ѡ�������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;			//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit; 		//16bit
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow =0x0000;	
//	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;	
	
	//RTR:0 ����֡   IDE:0 ��׼��ʶ��  Ҫ��MSB  
#if MAIN_BOARD	 
	#if CAN_FILTER_LIST_MODE		//�б�ģʽ	
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;			
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow =0x0000;	
		CAN_FilterInitStructure.CAN_FilterIdHigh=CAN_FILTER_STD_DATA_SUB2;							//����SUB2��׼����֡
		CAN_FilterInitStructure.CAN_FilterIdLow =CAN_FILTER_STD_DATA_SUB1;							//����SUB1��׼����֡
	#else 											//����λģʽ			
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;			
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;														//���θ�(��ʹ�ã�
		CAN_FilterInitStructure.CAN_FilterMaskIdLow =CAN_FILTER_MASK_STD_DATA_DOUBLE;		//���ε�(��ȫƥ��)		
		CAN_FilterInitStructure.CAN_FilterIdHigh		=0xFFFF;														//��ID  (��ʹ�ã�
		CAN_FilterInitStructure.CAN_FilterIdLow 		=CAN_FILTER_STD_DATA_DOUBLE;				//��ID	(SUB1��2��׼����֡)		
		

	#endif
	
#elif SUB_BOARD1
	#if CAN_FILTER_LIST_MODE		//�б�ģʽ
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
		CAN_FilterInitStructure.CAN_FilterIdHigh		= 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;	
		CAN_FilterInitStructure.CAN_FilterIdLow 		= CAN_FILTER_STD_REMOTE_SUB1;		//������SUB1Զ��֡
	#endif
#elif SUB_BOARD2
	#if CAN_FILTER_LIST_MODE		//�б�ģʽ
		CAN_FilterInitStructure.CAN_FilterMode			= CAN_FilterMode_IdList;			//�б�ģʽ
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;
		CAN_FilterInitStructure.CAN_FilterIdHigh		= 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;	
		CAN_FilterInitStructure.CAN_FilterIdLow 		= CAN_FILTER_STD_REMOTE_SUB2;		//������SUB2Զ��֡
	#endif
#endif

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//ѡ��FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;							//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);	
	
#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);														//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}  

#if CAN_RX0_INT_ENABLE	
		    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	//receive message
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
		can_rx_flag = true;	
	}
	//FIFO 0 full
	if(CAN_GetITStatus(CAN1,CAN_IT_FF0) == SET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
		CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
		can_rx_flag = true;	
	}	
	//error ����can
	if(CAN_GetITStatus(CAN1,CAN_IT_ERR) == SET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_ERR);
		CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal);			//250k bps
	}	
	
//	//send buff empty
//	if(CAN_GetITStatus(CAN1,CAN_IT_TME) == SET)
//	{
//		CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);
//		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
//		
//	}

}
#endif

//return:	0 success 
//				1 fail
u8 Can_Send_Msg(u8* msg,u8 len,FRAME_TYPE frame_type)
{	
	u8 mbox;
	u16 i=0;
#if REMOTE_FRAME_TEST
		#if MAIN_BOARD
//			TxMessage.StdId=CAN_MAIN_STDID;			// ��׼��ʶ�� 
		#elif SUB_BOARD1
			TxMessage.StdId=CAN_SUB1_STDID;			// ��׼��ʶ�� 
		#elif SUB_BOARD2
			TxMessage.StdId=CAN_SUB2_STDID;			// ��׼��ʶ�� 
		#endif
#else	
	#if MAIN_BOARD
		TxMessage.StdId=CAN_MAIN_STDID;			// ��׼��ʶ�� 
	#elif SUB_BOARD1
		TxMessage.StdId=CAN_SUB1_STDID;			// ��׼��ʶ�� 
	#elif SUB_BOARD2
		TxMessage.StdId=CAN_SUB2_STDID;			// ��׼��ʶ�� 
	#endif 
#endif	

	
	if(frame_type == REMOTE_FRAME)
	{
		TxMessage.RTR=CAN_RTR_Remote;			// Զ��֡
	}
	else
	{
		TxMessage.RTR=CAN_RTR_Data;				// ����֡
	}
	
	TxMessage.ExtId=0x00;						// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard;	// ��׼֡
	
	TxMessage.DLC=len;							// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			 
	
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}
////can�ڽ������ݲ�ѯ
////buf:���ݻ�����;	 
////����ֵ:0,�����ݱ��յ�;
////		 ����,���յ����ݳ���;
//u8 Can_Receive_Msg(u8 *buf)
//{		   		   
//	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
//	return RxMessage.DLC;	
//}