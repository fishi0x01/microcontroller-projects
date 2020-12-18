#include <LiquidCrystal_I2C.h>

#include "DHT.h"

#define HEAT_INDEX_THRESH 23
#define OPEN_CLOSE_MS 5000
#define WINDOW_CLOSE_PIN 16
#define WINDOW_OPEN_PIN 17

#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setupLCD()
{
    lcd.init();
    // lcd.backlight();
}

void setupDHT() { dht.begin(); }

void setup()
{
    Serial.begin(115200);
    pinMode(WINDOW_CLOSE_PIN, OUTPUT);
    pinMode(WINDOW_OPEN_PIN, OUTPUT);
    setupDHT();
    setupLCD();
}

float getTemperature() { return dht.readTemperature(); }

float getHumidity() { return dht.readHumidity(); }

float getHeatIndex(float temperature, float humidity)
{
    return dht.computeHeatIndex(temperature, humidity, false);
}

bool windowOpened = true;
void showData(float temperature, float humidity)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print((int)temperature);
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print("F: ");
    lcd.print((int)humidity);
    lcd.print("%");
    String message = "Geschlossen";
    if (windowOpened) message = "Offen";
    lcd.setCursor(0, 1);
    lcd.print(message);
}

void handleWindow(float heatIndex)
{
    if (heatIndex > HEAT_INDEX_THRESH && !windowOpened) {
        Serial.println("Open");
        windowOpened = true;
        digitalWrite(WINDOW_OPEN_PIN, HIGH);
        delay(OPEN_CLOSE_MS);
        digitalWrite(WINDOW_OPEN_PIN, LOW);
    }
    else if (heatIndex <= HEAT_INDEX_THRESH && windowOpened) {
        Serial.println("Close");
        windowOpened = false;
        digitalWrite(WINDOW_CLOSE_PIN, HIGH);
        delay(OPEN_CLOSE_MS);
        digitalWrite(WINDOW_CLOSE_PIN, LOW);
    }
}

void loop()
{
    delay(3000);

    float temperature = getTemperature();
    float humidity = getHumidity();
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }
    float heatIndex = getHeatIndex(temperature, humidity);
    handleWindow(heatIndex);
    showData(temperature, humidity);
}
