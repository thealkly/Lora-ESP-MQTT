#include <SoftwareSerial.h>
#include <avr/sleep.h> // Library für den Tiefschlafmodus
#include <avr/power.h> // Library zur Steuerung der Stromsparmodi

#define button 7
int analogRead01;
SoftwareSerial lora(2, 3);

String lora_RX_address = "0"; // Lora RX-Adresse eingeben
unsigned long sleepInterval = 30000; // Schlafintervall in Millisekunden (hier 30 Sekunden)
unsigned long lastWakeTime = 0; // Zeitpunkt des letzten Aufwachens

void setup()
{
  pinMode(button, INPUT_PULLUP);
  Serial.begin(115200);
  lora.begin(115200);
  Serial.println(analogRead(A0));
  Serial.println("Boot abgeschlossen");
}

void loop()
{
  unsigned long currentTime = millis(); // Aktuelle Zeit abrufen

  if (currentTime - lastWakeTime >= sleepInterval)
  {
    lastWakeTime = currentTime; // Aktualisiere den Zeitpunkt des letzten Aufwachens
    wakeUp(); // Funktion zum Aufwachen aufrufen
  }

  // Hier wird der restliche Code platziert, der während der Aktivphasen ausgeführt wird
  Serial.println("Starte Auslesen");
  Serial.println(analogRead(A0));
  analogRead01 = analogRead(A0);
  // length(analogRead01)
  lora.println("AT+SEND=" + lora_RX_address + ",3," + String(analogRead01)); // AT+SEND=1,2,LO
  Serial.println("Daten gesendet");
  delay(15000); // 15 Sekunden warten
}

void goToSleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Tiefschlafmodus aktivieren
  sleep_enable(); // Tiefschlaf aktivieren
  power_all_disable(); // Alle Peripheriegeräte ausschalten

  sleep_mode(); // In den Tiefschlafmodus wechseln

  // Code wird hier fortgesetzt, sobald der Arduino aufwacht
  power_all_enable(); // Peripheriegeräte wieder aktivieren
  sleep_disable(); // Tiefschlaf deaktivieren
}

void wakeUp()
{
  goToSleep(); // Den Arduino in den Tiefschlafmodus versetzen
}
