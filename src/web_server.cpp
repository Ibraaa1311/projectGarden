// ======================================================
// web_server.cpp
// Implementasi Web Server, Authentication, API, Routing
// ======================================================

#include "web_server.h"

#include <WebServer.h>
#include <Preferences.h>
#include <esp_system.h>

#include "config.h"
#include "hardware.h"
#include "wifi_manager.h"
#include "user_manager.h"
#include "web_pages.h"

// ======================================================
// GLOBAL OBJECTS
// ======================================================
static WebServer server(80);
static Preferences sessionPrefs;

// ======================================================
// AUTHENTICATION HELPERS
// ======================================================
static String makeSessionKey(int index) {
  return "s" + String(index);
}

static String buildSessionCookie(const String &token) {
  String cookie = String(SESSION_COOKIE_NAME) + "=" + token;
  cookie += "; Path=/; Max-Age=" + String(SESSION_TIMEOUT / 1000UL);
  cookie += "; HttpOnly; SameSite=Lax";
  return cookie;
}

static String buildExpiredSessionCookie() {
  return String(SESSION_COOKIE_NAME) +
         "=; Path=/; Max-Age=0; HttpOnly; SameSite=Lax";
}

static String getSessionCookieToken() {
  String cookies = server.header("Cookie");
  String cookieName = String(SESSION_COOKIE_NAME) + "=";
  int start = cookies.indexOf(cookieName);

  if (start < 0) {
    return "";
  }

  start += cookieName.length();
  int end = cookies.indexOf(';', start);
  if (end < 0) {
    end = cookies.length();
  }

  String token = cookies.substring(start, end);
  token.trim();
  return token;
}

static String generateSessionToken() {
  char token[25];
  snprintf(token, sizeof(token), "%08lx%08lx%08lx",
           (unsigned long)esp_random(),
           (unsigned long)esp_random(),
           (unsigned long)millis());
  return String(token);
}

bool isAuthenticated() {
  String browserToken = getSessionCookieToken();

  if (browserToken.length() == 0) {
    return false;
  }

  sessionPrefs.begin(PREF_SESSION_NAMESPACE, true);
  int count = sessionPrefs.getInt("count", 0);

  for (int i = 0; i < count; i++) {
    String storedToken =
        sessionPrefs.getString(makeSessionKey(i).c_str(), "");

    if (storedToken == browserToken) {
      sessionPrefs.end();
      server.sendHeader("Set-Cookie", buildSessionCookie(browserToken));
      return true;
    }
  }

  sessionPrefs.end();
  return false;
}

void createSession() {
  String token = generateSessionToken();

  sessionPrefs.begin(PREF_SESSION_NAMESPACE, false);
  int count = sessionPrefs.getInt("count", 0);

  if (count >= MAX_WEB_SESSIONS) {
    for (int i = 1; i < count; i++) {
      String existingToken =
          sessionPrefs.getString(makeSessionKey(i).c_str(), "");
      sessionPrefs.putString(makeSessionKey(i - 1).c_str(), existingToken);
    }

    count = MAX_WEB_SESSIONS - 1;
  }

  sessionPrefs.putString(makeSessionKey(count).c_str(), token);
  sessionPrefs.putInt("count", count + 1);
  sessionPrefs.end();

  server.sendHeader("Set-Cookie", buildSessionCookie(token));
}

void destroySession() {
  String browserToken = getSessionCookieToken();

  sessionPrefs.begin(PREF_SESSION_NAMESPACE, false);
  int count = sessionPrefs.getInt("count", 0);

  for (int i = 0; i < count; i++) {
    String storedToken =
        sessionPrefs.getString(makeSessionKey(i).c_str(), "");

    if (storedToken != browserToken) {
      continue;
    }

    for (int j = i; j < count - 1; j++) {
      String nextToken =
          sessionPrefs.getString(makeSessionKey(j + 1).c_str(), "");
      sessionPrefs.putString(makeSessionKey(j).c_str(), nextToken);
    }

    sessionPrefs.remove(makeSessionKey(count - 1).c_str());
    sessionPrefs.putInt("count", count - 1);
    break;
  }

  sessionPrefs.end();
  server.sendHeader("Set-Cookie", buildExpiredSessionCookie());
}

