#include <lpc21xx.h>
#include "can.h"
#include "delay.h"
#include "sonic.h"
//#include "lcd.h"
//#include "lcd_defines.h"
int main()
{
    CANF txF;
    unsigned int distance;
    //InitLCD();
    Init_CAN1();
    INIT_Ultrasonic(); // T0PR is handled inside this function now
    
    txF.ID = 0x201;     // ID expected by the Main Node
    txF.bfv.RTR = 0;    // Data Frame
    txF.bfv.DLC = 1;    // 1 Byte of data
//	StrLCD("reverse");
//	delay_s(4);
  //  CmdLCD(CLR);

    while(1)
    {
        distance = dist_cal();
	
//		CmdLCD(GOTO_L1_POSN0);
//		StrLCD("Dist:        ");
//		CmdLCD(GOTO_L1_POSN0+6);
//		intLCD(distance);
		//less than 20 cm
        if(distance > 2 && distance < 200)
        {
            txF.Data1 = 1; // Object Detected
        }
        else
        {
            txF.Data1 = 0; // Path Clear
        }
       if(C1SR & 0x04) 
        {
            CAN1_Tx(txF);
        }
       delay_ms(100);
    }
}
