# 🌤️ ESP32 Wi-Fi Weather & Clock Station (16x2 I2C LCD)

A compact ESP32 project that connects to Wi-Fi to display real-time NTP clock data alongside current weather metrics. Data is rendered on a standard 16x2 I2C LCD screen, fetching weather conditions directly from the free **Open-Meteo API** (no API key required).

---

## 📸 Overview

* **Line 1 (Top):** Displays Day, Date, Month, and 24-hour Time (e.g., `Wed 04 Nov 14:05`) synchronized via Network Time Protocol (NTP).
* **Line 2 (Bottom):** Toggles every **4 seconds** between:
  1. Live **Temperature & Humidity** (e.g., `28.6C  Hum 65%`)
  2. Short **Weather Condition** (e.g., `Clear`, `Cloudy`, `Drizzle`, `Rain`, `Storm`)
* **Refresh Rate:** Weather data automatically updates every 10 minutes.

---

## 🛠️ Hardware Requirements

* **ESP32 Development Board**
* **16x2 Character LCD** with **I2C Interface Board (PCF8574)**
* **4x Female-to-Female Jumper Wires**
* **USB Cable** (for power & programming)

---

## 🔌 Wiring Table

Connect your 16x2 I2C LCD to the ESP32 as outlined below:

| LCD (I2C) Pin | ESP32 Pin | Wire Description |
| :--- | :--- | :--- |
| **GND** | **GND** | Ground Connection |
| **VCC** | **VIN** | 5V Power Supply |
| **SDA** | **GPIO 21** | I2C Serial Data Line |
| **SCL** | **GPIO 22** | I2C Serial Clock Line |

> **Note:** If your LCD display powers on but shows no text or block characters, try adjusting the small blue potentiometer on the back of the I2C module using a small screwdriver to set the contrast. If the display remains blank, you can change the I2C address in code from `0x27` to `0x3F`.

---

## 💻 Required Arduino Libraries

Ensure you have installed the following libraries via **Tools → Manage Libraries...** in the Arduino IDE:

1. **`LiquidCrystal I2C`** by *Frank de Brabander*
2. **`ArduinoJson`** by *Benoît Blanchon* (**Version 7.x**)

---

## ⚙️ Configuration & Code Setup

Before uploading, modify the top configuration variables in your sketch to reflect your network and geographic location:

```cpp
// ---- EDIT THESE ----
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// Latitude & Longitude for your city (Default set to New Delhi, India)
const float LAT = 28.61;          
const float LON = 77.20;          

// Timezone Offset (Default set to Indian Standard Time: UTC+5:30 -> 19800 seconds)
const long  GMT_OFFSET_SEC = 19800; 
// --------------------
```

---

## 🚀 How to Flash & Run

1. Connect your ESP32 board to your computer via USB.
2. Select your board model under **Tools → Board → ESP32 Arduino**.
3. Select the correct serial port under **Tools → Port**.
4. Click **Upload**.
5. Once flashed, the screen will display `Connecting WiFi`, print the assigned IP address once connected, and begin fetching weather and time details automatically!
