// ESP32 Wi-Fi Weather + Clock Station (16x2 I2C LCD)
// Libraries (Library Manager): "LiquidCrystal I2C" by Frank de Brabander, "ArduinoJson" v7
// Weather: Open-Meteo (free, no API key). Time: NTP.
//credits to am_i_agrim
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

// ---- EDIT THESE ----
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const float LAT = 28.61;          // your city's latitude
const float LON = 77.20;          // your city's longitude
const long  GMT_OFFSET_SEC = 19800; // IST = UTC+5:30
// --------------------

LiquidCrystal_I2C lcd(0x27, 16, 2); // try 0x3F if the screen stays blank

float temp = 0;
int hum = 0, wcode = 0;
bool haveWeather = false;
bool showDesc = false;
unsigned long lastWeather = 0, lastSwitch = 0;

const char* describe(int c) {
  if (c == 0) return "Clear";
  if (c <= 3) return "Cloudy";
  if (c <= 48) return "Fog";
  if (c <= 57) return "Drizzle";
  if (c <= 67) return "Rain";
  if (c <= 77) return "Snow";
  if (c <= 82) return "Showers";
  if (c <= 86) return "Snow";
  return "Storm";
}

void printLine(int row, const char* text) {
  char buf[17];
  snprintf(buf, sizeof(buf), "%-16s", text);
  lcd.setCursor(0, row);
  lcd.print(buf);
}

void fetchWeather() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(LAT, 2) +
               "&longitude=" + String(LON, 2) +
               "&current=temperature_2m,relative_humidity_2m,weather_code";
  if (!http.begin(client, url)) return;
  if (http.GET() == 200) {
    JsonDocument doc;
    if (!deserializeJson(doc, http.getString())) {
      temp = doc["current"]["temperature_2m"];
      hum = doc["current"]["relative_humidity_2m"];
      wcode = doc["current"]["weather_code"];
      haveWeather = true;
    }
  }
  http.end();
}

void setup() {
  lcd.init();
  lcd.backlight();
  printLine(0, "Connecting WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);

  printLine(0, "Connected!");
  printLine(1, WiFi.localIP().toString().c_str());
  delay(1500);

  configTime(GMT_OFFSET_SEC, 0, "pool.ntp.org", "time.nist.gov");
  fetchWeather();
  lastWeather = millis();
}

void loop() {
  // Refresh weather every 10 minutes
  if (millis() - lastWeather > 600000UL) {
    fetchWeather();
    lastWeather = millis();
  }

  // Swap bottom line between numbers and description every 4 seconds
  if (millis() - lastSwitch > 4000) {
    showDesc = !showDesc;
    lastSwitch = millis();
  }

  struct tm t;
  if (getLocalTime(&t)) {
    char line1[17];
    strftime(line1, sizeof(line1), "%a %d %b %H:%M", &t); // e.g. Wed 04 Nov 14:05
    printLine(0, line1);
  } else {
    printLine(0, "Syncing time...");
  }

  if (!haveWeather) {
    printLine(1, "Getting weather");
  } else {
    char line2[17];
    if (showDesc) snprintf(line2, sizeof(line2), "%s", describe(wcode));
    else snprintf(line2, sizeof(line2), "%.1fC  Hum %d%%", temp, hum);
    printLine(1, line2);
  }

  delay(500);
}
