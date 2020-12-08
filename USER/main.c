#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "can.h"
#include "protocol.h"
#include "common.h"
#include "stdio.h"
#include "rtc.h"


/************************************************
 ALIENTEK精英STM32开发板实验25
 CAN 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 {	

	u8 key;
	u8 t=0;
	u8 cnt=0;
	u8 canbuf[8];
	CanRxMsg RxMessage;
	//u8 mode=CAN_Mode_Normal;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200	
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD	
	KEY_Init();				//按键初始化
	RTC_Init();	  			//RTC初始化
	protocol_init();
	 
  printf("main start\r\n");
	printf("VERSION:%s%s\r\n",SOFT_NAME,SOFT_VERSION);
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps    
 	while(1)
	{
		key=KEY_Scan(0);
		switch(key){
			case KEY0_PRES:
				printf("KEY0_PRES\r\n"); 
				break;
			case KEY1_PRES:
				printf("KEY1_PRES\r\n");
				break;
			case WKUP_PRES:
				printf("WKUP_PRES\r\n");
				break;
			case KEY_LANE_1_PRES:
				printf("KEY_LANE_1_PRES\r\n");
				set_is_test(1);
				//set_lane_mode(MODE_LANE_1);
				break;
			case KEY_LANE_2_PRES:
				printf("KEY_LANE_2_PRES\r\n");
				set_is_test(1);
				//set_lane_mode(MODE_LANE_2);
				break;
			case KEY_LANE_3_PRES:
				printf("KEY_LANE_3_PRES\r\n");
				set_is_test(1);
				//set_lane_mode(MODE_LANE_3);
				break;
			defalut:
				break;
		}
		protocol_handler(canbuf,&RxMessage);
		t++;
		//delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//提示系统正在运行	
			t=0;
			cnt++;
		}
	}
}



