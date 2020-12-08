#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"
#include "string.h"

//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 Can_Send_Msg(u8* msg,u8 len);						//��������

u8 Can_Receive_Msg(u8 *buf);							//��������

#define CAN_RX_LEN  30          

extern u8  CAN_RX_BUF[CAN_RX_LEN];
u8 CAN_SendMsg(u8* data1, u8* data2);
u8 CAN_GetMsg(u8 *msg1,u8 *msg2);

#endif

















