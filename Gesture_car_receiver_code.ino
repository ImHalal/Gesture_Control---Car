#include <SoftwareSerial.h>

// HC-05 Bluetooth Module
SoftwareSerial BTSerial(4, 5);  // RX, TX

// L298N Motor Driver Pins
const int IN1 = 3;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

String input = "";

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  while (BTSerial.available()) {
    char c = BTSerial.read();
    if (c == '\n') {
      input.trim(); // Remove whitespace and newline
      input.toUpperCase();

      Serial.print("Received: ");
      Serial.println(input);

      // Motor control logic
      if (input == "B") {  // Forward
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      } else if (input == "F") {  // Backward
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      } else if (input == "R") {  // Left
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      } else if (input == "L") {  // Right
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      } else if (input == "I") {  // Idle / Stop
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }

      input = ""; // Reset input
    } else {
      input += c;
    }
  }
}
