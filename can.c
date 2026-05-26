#include <lpc21xx.h>
#include "can_defines.h"
#include "can.h"

/**
 * @brief Cold-start initialization configurations establishing bit timing registers for CAN Controller 1.
 */
void Init_CAN1(void)
{
    // Map alternate system pin logic options to turn pin P0.25 into active CAN Receive link (RD1)
    PINSEL1 |= RD1_PIN;
    
    // Command CAN1 controller into configuration mode by asserting Reset Mode bit
    C1MOD |= 1 << RM_BIT;
    
    // Bypass Acceptance Filter logic tracks to pass all active bus traffic into local memory
    AFMR &= ~(1 << AccOFF_BIT);
    AFMR |= 1 << AccBP_BIT; // Enable Acceptance Filter Bypass option
    
    // Configure Bit Timing Registers using calculated preprocessor layout variables
    C1BTR = BTR_LVAL;
    
    // Switch off Reset Mode bit to release CAN1 controller into active run-time networking mode
    C1MOD &= ~(1 << RM_BIT);
}

/**
 * @brief Dispatches packed data frame payload sets across active CAN network connections.
 */
void CAN1_Tx(CANF txF)
{
    // Busy-wait loops check Transmit Buffer Status bit to hold processing until buffer 1 clears
    while(((C1GSR >> TBS1_BIT) & 1) == 0);
    
    C1TID1 = txF.ID; // Load target tracking message identification variables down into Transmit registers
    
    // Compile frame control info combining custom tracking parameters into Transmit Frame Info layout registers
    C1TFI1 = (txF.bfv.RTR << RTR_BIT) | (txF.bfv.DLC << DLC_BITS);
    
    // Process packaging constraints if frame information represents typical data transmissions
    if(txF.bfv.RTR == 0)
    {
        C1TDA1 = txF.Data1;  // Map payload bytes 1 through 4 down into Data Transmit register A
        C1TDB1 = txF.Data2;  // Map payload bytes 5 through 8 down into Data Transmit register B
    }
    
    // Trigger transmission execution loop sequence flags in Command register area
    C1CMR |= 1 << STB1_BIT | 1 << TR_BIT;
    
    // Standby busy-wait loop holds execution tracking until Transmission Compliance Status clears verify success
    while(((C1GSR >> TCS1_BIT) & 1) == 0);
}

/**
 * @brief Extracts received data payloads down from internal peripheral storage stacks.
 */
void CAN1_Rx(CANF *rxF)
{
    // Standby busy-wait loop blocks execution tracking until Receiver Buffer Status registers confirm new data
    while(((C1GSR >> RBS_BIT) & 1) == 0);
    
    rxF->ID = C1RID; // Pull message identifier codes out from active tracking register locations
    
    // Isolate Remote Transmission Request status values from internal tracking indicators data
    rxF->bfv.RTR = (C1RFS >> RTR_BIT) & 1;
    
    // Isolate Data Length Code byte count values from internal tracking indicators data
    rxF->bfv.DLC = (C1RFS >> DLC_BITS) & 15;
    
    // Read and record data blocks if incoming information configurations represent data payloads
    if(rxF->bfv.RTR == 0)
    {
        rxF->Data1 = C1RDA; // Capture payload bytes 1 through 4 out from Receive Register A
        rxF->Data2 = C1RDB; // Capture payload bytes 5 through 8 out from Receive Register B
    }
    
    // Dispatch Command Register flags requesting immediate release of active receive buffer segments
    C1CMR = 1 << RRB_BIT;
}
