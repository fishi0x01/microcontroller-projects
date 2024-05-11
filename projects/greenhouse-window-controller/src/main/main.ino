#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define WINDOW_CLOSE_PIN 32
#define WINDOW_OPEN_PIN 33
#define DHT_PIN 25

DHT dht(DHT_PIN, DHT22);

const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int temperature;
int humidity;

void setup()
{
    Serial.begin(115200);
    pinMode(WINDOW_CLOSE_PIN, OUTPUT);
    pinMode(WINDOW_OPEN_PIN, OUTPUT);
    dht.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    delay(2000);
}

void measure() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
}

void display() {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Temper.: ");
    lcd.print(temperature);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Feucht.: ");
    lcd.print(humidity);
    lcd.print(" %");
}

void loop()
{
    measure();
    display();
    delay(5000);
    //measure();
    //digitalWrite(WINDOW_OPEN_PIN, HIGH);
    //delay(5000);
    //measure();
    //digitalWrite(WINDOW_CLOSE_PIN, HIGH);
    //delay(5000);
    //measure();
    //digitalWrite(WINDOW_OPEN_PIN, LOW);
    //delay(5000);
    //measure();
    //digitalWrite(WINDOW_CLOSE_PIN, LOW);
    //delay(5000);
}

