#include <Servo.h>
#include <Arduino.h>

// Motor control pins
const int motorPin1A = 12;
const int motorPin2A = 9;
const int enablePinA = 3;
const int motorPin1B = 13;
const int motorPin2B = 8;
const int enablePinB = 11;

// Servo objects and pins
Servo continuousServo1, continuousServo2, servo180_1, servo180_2;
const int continuousServo1Pin = 4, continuousServo2Pin = 6, servo180_1Pin = 7, servo180_2Pin = 10;

void controlMotor(int pin1, int pin2, int enablePin, int speed);
void processSerialData();

void setup() {
  pinMode(motorPin1A, OUTPUT);
  pinMode(motorPin2A, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(motorPin1B, OUTPUT);
  pinMode(motorPin2B, OUTPUT);
  pinMode(enablePinB, OUTPUT);

  continuousServo1.attach(continuousServo1Pin);
  continuousServo2.attach(continuousServo2Pin);
  servo180_1.attach(servo180_1Pin);
  servo180_2.attach(servo180_2Pin);

  Serial.begin(9600);  // UART communication
  Serial.println("Arduino ready for binary data...");
}

void loop() {
  processSerialData();
}

// Process incoming data as bytes
void processSerialData() {
  if (Serial.available() >= 3) {  // Ensure we have 3 bytes to read
    char identifier = Serial.read();  // 1st byte: Identifier
    int value = (Serial.read() << 8) | Serial.read();  // 2nd and 3rd bytes: 16-bit integer value

    switch (identifier) {
      case 'L':  // Left motor
        controlMotor(motorPin1A, motorPin2A, enablePinA, value);
        Serial.print("Left motor speed: ");
        Serial.println(value);
        break;
        
      case 'R':  // Right motor
        controlMotor(motorPin1B, motorPin2B, enablePinB, value);
        Serial.print("Right motor speed: ");
        Serial.println(value);
        break;

      case 'C':  // Continuous Servo 1
        continuousServo1.write(value);
        Serial.print("Continuous Servo 1: ");
        Serial.println(value);
        break;

      case 'D':  // Continuous Servo 2
        continuousServo2.write(value);
        Serial.print("Continuous Servo 2: ");
        Serial.println(value);
        break;

      case 'S':  // 180-degree Servo 1
        servo180_1.write(value);
        Serial.print("180-degree Servo 1: ");
        Serial.println(value);
        break;

      case 'T':  // 180-degree Servo 2
        servo180_2.write(value);
        Serial.print("180-degree Servo 2: ");
        Serial.println(value);
        break;

      default:
        Serial.println("Unknown command");
        break;
    }
  }
}

// Function to control L298P motors
void controlMotor(int pin1, int pin2, int enablePin, int speed) {
  // Constrain the speed to the valid PWM range of 0 to 255
  // int pwmValue = constrain(abs(speed), 0, 255);  // Get absolute speed and constrain
  
  if (speed > 0) {
    // Move forward
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } 
  else if (speed < 0) {
    // Move backward
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    speed = -speed;
  } 
  else {
    // Stop the motor (brake)
    digitalWrite(pin2, HIGH);
    // pwmValue = 0;  // No speed
  }

  // Apply PWM to the enable pin to control motor speed
  analogWrite(enablePin, speed);
}