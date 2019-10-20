#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
 ALIENTEKս��STM32������ʵ��4
 ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {		
	 //add a line.
	u8 data,frame_flag=RESET;
 	u16 t;  
	u16 len;	
	u16 times=0;
	int i=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	RingBuff_Init();
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	 
 	while(1)
	{
		if(ringBuff.Lenght>=8)
		{
			frame_flag=SET;
		}
		if(frame_flag==SET)
		{
			for(i=0;i<8;i++)
			{
			 Read_RingBuff(&data);
			 while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
			 USART_SendData(USART1,data);
			}
			frame_flag=RESET;
		}
		if(ringBuff.Lenght>=8)
		{
			frame_flag=SET;
		}
		if(frame_flag==SET)
		{
			for(i=0;i<8;i++)
			{
			 Read_RingBuff(&data);
			 while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
			 USART_SendData(USART1,data);
			}
			frame_flag=RESET;
		}
	}	 
 }

