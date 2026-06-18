#include "mqtt_manager.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include "hardware.h"
#include "config.h"

// ========================================
// WIFI CLIENT
// ========================================
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ========================================
// MQTT CONFIG
// ========================================
const char* MQTT_SERVER = "154.19.38.42";
const int   MQTT_PORT   = 1883;

const char* MQTT_USER = "iot-pagi";
const char* MQTT_PASS = "iot-pagi-pass";

// ========================================
// MQTT TOPICS
// ========================================

// PUBLISH
const char* TOPIC_SOIL_STATE =
  "smarthome/pagi/taman/tanah/state";

const char* TOPIC_RAIN_STATE =
  "smarthome/pagi/taman/hujan/state";

// SENSOR HEALTH
const char* TOPIC_SOIL_HEALTH =
  "smarthome/pagi/taman/tanah/health";

const char* TOPIC_RAIN_HEALTH =
  "smarthome/pagi/taman/hujan/health";

// ACTUATOR STATE
const char* TOPIC_PUMP_STATE =
  "smarthome/pagi/taman/pompa/state";

const char* TOPIC_JEMURAN_STATE =
  "smarthome/pagi/taman/jemuran/state";

// SUBSCRIBE
const char* TOPIC_PUMP_SET =
  "smarthome/pagi/taman/pompa/set";

const char* TOPIC_JEMURAN_SET =
  "smarthome/pagi/taman/jemuran/set";

const char* TOPIC_FIRE_STATE =
  "smarthome/pagi/dapur/api/state";

// ========================================
// PREVIOUS STATE (untuk deteksi perubahan)
// ========================================
static String prev_soilStatus   = "";
static String prev_rainStatus   = "";
static String prev_pumpState    = "";
static String prev_jemuranState = "";
static int    prev_soilConnected  = -1;
static int    prev_rainConnected  = -1;

// Timestamp saat terakhir connect ke MQTT.
// Pesan retained dari broker tiba dalam milidetik pertama
// setelah subscribe — kita abaikan command selama
// MQTT_RETAIN_IGNORE_MS setelah connect.
static unsigned long mqttConnectedAt      = 0;
#define MQTT_RETAIN_IGNORE_MS 2000UL

// ========================================
// MQTT CALLBACK
// ========================================
void mqttCallback(char* topic,
                  byte* payload,
                  unsigned int length) {

  String message;

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  String topicStr = String(topic);

  Serial.println("========== MQTT ==========");
  Serial.print("Topic   : ");
  Serial.println(topicStr);

  Serial.print("Message : ");
  Serial.println(message);

  // ====================================
  // PUMP CONTROL
  // ====================================
  if (topicStr == TOPIC_PUMP_SET) {

    // Abaikan retained message yang datang saat baru connect
    if (millis() - mqttConnectedAt < MQTT_RETAIN_IGNORE_MS) {
      Serial.println("[MQTT] Ignored retained message (pump)");
      return;
    }

    setManualMode();

    if (message == "ON") {
      setPump(true);
      Serial.println("Pump ON from MQTT");
    }

    else if (message == "OFF") {
      setPump(false);
      Serial.println("Pump OFF from MQTT");
    }
  }

  // ====================================
  // JEMURAN CONTROL
  // ====================================
  else if (topicStr == TOPIC_JEMURAN_SET) {

    // Abaikan retained message yang datang saat baru connect
    if (millis() - mqttConnectedAt < MQTT_RETAIN_IGNORE_MS) {
      Serial.println("[MQTT] Ignored retained message (jemuran)");
      return;
    }

    setManualMode();

    if (message == "IN") {
      setJemuranIn();
      Serial.println("Jemuran IN from MQTT");
    }

    else if (message == "OUT") {
      setJemuranOut();
      Serial.println("Jemuran OUT from MQTT");
    }
  }

  // ====================================
  // FIRE DETECTION
  // ====================================
  else if (topicStr == TOPIC_FIRE_STATE) {

    if (message == "FIRE") {

      Serial.println("FIRE DETECTED!");

      // Paksa manual mode
      setManualMode();

      // Nyalakan pompa
      setPump(true);

      Serial.println("Emergency System Activated");
    }
    else if (message == "SAFE") {
      Serial.println("Environment SAFE");

      // Kembali ke auto mode
      setAutoMode();

      Serial.println("Auto Mode Restored");
    }

  }

  mqttPublishStatus();
}

