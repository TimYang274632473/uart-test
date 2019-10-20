#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
 ALIENTEK战舰STM32开发板实验4
 串口实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {		
	 //add a line.
	u8 data,frame_flag=RESET;
 	u16 t;  
	u16 len;	
	u16 times=0;
	int i=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	RingBuff_Init();
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	 
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

