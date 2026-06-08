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
int rainDigitalValue = HIGH;

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

// Status kesehatan sensor dan timer pompa manual
bool soilSensorFault = false;
bool rainSensorFault = false;
bool pumpTimerActive = false;
unsigned long pumpTimeRemaining = 0;

// Object servo
Servo jemuran;

// Timer internal untuk deteksi fault dan fail-safe pompa
static unsigned long soilExtremeStart = 0;
static unsigned long rainExtremeStart = 0;
static unsigned long rainConflictStart = 0;
static unsigned long autoPumpNoChangeStart = 0;
static unsigned long pumpStartedAt = 0;
static unsigned long manualPumpTimerEnd = 0;
static int autoPumpStartPercent = 0;
static bool autoPumpTracking = false;

// ======================================================
// SETUP HARDWARE
// ======================================================
void hardwareSetup() {
  // Relay pompa
  pinMode(RELAY_PUMP, OUTPUT);
  digitalWrite(RELAY_PUMP, PUMP_OFF_LEVEL);

  // Digital output rain sensor
  pinMode(RAIN_DO_PIN, INPUT);

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

  // 5. Pantau kesehatan sensor berdasarkan kondisi aktuator terbaru
  updateSensorHealth();

  // 6. Pantau timer manual dan batas maksimum pompa
  updatePumpFailsafe();
}

