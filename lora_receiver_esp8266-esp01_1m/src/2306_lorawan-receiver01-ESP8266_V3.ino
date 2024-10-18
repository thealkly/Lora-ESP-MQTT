// Version 3.0 - Code updates:
// - Added separate MQTT topic for ADC readings
// - Improved parsing and handling of incoming data from LoRa

#include <SoftwareSerial.h>
#include "arduino_secrets.h"
#define LED 2  // Avoiding pin conflict by using pin 2 for LED

String incomingString;
SoftwareSerial lora(12, 14); // LoRa serial communication on pins 12 (TX) and 14 (RX)
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define PIN1 14  // NodeMCU pin D5
#define PIN2 12  // NodeMCU pin D6

const char *WIFI_SSID = SECRET_SSID;
const char *WIFI_PASSWORD = SECRET_PWD;

const char *MQTT_HOST = "192.168.1.13";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266-lorawan01-NodeMCU";
const char *MQTT_USER = SECRET_MQTUSER;
const char *MQTT_PASSWORD = SECRET_MQTPW;
const char *TOPIC_ADC = "lorawan/garden/sensorA0/adc";
const char *TOPIC_VOLTAGE = "lorawan/garden/sensorA0/voltage";
const char *TOPIC_CURRENT = "lorawan/garden/sensorA0/current";

WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println("Start boot..");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  while (!client.connected()) {
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT broker");
      // Subscribe to topics if needed here
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("Connected and ready to receive data.");
  
  lora.begin(115200);
  lora.setTimeout(500);
  Serial.println("Boot done");
}

void loop() {
  mqttClient.loop();
  if (lora.available()) {
    incomingString = lora.readString();
    Serial.println("Incoming string:");
    Serial.println(incomingString);

    char dataArray[50];  // Increased buffer size to accommodate more data
    incomingString.toCharArray(dataArray, 50);
    char* adcValue = strtok(dataArray, ",");
    char* voltage = strtok(NULL, ",");
    char* current = strtok(NULL, ",");

    if (adcValue != NULL && voltage != NULL && current != NULL) {
      Serial.println("ADC Value:");
      Serial.println(adcValue);
      Serial.println("Voltage:");
      Serial.println(voltage);
      Serial.println("Current:");
      Serial.println(current);

      mqttClient.publish(TOPIC_ADC, adcValue);
      mqttClient.publish(TOPIC_VOLTAGE, voltage);
      mqttClient.publish(TOPIC_CURRENT, current);
    } else {
      Serial.println("No valid data received");
    }
  }
  delay(500);
}
