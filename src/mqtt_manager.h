#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>

// ========================================
// MQTT SETUP
// ========================================
void mqttSetup();

// ========================================
// MQTT LOOP
// ========================================
void mqttLoop();

// ========================================
// PUBLISH STATUS
// ========================================
void mqttPublishStatus();

// ========================================
// CONNECTION STATUS
// ========================================
bool mqttConnected();

#endif