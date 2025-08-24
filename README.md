# Mobile-Robot

A mobile robot developed by undergraduate students for autonomous navigation and task execution. This project demonstrates advanced trajectory control and sensor integration for competitive robotics.

## Project Overview

The robot performs a complete autonomous mission sequence:
1. **Line Following** - 2-lap course navigation using 4 photo reflectors
2. **Garage Parking** - Precise 3-step parking maneuver with trajectory control
3. **Goal Search** - Wall-following and obstacle detection using PSD/ultrasonic sensors
4. **Button Press** - Automated manipulation with dual servo-controlled arm system

![Demo Video](video.gif)

## Hardware Components

- **Main Controller**: H8/3069F microcontroller (20MHz)
- **Sensors**:
  - 4x Photo reflectors (P5.0-P5.3) for line detection
  - PSD distance sensor GP2Y0A21 (10-60cm range)
  - Ultrasonic sensor HC-SR04 (forward obstacle detection)
  - 2x Rotary encoders (512 pulse/rev, 4x multiplication)
- **Actuators**:
  - 2x DC motors with 29:1 gear reduction
  - 2x Servo motors (RC1: arm positioning, RC2: magnet control)
- **Communication**: Arduino Uno for sensor processing and H8 interface

## Software Architecture

### H8 Main Controller (`Program/main/`)
- **main.c** - Core control with interrupt-driven state machine
- **Line Following** - Pattern-based differential steering (14 sensor combinations)
- **Trajectory Control** - 3rd-order polynomial path planning with real-time execution
- **PD Control** - Closed-loop position control (Kp=80, Kd=80*RsampTime)
- **Interrupt System** - 50Hz feedback control with 20ms sampling

### Arduino Sensor Processing (`Program/Arduino/`)
- **psd_sonic.ino** - Integrated sensor fusion with H8 communication
- **Sensor Averaging** - 100-sample filtering for noise reduction
- **Servo Control** - Sequential arm and magnet manipulation
- **Digital I/O** - Binary signal transmission to H8 controller

## Key Features

### Advanced Line Following
- 14 distinct sensor pattern responses for robust navigation
- Cross-marker detection for lap counting (4-lap sequence)
- Adaptive motor control with differential steering

### Precision Trajectory Control
- 3rd-order polynomial trajectory generation
- Multi-segment parking sequence: curve → straight → reverse curve
- Real-time encoder feedback with position error correction

### Intelligent Sensor Integration
- PSD threshold detection (15-30cm range for wall following)
- Ultrasonic averaging (100 samples) for reliable distance measurement
- Dual-phase goal approach: wall detection → obstacle avoidance

### Robust Control System
- PD controller with optimized gains for 196.5mm wheelbase
- Encoder-based odometry with overflow handling
- PWM motor control with directional logic and safety limits

## Project Structure

```
Mobile-Robot/
├── CAD/                    # Mechanical design drawings
│   └── cad_final_report/   # Component technical drawings
├── Program/
│   ├── main/              # H8 main controller code
│   │   ├── main.c         # Core control program
│   │   ├── common.h       # Hardware definitions
│   │   └── r3069.h        # H8 register definitions
│   ├── Arduino/           # Arduino sensor processing
│   │   ├── psd_sonic.ino  # Integrated sensor control
│   │   └── Run/main/      # Competition versions
│   └── 参考資料/           # Reference materials and examples
├── Photo/                 # Robot assembly photos
├── Report.pdf            # Technical documentation
└── video.gif            # Demonstration video
```

## Technical Specifications

- **Wheelbase**: 196.5mm (measured)
- **Wheel Radius**: 21.15mm with high-precision encoder feedback
- **Control Frequency**: 50Hz (20ms sampling rate)
- **Encoder Resolution**: 2048 counts/rev (512 × 4 multiplication)
- **Motor Reduction**: 29:1 gear ratio
- **Communication**: 115200 baud Arduino-H8 interface
- **Power**: Autonomous battery operation with motor brake control

## Mission Sequence Implementation

**Phase 1: Line Following (2 laps)**
- 4-sensor pattern recognition with 14 response cases
- Cross-marker detection for lap completion
- Adaptive speed control (35-250 PWM range)

**Phase 2: Garage Parking (3-step trajectory)**
- Step 1: 40×1.57cm curve at 90° (5s duration)
- Step 2: 50cm straight approach (3s duration)  
- Step 3: -40×1.57cm reverse curve at -105° (5s duration)

**Phase 3: Goal Search (sensor-guided navigation)**
- PSD wall detection (threshold: 15-30cm)
- Ultrasonic obstacle avoidance (threshold: 13cm)
- Sequential servo activation: RC1 positioning → RC2 magnet control

**Phase 4: Automated Manipulation**
- RC1 servo: 0-100° arm positioning (10° increments)
- RC2 servo: 0-180° magnet control (1° precision)
- Coordinated dual-servo button press sequence

## Documentation

- [Technical Report](Report.pdf) - Detailed system analysis
- [Demo Video](video.gif) - Competition performance
- Code Documentation - Inline comments in Japanese/English

---
*Developed by undergraduate robotics team | Project Subleader & Software Manager: YANG*
