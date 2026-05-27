# 🚘 CAN-Based Vehicle Safety & Monitoring System

A distributed, high-reliability automotive electronic control unit (ECU) network designed and developed using the **Controller Area Network (CAN)** protocol on the ARM7-based **NXP LPC2129** microcontroller platform. This architecture establishes real-time, coordinated multi-node communication to monitor critical powertrain metrics, manage smart lighting systems, and execute intelligent collision avoidance.

---

## 💡 1. Core Project Aim & System Insight

### 🎯 Project Aim
To design, implement, and validate an advanced automotive safety ecosystem where a centralized monitoring node updates critical operations, coordinates visual turn indicators, and evaluates rear hazard proximities through seamless, multi-node bus communication.

### 🧠 Prerequisite Engineering Insights
To fully comprehend or implement this architecture, the following engineering competencies are required:
* 💻 **Embedded-C Programming:** Advanced knowledge of register-level manipulation and modular system firmware layout.
* 📐 **LPC2129 Hardware Architecture:** Deep familiarity with General Purpose Input/Output (GPIO) interfaces, Analog-to-Digital Converters (ADC), Vector Interrupt Controllers (VIC), and native on-chip CAN registers.
* 🔌 **Protocol Architecture:** A foundational understanding of the physical and data link layers of the CAN communication protocol.

---

## ⚡ 2. The Critical Need for CAN & System Advantages

In conventional automotive design, point-to-point wiring configurations introduce severe systemic bottlenecks. This project highlights the modern shift to bus-based topologies:

* 📉 **Harness Weight Reduction:** Replaces heavy parallel wiring looms with a single twisted-pair differential communication line (CANH/CANL), reducing assembly costs and vehicle curbside weight.
* 🛡️ **Industrial Noise Immunity:** Operates via differential voltage signaling ($V_{DIFF} = V_{CANH} - V_{CANL}$), cancelling out common-mode electromagnetic interference (EMI) originating from the ignition system or alternator.
* 🚀 **Deterministic Prioritization:** Utilizes non-destructive bitwise arbitration where critical safety frames (e.g., proximity alerts) override routine telemetry frames without data corruption.
* 🩺 **Fault Confinement:** Built-in error tracking counters automatically quarantine failing nodes, maintaining the integrity of the remaining vehicle network.

---

## 🏗️ 3. Distributed Network Architecture Diagram

The system topology links three independent functional ECUs across a shared physical CAN network, terminated at each cable endpoint with standard $120\text{-}\Omega$ matching resistors ($R_T$) to eliminate signal reflections:

```text
               ===================== PHYSICAL CAN BUS LINE =====================
                  ||                                   ||                     ||
               [ CANH ]                             [ CANH ]               [ CANH ]
               [ CANL ]                             [ CANL ]               [ CANL ]
                  ||                                   ||                     ||
         +------------------+                 +------------------+   +------------------+
         |     MCP2551      |                 |     MCP2551      |   |     MCP2551      |
         | CAN Transceiver  |                 | CAN Transceiver  |   | CAN Transceiver  |
         +------------------+                 +------------------+   +------------------+
               ||   ||                               ||   ||                ||   ||
         +------------------+                 +------------------+   +------------------+
         |   On-Chip CAN    |                 |   On-Chip CAN    |   |   On-Chip CAN    |
         |    Controller    |                 |    Controller    |   |    Controller    |
         +------------------+                 +------------------+   +------------------+
         |     LPC2129      |                 |     LPC2129      |   |     LPC2129      |
         |  [ MAIN NODE ]   |                 | [INDICATOR NODE] |   | [REVERSE NODE]   |
         +------------------+                 +------------------+   +------------------+
          /       |        \                           |                      |
     [DS18B20]  [LCD]  [Switches]                  [8x LEDs]              [HC-SR04]
  (Temp Sensor)        (LISW/RISW/MODE)          (Turn Signals)       (Ultrasonic Radar)
```
## 🎛️ 4. Hardware Configuration & Pinout Interconnect Tables

For standalone physical deployment or simulation inside the Proteus VSM engine, use these precise microcontroller peripheral pin-mapping configurations.

## 📊 Comprehensive Pinout Architecture Matrix

