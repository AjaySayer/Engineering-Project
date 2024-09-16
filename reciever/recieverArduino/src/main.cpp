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

// Serial buffer size
const int BUFFER_SIZE = 4;

void controlMotor(int pin1, int pin2, int enablePin, int speed);
void receiveData();
void processMessage(char identifier, int value);

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
  receiveData();  // Process incoming data
}

#define DEBUG 0  // Set to 1 to enable debug, 0 to disable

void receiveData() {
  static byte buffer[BUFFER_SIZE];
  static int index = 0;
  static bool receiving = false;

  while (Serial.available()) {
    byte incoming = Serial.read();

    #if DEBUG
    Serial.print("Incoming byte: ");
    Serial.println(incoming, HEX);
    #endif

    if (incoming == 0xFF) {
      receiving = true;  // Start receiving
      index = 0;
      #if DEBUG
      Serial.println("Start Byte (0xFF) detected");
      #endif
    } else if (incoming == 0xFE && receiving) {
      #if DEBUG
      Serial.println("End Byte (0xFE) detected");
      #endif

      // End byte received, process buffer
      if (index == BUFFER_SIZE) {
        char identifier = buffer[0];
        int value = (buffer[1] << 8) | buffer[2];
        byte checksum = (identifier + buffer[1] + buffer[2]) & 0xFF;

        if (checksum == buffer[3]) {
          #if DEBUG
          Serial.println("Checksum verified. Processing message...");
          #endif
          processMessage(identifier, value);
        } else {
          #if DEBUG
          Serial.println("Checksum error: message discarded");
          #endif
        }
      }
      receiving = false;  // Reset receiving flag
    } else if (receiving && index < BUFFER_SIZE) {
      buffer[index++] = incoming;  // Store byte in buffer
      #if DEBUG
      Serial.print("Buffering byte: ");
      Serial.println(incoming, HEX);
      #endif
    }
  }
}



// Function to handle messages from the buffer
void processMessage(char identifier, int value) {
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

// Function to control L298P motors
void controlMotor(int pin1, int pin2, int enablePin, int speed) {
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
  }

  // Apply PWM to the enable pin to control motor speed
  analogWrite(enablePin, speed);
}
