#ifndef _LCD_H_
#define _LCD_H_

#include "types.h"

// --- Public Control and Data Driver Interfaces ---
void WriteLCD(u8 cmd);                    // Standard 8-bit bus strobe writer
void CmdLCD(u8 cmd);                      // Dispatches configuration instruction configurations
void CharLCD(u8 ascii);                   // Outputs single raw text character byte
void InitLCD(void);                       // Handles startup initialization configurations
void StrLCD(u8 *ptr);                     // Loops and prints null-terminated arrays strings
void intLCD(u32 num);                     // Converts and displays integer data records
void FloatLCD(f32 dec, u8 DP);            // Strips and displays floating numbers alongside point accuracy variables
void CustomChar(unsigned char *ch, int num); // Loads user custom character blocks to internal CGRAM structures

#endif
