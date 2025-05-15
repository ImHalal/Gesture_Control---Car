#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

//MPU6050
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data;

// HC-05
SoftwareSerial BTSerial(4, 5);

void setup() {
  Serial.begin(9600);

  //HC-05
  BTSerial.begin(9600); // HC-05 default baud

  //MPU6050
  Wire.begin();
  mpu.initialize();
}

void loop() {
  //MPU6050 Data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255);
  data.Y = map(ay, -17000, 17000, 0, 255);
  data.Z = map(az, -17000, 17000, 0, 255);

  //Optional: print to USB Serial
  Serial.print("X = ");
  Serial.print(data.X);
  Serial.print(" Y = ");
  Serial.print(data.Y);
  Serial.print(" Z = ");
  Serial.println(data.Z);

  //Send to Bluetooth

  //MPU6050 Data
  //BTSerial.print(data.X);
  //BTSerial.print(",");
  //BTSerial.print(data.Y);
  //BTSerial.print(",");
  //BTSerial.println(data.Z);

// L298N Data & Logic

char command = 'I'; // Default to Idle

// Determine direction based on Y axis
if (data.Y < 87) {
  command = 'F'; // Forward
} else if (data.Y > 187) {
  command = 'B'; // Backward
}
// Determine direction based on X axis only if Y is neutral
else if (data.X > 195) {
  command = 'L'; // Left
} else if (data.X < 95) {
  command = 'R'; // Right
}

// Send single command
BTSerial.println(command);

delay(100);

}
