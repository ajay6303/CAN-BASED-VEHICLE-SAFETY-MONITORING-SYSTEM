#include <lpc21xx.h>
#include "types.h"
#include "can.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"
#include "lcd_defines.h"

// Hardware Pin Definitions
#define MODE_SW 9  // P0.9
#define BUZZER  (1<<8)  // P0.8

// Custom LCD Character Data (Arrows)
unsigned char LeftArrow[8]  = {0x00,0x04,0x0c,0x1f,0x0c,0x04,0x00,0x00};
unsigned char RightArrow[8] = {0x00,0x04,0x06,0x1f,0x06,0x04,0x00,0x00};

// Global State Variables
unsigned char left_on = 0, right_on = 0;
volatile u8 vehicle_mode = 0; // 0-Forward, 1-Reverse
//unsigned int blink_counter = 0;
//unsigned char blink_state = 0;

// ISR for Indicator Switches (SW1/SW2)
void __irq Indicator_ISR(void) {
   	delay_ms(20);
    if(EXTINT & 0x02) {  // EINT1 (P0.14) - Left Indicator
        left_on = !left_on;
        right_on = 0;
        
       // EXTINT = 0x02;   // Clear interrupt
    } 
    else if(EXTINT & 0x04) { // EINT2 (P0.15) - Right Indicator
        right_on = !right_on;
        left_on = 0;
        
        //EXTINT = 0x04;   // Clear interrupt
    }
	EXTINT=0x06;
    VICVectAddr = 0;
}

void Init_System(void) {
    InitLCD();
    Init_CAN1();
    
    // Load Custom Arrows into CGRAM
    CustomChar(LeftArrow, 0);  // Store at address 0
    CustomChar(RightArrow, 1); // Store at address 1
    
    // Configure Buzzer (Output)
    IODIR0 |= BUZZER;
    IOSET0 = BUZZER; // Active Low: Set high to turn off

    // Configure External Interrupts for SW1 & SW2
    PINSEL0 |= 0xA0000000;
    VICVectAddr0 = (unsigned long)Indicator_ISR;
    VICVectCntl0 = 0x20 | 15; // Enable Slot 0 & Assign EINT1
    VICVectAddr1 = (unsigned long)Indicator_ISR;
    VICVectCntl1 = 0x20 | 16; // Enable Slot 1 & Assign EINT2
    VICIntEnable = (1 << 15) | (1 << 16);
}

int main() {
    CANF rx;
    int temp_raw, tp, tpd;
	 CANF tx;
    tx.ID = 0x101;       // Indicator Node ID
    tx.bfv.RTR = 0;
    tx.bfv.DLC = 1;
	
    Init_System();

    while(1) {
		//title
       CmdLCD(GOTO_L1_POSN0);
        StrLCD("Vehicle Monitor");

		//Temperature reading
        temp_raw = ReadTemp();
        tp = temp_raw >> 4;
        tpd = ((temp_raw & 0x0F) * 625) / 1000; // Accurate fractional part

        CmdLCD(GOTO_L2_POSN0);
        StrLCD("Temp = ");
        intLCD(tp);
        CharLCD('.');
        intLCD(tpd);
        StrLCD(" C  ");
		//vehicle mode
        if(((IOPIN0>>MODE_SW)&1)== 0) 
		{
            delay_ms(200); // Debounce
            vehicle_mode = !vehicle_mode;
			if(vehicle_mode==0)
			{
			    IOSET0 = BUZZER;
			}
            while(((IOPIN0>>MODE_SW)&1) == 0); 
        }
        
        CmdLCD(GOTO_L3_POSN0);
        if(vehicle_mode) StrLCD("Mode: REVERSE");
        else             StrLCD("Mode: FORWARD");

        /*//Indicator 
		blink_counter++;
        if(blink_counter > 40) 
		{ // Software Timer Toggle
            blink_state = !blink_state;
            blink_counter = 0;									  1222
        }
		  */
        CmdLCD(GOTO_L4_POSN0);
        StrLCD("Indicator: ");
        
        if(left_on)
		{  tx.Data1 = 0x01;
        	CAN1_Tx(tx);
            //if(blink_state) 
			{
				CharLCD(0); 
				StrLCD(" "); 
				CharLCD(1); 
				StrLCD(" "); 
			} // Blink Left Arrow
            //else            
			{
				delay_ms(100);
				CmdLCD(GOTO_L4_POSN0+11); 
				StrLCD("  "); 
			}
        }
        else if(right_on) 
		{	tx.Data1 = 0x02 ;
        	CAN1_Tx(tx);
      
            //if(blink_state) 
			{ 	CharLCD(0); 
				StrLCD(" "); 
				CharLCD(1); 
				StrLCD(" "); 
			} // Blink Right Arrow
            //else            
			{
			delay_ms(100);
			CmdLCD(GOTO_L4_POSN0+12); 
			 StrLCD("  "); }
        }
        else 
		{
			tx.Data1 = 0x00;
			CAN1_Tx(tx);
            CharLCD(0);StrLCD(" "); CharLCD(1); StrLCD(" "); // Both on (Idle)
        }
		  
        //REVERSE ALERT 
        if(vehicle_mode == 1) 
		{
            if(C1GSR & 0x01) 
			{ // If CAN message received
                CAN1_Rx(&rx);
                if(rx.ID == 0x201) 
				{ // Match Reverse Node ID
                    if(rx.Data1 == 1) 
					{	//buzzer on until 2 seconds
						IOCLR0 = BUZZER; // Object Detected!
						delay_ms(1000);
					}
                    else
					{  //for beap sound on and off
					   IOSET0 = BUZZER; // Clear
					   delay_ms(100);
					   IOCLR0=BUZZER;
                }
            }
        } 
         
        delay_ms(10); // Small delay for loop stability
    }
}
}