```text
                   +-------------------------------------------------------+
                   |             LPC2129 MICROCONTROLLER                   |
                   |                                                       |
                   |  ***************** MAIN NODE **********************   |
                   |                                                       |
                   |  [P0.0 - P0.7] ==================> [16x2 LCD DATA]    |
                   |  [P0.18] ------------------------> [LCD RS CONTROL]   |
                   |  [P0.19] ------------------------> [LCD EN CONTROL]   |
                   |                                                       |
                   |  [P0.16] <-------(1-Wire)--------- [DS18B20 SENSOR]   |
                   |  [P0.8] ------------------------> [BUZZER ALARM]      |
                   |                                                       |
                   |  [P0.14/EINT1] <---(Falling Edge)- [LEFT SWITCH]      |
                   |  [P0.15/EINT2] <---(Falling Edge)- [RIGHT SWITCH]     |
                   |  [P0.9] <---------(Polling)------ [MODE SWITCH]       |
                   |                                                       |
                   |  ******************* Indicator ********************   |
                   |  [p0.8 -p0.15] ==================> [ 8 LED's ]        |
                   |                                                       |
                   |  ******************** Reverse *********************   |
                   |                                                       |
                   |  [ 0.21 ] <----------(TRIG)---------- [ HC-SR04 ]     |
                   |  [ 0.22 ] -----------(ECHO)---------> [ HC-SR04 ]     |
                   +-------------------------------------------------------+

```
---
## ⚙️ 5. Step-by-Step Software Architecture Flowcharts
These procedural flowcharts illustrate the firmware state machines and execution loops running on each microcontroller.  
## 🧠 Flowchart 1: Main Node Core Loop & Service Routines (master.c)
```text
  [POWER-ON RESET]
       │
       ▼
[INITIALIZE PERIPHERALS] ──► Configure System Clocks & GPIO Pins
       │                 ──► Initialize 16x2 LCD Layout 
       │                 ──► Configure CAN Registers to 125 kbps
       │                 ──► Register Interrupt Vectors (EINT1 / EINT2)
       ▼
┌─────────────────────────────────────────────────────────────────────────┐
│ MAIN CODE CONTINUOUS EXECUTION LOOP                                     │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  1. READ ENGINE TEMPERATURE                                             │
│     │                                                                   │
│     ▼                                                                   │
│     Query DS18B20 Thermistor via 1-Wire Serial Protocol                 │
│     Convert Raw Binary Bytes into Celsius Temperature Metric            │
│     Update and Refresh Current Temperature Values on the LCD            │
│                                                                         │
│  2. MONITOR OPERATIONAL DRIVING MODE SWITCH                             │
│     │                                                                   │
│     ▼                                                                   │
│     Is MODE SWITCH Pressed?                                             │
│        ├──► [YES] ──► Toggle State Flag (Forward ◄──► Reverse)          |
│        │              Update System Status Banner on LCD Display        │
│        └──► [NO]  ──► Maintain Active Mode Setting                      │
│                                                                         │
│  3. EXECUTE CONTEXT-DEPENDENT PROCESSING                                │
│     │                                                                   │
│     ├─► IF (INTERRUPT's ACTIVE)                                         │
│     │      Check which Switch Interrupt Flag is Asserted                │
│     │         └──► [LEFT | RIGHT] ──► Encapsulate Directional Command   |
|     |                                 Data Broadcast ID 0x101 Frame over|
|     |                                 CAN Bus,if left,send 0x01 data if |
|     |                                 right,send 0x02 data Clear Local  |
|     |                                 Steering Flags                    │
│     │                                                                   │
│     └─► IF (REVERSE MODE ACTIVE)                                        │
│            Check CAN Receive Buffer for Incoming Frames                 │
│               └──► [FRAME RECEIVED] ──► Read ID 0x201 Byte Payload      |
│                             ├──► [DATA == 1] ──► Pull Alert Buzzer LOW  | 
│                             └──► [DATA == 0]                            |
│                                      ├──► Pull Alert Buzzer LOW         |
|                                      ├──► Delay                         |
|                                      └──► Pull Alert Buzzer HIGH        |
└─────────────────────────────────────────────────────────────────────────┘
       ▲
       │ (Loop Repeats Infinitely)
       └──────────────────────────────────────────────────────────────────┘

===========================================================================
HARDWARE INTERRUPT SERVICE ROUTINES (BACKGROUND TRIGGERS)
===========================================================================
When Steering Switch 1 Fired (LISW Press) ──► Enter EINT1 ISR ──► Set Left Turn Flag 
When Steering Switch 2 Fired (RISW Press) ──► Enter EINT2 ISR ──► Set Right Turn Flag

```
## 🚨 Flowchart 2: Turn Indicator Satellite Loop (indicator.c)

