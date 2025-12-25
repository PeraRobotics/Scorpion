# Scorpion - Micro Mouse Robot

![Project Status](https://img.shields.io/badge/Platform-ESP--IDF-blue)
![Organization](https://img.shields.io/badge/Robotics%20Society-UOP-red)

## Overview
**Scorpion** is a high-performance Micro Mouse robot developed by the **Robotics Society of the University of Peradeniya**. Designed to navigate and solve mazes autonomously, the robot is built on the **ESP32** platform using the **ESP-IDF (Espressif IoT Development Framework)** to ensure real-time performance and efficient hardware control.

---

## Project Structure
The repository is organized to follow the standard ESP-IDF build system:

* `main/`: Core application logic, including maze-solving algorithms and state machines.
* `components/`: **(Git Submodule)** Contains shared libraries, drivers for motor control, PID controllers, and sensor processing.
* `managed_components/`: External dependencies managed via the IDF Component Manager.
* `CMakeLists.txt`: Main build configuration.

---

## Setup & Installation


Method,Type,Primary Benefit
Touch,Input,Replaces physical buttons with touch-sensitive pads.
RMT,Out/In,Precise timing for LEDs and Infrared.
PCNT,Input,High-speed pulse counting without CPU load.
I2S,Out/In,Dedicated to high-quality digital audio.
Hall,Input,Detects magnets using the chip's internal circuitry.