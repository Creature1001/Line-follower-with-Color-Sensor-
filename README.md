# Line follower with Color Sensor
**ECE 528/L – Robotics and Embedded Systems with Lab**  
Final Project – Spring 2026  

**Authors:** Amanda Arst, Jazmin Martinez  
---

## Project Overview

This project focuses on designing and implementing an autonomous robot capable of navigating a maze while responding to colored markers along its path. The robot combines **line-following** and **real-time color detection** to modify its behavior dynamically.

The system uses a reflectance sensor array to follow a path and a color sensor to detect specific colors. Based on detected colors, the robot performs actions such as:
- Stopping  
- Rotating  
- Increasing speed  

The platform is built around the **MSP432 microcontroller**, integrating multiple peripherals including GPIO, PWM, timers, and I2C communication. Initial color detection testing was performed in Python before transitioning to a C++ embedded implementation. 

---

## System Architecture

<img width="612" height="257" alt="image" src="https://github.com/user-attachments/assets/c7e96804-3e42-4b1f-b6ee-14cb5fc0d78c" />


The system follows a modular architecture integrating sensing, processing, and actuation:

- **Inputs:**
  - Reflectance sensor array (line detection via GPIO)
  - Pmod COLOR sensor (RGB detection via I2C)

- **Processing:**
  - MSP432 microcontroller processes sensor data

- **Outputs:**
  - PWM signals for motor speed
  - Direction signals for motor control

- **Actuation:**
  - DRV8838 motor driver controls left and right DC motors

- **Power:**
  - External battery supply

The system diagram (see page 4 of the report) shows how sensor data flows into the microcontroller and is translated into motor control signals.

---

## Interfaces and Peripherals Used

### Hardware Components
- MSP432 Microcontroller  
- Pmod COLOR (TCS3472) sensor  
- DRV8838 Motor Driver  
- Reflectance Sensor Array  
- 2 × DC Motors  
- External Power Supply  

### Interfaces
- **GPIO:** Line sensors, motor direction, LED control  
- **PWM (Timer A):** Motor speed control  
- **Timers:** Delays and timed actions (e.g., speed boost)  
- **I2C (eUSCI_B):** Communication with color sensor  

### Key Pin Connections
- SDA → P6.4  
- SCL → P6.5  
- PWM → P2.5  
- Direction → P2.4  
- LED Enable → P5.0  

---

## Verification and Testing

### Functional Testing
- Line-following system performed reliably using reflectance sensors  
- Motor control via PWM enabled smooth and adjustable movement  
- Color detection worked accurately in controlled environments  

### Development Process
- Initial RGB data validation using Python  
- Transitioned to C++ using Digilent libraries for embedded integration  

### Observations
- The robot successfully:
  - Followed lines consistently  
  - Detected colors and triggered actions when tested independently  

- However, full system integration showed inconsistencies:
  - Difficulty combining real-time color detection with navigation  
  - Timing conflicts between sensing and movement  

### Issues Encountered
- Color sensor required onboard LED for reliable detection  
- Inconsistent color recognition (required approximate matching)  
- Mechanical instability due to angled sensor mounting  
- Sensor positioning affected accuracy and reliability  

---


## Conclusion

This project successfully demonstrated the integration of **line-following** and **color detection** in an embedded robotic system using the MSP432.

### Achievements
- Functional navigation using reflectance sensors  
- Real-time color detection and response  
- Successful integration of multiple peripherals  

### Limitations
- Inconsistent performance in full maze environment  
- Dependence on lighting conditions for color detection  
- Mechanical instability of sensor mounting  

### Future Improvements
- More secure sensor mounting  
- Improved color calibration techniques  
- Enhanced algorithms for color classification  
- Better synchronization between sensing and control  

Overall, the project highlights both the capabilities and challenges of integrating multiple subsystems in robotics applications. 

---
