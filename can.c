#include <lpc21xx.h>
#include "can_defines.h"
#include "can.h"

void Init_CAN1(void)

{

	//cfg p0.25 as CAN1_Rx Pin (RD1)

	PINSEL1|=RD1_PIN;

	//Reset CAN1 Controller

	C1MOD|=1<<RM_BIT;

	//all received messages are accepted

	AFMR&=~(1<<AccOFF_BIT);

	AFMR|=1<<AccBP_BIT;

	//set baud rate for CAN

	C1BTR=BTR_LVAL;

	//Enable CAN1 controller

	C1MOD&=~(1<<RM_BIT);

}


//Transfer data

void CAN1_Tx(CANF txF)

{

	//checking that the Tx buffer is empty

	while(((C1GSR>>TBS1_BIT)&1)==0);

	//cfg Tx ID

	C1TID1=txF.ID;

	//cfg RTR & DLC

	C1TFI1=(txF.bfv.RTR<<RTR_BIT)|(txF.bfv.DLC<<DLC_BITS);

	//checking whether D/R Frame to Transmit

	if(txF.bfv.RTR==0)

	{

		//ifD Frame,write to data transmit buffers

		C1TDA1=txF.Data1;  //bytes 1-4

		C1TDB1=txF.Data2;  //bytes 5-8

	}

	//select Tx Buffer 1& start Transmission

	C1CMR|=1<<STB1_BIT|1<<TR_BIT;

	//wait until tx complete

	while(((C1GSR>>TCS1_BIT)&1)==0);

}


//Receive data

void CAN1_Rx(CANF *rxF)

{

	//wait for CAN frame recv status

	while(((C1GSR>>RBS_BIT)&1)==0);

	//read 11-bit CAN id of recvd frame

	rxF->ID=C1RID;

	//&read & extract data| remote frame status

	rxF->bfv.RTR=(C1RFS>>RTR_BIT)&1;

	//& extract data length

	rxF->bfv.DLC=(C1RFS>>DLC_BITS)&15;

	//check if recvd frame is data frame

	if(rxF->bfv.RTR==0)

	{

		//extract data bytes 1-4

		rxF->Data1=C1RDA;

		//extract data bytes 5-8

		rxF->Data2=C1RDB;

	}

	//Release receive buffer command

	C1CMR=1<<RRB_BIT;

}
