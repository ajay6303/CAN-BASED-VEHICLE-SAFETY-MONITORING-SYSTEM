#ifndef _SONIC_H_
#define _SONIC_H_

void INIT_Ultrasonic(void);
unsigned int dist_cal(void);
void send_pulse(void);
unsigned int read_echo(void);

#endif
