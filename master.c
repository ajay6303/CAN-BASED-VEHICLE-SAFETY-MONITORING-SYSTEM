#include <lpc21xx.h>
#include "types.h"
#include "can.h"
#include "lcd.h"
#include "delay.h"
#include "DS18B20.h"
#include "lcd_defines.h"

// --- Hardware Control Layout Pin Mapping ---
#define MODE_SW 9       // Manual mode switch input pin on P0.9
#define BUZZER  (1<<8)  // Proximity audible alarm circuit output pin on P0.8

// --- Custom Graphics Matrix Data (5x8 Custom Pixel Symbol Definitions) ---
unsigned char LeftArrow[8]  = {0x00,0x04,0x0c,0x1f,0x0c,0x04,0x00,0x00};
unsigned char RightArrow[8] = {0x00,0x04,0x06,0x1f,0x06,0x04,0x00,0x00};

// --- Volatile Inter-process Shared Variables ---
unsigned char left_on = 0, right_on = 0;
volatile u8 vehicle_mode = 0; // System Functional Tracking: 0 = Forward Mode, 1 = Reverse Mode

/**
 * @brief Interrupt Service Routine mapped to External Interrupts 1 and 2 (Turn Indicators Control).
 */
void __irq Indicator_ISR(void) {
    delay_ms(20); // Software switch debounce filtration processing
    
    if(EXTINT & 0x02) {       // Check if EINT1 (P0.14) triggered interrupt - Left Turn Command
        left_on = !left_on;   // Invert Left Turn active assignment tracking flag
        right_on = 0;         // Electrical interlock safety override: enforce opposing channel off
    } 
    else if(EXTINT & 0x04) {  // Check if EINT2 (P0.15) triggered interrupt - Right Turn Command
        right_on = !right_on; // Invert Right Turn active assignment tracking flag
        left_on = 0;          // Electrical interlock safety override: enforce opposing channel off
    }
    
    EXTINT = 0x06;            // Clear handled EINT1 and EINT2 latched flag items in peripheral register
    VICVectAddr = 0;          // Signals end of interrupt vector service operation to the VIC controller
}

/**
 * @brief Aggregated Hardware Component and Core Vector Initialization Routine.
 */
void Init_System(void) {
    InitLCD();
    Init_CAN1();
    
    // Store user arrow bit configurations into character controller CGRAM positions
    CustomChar(LeftArrow, 0);  // Store Custom Left Arrow at CGRAM Address Slot index 0
    CustomChar(RightArrow, 1); // Store Custom Right Arrow at CGRAM Address Slot index 1
    
    // Initialize Alert Buzzer Pin Properties
    IODIR0 |= BUZZER;
    IOSET0 = BUZZER; // Active-Low Layout Pin Configuration: Drive high to deactivate initially

    // Configure Pin Select and Vector Interrupt routing structures
    PINSEL0 |= 0xA0000000;                       // Swap P0.14 and P0.15 functions to EINT1 and EINT2 hardware lines
    VICVectAddr0 = (unsigned long)Indicator_ISR; // Load custom service function pointer to VIC Slot 0
    VICVectCntl0 = 0x20 | 15;                    // Enable Slot 0 and link to Peripheral source channel 15 (EINT1)
    VICVectAddr1 = (unsigned long)Indicator_ISR; // Load custom service function pointer to VIC Slot 1
    VICVectCntl1 = 0x20 | 16;                    // Enable Slot 1 and link to Peripheral source channel 16 (EINT2)
    VICIntEnable = (1 << 15) | (1 << 16);        // Globals: Enable interrupt delivery channels 15 and 16
}