```text
[START] ──► Initialize Ports ──► Configure CAN Controller Core ──► Turn All LEDs OFF
                                                                          │
┌─────────────────────────────────────────────────────────────────────────┘
▼
[POLL CAN RECEIVE BUFFER REGISTER] ◄──────────────────────────────────────┐
       │                                                                  │
       ├──► [BUFFER EMPTY] ──► Maintain Current State ────────────────────┤
       │                                                                  │
       └──► [FRAME CAPTURED]                                              │
                 │                                                        │
                 ▼                                                        │
       Does Frame Identifier Match Target ID 0x101?                       │
                 ├──► [NO]  ──► Discard Packet and Release Buffer ────────┤
                 │                                                        │
                 └──► [YES] ──► Parse Byte Payload                        │
                                   │                                      │
                                   ├─► IF (DATA PAYLOAD == 1)             │
                                   │      Execute Left Turn Animation:    │
                                   │      Scroll LEDs Right-to-Left       |
                                   │                                      │
                                   └─► IF (DATA PAYLOAD == 2)             │
                                          Execute Right Turn Animation:   │
                                          Scroll LEDs Left-to-Right       |
```
---
## 📡 Flowchart 3: Rear Collision Avoidance Loop (reverse.c)

```text
[START] ──► Initialize Hardware Timer 0 Modules ──► Set CAN Communication Parameters
                                                                          │
┌─────────────────────────────────────────────────────────────────────────┘
▼
[GENERATE TRIGGER OUTPUT PULSE] ──► Assert P0.21 HIGH for 10 Microseconds 
       │
       ▼
[MEASURE ACOUSTIC TIME-OF-FLIGHT]
       │
       ▼
Wait for Echo Input (P0.22) to Transition HIGH ──► Start Timer Counter 
Wait for Echo Input (P0.22) to Transition LOW  ──► Stop Timer Counter 
       │
       ▼
[COMPUTE TARGET RANGE PROFILE]
       │
       ▼
Calculate Distance: cm = (Timer Register Counts) / 58
       │
       ▼
Is Computed Clearance Below the Preset Safe Limit Value? 
       │
       ├──► [YES] (Obstacle Detected) ──► Prepare CAN Frame, Data Byte = 1 
       │                                  Set Message Identifier = 0x201 
       │                                                                   │
       └──► [NO]  (Path Is Clear)     ──► Prepare CAN Frame, Data Byte = 0 
                                          Set Message Identifier = 0x201 
       │                                                                   │
       ▼                                                                   ▼
[BROADCAST MESSAGE FRAME OVER DETACHED TRANSCEIVER TERMINALS INTO THE NETWORK BACKBONE]
       │
       ▼
Delay 60ms to Stabilize Transducer Sensor Elements ──► Repeat Loop Unconditionally
```
---
## 🛠️ 6. Sequential Implementation & Verification Strategy

To implement this project systematically, follow this module testing sequence to isolate potential points of hardware or software failure before integrating the distributed network:

```text
  +---------------------------+       +---------------------------+       +---------------------------+
  |         STEP 1            |       |         STEP 2            |       |         STEP 3            |
  | Verify Character Display  | ───&gt| Calibrate 1-Wire Telemetry| ───&gt| Validate Interrupt Lines  |
  | Commands on 16x2 LCD      |       | for DS18B20 Sensor        |       | and LCD Counter Updates   |
  +---------------------------+       +---------------------------+       +---------------------------+
                                                                                        │
                                                                                        ▼
  +---------------------------+       +---------------------------+       +---------------------------+
  |         STEP 6            |       |         STEP 5            |       |         STEP 4            |
  | Compile Multi-Node Main   | ◄───  | Test Loopback Modes       | ◄───  | Verify Range Calculations |
  | Code & System Integration |       | on Isolated CAN Drivers   |       | for Ultrasonic Sensors    |
  +---------------------------+       +---------------------------+       +---------------------------+

```
---
### 💻 7. Software IDE, Toolchain, & Firmware Deployment
## ⚙️ Development Infrastructure Requirements
* **Firmware IDE:** Keil µVision 4.
* **Flashing Software Utility:** Flash Magic.
* **Simulation Environment:** vector lpc2129 hardware kit.

---
## 🚀 Compilation & Flashing Instructions
* Launch the Keil µVision 4 environment.  
* Select Project -> New uVision Project, choose the NXP LPC2129 device profile from the vendor repository, and confirm the inclusion of the low-level system startup code assembly block (Startup.s).
* Set up three separate development workspaces or target build profiles to handle the source files for each distinct module: master.c, indicator.c, and reverse.c.
* Open the project configuration menu (Options for Target), navigate to the Output tab, and ensure the checkbox labeled "Create HEX File" is selected.
* Click Rebuild All Target Files to compile the source code modules and generate the final production .hex binaries.
* Connect your development board to your computer using a USB-to-UART converter interface wired to your microcontroller's primary ISP serial programming ports.
* Open Flash Magic, select your active serial COM port, set the target device profile to LPC2129, choose the compiled .hex file, and click Start to flash the code onto the chip.  
  
---
