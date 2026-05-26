🚗 CAN-Based Vehicle Safety & Monitoring System
An industrial-grade, distributed Electronic Control Unit (ECU) automotive network engineered for the ARM7-based NXP LPC2129 microcontroller platform. This project demonstrates an on-chip multi-node Controller Area Network (CAN) designed to monitor critical vehicle parameters, manage interactive turn signals, and process real-time proximity alerts via differential bus communication.  
PDF
+ 3
📌 1. Project Overview & Core Objective
In modern automotive engineering, traditional point-to-point wiring looms are heavy, costly, complex to troubleshoot, and highly vulnerable to electromagnetic interference (EMI). This project resolves these limitations by implementing a decentralized, fault-tolerant CAN Bus network.  
PDF
🎯 System Aim
The central objective is to build a simulated automotive dashboard and safety environment where a central hub (Main Node) seamlessly tracks power-train thermal metrics, broadcasts driver directional intentions, and evaluates rear collision risks by interfacing with dedicated satellite ECUs (Indicator Node and Reverse Alert Node).  
PDF
⚡ 2. Why CAN? System Advantages
Harness Reduction: Replaces bulky parallel wiring with a single, lightweight twisted-pair differential bus line (CANH/CANL), reducing vehicle weight and manufacturing costs.  
PDF
+ 2
High Reliability & Noise Immunity: Uses differential signaling to cancel out common-mode automotive electrical noise.
Prioritized Message ID Frame Handling: Ensures critical safety notifications (such as proximity hazards) take immediate bus precedence over routine sensor updates.
Error Detection & Confinement: Embedded on-chip controllers automatically track, flags, and quarantine faulty nodes without taking down the entire vehicle communication system.  
PDF
🏗️ 3. Distributed Network Architecture
The network consists of three distinct functional units connected across a common physical CAN network layer terminated with standard 120-ohm bus resistors (R 
T
​	
 ) to eliminate signal reflections:  
PDF
+ 1
                     =================== CAN BUS LINE ===================
                        ||                          ||                ||
                     [ CANH ]                    [ CANH ]          [ CANH ]
                     [ CANL ]                    [ CANL ]          [ CANL ]
                        ||                          ||                ||
                 +--------------+            +--------------+  +--------------+
                 |   MCP2551    |            |   MCP2551    |  |   MCP2551    |
                 | Transceiver  |            | Transceiver  |  | Transceiver  |
                 +--------------+            +--------------+  +--------------+
                        ||                          ||                ||
                 +--------------+            +--------------+  +--------------+
                 |  LPC2129 MCU |            |  LPC2129 MCU |  |  LPC2129 MCU |
                 | (MAIN NODE)  |            | (INDICATOR)  |  |  (REVERSE)   |
                 +--------------+            +--------------+  +--------------+
                   /    |     \                     |                 |
             DS18B20   LCD   Switches             8x LEDs          HC-SR05
🛰️ Node Breakdown
Main Node (Central Instrumentation Cluster): Serves as the primary operational hub. It monitors local engine block thermal dynamics and decodes incoming danger signals from the network to drive warning systems.  
PDF
+ 4
Indicator Node (Body Control ECU): Acts as a smart lighting module that continuously sniffs the bus for driver steering switch frames and flashes visual signals accordingly.  
PDF
+ 1
Reverse Alert Node (Proximity Defense ECU): Dedicated solely to high-frequency rear ultrasonic scanning, measuring distance profiles, and flashing high-priority data onto the bus when safety thresholds are breached.  
PDF
+ 2
🎛️ 4. Hardware Specifications & Functional Mapping
🧩 Hardware Component Stack
Microcontroller: NXP LPC2129 (ARM7TDMI-S base core containing twin integrated physical layer On-Chip CAN Registers).  
PDF
+ 3
Physical Transceiver: MCP2551 (Converts logic-level TTL UART signals into robust differential CANH/CANL bus voltages).  
PDF
+ 1
Thermal Sensor: DS18B20 (Digital thermistor providing high-precision temperature tracking via a 1-Wire protocol).  
PDF
+ 1
Proximity Sensor: HC-SR05 / HC-SR04 Ultrasonic Radar Module.  
PDF
Human-Machine Interface: Standard 16x2 Alphanumeric Character LCD , tactile input switches , and an active warning buzzer.  
PDF
+ 2
📊 Peripheral Interface Pin Mapping Table
Node Block	System Component	Microcontroller Peripheral Block	Functional Description
All Nodes	MCP2551 CAN Transceiver	
On-Chip CAN Controller (Tx1/Rx1 or Tx2/Rx2)
Handles standard physical layer data frames across the CAN infrastructure.
Main Node	DS18B20 Temp Sensor	
GPIO Line Configured for 1-Wire Bus Protocol
Measures active vehicle engine block thermal ranges.
Main Node	16x2 Character LCD	
Parallel GPIO Data/Control Pins (RS, EN, D4-D7)
Renders real-time telemetry, operational status, and hazards.
Main Node	Left Indicator Switch (LISW)	
External Interrupt 1 Line (EINT1)
Triggers a message transmission for left turn lane signaling.
Main Node	Right Indicator Switch (RISW)	
External Interrupt 2 Line (EINT2)
Triggers a message transmission for right turn lane signaling.
Main Node	Mode Select Toggle (MODE SW)	
GPIO Input Pin with Polling Routine
Toggles system state machines between Forward and Reverse modes.
Main Node	Safety Alert Buzzer	
GPIO Digital Output Line
Generates acoustic warning signals during critical reverse hazards.
Indicator Node	8x LED Warning Bank	
8-Bit Dedicated Parallel GPIO Port
Simulates vehicle cascading or sequential turn indicators.
Reverse Node	HC-SR05 Ultrasonic Radar	
GPIO Trigger Output + Timer Capture Echo Input
Calculates precise object distance parameters via air-wave propagation.
⚙️ 5. Software Architecture & Firmware Workflows
The software framework is written entirely in Modular Embedded C targeting direct register manipulation inside the ARM7 core.  
PDF
+ 1
🔄 1. Main Node Firmware State Machine (main_node.c)
Initialization Loop: Configures peripheral clocks, initializes the 16x2 LCD, registers the EINT1/EINT2 external interrupts, and sets the CAN controller bit timing registers to 125 kbps.  
PDF
+ 2
Background Process: 1. Reads raw data from the DS18B20 sensor, scales the values, and prints the updated engine temperature to the LCD.
2. Continuously monitors the MODE SW pin status.  
PDF
+ 2
Interrupt Logic (SW1 / SW2 Press): Captures physical steering shifts instantly. The corresponding ISR forms a CAN data frame containing the directional flag and broadcasts it onto the bus.  
PDF
Mode Constraints:
Forward Mode (Default): Ignores incoming obstacle packets; standard operation displays remain active on screen.  
PDF
+ 1
Reverse Mode: The node actively listens for proximity packets. If a packet containing a hazard flag is detected, the MCU pulls the buzzer line high to sound an alarm.  
PDF
+ 4
🔄 2. Indicator Node State Machine (indicator_node.c)
Background Process: Remains in a low-overhead block waiting loop, polling the internal CAN receiver buffers for valid frame receipts.  
PDF
Execution Logic: When a directional frame arrives from the Main Node, the system enters one of two shift sequences:  
PDF
+ 3
Left Turn Received: Runs an active-low bitwise barrel shift that scrolls the 8-LED bank sequentially from Right to Left.  
PDF
Right Turn Received: Runs an active-low bitwise barrel shift that scrolls the 8-LED bank sequentially from Left to Right.  
PDF
🔄 3. Reverse Alert Node State Machine (reverse_node.c)
Background Process: 1. Issues a 10-microsecond high pulse on the ultrasonic trigger pin.
2. Measures the duration of the incoming pulse on the echo pin using a hardware timer.
3. Calculates distance: Distance (cm)= 
2
Time×0.0343
​	
 .  
