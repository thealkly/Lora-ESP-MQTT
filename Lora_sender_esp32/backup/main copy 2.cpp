#include <Arduino.h>
#include <WiFi.h>     // Einschließen der WiFi-Bibliothek
#include <esp_bt.h>   // Bibliothek für Bluetooth-Funktionalität
#include <driver/adc.h> 
#include <SoftwareSerial.h>


//board used is an ESP 32 WROOM -32D
// Sensor Input is connected to GPIO 34 (Analog ADC1_CH6) 
const int ADC1_PIN = 34;
const int sleepTimeMin = 1;

// Variable for ADC1 Storage
int ADC1Value = 0;

void goToSleep() {
  // Konfigurieren der Wakeup-Quelle, um nach 60 Sekunden aufzuwachen
  esp_sleep_enable_timer_wakeup(sleepTimeMin * 60 * 1000000); // 60 Sekunden * 1.000.000 Mikrosekunden

  Serial.println("Going to sleep now");
  esp_deep_sleep_start(); // In den Tiefschlaf wechseln
}

void setup() {
  Serial.begin(115200); // Initialisiert die serielle Kommunikation
  Serial.println("Woke up!");

  // Deaktivieren von WiFi und Bluetooth
  WiFi.mode(WIFI_OFF); // WiFi explizit ausschalten
  btStop();            // Bluetooth explizit ausschalten

  // Initialisierung von ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}

void loop() {
  // Auslesen des ADC-Wertes
  ADC1Value = adc1_get_raw(ADC1_CHANNEL_6);
  Serial.print("ADC Value: ");
  Serial.println(ADC1Value);

  Serial.println("Start deep sleep now...");
  goToSleep();
}
