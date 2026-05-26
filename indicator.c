#include <lpc21xx.h>
#include "can.h"
#include "delay.h"

// Hardware Setup: Using P0.8 to P0.15 for the 8 LEDs
#define LED_SIG 8
#define LED_MASK (0xff<<LED_SIG) 

// Global Variables
CANF rxF;
volatile unsigned char current_action = 0; // 0:Stop, 1:Left, 2:Right

void Check_For_Updates(void) {
    if(C1GSR & 0x01) { // Check if a CAN message is waiting in the buffer
        CAN1_Rx(&rxF);
        if(rxF.ID == 0x101) { // Ensure the message is from the Main Node
            current_action = rxF.Data1;
	//		delay_ms(10);
        }
    }
}

// Left Indication: Scroll LEDs one by one from Right to Left (P0.15 down to P0.8)
void Scroll_Left(void) {
    int i;
    for(i = 7; (i >=0); i--) 
	{
        Check_For_Updates(); // Check CAN inside the loop for instant response
		if(current_action != 0x01)
		{
		IOSET0=LED_MASK;
		return;
		}
       delay_ms(100);
        IOSET0 = LED_MASK;             // Turn all LEDs OFF (Active Low logic)
        IOCLR0 = (1 << (i+LED_SIG));     // Turn ONE LED ON
                   // Speed of the scroll
		        
    }
	
}

// Right Indication: Scroll LEDs one by one from Left to Right (P0.8 up to P0.15)
void Scroll_Right(void) {
    int i;
	for(i = 0; (i < 8) ; i++) 
	{
        Check_For_Updates(); // Check CAN inside the loop for instant response
        if((current_action != 0x02))
		{
			IOSET0=LED_MASK;
		return;
		}
    
    
		 delay_ms(100);
        IOSET0 = LED_MASK;             // Turn all LEDs OFF
        IOCLR0 = (1 << (i+LED_SIG));      // Turn ONE LED ON
       
    }
	
	
}

int main() {
    // 1. Initialization
    Init_CAN1();
    IODIR0 |= LED_MASK; // Configure P0.8-P0.15 as Output
    IOSET0 = LED_MASK;  // Turn all LEDs OFF initially

    // 2. Main Executive Loop
    while(1) {
         Check_For_Updates(); // Constantly listen for the Main Node

         if(current_action == 0x01) {
		    Scroll_Left();
        } 
        else if(current_action == 0x02) {
		    Scroll_Right();
        } 
        else {
            IOSET0 = LED_MASK; // All LEDs OFF if action is 0 (Stop)
        }
    }
}
