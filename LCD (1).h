//LCD.h
#ifndef _LCD_H_
#define _LCD_H_

#include "types.h"

void WriteLCD(u8 cmd);
void CmdLCD(u8 cmd);
void CharLCD(u8 ascii);
void InitLCD(void);
void StrLCD(u8 *ptr);
void intLCD(u32 num);
void FloatLCD(f32 dec,u8  DP);
void CustomChar(unsigned char *ch,int num);

#endif
