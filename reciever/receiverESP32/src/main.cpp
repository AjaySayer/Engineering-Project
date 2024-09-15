#include <BleGamepad.h>
#include <Arduino.h>

// Initialize BLE gamepad
BleGamepad bleGamepad;
HardwareSerial SerialComm(1);  // UART1 for TX/RX communication

// Motor and servo variables
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;
int continuousServo1 = 90;
int continuousServo2 = 90;
int servo180_1 = 90;
int servo180_2 = 90;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);  // For debugging
  
  // Initialize UART for communication with Arduino
  SerialComm.begin(9600, SERIAL_8N1, 16, 17);  // UART1, TX on GPIO 17, RX on GPIO 16

  // Start BLE Gamepad
  bleGamepad.begin();

  Serial.println("ESP32 is ready to connect to Xbox Controller...");
}

void loop() {
  // Check if the gamepad is connected
  if (bleGamepad.isConnected()) {
    // Get joystick input (range is -32768 to 32767)
    int leftStickY = bleGamepad.getLeftYAxis();
    int rightStickY = bleGamepad.getRightYAxis();
    // Map joystick values to motor speed (-255 to 255)
    leftMotorSpeed = map(leftStickY, -32768, 32767, -255, 255);
    rightMotorSpeed = map(rightStickY, -32768, 32767, -255, 255);

    // Get button states (can map to servos)
    bool buttonA = bleGamepad.isButtonPressed(BUTTON_A);  // Xbox A button
    bool buttonB = bleGamepad.isButtonPressed(BUTTON_B);  // Xbox B button
    int dpadX = bleGamepad.getDpadX();                    // D-pad X-axis

    // Map buttons/D-Pad to servos
    continuousServo1 = buttonA ? 180 : 90;
    continuousServo2 = buttonB ? 180 : 90;
    servo180_1 = map(dpadX, -32768, 32767, 0, 180);
    servo180_2 = map(dpadX, -32768, 32767, 0, 180);

    // Debug joystick values
    Serial.print("Left Motor Speed: ");
    Serial.println(leftMotorSpeed);
    Serial.print("Right Motor Speed: ");
    Serial.println(rightMotorSpeed);
    Serial.print("Continuous Servo 1: ");
    Serial.println(continuousServo1);
    Serial.print("Continuous Servo 2: ");
    Serial.println(continuousServo2);
    Serial.print("180-degree Servo 1: ");
    Serial.println(servo180_1);
    Serial.print("180-degree Servo 2: ");
    Serial.println(servo180_2);

    // Send all data over UART to Arduino
    SerialComm.print("L");
    SerialComm.print(leftMotorSpeed);
    SerialComm.print("R");
    SerialComm.print(rightMotorSpeed);
    SerialComm.print("C1");
    SerialComm.print(continuousServo1);
    SerialComm.print("C2");
    SerialComm.print(continuousServo2);
    SerialComm.print("S1");
    SerialComm.print(servo180_1);
    SerialComm.print("S2");
    SerialComm.print(servo180_2);
    SerialComm.println();  // End of line for Arduino

    delay(50);  // Avoid flooding the serial line
  } else {
    Serial.println("Waiting for Xbox controller connection...");
    delay(1000);
  }
}
