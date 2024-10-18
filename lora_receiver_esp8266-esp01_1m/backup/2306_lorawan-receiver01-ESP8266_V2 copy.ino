#include <SoftwareSerial.h>
#include "arduino_secrets.h"
#define LED 2  // Geändert von 13 auf 2, um Konflikte zu vermeiden

//läuift für esp8266 jetzt 24.4.2024
String incomingString;
SoftwareSerial lora(12, 14); // Geändert von 13 auf 14 für RX, TX bleibt auf 12
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define PIN1 14  //D5 NodeMCU
#define PIN2 12  //D6 NodeMCU
#define DELAY 30

const char *WIFI_SSID = SECRET_SSID;
const char *WIFI_PASSWORD = SECRET_PWD;

const char *MQTT_HOST = "192.168.1.13";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266-lorawan01-NodeMCU";
const char *MQTT_USER = SECRET_MQTUSER;
const char *MQTT_PASSWORD = SECRET_MQTPW;
const char *TOPIC = "lorawan/garden/sensorA0";
const char *TOPICSTATUS = "lorawan/garden/sensorA0/status";

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
    } else {
      delay(500);
      Serial.print(".");
    }
  }
  mqttClient.subscribe(TOPIC);
  Serial.println("Connected and subscribed to:");
  Serial.println(TOPIC);
  
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

    char dataArray[30]; 
    incomingString.toCharArray(dataArray, 30);
    char* data = strtok(dataArray, ",");
    for (int i = 0; i < 2; i++) {  // Loop to get to the third token
      data = strtok(NULL, ",");
      if (data == NULL) break;
    }

    if (data != NULL) {
      Serial.println("Incoming string data:");
      Serial.println(data);
      mqttClient.publish(TOPIC, data);
    } else {
      Serial.println("No valid data received");
    }
  }
  delay(500);
}
