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
  char identifier; // Command identifier for the value being sent
  int value;       // The new value for that command
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
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("end of setup func");
  Serial.println("Peer Added?");
}

void loop() {

    joystick1();
    joystick2();
    joystick3();

    // Check if the button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
      char resetMsg[] = "RESET";
      esp_now_send(receiverMAC, (uint8_t *) resetMsg, sizeof(resetMsg));
  }
}


void joystick1() {
  int potValueX = analogRead(34);
  int potValueY = analogRead(35);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 50 || mappedValueX <= -50) {
      myData.value = mappedValueX;
      myData.identifier = 'L'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 0;
      myData.identifier = 'L'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueX1 = mappedValueX; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.value = mappedValueY;
      myData.identifier = 'C'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 90; // Default position
      myData.identifier = 'C'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueY1 = mappedValueY; // Update the previous value
  }
}

void joystick2() {
  int potValueX = analogRead(32);
  int potValueY = analogRead(33);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 50 || mappedValueX <= -50) {
      myData.value = mappedValueX;
      myData.identifier = 'R'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 0;
      myData.identifier = 'R'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueX1 = mappedValueX; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.value = mappedValueY;
      myData.identifier = 'D'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 90; // Default position
      myData.identifier = 'D'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueY1 = mappedValueY; // Update the previous value
  }
}

void joystick3() {
  int potValueX = analogRead(36);
  int potValueY = analogRead(39);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 65 || mappedValueX <= 115) {
      myData.value = mappedValueX;
      myData.identifier = 'S'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 0;
      myData.identifier = 'S'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueX1 = mappedValueX; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      myData.value = mappedValueY;
      myData.identifier = 'T'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 90; // Default position
      myData.identifier = 'T'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
    prevMappedValueY1 = mappedValueY; // Update the previous value
  }
}

