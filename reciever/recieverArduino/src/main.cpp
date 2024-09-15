#include <Arduino.h>


// Define motor control pins for motor A
const int motorPin1 = 12;   // Direction IN1
const int motorPin2 = 13;   // Direction IN2
const int enablePin = 3;    // PWM speed control for Motor A

void setup() {
  // Set the motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("L298P Motor A Test Starting...");
}

void loop() {
  // Set motor A to run forward at full speed
  Serial.println("Running motor A forward...");
  digitalWrite(motorPin1, HIGH);
  Serial.println("motorPin1 set to HIGH (Forward)");
  digitalWrite(motorPin2, LOW);
  Serial.println("motorPin2 set to LOW");
  analogWrite(enablePin, 255);  // Full speed (PWM 255)
  Serial.println("PWM set to 255 (Full speed)");

  delay(3000);  // Run for 3 seconds

  // // Stop the motor
  // Serial.println("Stopping motor A...");
  // analogWrite(enablePin, 0);  // Stop motor
  // Serial.println("PWM set to 0 (Motor stopped)");
  // delay(2000);

  // // Set motor A to run backward at full speed
  // Serial.println("Running motor A backward...");
  // digitalWrite(motorPin1, LOW);
  // Serial.println("motorPin1 set to LOW");
  // digitalWrite(motorPin2, HIGH);
  // Serial.println("motorPin2 set to HIGH (Backward)");
  // analogWrite(enablePin, 255);  // Full speed
  // Serial.println("PWM set to 255 (Full speed)");

  // delay(3000);  // Run for 3 seconds

  // // Stop the motor
  // Serial.println("Stopping motor A...");
  // analogWrite(enablePin, 0);  // Stop motor
  // Serial.println("PWM set to 0 (Motor stopped)");
  // delay(2000);
}
