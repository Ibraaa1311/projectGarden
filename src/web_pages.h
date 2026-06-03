// ======================================================
// web_pages.h
// Deklarasi fungsi generator HTML
// ======================================================

#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <Arduino.h>

// ======================================================
// PAGE GENERATORS
// ======================================================

// Halaman login
String getLoginPage(const String &errorMessage = "");

// Halaman dashboard utama
String getDashboardPage();

// ======================================================
// PARTIAL GENERATORS
// ======================================================

// CSS utama
String getGlobalStyles();

// JavaScript utama
String getGlobalScripts();

// Sidebar navigation
String getSidebarHTML();

// Section Dashboard
String getDashboardSectionHTML();

// Section Konfigurasi WiFi
String getWiFiSectionHTML();

// Section Manajemen User
String getUserManagementSectionHTML();

// ======================================================
// HELPERS
// ======================================================

// Escape string agar aman untuk HTML
String htmlEscape(const String &input);

#endif // WEB_PAGES_H