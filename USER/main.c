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
 ALIENTEK��ӢSTM32������ʵ��25
 CAN ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 {	

	u8 key;
	u8 t=0;
	u8 cnt=0;
	u8 canbuf[8];
	CanRxMsg RxMessage;
	//u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD	
	KEY_Init();				//������ʼ��
	RTC_Init();	  			//RTC��ʼ��
	protocol_init();
	 
  printf("main start\r\n");
	printf("VERSION:%s%s\r\n",SOFT_NAME,SOFT_VERSION);
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������500Kbps    
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
			LED0=!LED0;//��ʾϵͳ��������	
			t=0;
			cnt++;
		}
	}
}



