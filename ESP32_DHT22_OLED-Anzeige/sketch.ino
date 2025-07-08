#include "DHT.h"                 // DHT Bibliothek einbinden
#include <Wire.h>               // I2C-Kommunikation (für OLED)
#include <Adafruit_GFX.h>       // Grundlegende Grafikfunktionen
#include <Adafruit_SSD1306.h>   // SSD1306 OLED-Bibliothek

// DHT22 Sensor Konfiguration
#define DHTPIN 15               // DHT22 Signal-Pin am ESP32
#define DHTTYPE DHT22           // Sensortyp DHT22 (auch bekannt als AM2302 oder AM2321)

// OLED-Display Größe
#define SCREEN_WIDTH 128        // Breite in Pixel
#define SCREEN_HEIGHT 64        // Höhe in Pixel

// Initialisierung von Sensor und Display
DHT dht(DHTPIN, DHTTYPE);  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // I2C ohne Reset-Pin

//============================== SETUP ==============================

void setup() {
  Serial.begin(115200);  // Serielle Kommunikation starten
  Serial.println("------------- Test -------------");

  dht.begin();  // DHT22 Sensor starten

  // OLED-Display starten
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Zuordnung nicht möglich"));  // Fehlerausgabe bei I2C-Problemen
    for (;;);  // Endlosschleife im Fehlerfall
  }

  delay(1000);  // Kurze Pause

  display.clearDisplay();          // Bildschirm löschen
  display.setTextColor(WHITE);     // Textfarbe auf Weiß setzen
}

void loop() {
  float h = dht.readHumidity();     // Luftfeuchtigkeit auslesen
  float t = dht.readTemperature();  // Temperatur auslesen

  // Wenn Sensorwerte ungültig sind (Not a Number)
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Sensor konnte nicht korrekt gelesen werden!");
    return;  // Fehler – abbrechen
  }

  delay(1000);               // Wartezeit 1 Sekunde
  display.clearDisplay();    // OLED löschen

  // ===== Anzeige der Temperatur =====
  display.setTextSize(1);           // Kleine Schrift
  display.setCursor(0, 0);          // Start oben links
  display.print("Temperatur: ");    // Beschriftung

  display.setTextSize(2);           // Größere Schrift
  display.setCursor(0, 10);         // Neue Zeile
  display.print(t);                 // Temperaturwert
  display.print(" ");

  display.setTextSize(1);           // Kleine Schrift für Symbol
  display.cp437(true);              // Sonderzeichen aktivieren
  display.write(167);               // ASCII-Code für Grad-Zeichen (°)
  display.setTextSize(2);           
  display.print("C");               // Celsius

  // ===== Anzeige der Luftfeuchtigkeit =====
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Luftfeuchtigkeit: ");

  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);                 // Luftfeuchte-Wert
  display.print(" %");              // Prozentzeichen

  // ===== Ausgabe auf dem Display anzeigen =====
  display.display();
}
