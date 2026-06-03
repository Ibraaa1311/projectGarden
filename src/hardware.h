// ======================================================
// hardware.h
// Deklarasi fungsi dan variabel hardware
// ======================================================

#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

// ======================================================
// GLOBAL STATUS VARIABLES
// ======================================================
// Nilai mentah sensor
extern int soilValue;
extern int rainValue;

// Persentase sensor (0 - 100)
extern int soilPercent;
extern int rainPercent;

// Status sensor
extern String soilStatus;      // DRY / WET
extern String rainStatus;      // RAINING / CLEAR

// Status aktuator
extern String pumpState;       // ON / OFF
extern String jemuranState;    // IN / OUT

// Mode sistem
extern bool manualMode;        // true = MANUAL, false = AUTO

// ======================================================
// SETUP & UPDATE
// ======================================================

// Inisialisasi seluruh hardware
void hardwareSetup();

// Membaca sensor dan menjalankan logika otomatis
void hardwareUpdate();

// ======================================================
// SENSOR FUNCTIONS
// ======================================================

// Membaca nilai analog sensor
void readSensors();

// Menghitung persentase sensor
void calculatePercentages();

// Menentukan status DRY/WET dan RAINING/CLEAR
void updateSensorStatus();

// ======================================================
// ACTUATOR FUNCTIONS
// ======================================================

// Kontrol pompa
void setPump(bool on);

// Kontrol jemuran
void setJemuranIn();
void setJemuranOut();

// ======================================================
// MODE CONTROL
// ======================================================

// Mengaktifkan mode manual
void setManualMode();

// Mengaktifkan mode otomatis
void setAutoMode();

// ======================================================
// AUTOMATION
// ======================================================

// Menjalankan logika otomatis berdasarkan sensor
void runAutomation();

// ======================================================
// HELPER FUNCTIONS
// ======================================================

// Mendapatkan mode sebagai string
String getModeString();

// Debug ke Serial Monitor
void printSystemStatus();

#endif // HARDWARE_H