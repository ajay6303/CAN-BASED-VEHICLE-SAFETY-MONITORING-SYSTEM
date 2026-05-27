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



---

## 🎛️ 4. Hardware Configuration & Pinout Interconnect Tables

For standalone physical deployment or simulation inside the Proteus VSM engine, use these precise microcontroller peripheral pin-mapping configurations.

## 📊 Comprehensive Pinout Architecture Matrix



---

## ⚙️ 5. Step-by-Step Software Architecture Flowcharts
These procedural flowcharts illustrate the firmware state machines and execution loops running on each microcontroller.  

## 🧠 Flowchart 1: Main Node Core Loop & Service Routines (master.c)



## 🚨 Flowchart 2: Turn Indicator Satellite Loop (indicator.c)



## 📡 Flowchart 3: Rear Collision Avoidance Loop (reverse.c)



---

## 🛠️ 6. Sequential Implementation & Verification Strategy

To implement this project systematically, follow this module testing sequence to isolate potential points of hardware or software failure before integrating the distributed network:



---

### 💻 7. Software IDE, Toolchain, & Firmware Deployment

## ⚙️ Development Infrastructure Requirements
* **Firmware IDE:** Keil µVision 4.
* **Flashing Software Utility:** Flash Magic.
* **Simulation Environment:** Vector LPC2129 hardware kit.

---

## 🚀 Compilation & Flashing Instructions
* Launch the Keil µVision 4 environment.  
* Select **Project -> New uVision Project**, choose the **NXP LPC2129** device profile from the vendor repository, and confirm the inclusion of the low-level system startup code assembly block (**Startup.s**).
* Set up three separate development workspaces or target build profiles to handle the source files for each distinct module: **master.c**, **indicator.c**, and **reverse.c**.
* Open the project configuration menu (**Options for Target**), navigate to the **Output tab**, and ensure the checkbox labeled **"Create HEX File"** is selected.
* Click **Rebuild All Target Files** to compile the source code modules and generate the final production .hex binaries.
* Connect your development board to your computer using a **USB-to-UART converter interface** wired to your microcontroller's primary ISP serial programming ports.
* Open **Flash Magic**, select your active serial COM port, set the target device profile to LPC2129, choose the compiled .hex file, and click **Start** to flash the code onto the chip.  
  
---

## ⚡ 8. Real-World Project Advantages & Verification Metrics

## 🌟 Key Engineering Advantages
* **Harness Simplification**
* **Industrial Noise Immunity**
* **Deterministic Message Prioritization**
* **Fault Isolation**
---
