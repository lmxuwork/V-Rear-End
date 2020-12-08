#include "led.h"
#include "delay.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);						 //PB.5 输出高


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED1-->PE.0 端口配置, 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_ResetBits(GPIOC,GPIO_Pin_11); 						 //PE.0 输出
	//GPIO_SetBits(GPIOC,GPIO_Pin_11);
	printf("PE0:%d\r\n",GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11));
}

uint8_t is_warning = 0;

void warn(int status)
{
	
	uint8_t ret = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
	//printf("status warning:%d ret:%d\r\n",status,ret);
	if(status != ret){
		if(status){
			GPIO_SetBits(GPIOC,GPIO_Pin_11);
		}else{
			GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		}
		is_warning = status;
		printf("status warning:%d status:%d\r\n",is_warning,GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11));
	}
}
 
