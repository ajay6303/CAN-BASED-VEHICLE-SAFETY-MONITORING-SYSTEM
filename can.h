#ifndef _CAN_H_
#define _CAN_H_

#include "types.h"

// --- Controller Area Network Communication Core Frame Blueprint Data Model ---
typedef struct CAN_Frame
{
    u32 ID; // Standard 11-bit or Extended 29-bit network message identifier structure

    struct BitField
    {
        u8 RTR :1; // Remote Transmission Request bit allocation (0 = Data Frame, 1 = Remote Request)
        u8 DLC :4; // Data Length Code allocating bounds tracking up to 8 bytes maximum parameters
    } bfv; 

    u32 Data1; // Combined 32-bit container wrapping payload data bytes 1 through 4
    u32 Data2; // Combined 32-bit container wrapping payload data bytes 5 through 8

} CANF;

// --- CAN Low Level Control Layer Implementations API ---
void Init_CAN1(void);     // Configures interface register parameters and establishes link bit-rates
void CAN1_Tx(CANF txF);   // Dispatches completed frame structures onto the bus architecture
void CAN1_Rx(CANF *rxF);  // Intercepts and copies received frames down from peripheral register sets

#endif
