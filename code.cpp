/* This code makes a contactless thermometer using an ESP32. It reads temperature from an infrared sensor and 
   shows it on a small screen. If the temperature gets too high, it warns you. 
   You can also see the temperature on your phone or computer because it connects to Wi-Fi and sends the data 
  to the cloud. It even has a laser pointer to help you aim and a sensor to know when something is close 
  enough to measure.*/


#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "thingProperties.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LASER_PIN 4 
#define WIFI_LED_PIN 2
#define PROXIMITY_PIN 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

const long interval = 1200;
unsigned long previousMillis = 0;
unsigned long wifiPreviousMillis = 0;
double prevTemp = 0.0;
bool highTempAlertActive = false;
bool wifiBlinkState = false;
int previousWiFiStatus = WL_IDLE_STATUS;

void displayTemperature(double temp);
void displayTemperatureChangeIndicator(double temp);
void handleHighTempAlert(double temp);
void displayProximityMessage();
void handleWiFiStatus();

void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);
  mlx.begin(); 
  mlx.writeEmissivity(0.95); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW); 
  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(PROXIMITY_PIN, INPUT);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
}

void loop() {
    ArduinoCloud.update();

    bool objectDetected = digitalRead(PROXIMITY_PIN) == HIGH;

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (objectDetected) {
            float temp_obj = mlx.readObjectTempC();
            iRTemp = temp_obj;
            digitalWrite(LASER_PIN, HIGH); 

            displayTemperature(temp_obj);
            displayTemperatureChangeIndicator(temp_obj);
            handleHighTempAlert(temp_obj);

        } else {
            digitalWrite(LASER_PIN, LOW); 
            displayProximityMessage();
        }

        display.display(); 
    }

    if (currentMillis - wifiPreviousMillis >= 5000) {  
        wifiPreviousMillis = currentMillis;
        int currentStatus = WiFi.status();

        if (currentStatus != previousWiFiStatus) {
            previousWiFiStatus = currentStatus;
            handleWiFiStatus();
            display.display(); 
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(WIFI_LED_PIN, HIGH);
    } 
}

void displayProximityMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30, 28); 
  display.println("Move Away!!");
  display.display();
}

void displayTemperature(double temp) {
  display.clearDisplay();
  display.setTextSize(3); 
  display.setTextColor(WHITE);

  String tempStr = String(temp, 1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(tempStr, 0, 0, &x1, &y1, &w, &h);

  int tempX = (SCREEN_WIDTH - w) / 2;
  display.setCursor(tempX, 28);
  display.print(temp, 1);
  display.display();
}

void displayTemperatureChangeIndicator(double temp) {
  if (temp > prevTemp + 2) {
    display.drawTriangle(5, 54, 15, 54, 10, 44, WHITE); 
  } else if (temp < prevTemp - 2) {
    display.drawTriangle(5, 44, 15, 44, 10, 54, WHITE); 
  }
  prevTemp = temp;
}

void handleHighTempAlert(double temp) {
  if (temp > 39 && !highTempAlertActive) {
    highTempAlertActive = true;
    displayHighTempAlert();
    delay(500); 
  } else if (temp <= 39) {
    highTempAlertActive = false;
  }
}

void displayHighTempAlert() {
  display.clearDisplay();
  display.drawTriangle(SCREEN_WIDTH / 2 - 10, 30, SCREEN_WIDTH / 2 + 10, 30, SCREEN_WIDTH / 2, 10, WHITE); 
  display.fillTriangle(SCREEN_WIDTH / 2 - 10, 30, SCREEN_WIDTH / 2 + 10, 30, SCREEN_WIDTH / 2, 10, WHITE); 
  display.setCursor(35, 40);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("HIGH TEMP!");
  display.display();
}


void handleWiFiStatus() {
  int wifiX = SCREEN_WIDTH - 8;
  int wifiY = 3;

  if (WiFi.status() == WL_CONNECTED) {
    display.fillCircle(wifiX, wifiY, 3, WHITE); 
    digitalWrite(WIFI_LED_PIN, HIGH); 
  } else {
    if (wifiBlinkState) {
      display.fillCircle(wifiX, wifiY, 3, WHITE); 
    } else {
      display.fillCircle(wifiX, wifiY, 3, BLACK); 
    }
    wifiBlinkState = !wifiBlinkState;
    digitalWrite(WIFI_LED_PIN, LOW); 
  }
}

void displayWiFiSymbol(bool isConnected) {
  int dotX = SCREEN_WIDTH - 6;  
  int dotY = 3;  
  int dotRadius = 3;  
  
  if (isConnected) {
    display.fillCircle(dotX, dotY, dotRadius, WHITE);  
  } else {
    if (wifiBlinkState) {
      display.fillCircle(dotX, dotY, dotRadius, WHITE);  
    } else {
      display.fillCircle(dotX, dotY, dotRadius, BLACK);  
    }
    wifiBlinkState = !wifiBlinkState;
  }
}
