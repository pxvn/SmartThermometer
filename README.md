# Smart Non-Contact Thermometer with ESP32 and Arduino Cloud

[![ViewCount](https://views.whatilearened.today/views/github/YOUR_USERNAME/YOUR_REPO_NAME.svg)](#)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](#license)
[![Arduino Cloud](https://img.shields.io/badge/Arduino-Cloud-blue.svg)](#arduino-cloud-setup)
[![ESP32](https://img.shields.io/badge/ESP32-PlatformIO-blue.svg)](#hardware-required)
[![MLX90614 Sensor](https://img.shields.io/badge/MLX90614-Sensor-green.svg)](#hardware-required)
[![Adafruit GFX](https://img.shields.io/badge/Adafruit-GFX-orange.svg)](#software-setup)
[![SSD1306 OLED](https://img.shields.io/badge/SSD1306-OLED-yellow.svg)](#hardware-required)
[![Last Commit](https://img.shields.io/github/last-commit/YOUR_USERNAME/YOUR_REPO_NAME)](#)

Accurately measure temperatures without contact using an ESP32, MLX90614 IR sensor, and OLED display. Seamlessly monitor and visualize data remotely through Arduino IoT Cloud.

## Features

-   Non-contact temperature measurement
-   Animated OLED display with animations
    *   Thermometer icon (startup)
    *   Circular loading animation with "Initializing" text (2 seconds)
    *   Large, centered temperature display with "°C"
    *   Temperature change indicators (up/down arrows)
    *   High-temperature alerts (flashing "HIGH TEMP!" at >39°C)
    *   Wi-Fi connection status icon
-   **Arduino IoT Cloud integration** for remote monitoring on any device
-   1.2-second refresh interval for temperature updates
-   Buzzer feedback on startup

   ## Possible Uses

-   Medical temperature checks
-   Industrial temperature monitoring
-   Food safety
-   Electronics temperature testing
-   Family health monitoring

## Hardware Required

-   ESP32 dev. board
-   MLX90614 IR temperature sensor
-   0.96 inch I2C OLED display (SSD1306)
-   Laser module (for aiming)
-   Buzzer (optional, for alerts)

## MLX90614 Sensor Specifications

-   **Temperature Range:** -40°C to 125°C, -40°C to 85°C (ambient)
-   **Accuracy:**  ±0.5°C 
-   **Field of View:** ~50°
-   **Emissivity:**  (0.1 to 1.0)
-   **I2C Interface**

## Software Setup

-  **Arduino IoT Cloud:** 
    *   Create an account on the [Arduino IoT Cloud](https://create.arduino.cc/iot/home).

## Arduino Cloud Setup

1.  **Create a Thing:**
    *   Log into your Arduino IoT Cloud account.
    *   Create a new Thing and associate it with your ESP32 board.
2.  **Add a Property:**
    *   Add a property of type **Temperature Sensor (°C)** to your Thing. This will be used to store and display the temperature readings in your dashboard. You can name 
        this property something like `Temperature`.
3.  **Update the Code:**
    *   In your Arduino sketch, replace `YOUR_THING_PROPERTY_NAME` with the actual name of the temperature property you created in the Arduino Cloud.
    *   The code will automatically update the property value in the cloud whenever a new temperature reading is taken.

### Dashboard Creation

1.  **Arduino IoT Cloud Dashboard:**
    *   In your Arduino IoT Cloud account, go to the **Dashboards** section.
    *   Create a new dashboard and add a widget to display the temperature property. You can customize the appearance of the widget (e.g., gauge, graph, etc.).

## Wiring Diagram


## Flowchart

```mermaid
graph TD
    A[Start] --> B[Initialize Hardware & Arduino Cloud]
    B --> C[Display Thermometer Symbol]
    C --> D[Display Loading Animation]
    D --> E[Read Temperature]
    E --> F{Temperature > 39°C?}
    F --> |Yes| G[Display High Temp Alert]
    F --> |No| H[Display Temperature]
    G --> H
    H --> I[Check Wi-Fi Status]
    I --> |Connected| J[Display Wi-Fi Symbol]
    I --> |Not Connected| K[No Wi-Fi Symbol]
    J --> M[Send Data to Arduino Cloud]
    K --> M
    M --> L[Wait for Interval]
    L --> E
