#include <SoftwareSerial.h>
#include "arduino_secrets.h"
#define LED 13

String incomingString;
SoftwareSerial lora(12, 13);
// D6, D7 eingang sind
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>




#define PIN1 14  //D5 nodemuc
#define PIN2 12  //D6 nodemcu
#define DELAY 30
//I2C
//I2C can be used to connect up to 127 nodes via a bus that only requires two data wires, known as SDA and SCL.
//SDA=4 => D2.
//SCL=5 => D1

const char *WIFI_SSID = SECRET_SSID;
const char *WIFI_PASSWORD = SECRET_PWD;

const char *MQTT_HOST = "192.168.1.13";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266-lorawan01-NodeMCU";
const char *MQTT_USER = SECRET_MQTUSER;
const char *MQTT_PASSWORD = SECRET_MQTPW;
;
const char *TOPIC = "p11/lorawan/garden/soilmoisture01";
const char *TOPICSTATUS = "p11/lorawan/garden/soilmoisture01/status";
long lastMsg = 0;
float temp = 0;


WiFiClient client;
PubSubClient mqttClient(client);





void setup() {
  Serial.begin(115200);
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  digitalWrite(PIN1, LOW);
  digitalWrite(PIN2, LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to Wi-Fi");

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
 // mqttClient.setCallback(callback);

  while (!client.connected()) {
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(500);
      Serial.print(".");
    }
  }

  mqttClient.subscribe(TOPIC);
  Serial.println(TOPIC);
  Serial.println("connected");
  lora.begin(115200);
  lora.setTimeout(500);
  Serial.println("boot done");
}

void loop() {
 mqttClient.loop();
 if (lora.available()) {
    incomingString = lora.readString();
    Serial.println("incoming string");
    Serial.println(incomingString);

    char dataArray[30]; 
    incomingString.toCharArray(dataArray,30);
    char* data = strtok(dataArray, ",");
    data = strtok(NULL, ",");
    data = strtok(NULL, ",");
    Serial.println("incoming string");
    Serial.println(data);
    mqttClient.loop();

    mqttClient.publish(TOPIC, data);
  
      }
  delay(500);
  
 
}