// ======================================================
// INTERNAL HELPERS
// ======================================================
static bool requireAuth() {
  if (!isAuthenticated()) {
    server.sendHeader("Location", "/login");
    server.send(302, "text/plain", "");
    return false;
  }
  return true;
}

static void sendText(const String &text) {
  server.send(200, "text/plain", text);
}

// ======================================================
// SETUP & LOOP
// ======================================================
void webServerSetup() {
  const char *headerKeys[] = {"Cookie"};
  server.collectHeaders(headerKeys, 1);

  // Pages
  server.on("/", handleRoot);
  server.on("/login", HTTP_GET, handleLoginPage);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", HTTP_GET, handleLogout);

  // API
  server.on("/api/status", HTTP_GET, handleApiStatus);
  server.on("/api/users", HTTP_GET, handleApiUsers);
  server.on("/api/networks", HTTP_GET, handleApiNetworks);

  // Hardware
  server.on("/pump/on", HTTP_GET, handlePumpOn);
  server.on("/pump/off", HTTP_GET, handlePumpOff);
  server.on("/jemur/in", HTTP_GET, handleJemurIn);
  server.on("/jemur/out", HTTP_GET, handleJemurOut);
  server.on("/auto", HTTP_GET, handleAutoMode);

  // WiFi
  server.on("/savewifi", HTTP_POST, handleSaveWiFi);
  server.on("/resetwifi", HTTP_POST, handleResetWiFi);

  // User Management
  server.on("/users/add", HTTP_POST, handleAddUser);
  server.on("/users/update", HTTP_POST, handleUpdateUser);
  server.on("/users/delete", HTTP_POST, handleDeleteUser);

  // 404
  server.onNotFound([]() {
    if (!requireAuth()) return;

    server.send(404, "text/plain", "404 Not Found");
  });

  server.begin();
  Serial.println("Web server started.");
}

void webServerLoop() {
  server.handleClient();
}

// ======================================================
// PAGE HANDLERS
// ======================================================
void handleRoot() {
  if (!isAuthenticated()) {
    server.sendHeader("Location", "/login");
    server.send(302, "text/plain", "");
    return;
  }

  server.send(200, "text/html", getDashboardPage());
}

void handleLoginPage() {
  if (isAuthenticated()) {
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
    return;
  }

  server.send(200, "text/html", getLoginPage());
}

void handleLogin() {
  String username = server.arg("username");
  String password = server.arg("password");

  if (authenticateUser(username, password)) {
    createSession();

    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
    return;
  }

  server.send(200, "text/html",
              getLoginPage("Username atau password salah."));
}

void handleLogout() {
  destroySession();

  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "");
}

static String getUptimeString() {
  unsigned long seconds = millis() / 1000;
  unsigned long days = seconds / 86400;
  seconds %= 86400;
  unsigned long hours = seconds / 3600;
  seconds %= 3600;
  unsigned long minutes = seconds / 60;
  seconds %= 60;

  String result = String(days) + "d " + String(hours) + "h " +
                  String(minutes) + "m " + String(seconds) + "s";
  return result;
}