// ======================================================
// READ SENSOR VALUES
// ======================================================
void readSensors() {
  soilValue = analogRead(SOIL_PIN);
  rainValue = analogRead(RAIN_PIN);
  rainDigitalValue = digitalRead(RAIN_DO_PIN);
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
// SENSOR HEALTH MONITORING
// ======================================================
void updateSensorHealth() {
  unsigned long now = millis();

  // Soil fault: nilai ADC ekstrem terus menerus selama 30 detik.
  bool soilExtreme = (soilValue <= 5 || soilValue >= 4090);
  if (soilExtreme) {
    if (soilExtremeStart == 0) soilExtremeStart = now;
    if (!soilSensorFault &&
        now - soilExtremeStart >= SENSOR_FAULT_TIME) {
      soilSensorFault = true;
      setPump(false);
      Serial.println("FAILSAFE: Soil Sensor Fault Detected");
    }
  } else {
    soilExtremeStart = 0;
  }

  // Soil fault: pompa otomatis ON tetapi kelembapan tidak berubah > 3%.
  bool autoPumpOn = (!manualMode && pumpState == STATUS_ON);
  if (autoPumpOn) {
    if (!autoPumpTracking) {
      autoPumpTracking = true;
      autoPumpNoChangeStart = now;
      autoPumpStartPercent = soilPercent;
    }

    if (abs(soilPercent - autoPumpStartPercent) > 3) {
      autoPumpNoChangeStart = now;
      autoPumpStartPercent = soilPercent;
    }

    if (!soilSensorFault &&
        now - autoPumpNoChangeStart >= SOIL_NO_CHANGE_TIME) {
      soilSensorFault = true;
      setPump(false);
      Serial.println("FAILSAFE: Soil Sensor Fault Detected");
    }
  } else {
    autoPumpTracking = false;
    autoPumpNoChangeStart = 0;
  }

  bool rainDoRaining = (rainDigitalValue == RAIN_DO_RAIN_LEVEL);

  // Rain fault: nilai AO ekstrem hanya dianggap fault jika berlawanan
  // dengan DO, agar kondisi clear normal di nilai ADC tinggi tidak false alarm.
  bool rainExtreme = (rainValue <= 5 && !rainDoRaining) ||
                     (rainValue >= 4090 && rainDoRaining);
  if (rainExtreme) {
    if (rainExtremeStart == 0) rainExtremeStart = now;
  } else {
    rainExtremeStart = 0;
  }

  // Rain fault: AO dan DO bertentangan saat AO jelas basah/kering.
  // Area tengah diabaikan agar noise analog tidak mudah menjadi fault.
  bool rainAoRaining = (rainPercent >= RAIN_CONFLICT_WET_PERCENT);
  bool rainAoClear = (rainPercent <= RAIN_CONFLICT_DRY_PERCENT);
  bool rainConflict = (rainAoRaining && !rainDoRaining) ||
                      (rainAoClear && rainDoRaining);
  if (rainConflict) {
    if (rainConflictStart == 0) rainConflictStart = now;
  } else {
    rainConflictStart = 0;
  }

  if (!rainSensorFault &&
      ((rainExtremeStart != 0 &&
        now - rainExtremeStart >= SENSOR_FAULT_TIME) ||
       (rainConflictStart != 0 &&
        now - rainConflictStart >= SENSOR_FAULT_TIME))) {
    rainSensorFault = true;
    Serial.println("FAILSAFE: Rain Sensor Fault Detected");
  }
}

// ======================================================
// ACTUATOR CONTROL
// ======================================================
void setPump(bool on) {
  digitalWrite(RELAY_PUMP, on ? PUMP_ON_LEVEL : PUMP_OFF_LEVEL);
  pumpState = on ? STATUS_ON : STATUS_OFF;

  // Catat awal pompa menyala untuk pump timeout.
  if (on && pumpStartedAt == 0) {
    pumpStartedAt = millis();
  } else if (!on) {
    pumpStartedAt = 0;
    cancelManualPumpTimer();
  }
}

void startManualPumpTimer(unsigned long durationMs) {
  if (durationMs == 0) durationMs = DEFAULT_MANUAL_PUMP_TIME;

  // Timer manual hanya mengatur durasi ON, kontrol manual tetap diizinkan.
  setManualMode();
  pumpTimerActive = true;
  manualPumpTimerEnd = millis() + durationMs;
  pumpTimeRemaining = durationMs;
  setPump(true);
}

void cancelManualPumpTimer() {
  pumpTimerActive = false;
  pumpTimeRemaining = 0;
  manualPumpTimerEnd = 0;
}

void updatePumpFailsafe() {
  unsigned long now = millis();

  // Countdown timer manual pompa.
  if (pumpTimerActive) {
    if (pumpState != STATUS_ON) {
      cancelManualPumpTimer();
    } else if ((long)(manualPumpTimerEnd - now) <= 0) {
      setPump(false);
      Serial.println("Manual Pump Timer Expired");
    } else {
      pumpTimeRemaining = manualPumpTimerEnd - now;
    }
  }

  // Fail-safe maksimum runtime pompa untuk mode manual dan otomatis.
  if (pumpState == STATUS_ON && pumpStartedAt != 0 &&
      now - pumpStartedAt >= MAX_PUMP_RUNTIME) {
    setPump(false);
    setManualMode();
    Serial.println("FAILSAFE: Pump Timeout");
  }
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
  cancelManualPumpTimer();
  manualMode = false;
}

// ======================================================
// AUTOMATION LOGIC
// ======================================================
void runAutomation() {
  // Pompa:
  // Tanah kering -> ON
  // Tanah basah -> OFF
  // Auto watering dikunci saat soil sensor fault.
  if (soilSensorFault) {
    setPump(false);
  } else if (soilStatus == STATUS_DRY) {
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

void resetSensorFaults() {
  // Reset fault manual dari dashboard.
  soilSensorFault = false;
  rainSensorFault = false;
  soilExtremeStart = 0;
  rainExtremeStart = 0;
  rainConflictStart = 0;
  autoPumpNoChangeStart = 0;
  autoPumpTracking = false;
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

  Serial.print("Soil Fault     : ");
  Serial.println(soilSensorFault ? "FAULT" : "NORMAL");

  Serial.print("Rain Fault     : ");
  Serial.println(rainSensorFault ? "FAULT" : "NORMAL");
}
