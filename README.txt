============================================================
README - Arduino Bluetooth-Controlled Robot (L298N + MPU6050)
============================================================


Project Description
-------------------

This project demonstrates how to build a basic robot controlled via Bluetooth using an HC-05 module, an L298N motor driver, and an MPU6050 accelerometer. 
The robot receives directional commands from a remote MPU6050 controller and moves accordingly: Forward, Backward, Left, Right, or Idle (Stop).

This setup can be extended for gesture-based control, obstacle avoidance, remote telemetry, and many more.


Hardware Components
-------------------

Receiver Side (Robot):
- Chassis
- Arduino Nano
- HC-05 Bluetooth Module
- L298N Motor Driver Module
- 4 DC Motors
- Power Supply (e.g., 7.4V battery)
- Jumper wires / tin
- Breadboard / PCB

Transmitter Side (Remote Controller):
- Body
- Arduino Nano
- MPU6050 Module
- HC-05 Bluetooth Module
- Power source (e.g., 7.4V battery)
- Jumper wires / tin
- Breadboard / PCB

Charger (if using rechargeable power supply (e.g., double 18650 Li-ion 3.7V, (7.4V)):
- 2 TP4056 Module
- 2 Two Battery Slot


Wiring Summary
--------------

Receiver (Bluetooth Robot):

HC-05 Module:
- VCC → 5V (from Nano)
- GND → GND (from Nano)
- TX → Pin 4 (or any available digital pin: 2–9)
- RX → Pin 5 (with voltage divider (optional), or any from 2–9)

L298N Motor Driver:
- IN1 → Pin 3
- IN2 → Pin 6
- IN3 → Pin 9
- IN4 → Pin 10
(Alternate: use any 4 of pins 3, 5, 6, 9, 10, 11 — best: 9 & 11 (since there's no collided timer on these pins))

- OUT1, OUT2 → Left DC Motor
- OUT3, OUT4 → Right DC Motor
- L298N VCC → 12V (from battery via switch) → Nano VIN
- L298N GND → Battery negative → Nano GND

Transmitter (MPU6050 + Bluetooth):

MPU6050:
- VCC → 5V (from Nano)
- GND → GND (from Nano)
- SDA → A4
- SCL → A5

HC-05:
- VCC → 5V (from Nano)
- GND → GND (from Nano)
- TX → Pin 4 (or any available digital pin: 2–9)
- RX → Pin 5 (with voltage divider, or any from 2–9)

Power Supply:
- Battery positive → Switch → Nano VIN
- Battery negative → Switch → Nano GND

Charger:
- TP4056 B+ → Battery Positive
- TP4056 B- → Battery Negative

Arduino Library Dependencies
----------------------------

Install the following libraries via Arduino IDE Library Manager:

1. SoftwareSerial (built-in)
2. Wire (built-in)
3. MPU6050_light by Electronic Cats
   - Install via Library Manager or Internet: Search for "MPU6050 Electronic Cats"


Code Description
----------------

1. **Gesture_car_transmitter_code | (MPU6050 Controller):**
   - Reads accelerometer (X/Y) values.
   - Sends command ('F', 'B', 'L', 'R', 'I') via Bluetooth based on tilt.

2. **Gesture_car_receiver_code | (Motor Driver):**
   - Receives command via Bluetooth.
   - Controls L298N motor driver accordingly.
   - Receive data from Transmitter.
   - Movement directions:
     - 'F' → Forward
     - 'B' → Backward
     - 'L' → Turn Left
     - 'R' → Turn Right
     - 'I' → Idle / Stop


How to Use
----------

1. Upload the **Gesture_car_transmitter_code** to the controller.
2. Upload the **Gesture_car_receiver_code** to the robot.
3. Make sure HC-05 modules paired correctly:
  → Hold the HC-05 button when powering it up until it blinks slowly (to enter HC-05 AT mode)
  → Test it by send "AT" in Serial Monitor, if it responds back by saying "OK", that means it works
  → For Slave HC-05:
      AT+ROLE=0                 // Set role to Slave
      AT+NAME=Slave             // (Optional) Set custom module name
      AT+UART=9600,0,0          // Set baud rate (9600), stop bit, and parity
      AT+ADDR?                  // Returns device MAC address (needed by master)

  → For Master HC-05:
      AT+ROLE=1                 // Set role to Master
      AT+CMODE=0                // Connect to a specific address
      AT+ADDR=<slave_address>   // Replace with slave MAC address (e.g., 98D3:31:F4:5B:1B → Replace ":" with ",")
      AT+INIT                   // Initialize SPP profile (if not already initialized)
      AT+LINK=<slave_address>   // Attempt to connect to slave

4. Power both devices and ensure Bluetooth pairing between HC-05 modules.
5. Tilt the transmitter to send motion commands.
6. Robot responds with real-time movement.


Troubleshooting
---------------

MPU6050 (Electronic Cats Library Variant)
---------------------------------
- MPU6050 returns all zeros:
  → Check SDA and SCL wiring (A4 and A5 on Uno/Nano).
  → Make sure the module is receiving 5V (or 3.3V if required).
  → Confirm the I2C address is correct (default is 0x68).
  → If using multiple I2C devices, ensure no address conflict.

- MPU6050 status not zero (returns 1 or 2):
  → Indicates initialization failure; try delaying and retrying `mpu.begin()` in a loop.
  → Ensure MPU6050 is powered before calling `mpu.begin()`.

- Random or drifting angles:
  → Run `mpu.calcGyroOffsets()` after a few seconds of stability during setup.
  → Avoid movement during gyro calibration.

- "MPU not responding" errors:
  → Check if the I2C lines are too long or exposed to noise.
  → Try adding 4.7kΩ pull-up resistors on SDA and SCL.

L298N Motor Driver
------------------
- No motor movement:
  → Check power supply (motors need separate 6V–12V source).
  → Ensure motor power (VCC) and logic power (5V) are correctly connected.
  → Check all `IN1`–`IN4` wires and ensure correct pinMode setup in code.
  → Make sure `EN A/B` are either connected to 5V (always on) or driven with PWM.

- Motors spin only one direction or behave erratically:
  → Double-check logic signals (IN1–IN4) against the datasheet.
  → Ensure you're not sending contradictory signals (e.g., IN1 & IN2 both HIGH).

- L298N heating up:
  → Check for motor stall or excessive current draw.
  → Ensure proper heat dissipation if drawing >1A continuously.

HC-05 Bluetooth Module
----------------------
- Bluetooth not pairing:
  → Ensure one HC-05 is in **master** mode and the other in **slave** mode.
  → Use AT commands to configure mode if needed.
  → Double-check RX/TX wiring (remember: HC-05 RX is **not 5V-tolerant**).

- How to know when HC-05 is paired:
  → LED blinks slowly (~2 seconds interval) when paired.
  → When unpaired, the LED blinks rapidly (~every 0.5 seconds).

- Arduino cannot send/receive via HC-05:
  → Make sure `SoftwareSerial` pins do not conflict with other functions.
  → Use a **voltage divider** on Arduino TX → HC-05 RX.

- Serial Monitor shows garbage values:
  → Check baud rate. Default HC-05 baud is 9600 unless changed via AT mode.
  → Avoid opening Serial Monitor if SoftwareSerial uses pins 0 and 1.

- Nothing received over Bluetooth:
  → Confirm both modules are on same baud rate and power supply is stable.
  → Try swapping TX/RX lines if unsure of orientation.


**If those procedures do not work, try searching further on the internet. If the problem still persists, there is a possibility that the module may be faulty or damaged.**


General Tips :
- Always share **common ground** between modules.
- Use **short jumper wires** and avoid breadboards for power-intensive connections.
- Power the MPU6050 and HC-05 with clean 5V regulated supply (Arduino 5V pin).
- For stable readings, **wait a few seconds** after startup before reading sensor data.


License
-------

Use and modify freely!
No credit necessary — though a shoutout would be appreciated.


Author
------

GitHub: HalalBrother / ImHalal
============================================================
