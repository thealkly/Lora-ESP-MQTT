#include <Arduino.h>
#include <WiFi.h>     // Einschließen der WiFi-Bibliothek
#include <esp_bt.h>   // Bibliothek für Bluetooth-Funktionalität
#include <driver/adc.h> 
#include <HardwareSerial.h>

// Läuft zum ersten Mal! Version 1.0 auf einem ESP32

// Board used is an ESP 32 WROOM -32D
// Sensor Input is connected to GPIO 34 (Analog ADC1_CH6) 
const int ADC1_PIN = 34;
const int sleepTimeMin = 1;

// Verwenden von HardwareSerial für das LoRa-Modul
HardwareSerial lora(1); // Serial1 verwenden (UART1)

// Lora 3,3v und GND, TX = GPIO17, RX = GPIO16
String lora_RX_address = "0"; // Lora RX-Adresse eingeben
unsigned long sleepInterval = 30000; // Schlafintervall in Millisekunden (hier 30 Sekunden)
unsigned long lastWakeTime = 0; // Zeitpunkt des letzten Aufwachens

// Variable für den ADC-Wert (Dummy-Wert)
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
  
  // HardwareSerial für LoRa auf den Pins GPIO16 (RX) und GPIO17 (TX) starten
  lora.begin(115200, SERIAL_8N1, 16, 17);

  // Deaktivieren von WiFi und Bluetooth
  WiFi.mode(WIFI_OFF); // WiFi explizit ausschalten
  btStop();            // Bluetooth explizit ausschalten

  // Initialisierung von ADC (bleibt, auch wenn ADC nicht genutzt wird)
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}

void loop() {
  // Dummy-Wert für den ADC-Wert
  ADC1Value = 5; // Dummy-Wert
  Serial.print("ADC Value: ");
  Serial.println(ADC1Value);

  // Senden des Dummy-Wertes über LoRa
  String message = String(ADC1Value);
  int length = message.length();

  // Sende den Befehl an das LoRa-Modul
  lora.println("AT+SEND=" + lora_RX_address + "," + String(length) + "," + message);
  Serial.println("Daten gesendet");

  // Überprüfen der Antwort des LoRa-Moduls
  delay(500); // Wartezeit für die Antwort
  if (lora.available()) {
    String response = lora.readString();
    Serial.print("Antwort vom LoRa-Modul: ");
    Serial.println(response); // Zeige die Antwort auf dem Serial-Monitor
  } else {
    Serial.println("Keine Antwort vom LoRa-Modul erhalten");
  }

  delay(5000); // 5 Sekunden warten

  // Optional: Start deep sleep mode (falls benötigt)
   Serial.println("Start deep sleep now...");
   goToSleep();
}
