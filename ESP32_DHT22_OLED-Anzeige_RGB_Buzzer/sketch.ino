#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==== Konfiguration ==== //
#define DHTPIN 15
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define LED_RED    18
#define LED_GREEN  23
#define BUZZER_PIN 27

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Display starten
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED nicht gefunden"));
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  // RGB-LED & Buzzer Pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Sensorfehler!");
    return;
  }

  delay(1000);
  display.clearDisplay();

  // Temperatur anzeigen
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperatur: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(temp);
  display.cp437(true);
  display.write(167); // Gradzeichen
  display.print("C");

  // Luftfeuchtigkeit anzeigen
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Luftfeuchtigkeit: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(hum);
  display.print(" %");

  display.display();

  // === Ampel-Logik === //
  bool kritisch = false;
  bool grenzwert = false;

 // ROT: kritisch
  if (temp < 16 || temp > 30 || hum < 30 || hum > 70) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    kritisch = true;
  }
 // GELB: grenzwertig (rot+grün gleichzeitig)
  else if ((temp >= 16 && temp < 20) || (temp > 25 && temp <= 30) ||
           (hum >= 30 && hum < 40) || (hum > 60 && hum <= 70)) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    grenzwert = true;
  }
  // GRÜN: optimal
  else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }

  // === Buzzer bei kritischen Werten ===
  if (kritisch) {
    tone(BUZZER_PIN, 2000, 500);  // 2kHz Ton, 0.5 Sekunde
  } else {
    noTone(BUZZER_PIN);          // Ton ausschalten
  }
}
