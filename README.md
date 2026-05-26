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

📊 Comprehensive Pinout Architecture Matrix

                   +-------------------------------------------------------+
                   |             LPC2129 MICROCONTROLLER                   |
                   |                                                       |
                   |  [P0.25/TD2] --------(TX)--------> [MCP2551]          |
                   |  [P0.24/RD2] <-------(RX)--------- [TRANSCEIVER]      |
                   |                                                       |
                   |  [P0.0 - P0.7] ==================> [16x2 LCD DATA]    |
                   |  [P0.16] ------------------------> [LCD RS CONTROL]   |
                   |  [P0.17] ------------------------> [LCD EN CONTROL]   |
                   |                                                       |
                   |  [P0.18] <-------(1-Wire)--------- [DS18B20 SENSOR]   |
                   |  [P0.19] ------------------------> [BUZZER ALARM]     |
                   |                                                       |
                   |  [P0.14/EINT1] <---(Falling Edge)- [LEFT SWITCH]      |
                   |  [P0.15/EINT2] <---(Falling Edge)- [RIGHT SWITCH]     |
                   |  [P0.20] <---------(Polling)------ [MODE SWITCH]      |
                   +-------------------------------------------------------+

    
