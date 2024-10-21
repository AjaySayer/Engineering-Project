#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "sensitiveInformation.h"

// Initialize WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT Topics
const char* topicLeftMotor = "esp32/motor1/speed";
const char* topicRightMotor = "esp32/motor2/speed";
const char* topicServo1 = "esp32/servo1/speed";
const char* topicServo2 = "esp32/servo2/speed";
const char* topicServo180_1 = "esp32/servo180_1/angle";
const char* topicServo180_2 = "esp32/servo180_2/angle";

// Variables to track the previous state
int prevMappedValueX1 = 0;
int prevMappedValueY1 = 90;
int prevMappedValueX2 = 0;
int prevMappedValueY2 = 90;
int prevMappedValueX3 = 90;
int prevMappedValueY3 = 90;

void setup_wifi();
void reconnect();
void joystick1();
void joystick2();
void joystick3();

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  joystick1();
  joystick2();
  joystick3();

  delay(50);
}

void joystick1() {
  int potValueX = analogRead(34);
  int potValueY = analogRead(35);

  int mappedValueX = map(potValueX, 0, 4095, -255, 255);
  int mappedValueY = map(potValueY, 0, 4095, 0, 180);

  if (mappedValueX != prevMappedValueX1) {
    if (mappedValueX >= 50 || mappedValueX <= -50 ) {
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicLeftMotor, msg);
      prevMappedValueX1 = mappedValueX;
    } else {
      char msg[50];
      mappedValueX = 0;
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicLeftMotor, msg);
      prevMappedValueX1 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY1) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo1, msg);
      prevMappedValueY1 = mappedValueY;
    } else {
      char msg[50];
      mappedValueY = 90;
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo1, msg);
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
    if (mappedValueX >= 50 || mappedValueX <= -50 ) {
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicRightMotor, msg);
      prevMappedValueX2 = mappedValueX;
    } else {
      char msg[50];
      mappedValueX = 0;
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicRightMotor, msg);
      prevMappedValueX2 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY2) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo2, msg);
      prevMappedValueY2 = mappedValueY;
    } else {
      char msg[50];
      mappedValueY = 90;
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo2, msg);
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
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicServo180_1, msg);
      prevMappedValueX3 = mappedValueX;
    } else {
      char msg[50];
      mappedValueX = 90;
      snprintf(msg, 50, "%d", mappedValueX);
      client.publish(topicServo180_1, msg);
      prevMappedValueX3 = mappedValueX;
    }
  }
  if (mappedValueY != prevMappedValueY3) {
    if (mappedValueY <= 65 || mappedValueY >= 115) {
      char msg[50];
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo180_2, msg);
      prevMappedValueY3 = mappedValueY;
    } else {
      char msg[50];
      mappedValueY = 90;
      snprintf(msg, 50, "%d", mappedValueY);
      client.publish(topicServo180_2, msg);
      prevMappedValueY3 = mappedValueY;
    }
  }
}
