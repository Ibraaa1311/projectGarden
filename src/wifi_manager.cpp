// ======================================================
// wifi_manager.cpp
// Implementasi manajemen WiFi dan Access Point
// ======================================================

#include "wifi_manager.h"
#include "config.h"

#include <WiFi.h>
#include <Preferences.h>

// ======================================================
// GLOBAL OBJECT
// ======================================================
Preferences wifiPrefs;

// ======================================================
// SETUP
// ======================================================
void wifiManagerSetup() {
  String ssid, password;

  // Ambil kredensial dari Preferences
  loadWiFiCredentials(ssid, password);

  // Jika tidak ada kredensial, langsung AP mode
  if (ssid.length() == 0) {
    Serial.println("No saved WiFi credentials.");
    startAccessPoint();
    return;
  }

  // Coba connect ke WiFi
  if (!connectToWiFi(ssid, password)) {
    Serial.println("Failed to connect to saved WiFi.");
    startAccessPoint();
  }
}

// ======================================================
// STORAGE FUNCTIONS
// ======================================================
void saveWiFiCredentials(const String &ssid, const String &password) {
  wifiPrefs.begin(PREF_WIFI_NAMESPACE, false);

  wifiPrefs.putString(PREF_KEY_WIFI_SSID, ssid);
  wifiPrefs.putString(PREF_KEY_WIFI_PASS, password);

  wifiPrefs.end();

  Serial.println("WiFi credentials saved.");
}

void loadWiFiCredentials(String &ssid, String &password) {
  wifiPrefs.begin(PREF_WIFI_NAMESPACE, true);

  ssid = wifiPrefs.getString(PREF_KEY_WIFI_SSID, "");
  password = wifiPrefs.getString(PREF_KEY_WIFI_PASS, "");

  wifiPrefs.end();
}

void resetWiFiCredentials() {
  wifiPrefs.begin(PREF_WIFI_NAMESPACE, false);

  wifiPrefs.remove(PREF_KEY_WIFI_SSID);
  wifiPrefs.remove(PREF_KEY_WIFI_PASS);

  wifiPrefs.end();

  Serial.println("WiFi credentials cleared.");
}

// ======================================================
// CONNECTION FUNCTIONS
// ======================================================
bool connectToWiFi(const String &ssid, const String &password) {
  if (ssid.length() == 0) {
    return false;
  }

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // Pastikan mode station
  WiFi.disconnect(true);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startTime = millis();

  // Tunggu maksimal 15 detik
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 15000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected successfully.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("WiFi connection failed.");
  return false;
}

void startAccessPoint() {
  Serial.println("Starting Access Point...");

  // Mode AP
  WiFi.disconnect(true);
  delay(500);

  WiFi.mode(WIFI_AP);

  // AP tanpa password (sesuai config)
  if (strlen(AP_PASSWORD) == 0) {
    WiFi.softAP(AP_SSID);
  } else {
    WiFi.softAP(AP_SSID, AP_PASSWORD);
  }

  delay(500);

  Serial.println("Access Point started.");
  Serial.print("SSID: ");
  Serial.println(AP_SSID);

  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
}

// ======================================================
// HELPER FUNCTIONS
// ======================================================
String getCurrentIP() {
  if (isAPMode()) {
    return WiFi.softAPIP().toString();
  }

  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.localIP().toString();
  }

  return "0.0.0.0";
}

String getCurrentSSID() {
  if (isAPMode()) {
    return String(AP_SSID);
  }

  if (WiFi.status() == WL_CONNECTED) {
    return WiFi.SSID();
  }

  return "";
}

bool isAPMode() {
  wifi_mode_t mode = WiFi.getMode();
  return (mode == WIFI_AP || mode == WIFI_AP_STA);
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// ======================================================
// WIFI SCANNING
// ======================================================

// Variables for caching WiFi scan results
static unsigned long lastScanTime = 0;
static String cachedNetworksJSON = "";
const unsigned long SCAN_CACHE_DURATION = 30000; // 30 seconds

String getAvailableNetworksJSON() {
  unsigned long currentTime = millis();

  // Return cached results if still valid
  if (lastScanTime > 0 && (currentTime - lastScanTime) < SCAN_CACHE_DURATION) {
    return cachedNetworksJSON;
  }

  String connectedSSID = getCurrentSSID();
  String jsonResult = "{\"networks\":[";

  // Perform WiFi scan
  int networkCount = WiFi.scanNetworks();

  if (networkCount == -1) {
    // Scan failed
    jsonResult += "]}";
    return jsonResult;
  }

  bool first = true;
  for (int i = 0; i < networkCount; i++) {
    if (!first) jsonResult += ",";
    first = false;

    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    uint8_t encryption = WiFi.encryptionType(i);
    bool secure = (encryption != WIFI_AUTH_OPEN);
    bool connected = (ssid == connectedSSID);

    jsonResult += "{";
    jsonResult += "\"ssid\":\"" + ssid + "\",";
    jsonResult += "\"rssi\":" + String(rssi) + ",";
    jsonResult += "\"secure\":" + String(secure ? "true" : "false") + ",";
    jsonResult += "\"connected\":" + String(connected ? "true" : "false");
    jsonResult += "}";
  }

  jsonResult += "]}";

  // Clean up scan results
  WiFi.scanDelete();

  // Cache the result
  lastScanTime = currentTime;
  cachedNetworksJSON = jsonResult;

  return jsonResult;
}