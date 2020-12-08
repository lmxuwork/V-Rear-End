#include "led.h"
#include "delay.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);						 //PB.5 �����


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED1-->PE.0 �˿�����, �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_ResetBits(GPIOC,GPIO_Pin_11); 						 //PE.0 ���
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
 
