
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "sys.h"
#include "typedefs.h"

extern LK_Can_Rec_Link Can_Rec_Link;

extern Can_Rec_Node can_msg_list[CAN_REC_SIZE];


void protocol_init(void);
u8 protocol_handler(u8 *buf, CanRxMsg *RxMessage);

#endif