int main() {
    CANF rx;
    int temp_raw, tp, tpd;
    CANF tx;
    
    // Setup message broadcast layout frame parameters for the Indicator Node array
    tx.ID = 0x101;       
    tx.bfv.RTR = 0;      // Establish as a Standard Data Frame payload
    tx.bfv.DLC = 1;      // Transmit 1 command byte
	
    Init_System();

    while(1) {
        // --- Render UI Static Banner Elements ---
        CmdLCD(GOTO_L1_POSN0);
        StrLCD("Vehicle Monitor");

        // --- Execute 1-Wire Thermal Acquisition Logic ---
        temp_raw = ReadTemp();
        tp = temp_raw >> 4;                    // Strip down the low-order bit fields to establish integer base unit
        tpd = ((temp_raw & 0x0F) * 625) / 1000; // Multiply remnant resolution ticks to establish accurate fractional parts

        // Print calculated temperatures to visible LCD characters area
        CmdLCD(GOTO_L2_POSN0);
        StrLCD("Temp = ");
        intLCD(tp);
        CharLCD('.');
        intLCD(tpd);
        StrLCD(" C  ");
        
        // --- Process Drive Mode Mechanical Push-Button Actions ---
        if(((IOPIN0 >> MODE_SW) & 1) == 0) 
        {
            delay_ms(200); // Debounce delay safety window filtering
            vehicle_mode = !vehicle_mode; // Cycle system state variables forward or backward
            
            if(vehicle_mode == 0)
            {
                IOSET0 = BUZZER; // Ensure alarming states resolve back off immediately upon exiting active reverse
            }
            while(((IOPIN0 >> MODE_SW) & 1) == 0); // Safety spin-lock holds execution until mechanical input is fully released
        }
        
        CmdLCD(GOTO_L3_POSN0);
        if(vehicle_mode) StrLCD("Mode: REVERSE");
        else             StrLCD("Mode: FORWARD");

        // --- Process Signal Indicator Layouts and Broadcast CAN Packets ---
        CmdLCD(GOTO_L4_POSN0);
        StrLCD("Indicator: ");
        
        if(left_on)
        {  
            tx.Data1 = 0x01; // Encode payload byte flag representing active Left signal commands
            CAN1_Tx(tx);     // Transmit information out over active bus structures
            
            CharLCD(0);      // Draw Custom Left Arrow shape onto active layout
            StrLCD(" "); 
            CharLCD(1); 
            StrLCD(" "); 
            
            delay_ms(100);
            CmdLCD(GOTO_L4_POSN0 + 11); 
            StrLCD("  ");    // Clear characters for simple visual blinking effect
        }
        else if(right_on) 
        {	
            tx.Data1 = 0x02; // Encode payload byte flag representing active Right signal commands
            CAN1_Tx(tx);
            
            CharLCD(0); 
            StrLCD(" "); 
            CharLCD(1);      // Draw Custom Right Arrow shape onto active layout
            StrLCD(" "); 
            
            delay_ms(100);
            CmdLCD(GOTO_L4_POSN0 + 12); 
            StrLCD("  ");    // Clear characters for simple visual blinking effect
        }
        else 
        {
            tx.Data1 = 0x00; // Reset network tracking values to signal neutral safe state
            CAN1_Tx(tx);
            CharLCD(0); StrLCD(" "); CharLCD(1); StrLCD(" "); // Default safe static display drawing layout
        }
		  
        // --- Process Reverse Proximity Alarming Context Rules ---
        if(vehicle_mode == 1) 
        {
            // Verify CAN Controller indicates a valid frame resides in buffer (RBS bit)
            if(C1GSR & 0x01) 
            { 
                CAN1_Rx(&rx); // Extract active packet variables fields down into memory stack structures
                if(rx.ID == 0x201) // Verify packet identification parameter matches reverse sensor node assignment
                { 
                    if(rx.Data1 == 1) 
                    {	
                        IOCLR0 = BUZZER; // Hazard condition active: Sound alarming structures
                        delay_ms(1000);
                    }
                    else
                    {  
                        // Sound continuous short heartbeat chirp sequences to confirm operational clearance pathing
                        IOSET0 = BUZZER; 
                        delay_ms(100);
                        IOCLR0 = BUZZER;
                    }
                }
            } 
        }
        delay_ms(10); // Loop cadence throttling window for background system stability
    }
}
