#include <lpc21xx.h>
#include "delay.h"

// Hardware Pin Definition (p0.16)
#define D (1<<16) 
#define READ_PIN (IOPIN0 & (1<<16))

unsigned char Reset(void)
{
    unsigned int presence;
    
    IODIR0 |= D;        // Set as Output
    IOCLR0 = D;         // Drive LOW
    delay_us(485);      // Reset pulse duration
    
    IODIR0 &= ~D;       // Release the line (Switch to Input)
    delay_us(70);       // Wait for sensor to pull line low
    
    presence = READ_PIN; // Read presence pulse
    delay_us(410);      // Wait for end of time slot
    
    // Return 1 if presence detected (presence == 0 because it's active low)
    return (presence == 0) ? 1 : 0;
}

void WriteBit(unsigned char dBit)
{
    IODIR0 |= D;        // Set as Output
    IOCLR0 = D;         // Start time slot (Drive LOW)
    
    if(dBit) {
        delay_us(5);    // Short low for Logic 1
        IODIR0 &= ~D;   // Release line so pull-up pulls it HIGH
        delay_us(55);
    } else {
        delay_us(60);   // Long low for Logic 0
        IODIR0 &= ~D;   // Release line
        delay_us(5);
    }
}

unsigned char ReadBit(void)
{
    unsigned char bit = 0;
    
    IODIR0 |= D;        // Set as Output
    IOCLR0 = D;         // Start time slot
    delay_us(2);        // Pull low briefly
    
    IODIR0 &= ~D;       // Release line for sensor to control
    delay_us(10);       // Wait for sensor to stabilize data
    
    if(READ_PIN) bit = 1;
    
    delay_us(50);       // Finish the time slot
    return bit;
}

void WriteByte(unsigned char Dout)
{
    unsigned char i;
    for(i = 0; i < 8; i++) {
        WriteBit(Dout & 0x01);
        Dout >>= 1;
    }
    delay_us(100);
}

unsigned char ReadByte(void)
{
    unsigned char i, Din = 0;
    for(i = 0; i < 8; i++) {
        if(ReadBit()) Din |= (1 << i);
    }
    return Din;
}

int ReadTemp(void)
{
    unsigned char low, high;
    int temp = 0;
    int timeout = 0;

    // 1. Start Temperature Conversion
    if(!Reset()) return 0x00; // Return 0 if sensor is not connected
    
    WriteByte(0xCC); // Skip ROM command
    WriteByte(0x44); // Convert T command
    
    // 2. Wait for conversion (DS18B20 pulls line low during conversion)
    // We add a timeout to prevent the system from hanging if the sensor fails
    while(ReadBit() == 0 && timeout < 200) { 
        timeout++;
        delay_ms(5); 
    }
    
    // 3. Read the Result
    Reset();
    WriteByte(0xCC); // Skip ROM command
    WriteByte(0xBE); // Read Scratchpad command
    
    low = ReadByte();  // Get LSB
    high = ReadByte(); // Get MSB
    
    temp = (high << 8) | low;
    
    return temp;
}
