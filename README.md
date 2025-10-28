# 🌞Smart Tracker: Catching Light with PID Power

Hey there! Welcome to the GitHub repo for my **Dual-Axis Light Tracker** project.  
This is where an **Arduino** gets smart — using four sensors and two servos to constantly face the brightest light.  
Think of it as a mini, self-adjusting sunflower! 🌻  

This isn't your average light-following robot; the **horizontal movement** is controlled by a **Proportional-Integral-Derivative (PID)** Control Loop.  
That’s the fancy bit that makes sure the tracker finds the light super-fast, locks on without wobbling, and ignores the small jitters that would throw off a simpler system.

---

##  What We're Trying to Achieve

- **True Dual-Axis:** Follow the light in all directions — left/right (pan) and up/down (tilt).  
- **Smooth Sailing:** Use PID logic to keep the tracker steady and accurate when it finds the sweet spot.  
- **Smart Sensing:** Use four LDRs in a smart cross-pattern to generate a precise error signal that tells the Arduino exactly where to move.

---

##  Parts List (What You'll Need)

| Component | Description |
|------------|--------------|
| **Arduino Uno** | The brain of the operation (or any compatible microcontroller). |
| **Hobby Servos (9g)** | Two servos — one for pan, one for tilt. |
| **LDRs (Light Sensors)** | Four sensors mounted in a quadrant. |
| **Resistors** | Four 10kΩ pull-down resistors for the LDR inputs. |
| **Breadboard & Wires** | For prototyping and connections. |

Important Build Tip:**  
> Mount your four LDRs with small dividers between them!  
> These create the shadows that help generate the "error" signal for accurate tracking.

---

The Code Magic — PID Explained

The magic happens with the **PID loop** on the horizontal axis.  
Instead of just guessing, it calculates *exactly* how much to move based on three factors:

- **Proportional (Kp):** How wrong we are right now.  
- **Integral (Ki):** How wrong we've been over time (helps overcome friction).  
- **Derivative (Kd):** How quickly the error is changing (helps prevent overshooting).  

**PID Formula:**

```math
Movement = (K_p × Current Error) + (K_i × Accumulated Error) + (K_d × Rate of Error Change)
 Tuning the System (The Fun Challenge!)
Parameter	Description	Suggested Value
Kp (Proportional)	Main control strength. Too high → oscillations.	0.5
Ki (Integral)	Helps overcome steady-state error. Too high → overshoot.	0.001
Kd (Derivative)	Damps oscillations. Too high → too sensitive.	5.0

Includes Integral Anti-Windup using:
integral_H = constrain(...)
to keep the system stable when servos hit their max/min positions (0° or 180°).

 Ready to Run?
Upload the code to your Arduino.

Open the Serial Monitor to view:

H_Error → Horizontal error

PID_Out → Servo correction signal

Tune your Kp, Ki, Kd values for the smoothest performance.

🔭 Future Scope
1. Dedicated PCB Design
Goal: Transition from breadboard to a custom Printed Circuit Board (PCB).
Benefits: Compact, robust, and ready for outdoor/long-term use.

PCB Features:

Dedicated headers for VCC/GND and all I/O pins.

Integrated pull-down resistors for LDR inputs.

Proper power regulation and decoupling capacitors for servos.
