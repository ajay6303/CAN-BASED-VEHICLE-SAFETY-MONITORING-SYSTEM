#include <lpc21xx.h>
#include "can.h"
#include "delay.h"

// --- Hardware Board Track Configuration Constants ---
#define LED_SIG 8                // Lower boundary output index for individual LED clusters (P0.8)
#define LED_MASK (0xff<<LED_SIG) // Byte mask blocking all 8 output pins spanning from P0.8 to P0.15

// --- Global Variable Infrastructure ---
CANF rxF;
volatile unsigned char current_action = 0; // System action register context: 0 = Idle, 1 = Left, 2 = Right

/**
 * @brief Evaluates background buffer status flags to intercept real-time steering data packets from the Master Node.
 */
void Check_For_Updates(void) {
    if(C1GSR & 0x01) {          // Check if any network packets are currently sitting in the receive buffer
        CAN1_Rx(&rxF);          // Flush data block down from hardware registers into variable stack structures
        if(rxF.ID == 0x101) {   // Confirm that the message originated from the Master Controller Node
            current_action = rxF.Data1; // Update global operational context variables with active instructions
        }
    }
}

/**
 * @brief Drives sequential illumination patterns from right to left across the array (P0.15 down to P0.8).
 */
void Scroll_Left(void) {
    int i;
    for(i = 7; (i >=0); i--) 
    {
        Check_For_Updates(); // Sample network lines directly within loop processing to capture state drops instantly
        if(current_action != 0x01)
        {
            IOSET0 = LED_MASK; // Active-Low Layout Safe Exit: Drive full mask high to blackout all indicators immediately
            return;
        }
        delay_ms(100);
        IOSET0 = LED_MASK;             // Clear all pins before lighting next index to keep display tracking moving cleanly
        IOCLR0 = (1 << (i + LED_SIG)); // Pull target pin to 0 to ground and illuminate the corresponding indicator LED
    }
}

/**
 * @brief Drives sequential illumination patterns from left to right across the array (P0.8 up to P0.15).
 */
void Scroll_Right(void) {
    int i;
    for(i = 0; (i < 8) ; i++) 
    {
        Check_For_Updates(); // Sample network lines directly within loop processing to capture state drops instantly
        if((current_action != 0x02))
        {
            IOSET0 = LED_MASK; // Active-Low Layout Safe Exit: Drive full mask high to blackout all indicators immediately
            return;
        }
        delay_ms(100);
        IOSET0 = LED_MASK;             // Clear all pins before lighting next index to keep display tracking moving cleanly
        IOCLR0 = (1 << (i + LED_SIG)); // Pull target pin to 0 to ground and illuminate the corresponding indicator LED
    }
}

int main() {
    // --- Hardware and Protocol Initialization Steps ---
    Init_CAN1();
    IODIR0 |= LED_MASK; // Configure entire sequential cluster pin set from P0.8 to P0.15 as digital Outputs
    IOSET0 = LED_MASK;  // Shut off full indicator grouping immediately by driving active-low configuration pins high

    // --- Master Loop ---
    while(1) {
         Check_For_Updates(); // Periodically verify status parameters against background incoming bus tracks

         if(current_action == 0x01) {
            Scroll_Left();       // Initiate directional movement loops
        } 
        else if(current_action == 0x02) {
            Scroll_Right();      // Initiate directional movement loops
        } 
        else {
            IOSET0 = LED_MASK;   // Enforce default idle state blackout rules when system data reports neutral
        }
    }
}
