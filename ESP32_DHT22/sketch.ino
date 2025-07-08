#include "DHT.h"  // DHT Bibliothek einbinden
#include <Adafruit_SSD1306.h>   // OLED Display
#include <Adafruit_GFX.h>       // Grafikbibliothek

#define DHTPIN 15       // DHT22 Sensor Signal auf ESP32 Eingang 15
#define DHTTYPE DHT22   // Definition des Sensortyps: DHT 22

// OLED-Display Einstellungen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1          // Kein Reset-Pin nötig

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);  // DHT22 Sensor initialisieren

void setup() {
  Serial.begin(115200);             // Baudrate für ESP32
  Serial.println("DHT22 Test!");    // Testmeldung
  dht.begin();                      // DHT Sensor starten

   // OLED initialisieren
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED konnte nicht gefunden werden!"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Starte Sensor...");
  display.display();
  delay(2000);
}

void loop() {
  delay(2000);  // Wartezeit von 2 Sekunden zwischen den Messungen

  float h = dht.readHumidity();     // Luftfeuchtigkeit auslesen
  float t = dht.readTemperature();  // Temperatur auslesen

  // Überprüfen, ob die Sensorwerte gültig sind
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Sensor Fehler!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensorfehler!");
    display.display();
    return;
  }

  // Serielle Ausgabe
  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(t);
  Serial.println(" °C");

  // OLED-Ausgabe
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Raumklima:");
  display.print("Temp: ");
  display.print(t);
  display.println(" C");
  display.print("Feuchte: ");
  display.print(h);
  display.println(" %");
  display.display();
}
