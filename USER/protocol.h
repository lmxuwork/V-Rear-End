
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "sys.h"
#include "typedefs.h"

#define MODE_LANE_1 	1	//KEY0按下
#define MODE_LANE_2	  2	//KEY1按下
#define MODE_LANE_3   3 //KEY_UP按下(即WK_UP/KEY_UP)

extern LK_Can_Rec_Link Can_Rec_Link;
extern Can_Rec_Node can_msg_list[CAN_REC_SIZE];

void set_is_test(u8 status);
void set_lane_mode(u8 mode);
void protocol_init(void);
void protocol_handler(u8 *buf, CanRxMsg *RxMessage);

#endif
