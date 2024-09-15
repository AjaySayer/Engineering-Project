#include <WiFi.h>
#include <PubSubClient.h>
#include "sensitiveInformation.h"  // Your credentials (ssid, password, mqttServer, etc.)

// MQTT Topics
const char* topicLeftMotor = "esp32/motor1/speed";
const char* topicRightMotor = "esp32/motor2/speed";
const char* topicServo1 = "esp32/servo1/speed";
const char* topicServo2 = "esp32/servo2/speed";
const char* topicServo180_1 = "esp32/servo180_1/angle";
const char* topicServo180_2 = "esp32/servo180_2/angle";

// Wi-Fi and MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length);
void sendToArduino(char identifier, int value);

void setup_wifi();
void reconnect();

void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 ready...");

  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// MQTT callback function
void callback(char* topic, byte* message, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, message, length);
  msg[length] = '\0';

  int value = atoi(msg);

  if (strcmp(topic, topicLeftMotor) == 0) {
    sendToArduino('L', value);
  } else if (strcmp(topic, topicRightMotor) == 0) {
    sendToArduino('R', value);
  } else if (strcmp(topic, topicServo1) == 0) {
    sendToArduino('C', value);
  } else if (strcmp(topic, topicServo2) == 0) {
    sendToArduino('D', value);
  } else if (strcmp(topic, topicServo180_1) == 0) {
    sendToArduino('S', value);
  } else if (strcmp(topic, topicServo180_2) == 0) {
    sendToArduino('T', value);
  }
}

// Send binary data over serial
void sendToArduino(char identifier, int value) {
  Serial.write(identifier);                // Send 1st byte (identifier)
  Serial.write((value >> 8) & 0xFF);       // Send 2nd byte (high byte of value)
  Serial.write(value & 0xFF);              // Send 3rd byte (low byte of value)
}

// Wi-Fi setup function
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");
}

// Reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    if (client.connect(mqttClient)) {
      client.subscribe(topicLeftMotor);
      client.subscribe(topicRightMotor);
      client.subscribe(topicServo1);
      client.subscribe(topicServo2);
      client.subscribe(topicServo180_1);
      client.subscribe(topicServo180_2);
    } else {
      delay(5000);
    }
  }
}