// ======================================================
// API HANDLERS
// ======================================================
void handleApiStatus() {
  if (!requireAuth()) return;

  String json = "{";
  json += "\"soilValue\":" + String(soilValue) + ",";
  json += "\"soilPercent\":" + String(soilPercent) + ",";
  json += "\"soilStatus\":\"" + soilStatus + "\",";
  json += "\"soilConnected\":" + String(soilConnected ? "true" : "false") + ",";
  json += "\"rainValue\":" + String(rainValue) + ",";
  json += "\"rainPercent\":" + String(rainPercent) + ",";
  json += "\"rainStatus\":\"" + rainStatus + "\",";
  json += "\"rainConnected\":" + String(rainConnected ? "true" : "false") + ",";
  json += "\"pumpState\":\"" + pumpState + "\",";
  json += "\"jemuranState\":\"" + jemuranState + "\",";
  json += "\"pumpTimerActive\":";
  json += (pumpTimerActive ? "true" : "false");
  json += ",";
  json += "\"pumpTimeRemaining\":" + String(getPumpTimeRemaining()) + ",";
  json += "\"uptime\":\"" + getUptimeString() + "\",";
  json += "\"manualMode\":";
  json += (manualMode ? "true" : "false");
  json += "}";

  server.send(200, "application/json", json);
}

void handleApiUsers() {
  if (!requireAuth()) return;

  server.send(200, "text/html", generateUsersTableHTML());
}

void handleApiNetworks() {
  if (!requireAuth()) return;

  String networksJSON = getAvailableNetworksJSON();
  server.send(200, "application/json", networksJSON);
}

// ======================================================
// HARDWARE COMMAND HANDLERS
// ======================================================
void handlePumpOn() {
  if (!requireAuth()) return;

  setManualMode();
  setPump(true);

  unsigned long durationMinutes = server.arg("duration").toInt();
  if (durationMinutes > 0) {
    startPumpTimer(durationMinutes * 60UL);
  } else {
    cancelPumpTimer();
  }

  sendText("Pompa ON");
}

void handlePumpOff() {
  if (!requireAuth()) return;

  setManualMode();
  setPump(false);
  cancelPumpTimer();

  sendText("Pompa OFF");
}

void handleJemurIn() {
  if (!requireAuth()) return;

  setManualMode();
  setJemuranIn();

  sendText("Jemuran IN");
}

void handleJemurOut() {
  if (!requireAuth()) return;

  setManualMode();
  setJemuranOut();

  sendText("Jemuran OUT");
}

void handleAutoMode() {
  if (!requireAuth()) return;

  setAutoMode();

  sendText("Auto Mode Enabled");
}

// ======================================================
// WIFI HANDLERS
// ======================================================
void handleSaveWiFi() {
  if (!requireAuth()) return;

  String ssid = server.arg("ssid");
  String password = server.arg("password");

  if (ssid.length() == 0) {
    server.send(400, "text/plain", "SSID tidak boleh kosong.");
    return;
  }

  saveWiFiCredentials(ssid, password);

  server.send(200, "text/plain",
              "WiFi berhasil disimpan. ESP32 akan restart.");

  delay(1000);
  ESP.restart();
}

void handleResetWiFi() {
  if (!requireAuth()) return;

  resetWiFiCredentials();

  server.send(200, "text/plain",
              "Konfigurasi WiFi dihapus. ESP32 akan restart.");

  delay(1000);
  ESP.restart();
}

// ======================================================
// USER MANAGEMENT HANDLERS
// ======================================================
void handleAddUser() {
  if (!requireAuth()) return;

  String username = server.arg("username");
  String password = server.arg("password");

  if (addUser(username, password)) {
    sendText("User berhasil ditambahkan.");
  } else {
    server.send(400, "text/plain",
                "Gagal menambahkan user.");
  }
}

void handleUpdateUser() {
  if (!requireAuth()) return;

  String indexStr = server.arg("index");
  String username = server.arg("username");
  String password = server.arg("password");

  int index = indexStr.toInt();

  if (updateUser(index, username, password)) {
    sendText("User berhasil diperbarui.");
  } else {
    server.send(400, "text/plain",
                "Gagal memperbarui user.");
  }
}

void handleDeleteUser() {
  if (!requireAuth()) return;

  String indexStr = server.arg("index");
  int index = indexStr.toInt();

  if (deleteUser(index)) {
    sendText("User berhasil dihapus.");
  } else {
    server.send(400, "text/plain",
                "Gagal menghapus user.");
  }
}