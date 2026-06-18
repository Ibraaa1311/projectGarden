// ======================================================
// config.h
// Konfigurasi global project Smart Garden
// ======================================================

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ======================================================
// SERIAL CONFIGURATION
// ======================================================
#define SERIAL_BAUDRATE 115200

// ======================================================
// PIN CONFIGURATION
// ======================================================
#define SOIL_PIN       34   // Analog input soil moisture
#define RAIN_PIN       35   // Analog input rain sensor
#define RELAY_PUMP     26   // Relay pompa
#define SERVO_PIN      25   // Servo jemuran

// ======================================================
// SENSOR THRESHOLDS
// ======================================================
// Sesuaikan berdasarkan hasil kalibrasi sensor

// Soil:
// 0-50% = DRY, 50-100% = WET
#define SOIL_THRESHOLD 2700
#define SOIL_DRY_ADC 3200
#define SOIL_WET_ADC 1400

// Rain (paling sensitif):
// nilai <= RAIN_THRESHOLD = RAINING
// nilai > RAIN_THRESHOLD = CLEAR
// Set tinggi agar sedikit air sudah dianggap hujan
#define RAIN_THRESHOLD 4000

// ======================================================
// TIMING
// ======================================================
#define SENSOR_READ_INTERVAL 2000UL   // 2 detik

// ======================================================
// ACCESS POINT CONFIGURATION
// ======================================================
#define AP_SSID "ESP32-TAMAN"
#define AP_PASSWORD ""     // Kosong = open network

// ======================================================
// SESSION CONFIGURATION
// ======================================================
#define SESSION_TIMEOUT 1800000UL   // 30 menit
#define SESSION_COOKIE_NAME "miniGardenSession"
#define MAX_WEB_SESSIONS 5

// ======================================================
// USER CONFIGURATION
// ======================================================
#define DEFAULT_USERNAME "admin"
#define DEFAULT_PASSWORD "admin"

// Maksimal jumlah user yang disimpan
#define MAX_USERS 10

// Maksimal panjang username/password
#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 32

// ======================================================
// UI CONFIGURATION
// ======================================================
#define DASHBOARD_REFRESH_INTERVAL 1000   // 1 detik (AJAX polling)

// ======================================================
// SENSOR HEALTH DETECTION
// ======================================================
// Jumlah sampel untuk mendeteksi sensor disconnect
#define SENSOR_SAMPLE_COUNT       5

// Selisih max-min ADC dalam satu burst sampel.
// Floating pin (kabel lepas) → loncat ratusan–ribuan count
// Sensor terpasang normal    → variansi < ~80 count
// Nilai 150 memberikan margin aman di tengah-tengah.
// Turunkan jika sensor kamu sangat noise; naikkan jika terlalu
// sering false-positive.
#define SENSOR_VARIANCE_THRESHOLD 150

// ======================================================
// HARDWARE LOGIC
// ======================================================
// Relay pompa pada rangkaian kamu:
// HIGH = ON
// LOW  = OFF
#define PUMP_ON_LEVEL  HIGH
#define PUMP_OFF_LEVEL LOW

// Posisi servo
#define SERVO_IN_ANGLE   0     // Jemuran masuk
#define SERVO_OUT_ANGLE  180   // Jemuran keluar

// ======================================================
// NAMESPACE / STORAGE KEYS
// ======================================================
#define PREF_WIFI_NAMESPACE  "wifi"
#define PREF_USERS_NAMESPACE "users"
#define PREF_SESSION_NAMESPACE "session"

// WiFi keys
#define PREF_KEY_WIFI_SSID "ssid"
#define PREF_KEY_WIFI_PASS "pass"

// Session keys
#define PREF_KEY_SESSION_TOKEN "token"

// ======================================================
// STATUS ENUMS (optional helper strings)
// ======================================================
#define STATUS_DRY      "DRY"
#define STATUS_WET      "WET"

#define STATUS_RAINING  "RAINING"
#define STATUS_CLEAR    "CLEAR"

#define STATUS_ON       "ON"
#define STATUS_OFF      "OFF"

#define STATUS_IN       "IN"
#define STATUS_OUT      "OUT"

// ======================================================
// PROJECT INFORMATION
// ======================================================
#define PROJECT_NAME    "Smart Garden"
#define PROJECT_VERSION "1.0.0"

#endif // CONFIG_H