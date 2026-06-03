// ======================================================
// wifi_manager.h
// Manajemen koneksi WiFi dan Access Point
// ======================================================

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// ======================================================
// SETUP
// ======================================================

// Inisialisasi WiFi:
// - Membaca SSID & password dari Preferences
// - Mencoba connect ke WiFi
// - Jika gagal, membuat Access Point
void wifiManagerSetup();

// ======================================================
// WIFI STORAGE
// ======================================================

// Menyimpan kredensial WiFi
void saveWiFiCredentials(const String &ssid, const String &password);

// Membaca kredensial WiFi
void loadWiFiCredentials(String &ssid, String &password);

// Menghapus kredensial WiFi
void resetWiFiCredentials();

// ======================================================
// CONNECTION CONTROL
// ======================================================

// Mencoba koneksi ke WiFi
bool connectToWiFi(const String &ssid, const String &password);

// Menjalankan Access Point
void startAccessPoint();

// ======================================================
// HELPER FUNCTIONS
// ======================================================

// Mendapatkan IP aktif
String getCurrentIP();

// Mendapatkan SSID yang sedang digunakan
String getCurrentSSID();

// Mengetahui apakah sedang AP mode
bool isAPMode();

// Mengetahui apakah WiFi terhubung
bool isWiFiConnected();

// ======================================================
// WIFI SCANNING
// ======================================================

// Struktur untuk menyimpan data jaringan WiFi
struct WiFiNetwork {
  String ssid;
  int rssi;
  bool secure;
};

// Scan jaringan WiFi yang tersedia
// Returns JSON string dengan format:
// {"networks":[{"ssid":"name","rssi":-42,"secure":true,"connected":false},...]}
String getAvailableNetworksJSON();

#endif // WIFI_MANAGER_H