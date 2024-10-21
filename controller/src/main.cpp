#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>

#define BUTTON_PIN 25

// Variables to track previous state
int prevMappedValueX1 = 0;
int prevMappedValueY1 = 90;
int prevMappedValueX2 = 0;
int prevMappedValueY2 = 90;
int prevMappedValueX3 = 90;
int prevMappedValueY3 = 90;

// MAC address of the receiver ESP32A4:CF:12:77:02:C4
uint8_t receiverMAC[] = {0xA4, 0xCF, 0x12, 0x77, 0x02, 0xC4}; // Replace with actual MAC

// Structure to send data
typedef struct struct_message {
  int motor1Speed;
  int motor2Speed;
  int servo1Angle;
  int servo2Angle;
  int servo180_1Angle;
  int servo180_2Angle;
} struct_message;

// Create a structured message
struct_message myData;

void joystick1();
void joystick2();
void joystick3();

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); // Set device as Station (STA)

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  joystick1();
  joystick2();
  joystick3();

  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Send reset command to the receiver
    char resetMsg[] = "RESET";
    Serial.println("Reset Button Pressed");
    esp_now_send(receiverMAC, (uint8_t *) resetMsg, sizeof(resetMsg));
    delay(1000);  // Debounce delay
  }



  // Send data to receiver ESP32
  esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));
  delay(50);
}

void joystick1() {
  int potValueX = analogRead(34);
  int potValueY = analogRead(35);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 50 || mappedValueX <= -50) {
      myData.motor1Speed = mappedValueX;
      prevMappedValueX1 = mappedValueX;
    } else {
      myData.motor1Speed = 0;
      prevMappedValueX1 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.servo1Angle = mappedValueY;
      prevMappedValueY1 = mappedValueY;
    } else {
      myData.servo1Angle = 90;
      prevMappedValueY1 = mappedValueY;
    }
  }
}

void joystick2() {
  int potValueX = analogRead(32);
  int potValueY = analogRead(33);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  if (mappedValueX != prevMappedValueX2) {
    if (mappedValueX >= 50 || mappedValueX <= -50) {
      myData.motor2Speed = mappedValueX;
      prevMappedValueX2 = mappedValueX;
    } else {
      myData.motor2Speed = 0;
      prevMappedValueX2 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY2) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.servo2Angle = mappedValueY;
      prevMappedValueY2 = mappedValueY;
    } else {
      myData.servo2Angle = 90;
      prevMappedValueY2 = mappedValueY;
    }
  }
}

void joystick3() {
  int potValueX = analogRead(36);
  int potValueY = analogRead(39);

  int mappedValueX = map(potValueX, 0, 4095, 0, 180);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  if (mappedValueX != prevMappedValueX3) {
    if (mappedValueX <= 65 || mappedValueX >= 115) {
      myData.servo180_1Angle = mappedValueX;
      prevMappedValueX3 = mappedValueX;
    } else {
      myData.servo180_1Angle = 90;
      prevMappedValueX3 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY3) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.servo180_2Angle = mappedValueY;
      prevMappedValueY3 = mappedValueY;
    } else {
      myData.servo180_2Angle = 90;
      prevMappedValueY3 = mappedValueY;
    }
  }
}
