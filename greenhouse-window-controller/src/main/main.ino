#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#include "DHT.h"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

#define HEAT_INDEX_THRESH 23
#define OPEN_CLOSE_MS 20000
#define WINDOW_CLOSE_PIN 16
#define WINDOW_OPEN_PIN 17
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define SLEEP_SECONDS 5
#define MAX_SENSOR_READ_ERRS 10

#define EEPROM_SIZE 1  /* Byes we want to store on the EEPROM */
#define WINDOW_MAGIC_ADDR 0
#define WINDOW_OPEN_MAGIC 107
#define WINDOW_CLOSE_MAGIC 9

DHT dht(DHT_PIN, DHT_TYPE);
const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Vars
float humidity;
float temperature;
float heatIndex;

void setup()
{
    Serial.begin(115200);
    pinMode(WINDOW_CLOSE_PIN, OUTPUT);
    pinMode(WINDOW_OPEN_PIN, OUTPUT);
    digitalWrite(WINDOW_OPEN_PIN, HIGH);
    digitalWrite(WINDOW_CLOSE_PIN, HIGH);
    dht.begin();
    EEPROM.begin(EEPROM_SIZE);
    //lcd.init();
    // lcd.backlight();
    // Give time to settle
    delay(2000);
}

void measureTemperature() { temperature = dht.readTemperature(); }

void measureHumidity() { humidity = dht.readHumidity(); }

void calculateHeatIndex()
{
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
}

void displayState()
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
    if (isWindowOpen()) message = "Offen";
    lcd.setCursor(0, 1);
    lcd.print(message);
}

bool isWindowOpen() {
    Serial.println(EEPROM.read(WINDOW_MAGIC_ADDR));
    return EEPROM.read(WINDOW_MAGIC_ADDR) == WINDOW_OPEN_MAGIC;
}

void displayError()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Messfehler");
    lcd.setCursor(0, 1);
    lcd.print("Neuer Versuch");
}

void handleWindow()
{
    bool windowOpen = isWindowOpen();
    if (heatIndex > HEAT_INDEX_THRESH && !windowOpen) {
        Serial.println("Open");
        EEPROM.write(WINDOW_MAGIC_ADDR, WINDOW_OPEN_MAGIC);
        EEPROM.commit();
        digitalWrite(WINDOW_OPEN_PIN, LOW);
        delay(OPEN_CLOSE_MS);
        digitalWrite(WINDOW_OPEN_PIN, HIGH);
    }
    else if (heatIndex <= HEAT_INDEX_THRESH && windowOpen) {
        Serial.println("Close");
        EEPROM.write(WINDOW_MAGIC_ADDR, WINDOW_CLOSE_MAGIC);
        EEPROM.commit();
        digitalWrite(WINDOW_CLOSE_PIN, LOW);
        delay(OPEN_CLOSE_MS);
        digitalWrite(WINDOW_CLOSE_PIN, HIGH);
    }
}

void sleep() {
    Serial.println(F("SLEEP"));
    esp_sleep_enable_timer_wakeup(SLEEP_SECONDS * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void loop()
{
    measureTemperature();
    measureHumidity();
    int errs = 0;
    while ((isnan(temperature) || isnan(humidity)) && errs < MAX_SENSOR_READ_ERRS) {
        Serial.println(F("Failed to read from DHT sensor!"));
        // displayError();
        errs++;
        delay(1000);
    }
    if (!(isnan(temperature) || isnan(humidity))) {
        calculateHeatIndex();
        handleWindow();
    }
    // displayState();
    sleep();
}
