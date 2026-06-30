#include <lpc21xx.h>
#include "lcd_defines.h"
#include "LCD.h"
#include "delay.h"

/**
 * @brief Low-level atomic function responsible for controlling data bus lanes and pulsing the E strobe.
 */
void WriteLCD(u8 cmd)
{
    IOCLR0 = 1 << RW; // Drive RW low to explicitly notify interface controller of raw write operations
    
    // Clear out historical values from target pin area and load raw output command data onto the bus
    IOPIN0 = (IOPIN0 & ~(0XFF << PIN)) | (cmd << PIN);
    
    IOSET0 = 1 << EN;  // Raise operational Enable strobe HIGH to build active data framing latch context
    delay_us(1);       // Enforce setup time requirements
    IOCLR0 = 1 << EN;  // Drop operational Enable strobe LOW to lock data into internal registers
    
    delay_ms(2);       // Internal execution delay allowed for standard command processing
}

/**
 * @brief Passes operational system commands down to device internal instruction slots.
 */
void CmdLCD(u8 cmd)
{
    IOCLR0 = 1 << RS;  // Pull RS line down to point internal tracking focus toward Command Registers
    WriteLCD(cmd);     // Strobe full instruction byte out onto active data paths
}

/**
 * @brief Dispatches raw character data patterns out onto the visible layout pixels map.
 */
void CharLCD(u8 ascii)
{
    IOSET0 = 1 << RS;  // Raise RS line HIGH to steer internal tracking focus toward Data Registers memory
    WriteLCD(ascii);   // Strobe full data byte out onto active data paths
}

/**
 * @brief Drives cold-start setup sequence routines to establish reliable 8-bit communication.
 */
void InitLCD(void)
{
    // Define full LCD configuration block control tracks and parallel pins as digital outputs
    IODIR0 |= ((0XFF << PIN) | (1 << RS) | (1 << RW) | (1 << EN));
    delay_ms(15);
    
    // Force device down standard functional hardware reset step loops
    CmdLCD(M_8BIT_1L);
    delay_ms(5);
    CmdLCD(M_8BIT_1L);
    delay_us(100);
    CmdLCD(M_8BIT_1L);
    
    // Load targeted run-time operational configurations configurations
    CmdLCD(M_8BIT_2L); // Set device options to track configurations across dual-line text blocks
    CmdLCD(D_ON);      // Activate visibility layers, dropping rendering trackers for default cursors
    CmdLCD(CLR);       // Flush all data contents from historical display buffer locations
    CmdLCD(SHIFT_C_R); // Command internal tracking loops to step text rendering tracks to the right
}

/**
 * @brief Formats and iterates across standard string objects until string pointer hit termination characters.
 */
void StrLCD(u8 *ptr)
{
    while(*ptr)
    {
        CharLCD(*ptr++); // Send character at current position and increment string pointer address
    }
}	
	
/**
 * @brief Numeric parser translating standard raw values up to textual numbers printable by display screens.
 */
void intLCD(u32 num)
{
    char a[10];
    int i = 0;
    
    if(num == 0)
        CharLCD('0'); // Immediate optimization evaluation capture to skip computational loops for simple empty conditions

    // Break values apart into character digits using mod base-10 math
    while(num)
    {
        a[i++] = (num % 10) + '0'; // Map remaining digits up to standard ASCII equivalent positions
        num /= 10;
    }
    
    // Loop through the isolated digits backward to render the string from left to right
    for(i -= 1; i >= 0; i--)
        CharLCD(a[i]);
}

/**
 * @brief Numeric floating values processor splitting real and decimal precision components.
 */
void FloatLCD(f32 dec, u8 DP)
{
    int n, i;
    if(dec < 0)
    {
        CharLCD('-'); // Identify negative signs immediately
        dec = -(dec);
    }
    
    n = dec;          // Truncate floating values to pull absolute natural whole number parts
    intLCD(n);        // Output base whole numbers
    CharLCD('.');     // Append separating punctuation decimal markers
    
    // Cycle fractional elements up past boundaries to separate individual numeric symbols
    for(i = 0; i < DP; i++)
    {
        dec = (dec - n) * 10;
        n = dec;
        CharLCD(n + 48); // Convert target calculation indexes directly to active printable ASCII character symbols
    }
}

/**
 * @brief Writes customized graphic patterns directly into target custom character volatile CGRAM tracks.
 */
void CustomChar(unsigned char *ch, int n)
{
    int i;
    // Divert focus away from regular display loops toward targeted CGRAM base index addresses
    CmdLCD(GOTO_CGRAM + (8 * n));

    // Strobe full pixel array elements sequentially into custom asset space allocations
    for(i = 0; i < 8; i++)
    {
        CharLCD(ch[i]);
    }
}
