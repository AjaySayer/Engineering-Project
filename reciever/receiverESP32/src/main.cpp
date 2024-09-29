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

const char* confirmTopic = "esp32/confirm";
// Wi-Fi and MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length);
void sendToArduino(char identifier, int value);

void setup_wifi();
void reconnect();

// HardwareSerial Serial1(1);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 20, 21);
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

int previousMessageL;
int previousMessageR;
int previousMessageC;
int previousMessageD;
int previousMessageS;
int previousMessageT;

// MQTT callback function
void callback(char* topic, byte* message, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, message, length);
  msg[length] = '\0';
  int value = atoi(msg);

  // if (value == 0) {
  //   zeros++;
  //   //Serial.println(zeros);
  //   if (zeros > 20) {
  //     if (strcmp(topic, topicLeftMotor) == 0) {
  //       sendToArduino('L', value);
  //     } else if (strcmp(topic, topicRightMotor) == 0) {
  //       sendToArduino('R', value);
  //     } else if (strcmp(topic, topicServo1) == 0) {
  //       sendToArduino('C', value);
  //     } else if (strcmp(topic, topicServo2) == 0) {
  //       sendToArduino('D', value);
  //     } else if (strcmp(topic, topicServo180_1) == 0) {
  //       sendToArduino('S', value);
  //     } else if (strcmp(topic, topicServo180_2) == 0) {
  //       sendToArduino('T', value);
  //     }
  //     zeros = 0;
  //   }
  //   return;
  // } else {

  

    if (strcmp(topic, topicLeftMotor) == 0) {
      if (value == previousMessageL) {
      return;
      } else {
        sendToArduino('L', value);
      }
      previousMessageL = value;
    } else if (strcmp(topic, topicRightMotor) == 0) {
      if (value == previousMessageR) {
        return;
      } else {
      sendToArduino('R', value);
      }
      previousMessageR = value;
    } else if (strcmp(topic, topicServo1) == 0) {
      if (value == previousMessageC) {
        return;
      } else {
      sendToArduino('C', value);
      }
      previousMessageC = value;
      
    } else if (strcmp(topic, topicServo2) == 0) {
      if (value == previousMessageD) {
        return;
      } else {
      sendToArduino('D', value);
      }
      previousMessageD = value;
      
    } else if (strcmp(topic, topicServo180_1) == 0) {
      if (value == previousMessageS) {
        return;
      } else {
      sendToArduino('S', value);
      }
      previousMessageS = value;
    
    } else if (strcmp(topic, topicServo180_2) == 0) {
      if (value == previousMessageT) {
        return;
      } else {
      sendToArduino('T', value);
      }
      previousMessageT = value;
    }
    client.publish(confirmTopic, "Callback confirm");
  // }
}

// Send binary data over serial
void sendToArduino(char identifier, int value) {
  byte sign = (value < 0) ? 1 : 0;  // Sign byte: 1 for negative, 0 for positive
  value = abs(value);  // Use absolute value for transmission
  byte checksum = (identifier + sign + (value >> 8) + (value & 0xFF)) & 0xFF;
  
  // Serial1.write(0xFF);                   // Start byte
  // Serial1.write(identifier);             // Send identifier
  // Serial1.write(sign);                   // Send sign byte
  // Serial1.write((value >> 8) & 0xFF);    // High byte
  // Serial1.write(value & 0xFF);           // Low byte
  // Serial1.write(checksum);               // Checksum
  // Serial1.write(0xFE);                   // End byte


  Serial.write(0xFF);                   // Start byte
  Serial.write(identifier);             // Send identifier
  Serial.write(sign);                   // Send sign byte
  Serial.write((value >> 8) & 0xFF);    // High byte
  Serial.write(value & 0xFF);           // Low byte
  Serial.write(checksum);               // Checksum
  Serial.write(0xFE);                   // End byte

  client.publish(confirmTopic, "sentToArduino");
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
    Serial.println("Connecting...");
    if (client.connect(mqttClient)) {
      client.subscribe(topicLeftMotor);
      client.subscribe(topicRightMotor);
      client.subscribe(topicServo1);
      client.subscribe(topicServo2);
      client.subscribe(topicServo180_1);
      client.subscribe(topicServo180_2);
      Serial.println("Connected");
    } else {
      delay(5000);
    }
  }
}
