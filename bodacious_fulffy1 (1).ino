#include <Servo.h>

// Servo Objects
Servo servo1; // Horizontal (Yaw) connected to D9 (as per your schematic)
Servo servo2; // Vertical (Pitch) connected to D10 (as per your schematic)

// LDR Pin Definitions (Requires 4 LDRs wired to a cross-shaped divider)
const int LDR_H_LEFT   = A0;  // Horizontal LDR 1 (Left)
const int LDR_H_RIGHT  = A1;  // Horizontal LDR 2 (Right)
const int LDR_V_TOP    = A2;  // Vertical LDR 3 (Top)
const int LDR_V_BOTTOM = A3;  // Vertical LDR 4 (Bottom)

// --- PID Controller Constants (The core of the complexity) ---
// These values are for the Horizontal (Yaw) axis. Tuning is critical.
const float Kp = 0.5;   // Proportional Gain: Affects response to current error.
const float Ki = 0.001; // Integral Gain: Corrects for steady-state error (e.g., drift).
const float Kd = 5.0;   // Derivative Gain: Dampens oscillations and improves settling time.

const int SETPOINT = 0; // The target light difference (LDR1 - LDR2 = 0)

// --- PID Variables (Horizontal Axis) ---
float error_H = 0;
float lastError_H = 0;
float integral_H = 0;
float derivative_H = 0;
float output_H = 0;

// --- System & Control Variables ---
int servoPos1 = 90; // Initial Horizontal position (middle)
int servoPos2 = 90; // Initial Vertical position (middle)
const int MAX_SERVO_STEP = 5; // Limits max PID movement per loop for smoother action
const int V_THRESHOLD = 50;   // Simple threshold for the vertical axis (can also be PID-controlled if needed)
const int V_STEP = 1;         // Movement step for the simpler vertical control

void setup() {
  servo1.attach(9);
  servo2.attach(10);
  Serial.begin(9600);
  
  // Set initial servo positions
  servo1.write(servoPos1);
  servo2.write(servoPos2);
  
  Serial.println("Dual-Axis Light Tracker Initialized (Horizontal: PID, Vertical: Simple)");
}

void loop() {
  // --- 1. HORIZONTAL CONTROL (PID) ---
  int ldrH_Left_Val = analogRead(LDR_H_LEFT);
  int ldrH_Right_Val = analogRead(LDR_H_RIGHT);

  // 1. Calculate the Error (P)
  error_H = (float)ldrH_Left_Val - (float)ldrH_Right_Val;

  // 2. Calculate the Integral (I)
  integral_H += error_H;
  // Anti-windup: Prevents the integral term from growing too large when the servo is maxed out.
  integral_H = constrain(integral_H, -2000.0, 2000.0);

  // 3. Calculate the Derivative (D)
  derivative_H = error_H - lastError_H;

  // 4. Calculate the Final PID Output
  output_H = (Kp * error_H) + (Ki * integral_H) + (Kd * derivative_H);

  // 5. Apply Output to Servo Position
  // Limit the change to prevent sudden, jerky movements
  output_H = constrain(output_H, (float)-MAX_SERVO_STEP, (float)MAX_SERVO_STEP);
  // Subtract/Add output: A positive error (Left brighter) means move Right (decrease angle)
  servoPos1 -= (int)round(output_H); 

  // Constrain and Write Horizontal Position
  servoPos1 = constrain(servoPos1, 0, 180);
  servo1.write(servoPos1);
  
  // Update last error for next loop
  lastError_H = error_H;


  // --- 2. VERTICAL CONTROL (Simple Differential) ---
  int ldrV_Top_Val = analogRead(LDR_V_TOP);
  int ldrV_Bottom_Val = analogRead(LDR_V_BOTTOM);
  
  // Calculate vertical difference
  int vDiff = ldrV_Top_Val - ldrV_Bottom_Val;

  if (abs(vDiff) > V_THRESHOLD) {
    if (vDiff > 0) { // Top is brighter, move servo up (angle decreases)
      servoPos2 -= V_STEP;
    } else { // Bottom is brighter, move servo down (angle increases)
      servoPos2 += V_STEP;
    }
  }
  
  // Constrain and Write Vertical Position
  servoPos2 = constrain(servoPos2, 0, 180);
  servo2.write(servoPos2);


  // --- Debugging ---
  Serial.print("H_Error:"); Serial.print(error_H);
  Serial.print(" | PID_Out:"); Serial.print(output_H);
  Serial.print(" | Pos1:"); Serial.print(servoPos1);
  Serial.print(" | V_Diff:"); Serial.print(vDiff);
  Serial.print(" | Pos2:"); Serial.println(servoPos2);

  delay(20); // Faster loop for better PID response
}