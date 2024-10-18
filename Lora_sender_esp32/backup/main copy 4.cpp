#include <Arduino.h>
#include <WiFi.h>
#include <esp_bt.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <driver/adc.h> 
#include <Adafruit_INA260.h>

Adafruit_INA260 ina260;

const int ADC1_PIN = 34;
const int sleepTimeMin = 1;
SoftwareSerial lora(17, 16);
String lora_RX_address = "0";
unsigned long sleepInterval = 30000;

int ADC1Value = 0;

void goToSleep() {
  esp_sleep_enable_timer_wakeup(sleepTimeMin * 60 * 1000000);
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  lora.begin(115200);
  WiFi.mode(WIFI_OFF);
  btStop();
  
  Wire.begin(); // Startet die I2C-Kommunikation
  if (!ina260.begin()) {
    Serial.println("Failed to find INA260 chip");
    while (1);
  }

  Serial.println("INA260 found and initialized");
  Serial.println("Woke up!");

  // Initialisierung von ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}

void loop() {
  ADC1Value = adc1_get_raw(ADC1_CHANNEL_6);
  float current_mA = ina260.readCurrent();
  float voltage_V = ina260.readBusVoltage();

  Serial.print("ADC Value: ");
  Serial.println(ADC1Value);
  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println(" mA");
  Serial.print("Voltage: ");
  Serial.print(voltage_V);
  Serial.println(" V");

  String message = "AT+SEND=" + lora_RX_address + ",3," + String(ADC1Value) + "," + String(current_mA) + "," + String(voltage_V);
  lora.println(message);
  Serial.println("Data sent: " + message);

  delay(5000); // 5 Sekunden warten
  Serial.println("Start deep sleep now...");
  goToSleep();
}