// ========================================
// MQTT RECONNECT
// ========================================
void reconnectMQTT() {

  while (!mqttClient.connected()) {

    Serial.println("Connecting to MQTT...");

    String clientId =
      "ESP32-TAMAN-" + String(random(0xffff), HEX);

    bool connected = mqttClient.connect(
      clientId.c_str(),
      MQTT_USER,
      MQTT_PASS
    );

    if (connected) {

      Serial.println("MQTT Connected");

      // Reset state cache supaya semua topic langsung
      // di-publish ulang ke broker setelah reconnect
      prev_soilStatus   = "";
      prev_rainStatus   = "";
      prev_pumpState    = "";
      prev_jemuranState = "";
      prev_soilConnected  = -1;
      prev_rainConnected  = -1;

      // Catat waktu connect untuk filter retained message
      mqttConnectedAt = millis();

      // ================================
      // SUBSCRIBE TOPICS
      // ================================
      mqttClient.subscribe(TOPIC_PUMP_SET);
      mqttClient.subscribe(TOPIC_JEMURAN_SET);
      mqttClient.subscribe(TOPIC_FIRE_STATE);

      Serial.println("MQTT Subscribe Success");

      mqttPublishStatus();

    } else {

      Serial.print("MQTT Failed, rc=");
      Serial.println(mqttClient.state());

      Serial.println("Retry MQTT in 5 seconds...");
      delay(5000);
    }
  }
}

// ========================================
// MQTT SETUP
// ========================================
void mqttSetup() {

  mqttClient.setServer(
    MQTT_SERVER,
    MQTT_PORT
  );

  mqttClient.setCallback(mqttCallback);

  Serial.println("MQTT Setup Done");
}

// ========================================
// MQTT LOOP
// ========================================
void mqttLoop() {

  // WiFi belum connect
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  // MQTT reconnect
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  mqttClient.loop();
}

// ========================================
// MQTT PUBLISH STATUS
// ========================================
void mqttPublishStatus() {

  if (!mqttClient.connected()) {
    return;
  }

  bool anyChange = false;

  // ====================================
  // TANAH STATE
  // ====================================
  if (soilStatus != prev_soilStatus) {
    mqttClient.publish(TOPIC_SOIL_STATE, soilStatus.c_str(), true);
    Serial.print("[MQTT] Tanah : ");
    Serial.println(soilStatus);
    prev_soilStatus = soilStatus;
    anyChange = true;
  }

  // ====================================
  // TANAH HEALTH
  // ====================================
  int soilConn = soilConnected ? 1 : 0;
  if (soilConn != prev_soilConnected) {
    mqttClient.publish(TOPIC_SOIL_HEALTH, soilConnected ? "OK" : "DISCONNECT", true);
    Serial.print("[MQTT] Tanah Health : ");
    Serial.println(soilConnected ? "OK" : "DISCONNECT");
    prev_soilConnected = soilConn;
    anyChange = true;
  }

  // ====================================
  // HUJAN STATE
  // ====================================
  if (rainStatus != prev_rainStatus) {
    mqttClient.publish(TOPIC_RAIN_STATE, rainStatus.c_str(), true);
    Serial.print("[MQTT] Hujan : ");
    Serial.println(rainStatus);
    prev_rainStatus = rainStatus;
    anyChange = true;
  }

  // ====================================
  // HUJAN HEALTH
  // ====================================
  int rainConn = rainConnected ? 1 : 0;
  if (rainConn != prev_rainConnected) {
    mqttClient.publish(TOPIC_RAIN_HEALTH, rainConnected ? "OK" : "DISCONNECT", true);
    Serial.print("[MQTT] Hujan Health : ");
    Serial.println(rainConnected ? "OK" : "DISCONNECT");
    prev_rainConnected = rainConn;
    anyChange = true;
  }

  // ====================================
  // PUMP STATE
  // ====================================
  if (pumpState != prev_pumpState) {
    mqttClient.publish(TOPIC_PUMP_STATE, pumpState.c_str(), true);
    Serial.print("[MQTT] Pompa : ");
    Serial.println(pumpState);
    prev_pumpState = pumpState;
    anyChange = true;
  }

  // ====================================
  // JEMURAN STATE
  // ====================================
  if (jemuranState != prev_jemuranState) {
    mqttClient.publish(TOPIC_JEMURAN_STATE, jemuranState.c_str(), true);
    Serial.print("[MQTT] Jemuran : ");
    Serial.println(jemuranState);
    prev_jemuranState = jemuranState;
    anyChange = true;
  }

  if (anyChange) {
    Serial.println("[MQTT] Publish selesai.");
  }
}

// ========================================
// MQTT STATUS
// ========================================
bool mqttConnected() {
  return mqttClient.connected();
}