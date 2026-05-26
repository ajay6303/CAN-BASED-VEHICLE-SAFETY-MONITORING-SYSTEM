# CAN-Based Vehicle Safety & Monitoring System (NXP LPC2129)

A distributed, multi-node automotive electronics and cluster monitoring network developed in Embedded C for the ARM7-based **NXP LPC2129** microcontroller platform. This system utilizes the chip's native on-chip CAN controller blocks to achieve real-time, fault-tolerant data bus networking.

## 🛠️ System Architecture & Bus Specifications
The system handles vehicle instrumentation and telemetry over an industrial **Controller Area Network (CAN)** bus operating at a transmission speed of **125 kbps** with a Peripheral Clock ($PCLK$) tuned to **15 MHz**:

1. **Main Node (Central Instrumentation Cluster — `master.c`)**
   * Continuously samples engine block thermal metrics from a digital `DS18B20` thermistor via a 1-Wire serial bus protocol.
   * Renders standard operation dashboards and telemetry warnings onto a 4-line alphanumeric LCD layout.
   * Monopolizes external hardware interrupts (`EINT1` / `EINT2`) via driver input switches to capture steering column signals.
   * Evaluates bus traffic to trigger localized safety alert buzzers upon reverse hazard conditions.

2. **Indicator Node (`indicator.c`)**
   * Serves as a localized lighting control module tracking the system CAN bus.
   * Extracts steering frames broadcast from the Main Node to command active-low streaming LED arrays (simulating cascading turn indicators).

3. **Reverse Alert Node (`reverse.c` / `sonic.c`)**
   * Computes trailing obstacle distance thresholds using an ultrasonic sensor array (`HC-SR04`).
   * Evaluates real-time proximity windows and flags hazards instantly over the CAN network interface (Identifier: `0x201`).

## 🎛️ Hardware Configuration Map
* **Microcontroller:** NXP LPC2129 (ARM7TDMI-S with On-Chip Twin CAN Hardware Registers)
* **Bus Line Transceivers:** MCP2551
* **Sensor Layer:** DS18B20 1-Wire Temperature Sensor, HC-SR04 Ultrasonic Radar Module
* **Outputs:** Alphanumeric Character LCD, Active High-Output Warning Buzzer, LED Arrays
* **Development Stack:** Keil µVision 4 IDE, Proteus VSM Hardware Simulation Engine
