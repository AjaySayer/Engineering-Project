#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials and MQTT server details
const char* ssid = "LAPTOP-APS";
const char* password = "7F03#x41";
const char* mqttServer = "192.168.137.1";
const int mqttPort = 1883;  // Default MQTT port

// Button pins
const int buttonPin1 = 34;
const int buttonPin2 = 39;
const int buttonPin3 = 36;

// Debounce timing
volatile unsigned long lastDebounceTime1 = 0;
volatile unsigned long lastDebounceTime2 = 0;
volatile unsigned long lastDebounceTime3 = 0;
const unsigned long debounceDelay = 200;  // Adjust this delay to suit your needs

// WiFi and MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

// Variables to track button states
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool button3Pressed = false;

// Interrupt service routines (ISRs) for the buttons
void IRAM_ATTR handleButton1() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime1 > debounceDelay) {
    button1Pressed = true;  // Set flag for button 1 press
    lastDebounceTime1 = currentTime;
  }
}

void IRAM_ATTR handleButton2() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime2 > debounceDelay) {
    button2Pressed = true;  // Set flag for button 2 press
    lastDebounceTime2 = currentTime;
  }
}

void IRAM_ATTR handleButton3() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime3 > debounceDelay) {
    button3Pressed = true;  // Set flag for button 3 press
    lastDebounceTime3 = currentTime;
  }
}

// Function to connect to WiFi
void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}

// Function to connect to MQTT server
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    // Attempt to connect
    if (client.connect("ESP32_Controller")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Set up button pins as inputs with pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  // Attach interrupts to buttons
  attachInterrupt(buttonPin1, handleButton1, FALLING);  // Trigger on button press
  attachInterrupt(buttonPin2, handleButton2, FALLING);
  attachInterrupt(buttonPin3, handleButton3, FALLING);

  // Connect to WiFi
  setupWifi();

  // Set up MQTT server
  client.setServer(mqttServer, mqttPort);
  Serial.println("Initialised");
}

void loop() {
  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Handle button 1 press
  if (button1Pressed) {
    client.publish("esp32/control/led1", "1");
    Serial.println("Button 1 pressed");
    button1Pressed = false;  // Reset the flag
  }

  // Handle button 2 press
  if (button2Pressed) {
    client.publish("esp32/control/led2", "1");
    Serial.println("Button 2 pressed");
    button2Pressed = false;  // Reset the flag
  }

  // Handle button 3 press
  if (button3Pressed) {
    client.publish("esp32/control/led3", "1");
    Serial.println("Button 3 pressed");
    button3Pressed = false;  // Reset the flag
  }
}
