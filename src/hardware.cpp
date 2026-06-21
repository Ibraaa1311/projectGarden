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

// Health sensor
bool soilConnected = true;
bool rainConnected = true;

// Status aktuator
String pumpState = STATUS_OFF;
String jemuranState = STATUS_OUT;

// Mode sistem
bool manualMode = false;

// Timer pompa manual
bool pumpTimerActive = false;
unsigned long pumpTimerStart = 0;
unsigned long pumpTimerDuration = 0;

// Object servo
Servo jemuran;

static void updatePumpTimer();

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

  // 2. Cek koneksi fisik sensor
  checkSensorHealth();

  // 3. Hitung persentase
  calculatePercentages();

  // 4. Update status
  updateSensorStatus();

  // 5. Jalankan otomatisasi jika AUTO mode
  if (!manualMode) {
    runAutomation();
  } else {
    updatePumpTimer();
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
void calculatePercentages() {
  // Soil:
  soilPercent = map(soilValue, SOIL_DRY_ADC, SOIL_WET_ADC, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // Rain:
  rainPercent = map(rainValue, 4095, 0, 0, 100);
  rainPercent = constrain(rainPercent, 0, 100);
}

// ======================================================
// SENSOR HEALTH CHECK
// ======================================================
// Cek apakah pin sensor "floating" (lepas) dengan mengukur variansi ADC dalam beberapa sampel.
// Jika variansi tinggi, kemungkinan pin lepas → sensor disconnect atau rusak.
static bool isFloatingPin(uint8_t pin) {
  int samples[SENSOR_SAMPLE_COUNT];
  for (int i = 0; i < SENSOR_SAMPLE_COUNT; i++) {
    samples[i] = analogRead(pin);
    delay(2);
  }

  int minVal = samples[0];
  int maxVal = samples[0];
  for (int i = 1; i < SENSOR_SAMPLE_COUNT; i++) {
    if (samples[i] < minVal) minVal = samples[i];
    if (samples[i] > maxVal) maxVal = samples[i];
  }

  int variance = maxVal - minVal;

  return (variance >= SENSOR_VARIANCE_THRESHOLD);
}

void checkSensorHealth() {
  // --- Soil Sensor ---
  soilConnected = !isFloatingPin(SOIL_PIN);
  if (!soilConnected) {
    Serial.println("[WARN] Soil sensor DISCONNECT atau RUSAK!");
  }

  // --- Rain Sensor ---
  rainConnected = !isFloatingPin(RAIN_PIN);
  if (!rainConnected) {
    Serial.println("[WARN] Rain sensor DISCONNECT atau RUSAK!");
  }
}

// ======================================================
// UPDATE STATUS
// ======================================================
void updateSensorStatus() {
  // Soil status
  if (soilPercent < 35) {
    soilStatus = STATUS_DRY;
  } else {
    soilStatus = STATUS_WET;
  }

  // Rain status
  if (rainValue <= RAIN_THRESHOLD) {
    rainStatus = STATUS_RAINING;
  } else {
    rainStatus = STATUS_CLEAR;
  }
}

// ======================================================
// ACTUATOR CONTROL
// ======================================================
// Pompa:
void setPump(bool on) {
  digitalWrite(RELAY_PUMP, on ? PUMP_ON_LEVEL : PUMP_OFF_LEVEL);
  pumpState = on ? STATUS_ON : STATUS_OFF;
}

void startPumpTimer(unsigned long durationSeconds) {
  if (!manualMode || durationSeconds == 0) {
    cancelPumpTimer();
    return;
  }

  pumpTimerDuration = durationSeconds * 1000UL;
  pumpTimerStart = millis();
  pumpTimerActive = true;
}

void cancelPumpTimer() {
  pumpTimerActive = false;
  pumpTimerStart = 0;
  pumpTimerDuration = 0;
}

unsigned long getPumpTimeRemaining() {
  if (!pumpTimerActive || pumpTimerDuration == 0) {
    return 0;
  }

  unsigned long elapsed = millis() - pumpTimerStart;
  if (elapsed >= pumpTimerDuration) {
    return 0;
  }

  unsigned long remainingMs = pumpTimerDuration - elapsed;
  return (remainingMs + 999UL) / 1000UL;
}

// Jemuran Masuk:
void setJemuranIn() {
  jemuran.write(SERVO_IN_ANGLE);
  jemuranState = STATUS_IN;
}

// Jemuran Keluar:
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
  cancelPumpTimer();
}

// ======================================================
// AUTOMATION LOGIC
// ======================================================
void runAutomation() {
  // Pompa:
  // Tanah kering -> ON
  // Tanah basah  -> OFF
  // Sensor rusak -> OFF
  if (!soilConnected) {
    setPump(false);
  } else if (soilStatus == STATUS_DRY) {
    setPump(true);
  } else {
    setPump(false);
  }

  // Jemuran:
  // Hujan        -> IN
  // Cerah        -> OUT
  // Sensor rusak -> IN
  if (!rainConnected) {
    setJemuranIn();
  } else if (rainStatus == STATUS_RAINING) {
    setJemuranIn();
  } else {
    setJemuranOut();
  }
}

static void updatePumpTimer() {
  if (!pumpTimerActive) {
    return;
  }

  if (getPumpTimeRemaining() == 0) {
    setPump(false);
    cancelPumpTimer();
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

  Serial.print("Soil Connected : ");
  Serial.println(soilConnected ? "YES" : "NO - DISCONNECT!");

  Serial.print("Rain Value     : ");
  Serial.println(rainValue);

  Serial.print("Rain Percent   : ");
  Serial.print(rainPercent);
  Serial.println("%");

  Serial.print("Rain Status    : ");
  Serial.println(rainStatus);

  Serial.print("Rain Connected : ");
  Serial.println(rainConnected ? "YES" : "NO - DISCONNECT!");

  Serial.print("Pump State     : ");
  Serial.println(pumpState);

  Serial.print("Jemuran State  : ");
  Serial.println(jemuranState);

  Serial.print("Mode           : ");
  Serial.println(getModeString());
}