PDF
+ 1
Decision Tree Logic:
If Distance<Safety Threshold Limit Value: Broadcasts a high-priority CAN frame containing Logic 1 to flag an immediate crash hazard.  
PDF
If Distance≥Safety Threshold Limit Value: Broadcasts a standard CAN frame containing Logic 0 to signal an open pathway.  
PDF
+ 1
🛠️ 6. Step-by-Step Implementation Sequence
To complete this project, follow this structured module testing sequence to verify hardware stability before compiling the unified main network program:  
PDF
+ 1
[Step 1: Alphanumeric LCD Setup] ---> [Step 2: DS18B20 1-Wire Verification]
                                                      |
                                                      v
[Step 4: Ultrasonic Sensor Calibr.] <--- [Step 3: External Interrupt Tuning]
              |
              v
[Step 5: CAN Loopback & Bus Test]  ---> [Step 6: Multi-Node Integration]
Step 1: Core HMI Display Test – Code a localized driver script to verify the 16x2 character LCD, ensuring character constants, strings, and integer variables display correctly.  
PDF
Step 2: 1-Wire Thermal Verification – Implement the precise timing logic required for the DS18B20 sensor, read the ambient temperature values, and route the data directly onto the LCD.  
PDF
Step 3: External Interrupt Tuning – Isolate the tactile switches to confirm the external interrupt routine functions properly. Increment an active count register each time an interrupt is triggered and confirm the count updates live on the LCD.  
PDF
Step 4: Ultrasonic Calibration – Run standalone distance tests on the HC-SR05 sensor module to verify the calculations return accurate physical distance measurements.  
PDF
Step 5: Isolated CAN Testing – Load a basic loopback or point-to-point CAN test script to check the hardware transceivers, confirm bit timings, and verify error-free transmission.  
PDF
Step 6: Integrated Multi-Node Compilation – Merge the individual drivers into the final master/satellite configurations across the three separate target microcontrollers.  
PDF
💻 7. Software Environment Setup & Deployment
⚙️ Development Tools
Compiler/IDE: Keil µVision 4 (C51/ARM compiler suite tools).  
PDF
Target Flashing Tool: Flash Magic utility software (handles ISP serial hex loading protocols).  
PDF
Simulation Engine: Proteus VSM Hardware Simulation Tool (for structural validation prior to building physical hardware circuits).
🚀 Compilation & Flashing Instructions
Open Keil µVision 4 on your development machine.  
PDF
Navigate to Project -> New uVision Project, choose the NXP LPC2129 device profile from the database, and ensure you include the default standard ARM startup code assembly block (Startup.s).
Create three separate workspaces or target environments for the Main, Indicator, and Reverse software routines.  
PDF
Under the project settings menu (Options for Target), navigate to the Output tab and check the box labeled "Create HEX File".
Click Rebuild All Target Files to compile the codebase and generate the production .hex binaries.
Connect your LPC2129 board to your computer using a USB-to-UART converter cable attached to your target ISP serial communication pins.  
PDF
Launch the Flash Magic utility tool, specify your active COM port connection, choose the target microchip, select the compiled target .hex file, and press Start to flash the code onto the chip.  
PDF
🏆 8. System Status & Final Verification
💡 Project Completion Sign-off: The embedded network is successfully deployed when temperature variables continuously stream to the LCD, directional switches invoke cascading LED movements across the Indicator node, and close-range obstructions trigger the central hazard buzzer when the system is set to reverse mode.  
