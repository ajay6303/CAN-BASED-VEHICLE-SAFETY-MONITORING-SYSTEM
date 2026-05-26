//LCD.c

#include <lpc21xx.h>
#include "lcd_defines.h"
#include "LCD.h"
#include "delay.h"

void WriteLCD(u8 cmd)
{
				IOCLR0=1<<RW;//RW=0,write operation
        IOPIN0=(IOPIN0&~(0XFF<<PIN))|(cmd<<PIN);
				IOSET0=1<<EN;
				delay_us(1);
				IOCLR0=1<<EN;
				delay_ms(2);//internal process
}

void CmdLCD(u8 cmd)
{
        IOCLR0=1<<RS;//RS=0,command reg selected
				WriteLCD(cmd);
}

//display characters
void CharLCD(u8 ascii)
{
	IOSET0=1<<RS;//RS=1,data reg selected
	WriteLCD(ascii);
}

//initilization LCD
void InitLCD(void)
{
        IODIR0|=((0XFF<<PIN) | (1<<RS) | (1<<RW) | (1<<EN));//PIN,RS,RW,EN make as output pins
        delay_ms(15);
        CmdLCD(M_8BIT_1L);
        delay_ms(5);
        CmdLCD(M_8BIT_1L);
        delay_us(100);
        CmdLCD(M_8BIT_1L);
        CmdLCD(M_8BIT_2L);
        CmdLCD(D_ON);
        CmdLCD(CLR);
        CmdLCD(SHIFT_C_R);

}

//display strings
void StrLCD(u8 *ptr)
{
	while(*ptr)
	{
		CharLCD(*ptr++);
	}
}	
	
//display integers
void intLCD(u32 num)
{
	char a[10];
	int i=0;
	if(num==0)
		CharLCD('0');
	/*if(num<0)
	{
		CharLCD('-');
		num=-(num);
	}*/
	while(num)
	{
		a[i++]=(num%10)+'0';
		num/=10;
	}
	for(i-=1;i>=0;i--)
		CharLCD(a[i]);
}

		//display float numbers
void FloatLCD(f32 dec,u8  DP)
{
	int n,i;
	if(dec<0)
	{
		CharLCD('-');
		dec=-(dec);
	}
		n=dec;
		intLCD(n);
		CharLCD('.');
		for(i=0; i<DP; i++)
		{
			dec=(dec-n)*10;
			n=dec;
			CharLCD(n+48);
		}
}
void CustomChar(unsigned char *ch,int n)
{
        int i;
        CmdLCD(GOTO_CGRAM+(8*n));

        for(i=0;i<8;i++)
        {
                CharLCD(ch[i]);
        }
		//CmdLCD(GOTO_L1_POSN0);
}
