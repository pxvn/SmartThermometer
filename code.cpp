#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "thingProperties.h"

// Pin Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LASER_PIN 12
#define BUZZER_PIN 4
#define WIFI_LED_PIN 2

// Display and Sensor Objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Arduino Cloud Property
// CloudTemperatureSensor iRTemp;

// Global Variables
const long interval = 1200;
unsigned long previousMillis = 0;
double prevTemp = 0.0;
bool highTempAlertActive = false;
const int loadingRadius = 10;
int loadingAngle = 0; 

void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);
  mlx.begin();  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);

  // Startup Animations
  displayThermometerSymbol();
  delay(500);
  showLoadingAnimation();
  tone(BUZZER_PIN, 1000, 100); // Short beep

  // Arduino Cloud Initialization
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
}

void loop() {
  ArduinoCloud.update(); 

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float temp_amb, temp_obj;

    // Temperature Measurement
    temp_amb = mlx.readAmbientTempC();
    temp_obj = mlx.readObjectTempC();
    iRTemp = temp_obj; 
    digitalWrite(LASER_PIN, HIGH);

    displayTemperature(temp_obj);

    // Temperature Change Indicators
    if (temp_obj > prevTemp + 4) {
      display.drawTriangle(0, 54, 10, 54, 5, 44, WHITE);
    } else if (temp_obj < prevTemp - 4) {
      display.drawTriangle(0, 44, 10, 44, 5, 54, WHITE);
    }
    prevTemp = temp_obj;

    // High-Temperature Alert
    if (temp_obj > 39 && !highTempAlertActive) {
      highTempAlertActive = true;
      displayHighTempAlert();
      delay(1000); 
    } else if (temp_obj <= 39) {
      highTempAlertActive = false;
    }
  
    if (WiFi.status() == WL_CONNECTED) {
      displayWiFiSymbol();
    }

    display.display();
    digitalWrite(LASER_PIN, LOW);
  }
}


// Display Functions
void displayThermometerSymbol() {
  display.clearDisplay();
  display.drawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10, 15, WHITE);
  display.fillRect(SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT / 2 - 10, 4, 20, WHITE);
  display.fillRect(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2 + 10, 12, 4, WHITE);
  display.display();
}

void showLoadingAnimation() {
  display.clearDisplay();
  display.setCursor(43, 24);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Initializing");

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  for (int i = 0; i < 100; i++) {
    display.drawCircle(centerX, centerY, loadingRadius, WHITE);
    display.fillCircle(centerX + loadingRadius * cos(loadingAngle), 
                      centerY + loadingRadius * sin(loadingAngle), 2, WHITE);
    loadingAngle += 0.2; 
    display.display();
    display.fillRect(centerX - loadingRadius, centerY - loadingRadius, loadingRadius * 2, loadingRadius * 2, BLACK);
    delay(15); 
  }
}

void displayTemperature(double temp) {
  display.clearDisplay();
  display.setTextSize(2); 
  display.setTextColor(WHITE);
  display.setCursor(20, 28); 
  display.print(temp, 1);
  display.setTextSize(2);
  display.print(" ");
  display.println("oC");
  display.display();
}


void displayHighTempAlert() {
  display.clearDisplay();
  display.drawTriangle(SCREEN_WIDTH / 2 - 10, 10, SCREEN_WIDTH / 2 + 10, 10, SCREEN_WIDTH / 2, 30, WHITE);
  display.fillTriangle(SCREEN_WIDTH / 2 - 10, 10, SCREEN_WIDTH / 2 + 10, 10, SCREEN_WIDTH / 2, 30, WHITE);
  display.setCursor(35, 40);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("HIGH TEMP!");
  display.display();
}

void displayWiFiSymbol() {
  
}
