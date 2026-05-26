#ifndef _LCD_DEFINES_H_
#define _LCD_DEFINES_H_

#define PIN 0   //lcd data pins d0(0.0) t0 d7(0.7)
#define RS 18    //register select 0.16
#define EN 19  //Enable 0.17
#define RW 24    // read / write 0.18

// lcd commands

#define CLR 0x01 // clear lcd
#define RET 0x02 //return curser to home
#define D_OFF 0x08 //display off
#define D_ON 0x0c //display on
#define D_ON_C_ON 0x0e //display on curser on
#define D_ON_C_BLK 0x0f //display on curser blink

//modes of operation
#define M_8BIT_1L 0x30 //mode 8 bit 1 line
#define M_8BIT_2L 0x38 //mode 8bit 2 line
#define M_4BIT_1L 0x20 //mode 4bit 1 line
#define M_4BIT_2L 0x28 //mode 4bit 2 line

//lines and positions
#define GOTO_L1_POSN0 0x80
#define GOTO_L2_POSN0 0xc0
#define GOTO_L3_POSN0 0x94
#define GOTO_L4_POSN0 0xd4
#define GOTO_CGRAM 0x40

//shifting commands
#define SHIFT_C_R 0x06 //shift curser right
#define SHIFT_D_L 0X10 //shift display left
#define SHIFT_D_R 0x14 //shift display right

#endif
