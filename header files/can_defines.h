#ifndef _CAN_DEFINES_H_
#define _CAN_DEFINES_H_

// --- Physical Layout Pin Settings Configurations ---
#define RD1_PIN 0x00040000           // Bit configuration setting pin P0.25 function paths to CAN1 RX (RD1)

// --- Advanced Network Clock Frequency Calculation Constants ---
#define PCLK 15000000                // Base Peripheral Clock frequency set to 15 MHz
#define BIT_RATE 125000              // Target operational network data transmission speed set to 125 kbps
#define QUANTA 20                    // Total time quanta count slices assigned per single bit width window
#define BRP (PCLK/(BIT_RATE*QUANTA)) // Baud Rate Prescaler logic calculation equation value
#define SAMPLE_POINT (0.7*QUANTA)    // Fix optimal sample checkpoint metrics inside 70% bit width envelopes
#define TSEG1 ((u32)SAMPLE_POINT-1)  // Time Segment 1 tick count definition block mapping
#define TSEG2 (QUANTA-(1+TSEG1))     // Time Segment 2 tick count definition block mapping

// Synchronization Jump Width constraints setup rules definitions mapping
#define SJW ((TSEG2>=5)?4:(TSEG2-1))

// --- Aggregated Constant Bit Map Config Blocks for BTR Control Registers ---
#define SAM 0 // Bus sampling configuration choice (0 = Sample single time, 1 = Sample three times)
#define BTR_LVAL (SAM<<23|((TSEG2-1)<<20)|((TSEG1-1)<<16)|((SJW-1)<<14)|(BRP-1))

// --- Command Register Bit Positions (CxCMR SFR) ---
#define TR_BIT 0   // Transmission Request command bit configuration tracker
#define RRB_BIT 2  // Release Receive Buffer command bit configuration tracker
#define STB1_BIT 5 // Select Transmit Buffer 1 command bit configuration tracker

// --- Global Status Register Bit Positions (CxGSR SFR) ---
#define RBS_BIT 0   // Receive Buffer Status alert indicator position track
#define TBS1_BIT 2  // Transmit Buffer Status 1 vacancy tracking indicator position
#define TCS1_BIT 3  // Transmission Compliance Status validation tracking indicator position

// --- Transmission Frame Info & Receive Frame Status Parameter Bit Envelopes ---
#define RTR_BIT 30   // Offset bit alignment tracking for Remote Transmission Requests data
#define DLC_BITS 16  // Offset bit alignment tracking for Data Length Code byte parameters

// --- Mode Control Register Configuration Targets (CxMOD SFR) ---
#define RM_BIT 0     // Reset Mode operational status override switch indicator positioning

// --- Acceptance Filter Mode Register Option Controls (AFMR SFR) ---
#define AccOFF_BIT 0 // Main Acceptance Filter Off control lane selector position switch
#define AccBP_BIT  1 // Acceptance Filter Bypass control lane selector position switch

#endif
