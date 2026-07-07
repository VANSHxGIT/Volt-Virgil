// ==========================================
// VOLTVIGIL MASTER FIRMWARE
// Hardware: ESP32 HW-777 Expansion, PZEM-004T, ZMPT101B, 12V Relay
// ==========================================

// --- COMPATIBILITY SHIELD (Fixes compilation errors) ---
#include <mbedtls/md5.h>
#define mbedtls_md5_starts_ret mbedtls_md5_starts
#define mbedtls_md5_update_ret mbedtls_md5_update
#define mbedtls_md5_finish_ret mbedtls_md5_finish

// --- CORE LIBRARIES ---
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- LOCAL PROJECT FILES ---
#include "config.h"
#include "dashboard.h"

// --- HARDWARE PIN MAPPING (HW-777 S-Pins) ---
#define RELAY_PIN 26
#define ZMPT_PIN 34
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

// --- OBJECT INITIALIZATION ---
PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
AsyncWebServer server(80);

String localIPStr = "LOCAL MODE";

String processor(const String& var){
  if(var == "MIN_VOLTS") return String(MIN_VOLTS);
  if(var == "MAX_VOLTS") return String(MAX_VOLTS);
  return String();
}

void setup() {
  Serial.begin(115200);
  
  // 1. Initialize Relay (Muscle) - Start in SAFE/OFF Mode
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 

  // 2. Initialize OLED (Eyes)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Allocation Failed!");
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,20);
  display.println("VoltVigil Booting...");
  display.display();

  // 3. Connect to WiFi
  WiFi.begin(SSID, PASSWORD);
  unsigned long startT = millis();
  // 5-second timeout so the system works even if router is off
  while (WiFi.status() != WL_CONNECTED && millis() - startT < 5000) { 
    delay(100); 
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    localIPStr = WiFi.localIP().toString();
  }

  // 4. Start Web Dashboard
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", DASHBOARD_HTML, processor);
  });
  
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    float v = pzem.voltage();
    String json = "{\"v\":" + String(isnan(v) ? 0 : v) + "}";
    request->send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {
  // --- 1. SENSOR READINGS ---
  float voltage = pzem.voltage();
  if(isnan(voltage)) voltage = 0.0;
  
  // Fast loop for future ZMPT wave plotting
  int waveValue = analogRead(ZMPT_PIN); 

  // --- 2. SAFETY LOGIC (Fast Response) ---
  bool isSafe = (voltage >= MIN_VOLTS && voltage <= MAX_VOLTS);
  digitalWrite(RELAY_PIN, isSafe ? HIGH : LOW);

  // --- 3. OLED DISPLAY UPDATE (Slower, non-blocking) ---
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();

    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println(localIPStr);
    display.println("---------------------");

    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(voltage, 1); display.println(" V");

    display.setTextSize(1);
    display.setCursor(0, 48);

    if (isSafe) {
      display.println("STATUS: EXCELLENT");
    } else {
      display.println("STATUS: !!TRIPPED!!");
    }

    display.display();
  }
}