#include <Arduino.h>

//board used is an ESP 32 WROOM -32D
// Sensor Input is connected to GPIO 34 (Analog ADC1_CH6) 
const int ADC1_PIN = 34;

// variable for ADC1 Storage
int ADC1Value = 0;

void goToSleep() {
  // Configure wakeup source to wake up after 60 seconds
  esp_sleep_enable_timer_wakeup(60 * 1000000); // 60 seconds * 1,000,000 microseconds
  Serial.println("Going to sleep now");
  esp_deep_sleep_start(); // Enter deep sleep
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Woke up!");
}

void loop() {
  // Reading potentiometer value
  Serial.println("start ADC1 read..");
  ADC1Value = analogRead(ADC1_PIN);
  Serial.println(ADC1Value);
  delay(500);
    // Go to sleep after reading
  Serial.println("start deep sleep now..");
  goToSleep();
}