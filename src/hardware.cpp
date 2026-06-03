// ======================================================
// hardware.cpp
// Implementasi kontrol sensor dan aktuator
// ======================================================

#include "hardware.h"
#include "config.h"
#include <ESP32Servo.h>

// ======================================================
// GLOBAL VARIABLES
// ======================================================

// Nilai mentah sensor
int soilValue = 0;
int rainValue = 0;

// Persentase sensor
int soilPercent = 0;
int rainPercent = 0;

// Status sensor
String soilStatus = STATUS_DRY;
String rainStatus = STATUS_CLEAR;

// Status aktuator
String pumpState = STATUS_OFF;
String jemuranState = STATUS_OUT;

// Mode sistem
bool manualMode = false;

// Object servo
Servo jemuran;

// ======================================================
// SETUP HARDWARE
// ======================================================
void hardwareSetup() {
  // Relay pompa
  pinMode(RELAY_PUMP, OUTPUT);
  digitalWrite(RELAY_PUMP, PUMP_OFF_LEVEL);

  // Servo jemuran
  jemuran.setPeriodHertz(50);
  jemuran.attach(SERVO_PIN, 500, 2400);

  // Kondisi awal
  setPump(false);
  setJemuranOut();

  Serial.println("Hardware initialized.");
}

// ======================================================
// UPDATE HARDWARE
// ======================================================
void hardwareUpdate() {
  // 1. Baca sensor
  readSensors();

  // 2. Hitung persentase
  calculatePercentages();

  // 3. Update status
  updateSensorStatus();

  // 4. Jalankan otomatisasi jika AUTO mode
  if (!manualMode) {
    runAutomation();
  }
}

// ======================================================
// READ SENSOR VALUES
// ======================================================
void readSensors() {
  soilValue = analogRead(SOIL_PIN);
  rainValue = analogRead(RAIN_PIN);
}

// ======================================================
// CALCULATE PERCENTAGES
// ======================================================
//
// Catatan:
// - ADC ESP32 berkisar 0 - 4095
// - Persentase di sini hanya estimasi visual
//
void calculatePercentages() {
  // Soil:
  // Nilai tinggi = DRY (100%)
  // Nilai rendah = WET (0%)
  soilPercent = map(soilValue, 0, 4095, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // Rain:
  // Nilai rendah = hujan deras (100%)
  // Nilai tinggi = tidak hujan (0%)
  rainPercent = map(rainValue, 4095, 0, 0, 100);
  rainPercent = constrain(rainPercent, 0, 100);
}

// ======================================================
// UPDATE STATUS
// ======================================================
void updateSensorStatus() {
  // Soil status berdasarkan persentase
  if (soilPercent < 50) {
    soilStatus = STATUS_DRY;
  } else {
    soilStatus = STATUS_WET;
  }

  // Rain status dengan sensitivitas paling tinggi
  if (rainValue <= RAIN_THRESHOLD) {
    rainStatus = STATUS_RAINING;
  } else {
    rainStatus = STATUS_CLEAR;
  }
}

// ======================================================
// ACTUATOR CONTROL
// ======================================================
void setPump(bool on) {
  digitalWrite(RELAY_PUMP, on ? PUMP_ON_LEVEL : PUMP_OFF_LEVEL);
  pumpState = on ? STATUS_ON : STATUS_OFF;
}

void setJemuranIn() {
  jemuran.write(SERVO_IN_ANGLE);
  jemuranState = STATUS_IN;
}

void setJemuranOut() {
  jemuran.write(SERVO_OUT_ANGLE);
  jemuranState = STATUS_OUT;
}

// ======================================================
// MODE CONTROL
// ======================================================
void setManualMode() {
  manualMode = true;
}

void setAutoMode() {
  manualMode = false;
}

// ======================================================
// AUTOMATION LOGIC
// ======================================================
void runAutomation() {
  // Pompa:
  // Tanah kering -> ON
  // Tanah basah -> OFF
  if (soilStatus == STATUS_DRY) {
    setPump(true);
  } else {
    setPump(false);
  }

  // Jemuran:
  // Hujan -> IN
  // Cerah -> OUT
  if (rainStatus == STATUS_RAINING) {
    setJemuranIn();
  } else {
    setJemuranOut();
  }
}

// ======================================================
// HELPER FUNCTIONS
// ======================================================
String getModeString() {
  return manualMode ? "MANUAL" : "AUTO";
}

// ======================================================
// SERIAL DEBUG
// ======================================================
void printSystemStatus() {
  Serial.println("========================================");
  Serial.print("Soil Value     : ");
  Serial.println(soilValue);

  Serial.print("Soil Percent   : ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Soil Status    : ");
  Serial.println(soilStatus);

  Serial.print("Rain Value     : ");
  Serial.println(rainValue);

  Serial.print("Rain Percent   : ");
  Serial.print(rainPercent);
  Serial.println("%");

  Serial.print("Rain Status    : ");
  Serial.println(rainStatus);

  Serial.print("Pump State     : ");
  Serial.println(pumpState);

  Serial.print("Jemuran State  : ");
  Serial.println(jemuranState);

  Serial.print("Mode           : ");
  Serial.println(getModeString());
}