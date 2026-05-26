#ifndef _CAN_H_
#define _CAN_H_

#include "types.h"

typedef struct CAN_Frame

{

	u32 ID;

	struct BitField

	{

		u8 RTR :1;

		u8 DLC :4;

	}bfv; //bit field variables

	u32 Data1,Data2;

	//u8 Data[8];

}CANF;


void Init_CAN1(void);

void CAN1_Tx(CANF txF);

void CAN1_Rx(CANF *rxF);

#endif
