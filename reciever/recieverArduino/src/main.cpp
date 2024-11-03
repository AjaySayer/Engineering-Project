#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Motor control pins
const int motorPin1A = A0;
const int motorPin2A = A1;
//const int enablePinA = 16;
const int motorPin1B = 19;
const int motorPin2B = 18;
//const int enablePinB = 5;


// Servo objects and pins
Servo continuousServo1, continuousServo2, servo180_1, servo180_2;
const int continuousServo1Pin = 15, continuousServo2Pin = 32, servo180_1Pin = 14, servo180_2Pin = 4;


// Structure to receive data
typedef struct struct_message {
  char identifier;  // Command identifier: L, R, C, D, S, T, X
  int value;        // Command value
} struct_message;

struct_message incomingData;

// Function declarations
void controlMotor(int pin1, int pin2, int speed);
void processMessage(char identifier, int value);
void set_motor_pwm(int IN1_PIN, int IN2_PIN, int pwm);

// Callback function when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {
  // Ensure the data length matches the size of our struct
  // Serial.println("Recieved Message");
  if (len == sizeof(struct_message)) {
    // Cast the incoming data to our struct
    struct_message incomingData;
    memcpy(&incomingData, data, sizeof(incomingData));

    // Process the message
    processMessage(incomingData.identifier, incomingData.value);
  } else {
    Serial.println("Received data does not match expected size");
  }
}

void setup() {
  // Initialize motor and servo pins
  pinMode(motorPin1A, OUTPUT);
  pinMode(motorPin2A, OUTPUT);
  //pinMode(enablePinA, OUTPUT);
  pinMode(motorPin1B, OUTPUT);
  pinMode(motorPin2B, OUTPUT);
  //pinMode(enablePinB, OUTPUT);

  continuousServo1.attach(continuousServo1Pin);
  continuousServo2.attach(continuousServo2Pin);
  servo180_1.attach(servo180_1Pin);
  servo180_2.attach(servo180_2Pin);

  // Initialize serial for debugging
  Serial.begin(9600);

  // Set up ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.print("INIT");

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing needed in the loop; everything happens via ESP-NOW callback
}

// Function to handle the messages
void processMessage(char identifier, int value) {
  switch (identifier) {
    case 'L':  // Left motor
      set_motor_pwm(motorPin1A, motorPin2A, value);
      Serial.print("Left motor speed: ");
      Serial.println(value);
      break;

    case 'R':  // Right motor
      controlMotor(motorPin1B, motorPin2B, value);
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

    case 'X': //Button
      Serial.print("Reset Button Pressed");
      ESP.restart();
      break;

    default:
      Serial.println("Unknown command");
      break;
  }
}

// Function to control DRV8833 motors
void controlMotor(int pin1, int pin2, int speed) {
  if (speed > 0) {
    // Move forward
    analogWrite(pin1, speed); // PWM on pin1 for speed control
    digitalWrite(pin2, LOW);  // Set pin2 low
  } 
  else if (speed < 0) {
    // Move backward
    digitalWrite(pin1, LOW);  // Set pin1 low
    analogWrite(pin2, speed); // PWM on pin2 for speed control
  } 
  else {
    // Stop the motor
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

void set_motor_pwm(int IN1_PIN, int IN2_PIN, int pwm)
{
  if (pwm < 0) {  // reverse speeds
    // Serial.println(-pwm);
    digitalWrite(IN2_PIN, LOW);
    digitalWrite(IN1_PIN, HIGH);
    //analogWrite(IN1_PIN, -pwm);

  } else if (pwm > 0) { // stop or forward
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    //analogWrite(IN2_PIN, pwm);
  } else {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
  }
}