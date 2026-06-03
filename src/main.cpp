// ======================================================
// main.cpp
// Entry point aplikasi Smart Garden
// ======================================================

#include <Arduino.h>

#include "config.h"
#include "hardware.h"
#include "wifi_manager.h"
#include "user_manager.h"
#include "web_server.h"
#include "mqtt_manager.h"

// Timer untuk pembacaan sensor
unsigned long lastSensorRead = 0;

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("      SMART GARDEN SYSTEM START");
  Serial.println("======================================");

  // 1. Inisialisasi hardware
  Serial.println("[1/5] Initializing hardware...");
  hardwareSetup();

  // 2. Inisialisasi user manager
  //    - Membuat user default admin/admin jika belum ada
  Serial.println("[2/5] Initializing user manager...");
  userManagerSetup();

  // 3. Koneksi WiFi / Access Point
  Serial.println("[3/5] Initializing WiFi...");
  wifiManagerSetup();

  // 4. Inisialisasi web server
  Serial.println("[4/5] Initializing web server...");
  webServerSetup();

  // 5. Inisialisasi MQTT
  Serial.println("[5/5] Initializing MQTT...");
  mqttSetup();

  // 5. Informasi sistem
  Serial.println("[5/5] System ready!");
  Serial.println("--------------------------------------");

  Serial.print("IP Address : ");
  Serial.println(getCurrentIP());

  Serial.print("WiFi Mode  : ");
  if (isAPMode()) {
    Serial.println("Access Point");
    Serial.println("SSID       : ESP32-TAMAN");
    Serial.println("URL        : http://192.168.4.1");
  } else {
    Serial.println("Station");
    Serial.print("SSID       : ");
    Serial.println(getCurrentSSID());
    Serial.print("URL        : http://");
    Serial.println(getCurrentIP());
  }

  Serial.println("--------------------------------------");
  Serial.println("Default Login");
  Serial.println("Username   : admin");
  Serial.println("Password   : admin");
  Serial.println("======================================");
}

// ======================================================
// LOOP
// ======================================================
void loop() {
  // Handle HTTP requests
  webServerLoop();

  // Handle MQTT
  mqttLoop();

  // Baca sensor secara periodik
  if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
    lastSensorRead = millis();

    // Update sensor dan aktuator
    hardwareUpdate();

    // Kirim update ke MQTT
    mqttPublishStatus();

    // Debug ke Serial
    // printSystemStatus();
  }

  // Delay kecil agar CPU tidak terlalu sibuk
  delay(10);
}