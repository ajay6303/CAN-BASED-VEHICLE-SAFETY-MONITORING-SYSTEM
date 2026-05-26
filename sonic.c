#include<lpc22xx.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"

// --- Peripheral Pin Allocations ---
#define trr_pin 21   // Trigger signal mapped to P0.21
#define echo_pin 22  // Incoming Echo pulse width read from P0.22

/**
 * @brief Prepares direction registers for ultrasonic hardware pins and diagnostic signals.
 */
void INIT_Ultrasonic(void)
{
    IODIR0 |= (1 << trr_pin);    // Set P0.21 (Trigger) as a digital Output pin
    IODIR0 &= ~(1 << echo_pin);  // Set P0.22 (Echo) as a digital Input pin
    
    // Configure pins P0.8 and P0.9 as diagnostic output indicators
    IODIR0 |= 3 << 8;
    IOSET0 = 3 << 8;             // Set initial output high state (Turn off Active-Low indicators)
}

/**
 * @brief Emits a precise 10-microsecond trigger pulse sequence to initiate range-finding.
 */
void send_pulse(void)
{
    IOSET0 = 1 << trr_pin;       // Drive Trigger output HIGH
    delay_us(10);                // Maintain HIGH state for required 10us duration
    IOCLR0 = 1 << trr_pin;       // Drop Trigger output back to LOW
}

/**
 * @brief Measures total pulse width of high signal arriving at Echo pin using Timer0.
 * @return 32-bit hardware timer counter accumulation ticks.
 */
unsigned int read_echo(void)
{
    unsigned duration = 0;
    
    // Busy-wait blocks execution until the Echo signal moves from low to HIGH
    while(((IOPIN0 >> echo_pin) & 1) == 0);
    
    T0TCR = 0X02;  // Reset Timer Counter (TC) and Prescale Counter (PC)
    T0TCR = 0X01;  // Enable Timer0 counting operations
    
    IOCLR0 = 1 << 8; // Diagnostics: Clear P0.8 to verify timing capture loop has started
    
    // Busy-wait blocks execution while the incoming Echo pin remains HIGH
    while((IOPIN0 >> echo_pin) & 1);
    
    IOCLR0 = 1 << 9; // Diagnostics: Clear P0.9 to verify trailing-edge capture has occurred
    T0TCR = 0X00;    // Disable Timer0 counting operations
    
    duration = T0TC; // Capture total accumulated ticks directly from Timer Counter register
    return duration;
}

/**
 * @brief Master range acquisition function.
 * @return Calculated proximity distance in integer centimeters.
 */
unsigned int dist_cal(void)
{
    unsigned pulse;
    float dis;
    
    send_pulse();         // Dispatch start pulse burst
    pulse = read_echo();   // Read raw signal time-of-flight ticks
    
    // Converts timer tick width into distance measurements using structural constant tracking
    dis = pulse / 59.0;
    
    return dis;
}
