

#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#define CAN_REC_SIZE 200
typedef struct{
	unsigned int id;
	unsigned char dlc;
	unsigned char data[8];
	unsigned char ready;
	void * next;
}Can_Rec_Node;


typedef struct{
	Can_Rec_Node * head;
	Can_Rec_Node * tail;
}LK_Can_Rec_Link;

#endif

