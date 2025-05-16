/*
 * ===================================================
 * Arduino Bluetooth-Controlled Robot Receiver
 * ---------------------------------------------------
 * Receives single-character directional commands via 
 * Bluetooth and drives a 2-motor robot using L298N.
 *
 * Components:
 * - Arduino Nano
 * - HC-05 Bluetooth Module
 * - L298N Dual H-Bridge Motor Driver
 *
 * Author: HalalBrother / ImHalal
 * GitHub: https://github.com/your-repo-link
 * License: Your choice
 * ===================================================
 */

#include <SoftwareSerial.h> // Enables software-based serial comms

// --------- HC-05 Bluetooth Module Setup ---------
SoftwareSerial BTSerial(4, 5); // RX = pin 4, TX = pin 5

// --------- L298N Motor Driver Pin Definitions ---------
// Left Motor: IN1 & IN2
// Right Motor: IN3 & IN4
const int IN1 = 3;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

String input = ""; // Buffer to store incoming Bluetooth data

// ===================================================
// SETUP: Runs once when Arduino is powered on
// ===================================================
void setup() {
  Serial.begin(9600);       // Start USB Serial Monitor
  BTSerial.begin(9600);     // Start Bluetooth Serial (must match transmitter)

  // Configure motor driver pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// ===================================================
// LOOP: Continuously checks for Bluetooth input
// ===================================================
void loop() {
  // Check if data is available from HC-05
  while (BTSerial.available()) {
    char c = BTSerial.read(); // Read a single character

    if (c == '\n') {
      // Full command received (ended with newline)

      input.trim();           // Remove extra spaces and newlines
      input.toUpperCase();    // Ensure case-insensitive matching

      // Debug print to Serial Monitor
      Serial.print("Received: ");
      Serial.println(input);

      // --------- Motor Control Logic ---------

      if (input == "B") {
        // Move Forward
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }
      else if (input == "F") {
        // Move Backward
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }
      else if (input == "R") {
        // Turn Left
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }
      else if (input == "L") {
        // Turn Right
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }
      else if (input == "I") {
        // Idle / Stop All Motors
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }

      input = ""; // Clear buffer after command is processed
    }
    else {
      input += c; // Append character to buffer
    }
  }
}
