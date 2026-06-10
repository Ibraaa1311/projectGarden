// ======================================================
// web_server.h
// Deklarasi web server dan routing
// ======================================================

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>

// ======================================================
// SETUP & LOOP
// ======================================================

// Inisialisasi semua route dan menjalankan web server
void webServerSetup();

// Dipanggil di loop() untuk menangani request
void webServerLoop();

// ======================================================
// AUTHENTICATION
// ======================================================

// Mengecek apakah user sedang login
bool isAuthenticated();

// Membuat session login
void createSession();

// Menghapus session login
void destroySession();

// ======================================================
// PAGE HANDLERS
// ======================================================

// GET /
void handleRoot();

// GET /login
void handleLoginPage();

// POST /login
void handleLogin();

// GET /logout
void handleLogout();

// ======================================================
// API HANDLERS
// ======================================================

// GET /api/status
void handleApiStatus();

// GET /api/users
void handleApiUsers();

// GET /api/networks
void handleApiNetworks();

// ======================================================
// HARDWARE COMMANDS
// ======================================================

// GET /pump/on
void handlePumpOn();

// GET /pump/off
void handlePumpOff();

// GET /jemur/in
void handleJemurIn();

// GET /jemur/out
void handleJemurOut();

// GET /auto
void handleAutoMode();

// GET /sensor/reset
void handleSensorReset();

// ======================================================
// WIFI HANDLERS
// ======================================================

// POST /savewifi
void handleSaveWiFi();

// POST /resetwifi
void handleResetWiFi();

// ======================================================
// USER MANAGEMENT HANDLERS
// ======================================================

// POST /users/add
void handleAddUser();

// POST /users/update
void handleUpdateUser();

// POST /users/delete
void handleDeleteUser();

#endif // WEB_SERVER_H
