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

  int potValue = analogRead(21);
  int mappedValue = map(potValue, 0, 4095, -90, 90);

  char msg[50];
  snprintf(msg, 50, "%d", mappedValue);
  client.publish(topicServo1, msg);

  Serial.print("Potentiometer value: ");
  Serial.print(potValue);
  Serial.print(" Mapped value: ");
  Serial.println(mappedValue);

  delay(1000);
}
