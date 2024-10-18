#include <Arduino.h>
#include <WiFi.h>     // Einschließen der WiFi-Bibliothek
#include <esp_bt.h>   // Bibliothek für Bluetooth-Funktionalität
#include <driver/adc.h> 
#include <SoftwareSerial.h>


// läuft zum erstem mal!! version 1.0 auf nem esp32

//board used is an ESP 32 WROOM -32D
// Sensor Input is connected to GPIO 34 (Analog ADC1_CH6) 
const int ADC1_PIN = 34;
const int sleepTimeMin = 1;

SoftwareSerial lora(17, 16);
//lora 3,3v und gnd, tx2 gpio17, gpio16 rx 3 


String lora_RX_address = "0"; // Lora RX-Adresse eingeben
unsigned long sleepInterval = 30000; // Schlafintervall in Millisekunden (hier 30 Sekunden)
unsigned long lastWakeTime = 0; // Zeitpunkt des letzten Aufwachens


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
  lora.begin(115200);
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
  // length(analogRead01)
  lora.println("AT+SEND=" + lora_RX_address + ",3," + String(ADC1Value)); // AT+SEND=1,2,LO
  Serial.println("Daten gesendet");
  delay(5000); // 5 Sekunden warten
   Serial.println("Start deep sleep now...");
   goToSleep();
}
