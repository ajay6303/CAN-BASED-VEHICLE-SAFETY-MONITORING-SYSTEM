#include <lpc21xx.h>
#include "can.h"
#include "delay.h"
#include "sonic.h"

int main()
{
    CANF txF;
    unsigned int distance;
    
    Init_CAN1();       // Initialize CAN Peripheral Controller Module 1
    INIT_Ultrasonic(); // Set up ultrasonic hardware interfacing pins and registers
    
    // --- Configure CAN Transmission Frame Parameter Details ---
    txF.ID = 0x201;          // Standard 11-bit CAN Identifier for the Reverse Sensor Node
    txF.bfv.RTR = 0;         // Set Frame Type to Data Frame (0 = Data Frame, 1 = Remote Request Frame)
    txF.bfv.DLC = 1;         // Data Length Code specified to transmit exactly 1 byte
    
    while(1)
    {
        distance = dist_cal(); // Retrieve current proximity distance window calculations
	
        // Evaluate obstacles residing within the functional 2cm to 20cm envelope
        if(distance > 2 && distance < 20)
        {
            txF.Data1 = 1; // Obstruction Alert: Load hazard warning flag byte
        }
        else
        {
            txF.Data1 = 0; // Clearance Confirmed: Load secure tracking flag byte
        }
        
        // Verify Transmit Buffer 1 is completely empty (TBS1 status bit validation)
        if(C1SR & 0x04) 
        {
            CAN1_Tx(txF); // Ship frame out onto active industrial networks via CAN Bus
        }
        
        delay_ms(100);    // System loop pacing delay to minimize unnecessary bus loading
    }
}
