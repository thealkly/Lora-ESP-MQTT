#include <Arduino.h>
#include <driver/adc.h>
#include <esp_sleep.h>
#include <WiFi.h>  // WiFi-Bibliothek hinzufügen

#define WAKE_UP_PERIOD 30  // Zeit in Sekunden zwischen den Messungen


// ADC1 Kanal 6 ist GPIO34 auf dem ESP32, passe den Kanal nach Bedarf an
#define ADC_PIN ADC1_CHANNEL_6


void setup() {
  Serial.begin(115200);  // Starte die serielle Kommunikation mit 115200 Baud

  // Initialisiere ADC
  adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit Auflösung
  adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_DB_11);  // Maximale Dämpfung für Messbereich ca. 0-3.9V

  // Schalte WiFi und Bluetooth ab, um Strom zu sparen
  WiFi.mode(WIFI_OFF);
  btStop();
  Serial.println("startup done ");
  // Konfiguriere den Timer für den Tiefschlaf
  esp_sleep_enable_timer_wakeup(WAKE_UP_PERIOD * 1000000);  // Timer für 30 Sekunden
}

void loop() {
  // Lese den Spannungswert vom definierten ADC-Pin
  Serial.println("start loop");
  int adcValue = adc1_get_raw(ADC_PIN);
  
  // Rechne den rohen ADC-Wert in eine Spannung um (Annahme: 3.3V Referenz)
  float voltage = adcValue * (3.3 / 4095.0);
  float rawvoltage = adcValue ;
  // Zeige den Spannungswert im seriellen Monitor an
  Serial.println("voltage  GPIO34");
  Serial.println(voltage);
  Serial.println(" V");
  Serial.println("raw voltage  GPIO34");
  Serial.println(rawvoltage);
  Serial.println(" raw");
  // Gehe in den Tiefschlafmodus, um Energie zu sparen
  Serial.println(" start deepsleep, good night");
  esp_deep_sleep_start();
}
