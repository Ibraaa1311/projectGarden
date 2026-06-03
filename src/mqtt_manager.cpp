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
  "smarthome/dapur/api/state";

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

  // ====================================
  // TANAH
  // ====================================
  mqttClient.publish(
    TOPIC_SOIL_STATE,
    soilStatus.c_str(),
    true
  );

  // ====================================
  // HUJAN
  // ====================================
  mqttClient.publish(
    TOPIC_RAIN_STATE,
    rainStatus.c_str(),
    true
  );
  
  // PUMP STATE
  mqttClient.publish(
    TOPIC_PUMP_STATE,
    pumpState.c_str(),
    true
  );
  
  // JEMURAN STATE
  mqttClient.publish(
    TOPIC_JEMURAN_STATE,
    jemuranState.c_str(),
    true
  );

  Serial.println("MQTT Status Published");

  Serial.print("Tanah : ");
  Serial.println(soilStatus);

  Serial.print("Hujan : ");
  Serial.println(rainStatus);
}

// ========================================
// MQTT STATUS
// ========================================
bool mqttConnected() {
  return mqttClient.connected();
}