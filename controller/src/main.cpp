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

int prevMessageSentX1 = 0;
int prevMessageSentY1 = 90;
int prevMessageSentX2 = 0;
int prevMessageSentY2 = 90;
int prevMessageSentX3 = 90;
int prevMessageSentY3 = 90;

int prevMessageSentButton = 0;

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
      myData.value = 1;
      myData.identifier = 'X';
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));
    } else {
      myData.value = 0;
      if (myData.value != prevMessageSentButton) {
        myData.identifier = 'X'; // Identifier for button
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    }
  }
  prevMessageSentButton = myData.value; // Update the previous value
}


void joystick1() {
  int potValueX = analogRead(34);
  int potValueY = analogRead(35);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 240 || mappedValueX <= -240) {
      myData.value = mappedValueX;
      myData.identifier = 'L'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 0;
      if(myData.value != prevMessageSentX1) {
        myData.identifier = 'L'; // Identifier for left motor
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentX1 = myData.value;
    prevMappedValueX1 = myData.value; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 10 || mappedValueY >= 170) {
      myData.value = mappedValueY;
      myData.identifier = 'C'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 93; // Default position
      if (myData.value != prevMessageSentY1) {
        myData.identifier = 'C'; // Identifier for servo
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentY1 = myData.value;
    prevMappedValueY1 = mappedValueY; // Update the previous value
  }


  
}

void joystick2() {
  int potValueX = analogRead(32);
  int potValueY = analogRead(33);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX2) {
    if (mappedValueX >= 240 || mappedValueX <= -240) {
      myData.value = mappedValueX;
      myData.identifier = 'R'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 0;
      if (myData.value != prevMessageSentX2) {
        myData.identifier = 'R'; // Identifier for left motor
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentX2 = myData.value;
    prevMappedValueX2 = mappedValueX; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY2) {
    if (mappedValueY <= 10 || mappedValueY >= 170) {
      myData.value = mappedValueY;
      myData.identifier = 'D'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 93; // Default position
      if (myData.value!= prevMessageSentY2) {
        myData.identifier = 'D'; // Identifier for servo
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentY2 = myData.value;
    prevMappedValueY2 = mappedValueY; // Update the previous value
  }
}

void joystick3() {
  int potValueX = analogRead(36);
  int potValueY = analogRead(39);

  int mappedValueX = map(potValueX, 0, 4095, 0, 180);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  // Send motor speed change
  if (mappedValueX != prevMappedValueX3) {
    if (mappedValueX <= 10 || mappedValueX >= 170) {
      myData.value = mappedValueX;
      myData.identifier = 'S'; // Identifier for left motor
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 90;
      if (myData.value!= prevMessageSentX3) {
        myData.identifier = 'S'; // Identifier for left motor
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentX3 = myData.value;
    prevMappedValueX3 = mappedValueX; // Update the previous value
  }

  // Send servo angle change
  if (mappedValueY != prevMappedValueY3) {
    if (mappedValueY <= 10 || mappedValueY >= 170) {
      myData.value = mappedValueY;
      myData.identifier = 'T'; // Identifier for servo
      esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
    } else {
      myData.value = 90; // Default position
      if (myData.value!= prevMessageSentY3) {
        myData.identifier = 'T'; // Identifier for servo
        esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData)); // Send the update
      }
    }
    prevMessageSentY3 = myData.value;
    prevMappedValueY3 = mappedValueY; // Update the previous value
  }
}

