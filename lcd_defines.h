#ifndef _LCD_DEFINES_H_
#define _LCD_DEFINES_H_

// --- Physical Layout Board Hardware Connections ---
#define PIN 0    // Parallel bus tracks D0 through D7 connected to pins P0.0 through P0.7
#define RS 18    // Register Select pin tied to controller line P0.18
#define EN 19    // Hardware Enable execution toggle line tied to controller line P0.19
#define RW 24    // Read/Write transmission tracking controller line tied to pin P0.24

// --- Core Structural Instruction Vector Command Sets ---
#define CLR 0x01           // Flushes all background data strings out of internal RAM records
#define RET 0x02           // Commands internal cursor tracking to snap back to index offset zero
#define D_OFF 0x08         // Disables visibility layers while securing hidden characters context data
#define D_ON 0x0c          // Activates visibility layers, ensuring background cursor indicators stay hidden
#define D_ON_C_ON 0x0e     // Activates visibility layers and forces underline cursor rendering on
#define D_ON_C_BLK 0x0f    // Activates screen visibility and applies flashing loops to the active tracking cursor

// --- Communication Bus Interface Operational Settings ---
#define M_8BIT_1L 0x30     // Establishes 8-bit bus configurations tracked across a single line layout
#define M_8BIT_2L 0x38     // Establishes 8-bit bus configurations tracked across dual line layouts
#define M_4BIT_1L 0x20     // Establishes 4-bit bus configurations tracked across a single line layout
#define M_4BIT_2L 0x28     // Establishes 4-bit bus configurations tracked across dual line layouts

// --- Character Display Matrix Boundary Mapping Constants ---
#define GOTO_L1_POSN0 0x80 // Hardware Address index target for Line 1 Char position 0
#define GOTO_L2_POSN0 0xc0 // Hardware Address index target for Line 2 Char position 0
#define GOTO_L3_POSN0 0x94 // Hardware Address index target for Line 3 Char position 0
#define GOTO_L4_POSN0 0xd4 // Hardware Address index target for Line 4 Char position 0
#define GOTO_CGRAM 0x40    // Root instruction command targeting custom graphic memory records

// --- Text Cursor Stepping and Motion Shift Configuration Options ---
#define SHIFT_C_R 0x06     // Sets auto-increment stepping parameters to push text tracks rightward
#define SHIFT_D_L 0X10     // Forces visible character window segments to step leftward
#define SHIFT_D_R 0x14     // Forces visible character window segments to step rightward

#endif
