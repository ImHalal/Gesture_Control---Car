/*
 * ================================================
 * Arduino Gesture-Based Bluetooth Transmitter
 * ----------------------------------------
 * Transmits directional commands via Bluetooth
 * based on MPU6050 accelerometer data.
 *
 * Components:
 * - Arduino Nano
 * - MPU6050 Accelerometer (Electronic Cats)
 * - HC-05 Bluetooth Module
 *
 * Author: HalalBrother / ImHalal
 * GitHub: https://github.com/your-repo-link
 * License: Your choice
 * ================================================
 */

#include <Wire.h>              // For I2C communication
#include <I2Cdev.h>            // I2C device helper (required by MPU6050 lib)
#include <MPU6050.h>           // Electronic Cats version of MPU6050 driver
#include <SoftwareSerial.h>    // For serial comm with HC-05

// --------- MPU6050 Setup ---------
MPU6050 mpu;                   // Create MPU6050 object

// Raw sensor values
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Struct for future expansion (if you send raw X/Y/Z via Bluetooth)
struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data; // Create data container

// --------- HC-05 Bluetooth Setup ---------
SoftwareSerial BTSerial(4, 5); // RX = pin 4, TX = pin 5 (connects to HC-05)

// ===================================================
// SETUP: Runs once during startup
// ===================================================
void setup() {
  Serial.begin(9600);         // USB Serial Monitor (for debugging)

  // Start Bluetooth serial (default HC-05 baud rate)
  BTSerial.begin(9600);

  // Initialize I2C for MPU6050
  Wire.begin();
  mpu.initialize();          // Starts MPU6050 IMU
}

// ===================================================
// LOOP: Runs repeatedly after setup()
// ===================================================
void loop() {
  // --- 1. Read raw MPU6050 data (accel & gyro) ---
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // --- 2. Normalize acceleration values to 0–255 scale ---
  //     These mapped values can be sent over Bluetooth or used as control triggers.
  data.X = map(ax, -17000, 17000, 0, 255);
  data.Y = map(ay, -17000, 17000, 0, 255);
  data.Z = map(az, -17000, 17000, 0, 255);

  // --- 3. Optional: Print to Serial Monitor for debugging ---
  Serial.print("X = ");
  Serial.print(data.X);
  Serial.print(" | Y = ");
  Serial.print(data.Y);
  Serial.print(" | Z = ");
  Serial.println(data.Z);

  // --- 4. Decide direction command based on tilt ---
  // Logic:
  // - Y axis determines Forward/Backward
  // - X axis determines Left/Right
  // - If neutral, send Idle ('I')

  char command = 'I'; // Default command = Idle

  // Forward: tilt forward (Y low)
  if (data.Y < 87) {
    command = 'F';
  }
  // Backward: tilt backward (Y high)
  else if (data.Y > 187) {
    command = 'B';
  }
  // If Y is neutral, check for left/right
  else if (data.X > 195) {
    command = 'L'; // Tilted left
  }
  else if (data.X < 95) {
    command = 'R'; // Tilted right
  }

  // --- 5. Send command via Bluetooth ---
  BTSerial.println(command);

  // --- 6. Delay for stability (reduce jitter) ---
  delay(100); // 100ms delay = ~10Hz command rate
}
