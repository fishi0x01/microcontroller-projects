#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define WINDOW_CLOSE_PIN 32
#define WINDOW_OPEN_PIN 33
#define DHT_PIN 25

DHT dht(DHT_PIN, DHT22);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

float temperature;
float humidity;
float heatIndex;

// https://en.wikipedia.org/wiki/Heat_index
const float HEAT_INDEX_THRESH = 23.0;

bool windowOpen = false;

void setup()
{
    Serial.begin(115200);
    pinMode(WINDOW_CLOSE_PIN, OUTPUT);
    pinMode(WINDOW_OPEN_PIN, OUTPUT);
    dht.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    // TODO: is it save to push close when window already closed?
    // ensure we have a closed window when we start
    closeWindow();
    delay(2000);
}

void measure() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
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

void closeWindow() {
    digitalWrite(WINDOW_CLOSE_PIN, HIGH);
    delay(2000);
    digitalWrite(WINDOW_CLOSE_PIN, LOW);
}

void openWindow() {
    digitalWrite(WINDOW_OPEN_PIN, HIGH);
    delay(2000);
    digitalWrite(WINDOW_OPEN_PIN, LOW);
}

void handleWindow() {
    if(heatIndex >= HEAT_INDEX_THRESH) {
        if(!windowOpen) {
            openWindow();
            windowOpen = true;
        }
    } else {
        if(windowOpen) {
            closeWindow();
            windowOpen = false;
        }
    }
}

void loop()
{
    measure();
    display();
    handleWindow();
    // 10s
    delay(10000);
}

