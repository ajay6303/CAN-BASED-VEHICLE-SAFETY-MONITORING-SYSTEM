/*#include <lpc21xx.h>
#include "delay.h"

#define TRIG_PIN (1<<21) // P0.21
#define ECHO_PIN (1<<22) // P0.22

void INIT_Ultrasonic(void)
{
    // 1. Configure Pins
    IODIR0 |= TRIG_PIN;   // Trigger as Output
    IODIR0 &= ~ECHO_PIN;  // Echo as Input
    IOCLR0 = TRIG_PIN;    // Ensure Trigger is low initially

    // 2. Configure Timer0 for 1 microsecond resolution
    // Assuming PCLK = 60MHz, PR = 59 gives 1 tick per 1us
    T0TCR = 0x02;         // Reset Timer
    T0PR  = 59;           // Prescale Register (60MHz / 60 = 1MHz)
    T0TCR = 0x00;         // Stop Timer
}

void send_pulse(void)
{
    IOCLR0 = TRIG_PIN;
    delay_us(2);
    IOSET0 = TRIG_PIN;
    delay_us(10);         // 10us high pulse required by HC-SR05
    IOCLR0 = TRIG_PIN;
}

unsigned int read_echo(void)
{
    unsigned int duration = 0;
    unsigned int timeout = 1000000; // Safety timeout

    // Wait for Echo to go HIGH
    while(!(IOPIN0 & ECHO_PIN) && timeout--);
    
    T0TC  = 0;            // Clear Timer Counter
    T0TCR = 0x01;         // Start Timer0

    // Wait for Echo to go LOW
    timeout = 1000000;    // Reset safety timeout
    while((IOPIN0 & ECHO_PIN) && timeout--);
    
    T0TCR = 0x00;         // Stop Timer0
    duration = T0TC;      // Read the Timer Counter (not TCR!)
    
    return duration;
}

unsigned int dist_cal(void)
{
    unsigned int pulse_time;
    send_pulse();
    pulse_time = read_echo();
    
    // Formula: Distance (cm) = (Time in us) / 58
    // If pulse_time is 0 or very high (timeout), return a safe high value
    if(pulse_time == 0 || pulse_time > 30000) return 400; 

    return (pulse_time / 58);
}	*/
  
#include<lpc22xx.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#define trr_pin 21
#define echo_pin 22

void INIT_Ultrasonic(void)
{
IODIR0|=(1<<trr_pin);
IODIR0&=~(1<<echo_pin);
IODIR0|=3<<8;
IOSET0=3<<8;
}
void send_pulse(void)
{
IOSET0=1<<trr_pin;
delay_us(10);
IOCLR0=1<<trr_pin;
}
unsigned int read_echo(void)
{

unsigned duration=0;
while(((IOPIN0>>echo_pin)&1)==0);
T0TCR=0X02;
T0TCR=0X01;
IOCLR0=1<<8;
while((IOPIN0>>echo_pin)&1);
IOCLR0=1<<9;
T0TCR=0X00;
duration=T0TC;
return duration;
}
unsigned int dist_cal(void)
{
unsigned pulse;
float dis;
send_pulse();
pulse=read_echo();
dis=pulse/59.0;
return dis;
}
	
