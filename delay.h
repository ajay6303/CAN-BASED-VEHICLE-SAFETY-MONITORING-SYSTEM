#ifndef _DELAY_H_
#define _DELAY_H_

typedef unsigned int u32;

// --- Time Pacing Inclusions API ---
void delay_us(u32 dlyus); // Software busy-wait tracking microsecond iterations
void delay_ms(u32 dlyms); // Software busy-wait tracking millisecond iterations
void delay_s(u32 dlys);   // Software busy-wait tracking whole second iterations

#endif
