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

void setup_wifi();
void reconnect();
void joystick1();

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

  // int potValue = analogRead(34);
  // int mappedValue = map(potValue, 0, 4095, -90, 90);

  // if ((mappedValue > 24) || (mappedValue < -24)) {
  //   mappedValue = 0;
  // } 
  // char msg[50];
  // snprintf(msg, 50, "%d", mappedValue);
  // client.publish(topicServo1, msg);

  // Serial.print("Potentiometer value: ");
  // Serial.print(potValue);
  // Serial.print(" Mapped value: ");
  // Serial.println(mappedValue);

  // delay(25);
}

void joystick1() {

  int potValueX = analogRead(34);
  int potValueY = analogRead(35);

  int mappedValueX = map(potValueX, 0, 4095, -90, 90);
  int mappedValueY = map(potValueY, 0, 4095, -90, 90);
  // Serial.println(potValueX);
  // Serial.println(potValueY);

  if ((mappedValueX < 24) && (mappedValueX > -24)) {
    mappedValueX = 0;
  }
  char msg[50];
  snprintf(msg, 50, "%d", mappedValueX);
  client.publish(topicServo1, msg);

  if ((mappedValueY < 24) && (mappedValueY > -24)) {
    mappedValueY = 0;
  } 
  //char msg[50];
  snprintf(msg, 50, "%d", mappedValueY);
  client.publish(topicServo2, msg);


}