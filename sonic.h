#ifndef _SONIC_H_
#define _SONIC_H_

// --- API Function Prototypes ---
void INIT_Ultrasonic(void);  // Pin setup and internal operational configs
unsigned int dist_cal(void); // Computes obstacle distance in centimeters
void send_pulse(void);       // Generates trigger signals required by HC-SR04/HC-SR05
unsigned int read_echo(void); // Evaluates echo pulse time-of-flight via hardware timer counters

#endif
