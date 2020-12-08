
#include "typedefs.h"
#include "protocol.h"
#include "can.h"
#include "stdio.h"
#include "delay.h"
#include "math.h"
#include "led.h"
#include "rtc.h" 
int count = 0;
int b = 0; 
LK_Can_Rec_Link Can_Rec_Link;

Can_Rec_Node can_msg_list[CAN_REC_SIZE];
uint32_t warn_timestamp;


union ObjMessage{
	unsigned long long value;
	struct{
		unsigned long long obj_id : 8;  //
		unsigned long long obj_range : 13; //距离 m
		unsigned long long obj_speed : 11; //速度 m/s
		unsigned long long obj_angle : 10; //角度 deg
		unsigned long long obj_latVel: 9;  //横向速度 m/s
		unsigned long long obj_resv31: 2;  //保留字段
		unsigned long long obj_dynProp: 3; //目标的动态属性
		unsigned long long obj_snr: 8;     // 目标的SNR: dB
		}bit;
	unsigned char buf[8];
};


typedef struct
{
    uint64_t resv        : 36;  //??
    uint64_t version     : 4;   //CAN????
    uint64_t measCounter : 16;  //??????
    uint64_t objectNum   : 8;   //????
}stObjStartMsg;


typedef struct
{
    uint64_t snr      :  8;  //SNR
    uint64_t dynProp  :  3;  //????
    uint64_t resv     :  2;
    uint64_t latVel   :  9;  //????
    uint64_t angle    : 10;  //??
    uint64_t velocity : 11;  //??
    uint64_t range    : 13;  //??
    uint64_t objId    :  8;  //??ID
}stObjInfoMsg;

typedef struct
{
	uint8_t ID;
	float distance;
	float speed;
	float angle;
	uint8_t SNR;	
}objNode;


void protocol_init(void)
{
	u32 i = 0;
	for(i = 0 ; i < (CAN_REC_SIZE-1) ; i++)
	{
	  can_msg_list[i].next = &(can_msg_list[i+1]);
	  can_msg_list[i].ready = 0;
	}
	can_msg_list[CAN_REC_SIZE-1].next = &(can_msg_list[0]);
	Can_Rec_Link.head = &(can_msg_list[0]);
	Can_Rec_Link.tail = &(can_msg_list[0]);
	warn_timestamp = RTC_GetCounter();
	count = b = 0;
	printf("timestamp: %d\r\n",warn_timestamp);
}


u8 protocol_handler(u8 *buf, CanRxMsg *RxMessage)
{
	//printf("time:%d\r\n",timecount);
	while(Can_Rec_Link.tail != Can_Rec_Link.head)
	{
		Can_Rec_Node * ptr = Can_Rec_Link.tail;
		uint32_t id = ptr->id;
    uint64_t msgdata = ((uint64_t)ptr->data[0] << 56) | ((uint64_t)ptr->data[1]   << 48)
                     | ((uint64_t)ptr->data[2] << 40) | ((uint64_t)ptr->data[3]   << 32)
                     | ((uint64_t)ptr->data[4] << 24) | ((uint64_t)ptr->data[5]   << 16)
                     | ((uint64_t)ptr->data[6] << 8)  | ((uint64_t)ptr->data[7]   << 0);
		//printf("id:%02x\r\n",id);
		if(ptr->ready == 1 ){
			//协议解析
			switch(id){
				case 0x06A0:
					{
					//target_Num = buf[0];
					//printf("\r\n\r\nA===>ID:%02X,Data: %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
					//				id,ptr->data[0],ptr->data[1],ptr->data[2],ptr->data[3],ptr->data[4],ptr->data[5],ptr->data[6],ptr->data[7]);						
						break;
					}
				case 0x06B0:
					{
						float x;
						float y;
						stObjInfoMsg *pMsg = (stObjInfoMsg *)&msgdata;
						objNode target;
						target.ID = pMsg->objId;
						target.distance = (float)pMsg->range / 5;
						target.speed = pMsg->velocity & 0x400 ? (float)((int16_t)(pMsg->velocity | 0xF800)) / 5
																									: (float)((int16_t)(pMsg->velocity & 0x3FF)) / 5;
						target.angle = pMsg->angle & 0x200 ? (float)((int16_t)(pMsg->angle | 0xFC00)) / 4
																							 : (float)((int16_t)(pMsg->angle & 0x1FF)) / 4;
						target.SNR = (pMsg->snr & 0xFF) / 2;
						x = target.distance * sinf( target.angle/180.0f*3.14159f);
						y = target.distance * cosf( target.angle/180.0f*3.14159f);

						if( x < 1.5f && x > -1.5f && y < 130 && y > 0){
							if(target.speed < -0.11f){ //40km/h
								printf("A(%d) : id=%d , dis=%f,spd=%f,ag=%f,SNR=%d,x=%f,y=%f,dyprop:%02x\r\n",
											count,target.ID,target.distance,target.speed,target.angle,target.SNR,x,y,pMsg->dynProp);
									printf("Warn(%d) : id=%d , dis=%.2f,spd=%.2f,ag=%.2f,SNR=%d,x=%.2f,y=%.2f\r\n",
											count,target.ID,target.distance,target.speed,target.angle,target.SNR,x,y);	
									warn(1);
									warn_timestamp = RTC_GetCounter();
									b=0;
							}else {
								b++;
							}
						}
						if( RTC_GetCounter() - warn_timestamp > 1 || b > 10){
									warn(0);
									count = 0;
									b = 0;
						}
						break;
					}
				case 0x06C0:
					{
						break;
					}
				case 0x05F0:  //帧结束
					{
						break;
					}
				
				default:
				 break;
			}
		}
		ptr->ready = 0;
		Can_Rec_Link.tail = (Can_Rec_Node *)(Can_Rec_Link.tail->next);
		if( RTC_GetCounter() - warn_timestamp > 1 || b > 10){
				warn(0);
				b = 0;
				count = 0;
		}
	}
	return RxMessage->DLC;	
}

//由于定义的结构体是小端模式，需要进行转换，8字节
void convert ( unsigned char * ary )
{
    unsigned char tmp;
    tmp = ary[0];
    ary[0] = ary[7];
    ary[7] = tmp;
    tmp = ary[1];
    ary[1] = ary[6];
    ary[6] = tmp;
    tmp = ary[2];
    ary[2] = ary[5];
    ary[5] = tmp;
    tmp = ary[3];
    ary[3] = ary[4];
    ary[4] = tmp;
}


