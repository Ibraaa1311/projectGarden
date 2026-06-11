// ======================================================
// web_pages.cpp
// Generator HTML untuk Login dan Dashboard
// ======================================================

#include "web_pages.h"

#include "config.h"
#include "hardware.h"
#include "wifi_manager.h"
#include "user_manager.h"

// ======================================================
// HELPER: HTML ESCAPE
// ======================================================
String htmlEscape(const String &input) {
  String output = input;
  output.replace("&", "&amp;");
  output.replace("<", "&lt;");
  output.replace(">", "&gt;");
  output.replace("\"", "&quot;");
  output.replace("'", "&#39;");
  return output;
}

// ======================================================
// SVG ICONS
// ======================================================

String getSvgIconDroplet() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="48" height="48" fill="#f9a850" xmlns="http://www.w3.org/2000/svg">
      <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.32 0z"/>
    </svg>
  )rawliteral";
}

String getSvgIconSun() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="48" height="48" fill="#3b82f6" xmlns="http://www.w3.org/2000/svg">
      <circle cx="12" cy="12" r="5"/>
      <line x1="12" y1="1" x2="12" y2="3" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="12" y1="21" x2="12" y2="23" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="4.22" y1="4.22" x2="5.64" y2="5.64" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="18.36" y1="18.36" x2="19.78" y2="19.78" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="1" y1="12" x2="3" y2="12" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="21" y1="12" x2="23" y2="12" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="4.22" y1="19.78" x2="5.64" y2="18.36" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
      <line x1="18.36" y1="5.64" x2="19.78" y2="4.22" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/>
    </svg>
  )rawliteral";
}

String getSvgIconArrowRight() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="32" height="32" fill="none" stroke="white" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <line x1="5" y1="12" x2="19" y2="12"/>
      <polyline points="12 5 19 12 12 19"/>
    </svg>
  )rawliteral";
}

String getSvgIconArrowLeft() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="32" height="32" fill="none" stroke="white" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <line x1="19" y1="12" x2="5" y2="12"/>
      <polyline points="12 19 5 12 12 5"/>
    </svg>
  )rawliteral";
}

String getSvgIconPower() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="32" height="32" fill="none" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <path d="M18.36 6.64a9 9 0 1 1-12.73 0M12 2v10"/>
    </svg>
  )rawliteral";
}

String getSvgIconCircle() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="32" height="32" fill="white" xmlns="http://www.w3.org/2000/svg">
      <circle cx="12" cy="12" r="10"/>
    </svg>
  )rawliteral";
}

String getSvgIconLock() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
      <rect x="3" y="11" width="18" height="11" rx="2" fill="none" stroke="#999" stroke-width="2"/>
      <path d="M7 11V7a5 5 0 0 1 10 0v4" fill="none" stroke="#999" stroke-width="2" stroke-linecap="round"/>
    </svg>
  )rawliteral";
}

String getSvgIconSignal(int bars) {
  if (bars > 4) bars = 4;
  if (bars < 0) bars = 0;

  String svg = R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" xmlns="http://www.w3.org/2000/svg">
  )rawliteral";

  // Bar 1 (bottom)
  String color1 = (bars >= 1) ? "#1b6b3d" : "#ddd";
  svg += "<rect x='3' y='18' width='3' height='4' fill='" + color1 + "'/>";

  // Bar 2
  String color2 = (bars >= 2) ? "#1b6b3d" : "#ddd";
  svg += "<rect x='8' y='14' width='3' height='8' fill='" + color2 + "'/>";

  // Bar 3
  String color3 = (bars >= 3) ? "#1b6b3d" : "#ddd";
  svg += "<rect x='13' y='10' width='3' height='12' fill='" + color3 + "'/>";

  // Bar 4 (top)
  String color4 = (bars >= 4) ? "#1b6b3d" : "#ddd";
  svg += "<rect x='18' y='6' width='3' height='16' fill='" + color4 + "'/>";

  svg += "</svg>";
  return svg;
}

String getSvgIconMap() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="#999" stroke-width="2" xmlns="http://www.w3.org/2000/svg">
      <path d="M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z"/>
      <circle cx="12" cy="10" r="3"/>
    </svg>
  )rawliteral";
}

String getSvgIconWiFi() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="#1b6b3d" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <path d="M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0"/>
      <line x1="12" y1="20" x2="12.01" y2="20"/>
    </svg>
  )rawliteral";
}

String getSvgIconClock() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="#999" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <circle cx="12" cy="12" r="10"/>
      <polyline points="12 6 12 12 16 14"/>
    </svg>
  )rawliteral";
}

String getSvgIconInfo() {
  return R"rawliteral(
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="#1b6b3d" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" xmlns="http://www.w3.org/2000/svg">
      <circle cx="12" cy="12" r="10"/>
      <line x1="12" y1="16" x2="12" y2="12"/>
      <line x1="12" y1="8" x2="12.01" y2="8"/>
    </svg>
  )rawliteral";
}

// ======================================================
// GLOBAL STYLES
// ======================================================
String getGlobalStyles() {
  return R"rawliteral(
<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

html {
  scroll-behavior: smooth;
}

body {
  font-family: 'Segoe UI', Arial, sans-serif;
  background: #f5f5f5;
  color: #333;
}

/* ================= HEADER ================= */
.header {
  background: linear-gradient(135deg, #1b6b3d 0%, #228b4a 100%);
  color: white;
  padding: 16px 24px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  box-shadow: 0 2px 8px rgba(0,0,0,0.1);
}

.header-brand {
  display: flex;
  align-items: center;
  gap: 12px;
  font-size: 20px;
  font-weight: 700;
}

.header-brand svg {
  width: 24px;
  height: 24px;
}

.header-device {
  background: rgba(255,255,255,0.2);
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 13px;
  font-weight: 600;
}

.header-actions {
  display: flex;
  gap: 12px;
  align-items: center;
}

.header-link {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  padding: 8px 14px;
  border-radius: 999px;
  text-decoration: none;
  color: white;
  background: rgba(255,255,255,0.18);
  font-size: 13px;
  font-weight: 600;
  transition: background 0.2s ease;
}

.header-link:hover {
  background: rgba(255,255,255,0.28);
}

/* Bottom nav hidden by default; shown on small screens via media query */
.bottom-nav { display: none; }

.container {
  display: flex;
  flex-wrap: wrap;
  gap: 24px;
  min-height: calc(100vh - 56px);
}

.login-wrapper {
  min-height: 100vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;
  background: #f5f5f5;
}

.login-card {
  width: 100%;
  max-width: 420px;
  background: white;
  padding: 40px;
  border-radius: 12px;
  box-shadow: 0 4px 16px rgba(0,0,0,0.08);
}

.login-card h1 {
  font-size: 28px;
  margin-bottom: 8px;
  color: #1b6b3d;
}

.login-card p {
  color: #666;
  margin-bottom: 24px;
}

.error-box {
  background: #fee;
  color: #c33;
  border: 1px solid #fcc;
  padding: 12px;
  border-radius: 8px;
  margin-bottom: 16px;
  font-size: 14px;
}

/* ================= SIDEBAR ================= */
.sidebar {
  width: 280px;
  background: #f0f8f4;
  border-right: 1px solid #ddd;
  padding: 24px 0;
  min-height: calc(100vh - 56px);
  overflow-y: auto;
}

.brand {
  font-size: 16px;
  font-weight: 700;
  margin-bottom: 24px;
  padding: 0 24px;
  color: #1b6b3d;
}

.menu-title {
  font-size: 12px;
  color: #999;
  margin: 16px 24px 8px;
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.nav {
  display: flex;
  flex-direction: column;
  gap: 0;
}

.nav a {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 12px 24px;
  text-decoration: none;
  color: #333;
  transition: all 0.2s;
  border-left: 3px solid transparent;
  font-size: 14px;
}

.nav a:hover {
  background: #e8f3ed;
  color: #1b6b3d;
}

.nav a.active {
  background: #d4e8df;
  color: #1b6b3d;
  border-left-color: #1b6b3d;
}

.nav a svg {
  min-width: 18px;
  width: 18px;
  height: 18px;
}

/* ================= MAIN ================= */
.main {
  flex: 1;
  padding: 32px;
  overflow-y: auto;
}

.main-header {
  margin-bottom: 24px;
}

.main-header h1 {
  font-size: 32px;
  margin-bottom: 4px;
  color: #1b6b3d;
}

.main-header p {
  color: #666;
  font-size: 14px;
}

.section {
  margin-bottom: 32px;
  display: none;
}

.section.active {
  display: block;
}

.section h2 {
  font-size: 18px;
  margin-bottom: 16px;
  color: #1b6b3d;
  display: flex;
  align-items: center;
  gap: 8px;
}

/* ================= STATUS CARDS ================= */
.status-cards {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
  gap: 16px;
  margin-bottom: 24px;
}

.status-card {
  background: white;
  border-radius: 12px;
  padding: 24px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.06);
}

.status-card-top {
  margin: -24px -24px 16px -24px;
  border-radius: 12px 12px 0 0;
}

.status-card-title {
  font-size: 14px;
  color: #999;
  margin-bottom: 12px;
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.status-content {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 20px;
}

.status-icon {
  width: 80px;
  height: 80px;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.status-icon svg {
  width: 100%;
  height: 100%;
}

.status-info {
  flex: 1;
}

.status-label {
  font-size: 12px;
  color: #999;
  margin-bottom: 4px;
}

.status-text {
  font-size: 14px;
  color: #666;
  line-height: 1.4;
}

.status-percent {
  width: 100px;
  height: 100px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
  border: 6px solid #ffa850;
  flex-shrink: 0;
}

.status-percent-value {
  font-size: 28px;
  font-weight: 700;
  color: #1b6b3d;
}

.status-percent-label {
  font-size: 10px;
  color: #999;
}

/* ================= CONTROL SECTION ================= */
.control-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(260px, 1fr));
  gap: 20px;
  margin-bottom: 24px;
}

.control-card {
  background: white;
  border-radius: 12px;
  padding: 24px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.06);
  display: flex;
  flex-direction: column;
  gap: 18px;
}

.mode-card {
  background: white;
  border-radius: 12px;
  padding: 24px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.06);
  margin-bottom: 24px;
}

.mode-card-content {
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  gap: 20px;
  align-items: center;
}

.control-title {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 16px;
  font-weight: 600;
  color: #1b6b3d;
  margin-bottom: 16px;
}

.button-group {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
}

button {
  border: none;
  border-radius: 8px;
  padding: 16px;
  font-size: 14px;
  font-weight: 600;
  color: white;
  cursor: pointer;
  transition: all 0.2s;
}

button:hover {
  transform: translateY(-1px);
  box-shadow: 0 4px 12px rgba(0,0,0,0.15);
}

button:active {
  transform: translateY(0);
}

.green  { background: #1b6b3d; }
.blue   { background: #2563eb; }
.red    { background: #dc2626; }
.yellow { background: #d97706; }

button:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

.control-status {
  margin-top: 16px;
  padding: 12px;
  background: #f0f8f4;
  border-radius: 8px;
  font-size: 13px;
  color: #666;
}

.control-status-label {
  color: #999;
  margin-bottom: 4px;
}

.control-status-value {
  font-weight: 600;
  color: #1b6b3d;
}

.timer-grid {
  display: grid;
  gap: 10px;
}

.timer-info {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 10px;
  margin-top: 28px;
}

/* ================= INFO BOX ================= */
.info-box {
  background: #f0f8f4;
  border-left: 4px solid #1b6b3d;
  padding: 16px;
  border-radius: 8px;
  margin-bottom: 20px;
  color: #666;
  font-size: 14px;
  line-height: 1.6;
}

/* ================= INFO SECTION ================= */
.info-section {
  background: white;
  border-radius: 12px;
  padding: 24px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.06);
}

.info-section h3 {
  font-size: 16px;
  font-weight: 600;
  margin-bottom: 16px;
  color: #1b6b3d;
  display: flex;
  align-items: center;
  gap: 8px;
}

.info-row {
  display: flex;
  justify-content: space-between;
  padding: 12px 0;
  border-bottom: 1px solid #eee;
  font-size: 14px;
}

.info-row:last-child {
  border-bottom: none;
}

.info-label {
  color: #999;
  display: flex;
  align-items: center;
  gap: 8px;
}

.info-value {
  font-weight: 600;
  color: #333;
}

.info-badge {
  display: inline-block;
  padding: 4px 10px;
  border-radius: 12px;
  font-size: 12px;
  font-weight: 600;
}

.info-badge.connected {
  background: #d4e8df;
  color: #1b6b3d;
}

.info-badge.warning {
  background: #fef3c7;
  color: #92400e;
}

/* ================= FORM ================= */
form {
  display: grid;
  gap: 16px;
}

.form-group {
  display: grid;
  gap: 8px;
}

.form-group label {
  font-size: 14px;
  font-weight: 600;
  color: #333;
}

input, textarea, select {
  width: 100%;
  padding: 12px;
  border-radius: 8px;
  border: 1px solid #ddd;
  background: white;
  color: #333;
  font-size: 14px;
  outline: none;
  font-family: inherit;
}

input:focus, textarea:focus, select:focus {
  border-color: #1b6b3d;
  box-shadow: 0 0 0 3px rgba(27, 107, 61, 0.1);
}

input:disabled {
  background: #f5f5f5;
  color: #999;
}

/* ================= TABLE ================= */
.table-wrapper {
  overflow-x: auto;
  background: white;
  border-radius: 12px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.06);
}

table {
  width: 100%;
  border-collapse: collapse;
}

th, td {
  padding: 12px 16px;
  text-align: left;
}

th {
  background: #f5f5f5;
  color: #666;
  font-size: 13px;
  font-weight: 600;
  text-transform: uppercase;
  letter-spacing: 0.5px;
  border-bottom: 2px solid #eee;
}

td {
  border-bottom: 1px solid #eee;
  color: #333;
  font-size: 14px;
}

tr:hover {
  background: #fafafa;
}

/* ================= BADGE ================= */
.badge {
  display: inline-block;
  padding: 6px 12px;
  border-radius: 4px;
  font-size: 12px;
  font-weight: 600;
}

.badge.success {
  background: #d4e8df;
  color: #1b6b3d;
}

.badge.warning {
  background: #fef3c7;
  color: #92400e;
}

.badge.danger {
  background: #fee2e2;
  color: #991b1b;
}

.network-item {
  background: white;
  border: 1px solid #ddd;
  border-radius: 8px;
  padding: 14px;
  display: flex;
  align-items: center;
  gap: 12px;
  cursor: pointer;
  transition: all 0.2s;
}

.network-item:hover {
  border-color: #1b6b3d;
  background: #f9fdf8;
}

.network-item.connected {
  background: #d4e8df;
  border-color: #1b6b3d;
}

.network-icon {
  font-size: 18px;
}

.network-info {
  flex: 1;
}

.network-name {
  font-weight: 600;
  color: #333;
  font-size: 14px;
}

.network-status {
  font-size: 12px;
  color: #999;
}

.network-lock {
  color: #999;
  font-size: 16px;
}

/* ================= RESPONSIVE ================= */
@media (max-width: 768px) {

  body {
    overflow-x: hidden;
  }

  .header {
    flex-direction: column;
    align-items: stretch;
    gap: 12px;
    padding: 14px;
  }

  .header-brand {
    justify-content: center;
    font-size: 18px;
  }

  .header-actions {
    width: 100%;
    justify-content: center;
  }

  .header-link {
    flex: 1;
    text-align: center;
  }

  .container {
    flex-direction: column;
    gap: 0;
  }

  /* ================= SIDEBAR ================= */

  .sidebar {
    display: none;
  }

  .nav {
    display: flex;
    flex-direction: row;
    min-width: max-content;
  }

  .nav a {
    border-left: none;
    border-bottom: 3px solid transparent;
    padding: 14px 18px;
    white-space: nowrap;
    font-size: 13px;
  }

  .nav a.active {
    border-bottom: 3px solid #1b6b3d;
    border-left: none;
  }

  /* ================= MAIN ================= */

  .main {
    padding: 16px;
  }

  .main-header h1 {
    font-size: 24px;
  }

  /* ================= STATUS ================= */

  .status-cards {
    grid-template-columns: 1fr;
  }

  .status-card {
    padding: 18px;
  }

  .status-content {
    flex-direction: column;
    align-items: center;
    text-align: center;
  }

  .status-icon {
    width: 60px;
    height: 60px;
  }

  .status-percent {
    width: 80px;
    height: 80px;
  }

  .status-percent-value {
    font-size: 22px;
  }

  /* ================= CONTROL ================= */

  .control-grid {
    grid-template-columns: 1fr;
  }

  .mode-card-content {
    grid-template-columns: 1fr;
  }

  .control-card {
    padding: 18px;
  }

  .button-group {
    grid-template-columns: 1fr;
  }

  .timer-info {
    grid-template-columns: 1fr;
  }

  button {
    width: 100%;
    padding: 14px;
    font-size: 13px;
  }

  button svg {
    width: 24px !important;
    height: 24px !important;
  }

  /* ================= INFO SYSTEM ================= */

  .info-section-grid,
  .system-grid {
    grid-template-columns: 1fr !important;
  }

  /* paksa info system jadi vertikal */
  div[style*="grid-template-columns:repeat(4,1fr)"] {
    grid-template-columns: 1fr !important;
    gap: 16px !important;
  }

  div[style*="border-right:1px solid #f0f0f0"] {
    border-right: none !important;
    border-bottom: 1px solid #eee;
    padding: 0 0 12px 0 !important;
  }

  /* ================= TABLE ================= */

  .table-wrapper {
    overflow-x: auto;
  }

  table {
    min-width: 600px;
  }

  /* ================= FORM ================= */

  input,
  select,
  textarea {
    font-size: 16px;
  }

  /* ================= LOGIN ================= */

  .login-card {
    padding: 24px;
  }

  .login-card h1 {
    font-size: 24px;
  }

  /* Bottom navigation for mobile: visible only inside this media query */
  .bottom-nav {
    display: flex;
    position: fixed;
    left: 0;
    right: 0;
    bottom: 0;
    height: 64px;
    background: #ffffff;
    border-top: 1px solid #e6e6e6;
    box-shadow: 0 -4px 12px rgba(0,0,0,0.06);
    align-items: center;
    justify-content: space-around;
    z-index: 80;
  }

  .bottom-nav a {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    gap: 4px;
    color: #666;
    text-decoration: none;
    font-size: 12px;
    width: 72px;
  }

  .bottom-nav a.active {
    color: #1b6b3d;
  }

  .main {
    padding-bottom: 88px;
  }
}

/* ================= MOBILE TABLE ================= */

.table-wrapper {
  width: 100%;
  overflow-x: auto;
}

.user-table {
  min-width: 100%;
}

@media (max-width: 768px) {

  .user-table th,
  .user-table td {
    padding: 12px 10px;
    font-size: 13px;
  }

  .user-table th:first-child,
  .user-table td:first-child {
    width: 50px;
  }

  .user-table th:last-child,
  .user-table td:last-child {
    width: 80px;
    text-align: center;
  }

  .action-buttons {
    gap: 10px;
  }

  .action-btn svg {
    width: 16px;
    height: 16px;
  }

}

</style>
)rawliteral";
}

// ======================================================
// HEADER
// ======================================================
String getHeaderHTML() {
  String html;

  html += "<header class='header'>";
  html += "<div class='header-brand'>";
  html += "<svg viewBox='0 0 24 24' width='24' height='24' fill='white' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M17 8C8 10 5 16 5 21c5 0 11-3 13-12 1-4-1-7-1-7s-3 2-7 6c3-1 5-1 7 0z'/>";
  html += "</svg>";
  html += " Smart Garden";
  html += "</div>";
  html += "<div class='header-device' id='currentTime'>--:--:--</div>";
  html += "<div class='header-actions'>";
  html += "<a class='header-link' href='/logout'>Logout</a>";
  html += "</div>";
  html += "</header>";

  return html;
}

// ======================================================
// SIDEBAR
// ======================================================
String getSidebarHTML() {
  String html;

  html += "<aside class='sidebar'>";

  html += "<nav class='nav'>";

  html += "<a href='#dashboard' class='active'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='currentColor' xmlns='http://www.w3.org/2000/svg'>";
  html += "<rect x='3' y='3' width='7' height='7'/><rect x='14' y='3' width='7' height='7'/>";
  html += "<rect x='3' y='14' width='7' height='7'/><rect x='14' y='14' width='7' height='7'/>";
  html += "</svg>";
  html += " Dashboard</a>";

  html += "<a href='#wifi'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='currentColor' stroke-width='2' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0'/>";
  html += "<line x1='12' y1='20' x2='12.01' y2='20'/>";
  html += "</svg>";
  html += " WiFi Config</a>";

  html += "<a href='#users'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='currentColor' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2'/>";
  html += "<circle cx='12' cy='7' r='4'/>";
  html += "</svg>";
  html += " User</a>";

  html += "</nav>";

  html += "</aside>";

  return html;
}

// ======================================================
// BOTTOM NAV (MOBILE)
// ======================================================
String getBottomNavHTML() {
  String html;
  html += "<nav class='bottom-nav' role='navigation'>";
  html += "<a href='#dashboard' class='active'>";
  html += "<svg viewBox='0 0 24 24' width='20' height='20' fill='currentColor' xmlns='http://www.w3.org/2000/svg'><rect x='3' y='3' width='7' height='7'/><rect x='14' y='3' width='7' height='7'/><rect x='3' y='14' width='7' height='7'/><rect x='14' y='14' width='7' height='7'/></svg>";
  html += "<span>Home</span>";
  html += "</a>";

  html += "<a href='#wifi'>";
  html += "<svg viewBox='0 0 24 24' width='20' height='20' fill='none' stroke='currentColor' stroke-width='2' xmlns='http://www.w3.org/2000/svg'><path d='M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0'/><line x1='12' y1='20' x2='12.01' y2='20'/></svg>";
  html += "<span>WiFi</span>";
  html += "</a>";

  html += "<a href='#users'>";
  html += "<svg viewBox='0 0 24 24' width='20' height='20' fill='currentColor' xmlns='http://www.w3.org/2000/svg'><path d='M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2'/><circle cx='12' cy='7' r='4'/></svg>";
  html += "<span>User</span>";
  html += "</a>";

  html += "</nav>";
  return html;
}

// ======================================================
// LOGIN PAGE
// ======================================================
String getLoginPage(const String &errorMessage) {
  String html;

  html += "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Login - Smart Garden</title>";
  html += getGlobalStyles();
  html += "</head><body>";

  html += "<div class='login-wrapper'>";
  html += "<div class='login-card'>";
  html += "<div style='display:flex; align-items:center; gap:8px; margin-bottom:16px;'>";
  html += "<svg viewBox='0 0 24 24' width='32' height='32' fill='#1b6b3d' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M17 8C8 10 5 16 5 21c5 0 11-3 13-12 1-4-1-7-1-7s-3 2-7 6c3-1 5-1 7 0z'/>";
  html += "</svg>";
  html += "<h1 style='margin:0; color:#1b6b3d;'>Smart Garden</h1>";
  html += "</div>";
  html += "<p>Silakan login untuk melanjutkan.</p>";

  if (errorMessage.length() > 0) {
    html += "<div class='error-box'>";
    html += htmlEscape(errorMessage);
    html += "</div>";
  }

  html += "<form method='POST' action='/login'>";
  html += "<input type='text' name='username' "
          "placeholder='Username' required>";
  html += "<input type='password' name='password' "
          "placeholder='Password' required>";
  html += "<button type='submit' class='green'>Login</button>";
  html += "</form>";

  html += "</div>";
  html += "</div>";

  html += "</body></html>";

  return html;
}

// ======================================================
// DASHBOARD SECTION
// ======================================================
String getDashboardSectionHTML() {
  String html;

  html += "<section class='section' id='dashboard'>";
  html += "<div class='main-header'>";
  html += "<h1>Dashboard</h1>";
  html += "</div>";

  // Status Cards
  html += "<div class='status-cards'>";

  // Soil Status Card
  html += "<div class='status-card' id='soilCard'>";
  html += "<div class='status-card-top' style='border-top: 4px solid #f9a850;'></div>";
  html += "<div class='status-card-title'>Status Tanah</div>";
  html += "<div class='status-content'>";
  html += "<div class='status-icon' id='soilIcon'>" + getSvgIconDroplet() + "</div>";
  html += "<div class='status-info'>";
  html += "<div class='status-label' id='soilStatus' style='color:#f9a850; font-weight:600;'>DRY</div>";
  html += "<div class='status-text'>Tanah kering, Siram tanaman diperlukan</div>";
  html += "</div>";
  html += "<div class='status-percent' style='border-color:#f9a850;'>";
  html += "<div class='status-percent-value' id='soilPercent'>15%</div>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  // Rain Status Card
  html += "<div class='status-card' id='rainCard'>";
  html += "<div class='status-card-top' style='border-top: 4px solid #3b82f6;'></div>";
  html += "<div class='status-card-title'>Status Hujan</div>";
  html += "<div class='status-content'>";
  html += "<div class='status-icon' id='rainIcon'>" + getSvgIconSun() + "</div>";
  html += "<div class='status-info'>";
  html += "<div class='status-label' id='rainStatus' style='color:#3b82f6; font-weight:600;'>CLEAR</div>";
  html += "<div class='status-text'>Tidak ada hujan, Cuaca cerah</div>";
  html += "</div>";
  html += "<div class='status-percent' style='border-color:#3b82f6;'>";
  html += "<div class='status-percent-value' id='rainPercent'>0%</div>";
  html += "</div>";
  html += "</div>";
  html += "</div>";


  html += "</div>";

  // Control Section
  html += "<div class='control-grid'>";

  html += "<div class='control-card'>";
  html += "<div class='control-title' style='color:#1b6b3d;'>";
  html += "Kontrol Jemuran</div>";
  html += "<div class='button-group'>";
  html += "<button class='green' onclick=\"sendCommand('/jemur/in')\" style='display:flex;flex-direction:column;align-items:center;gap:8px;'>";
  html += getSvgIconArrowRight();
  html += "<span style='font-size:12px; font-weight:500;'>JEMURAN MASUK</span>";
  html += "</button>";
  html += "<button class='yellow' onclick=\"sendCommand('/jemur/out')\" style='display:flex;flex-direction:column;align-items:center;gap:8px;'>";
  html += getSvgIconArrowLeft();
  html += "<span style='font-size:12px; font-weight:500;'>JEMURAN KELUAR</span>";
  html += "</button>";
  html += "</div>";
  html += "<div class='control-status'>";
  html += "<div class='control-status-label'>Status Jemuran</div>";
  html += "<div class='control-status-value' id='jemuranState'>POSISI: KELUAR</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='control-card'>";
  html += "<div class='control-title' style='color:#1b6b3d;'>";
  html += "Kontrol Pompa</div>";
  html += "<div class='button-group'>";
  html += "<button class='green' onclick=\"sendPumpOn()\" style='display:flex;flex-direction:column;align-items:center;gap:8px;'>";
  html += getSvgIconPower();
  html += "<span style='font-size:12px; font-weight:500;'>POMPA NYALA</span>";
  html += "</button>";
  html += "<button class='yellow' onclick=\"sendCommand('/pump/off')\" style='display:flex;flex-direction:column;align-items:center;gap:8px;'>";
  html += getSvgIconCircle();
  html += "<span style='font-size:12px; font-weight:500;'>POMPA MATI</span>";
  html += "</button>";
  html += "</div>";
  html += "<div class='control-status'>";
  html += "<div class='control-status-label'>Status Pompa</div>";
  html += "<div class='control-status-value' id='pumpState'>STATUS: MATI</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='control-card'>";
  html += "<div class='control-title' style='color:#1b6b3d;'>";
  html += "Manual Pump Timer</div>";
  html += "<div class='timer-grid'>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;'>Durasi Pompa(menit)</label>";
  html += "<input type='number' id='pumpDuration' min='1' max='120' value='5' "
          "style='width:100%;' aria-label='Durasi Pompa dalam menit'>";
  html += "</div>";
  html += "<div class='timer-info'>";
  html += "<div class='control-status' style='margin-top:0;'>";
  html += "<div class='control-status-label'>Timer Status</div>";
  html += "<div class='control-status-value' id='pumpTimerStatus'>Inactive</div>";
  html += "</div>";
  html += "<div class='control-status' style='margin-top:0;'>";
  html += "<div class='control-status-label'>Sisa Waktu</div>";
  html += "<div class='control-status-value' id='pumpTimeRemaining'>00:00</div>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "</div>"; // end control-grid

  // Mode Operasi card
  html += "<div class='control-card' style='margin-bottom:24px;'>";

  // Title
  html += "<div class='control-title' style='color:#1b6b3d;'>";
  html += "Mode Operasi</div>";

  html += "<div class='mode-card-content'>";

  // Mode label badge
  html += "<div id='modeBadge' style='display:flex;align-items:center;justify-content:center;gap:8px;"
          "padding:12px;border-radius:10px;background:#f0f8f4;'>";
  html += "<span id='modeDot' style='width:10px;height:10px;border-radius:50%;background:#1b6b3d;display:inline-block;'></span>";
  html += "<span id='modeLabel' style='font-size:15px;font-weight:700;color:#1b6b3d;letter-spacing:1px;'>AUTO</span>";
  html += "</div>";

  // Button
  html += "<button onclick=\"sendCommand('/auto')\" "
          "style='width:100%;display:flex;align-items:center;justify-content:center;gap:8px;"
          "background:#1b6b3d;padding:14px;border-radius:8px;font-size:12px;font-weight:600;margin-bottom:14px;'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='white' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<polyline points='23 4 23 10 17 10'/><polyline points='1 20 1 14 7 14'/>";
  html += "<path d='M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15'/>";
  html += "</svg>";
  html += "Aktifkan AUTO</button>";

  // Description
  html += "<p id='modeDesc' style='font-size:13px;color:#666;text-align:center;line-height:1.5;'>"
          "Sistem mengatur pompa dan jemuran secara otomatis berdasarkan sensor.</p>";

  html += "</div>"; // end mode-card-content
  html += "</div>"; // end Mode Operasi card

  // ── Informasi Sistem: full width di bawah control-grid ──
  html += "<div style='margin-top:20px; background:white; border-radius:12px; padding:20px 24px; box-shadow:0 2px 8px rgba(0,0,0,0.06);'>";

  html += "<div style='font-size:14px;font-weight:600;color:#1b6b3d;margin-bottom:16px;display:flex;align-items:center;gap:8px;'>";
  html += "<svg viewBox='0 0 24 24' width='16' height='16' fill='none' stroke='#1b6b3d' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<circle cx='12' cy='12' r='10'/><line x1='12' y1='16' x2='12' y2='12'/><line x1='12' y1='8' x2='12.01' y2='8'/>";
  html += "</svg> Informasi Sistem</div>";

  // 4 kolom sejajar horizontal
  html += "<div style='display:grid;grid-template-columns:repeat(3,1fr);gap:0;'>";

  // IP Address
  html += "<div style='padding:0 20px 0 0;border-right:1px solid #f0f0f0;'>";
  html += "<div style='font-size:11px;color:#999;text-transform:uppercase;letter-spacing:0.5px;margin-bottom:5px;display:flex;align-items:center;gap:5px;'>";
  html += "<svg viewBox='0 0 24 24' width='12' height='12' fill='none' stroke='#bbb' stroke-width='2' xmlns='http://www.w3.org/2000/svg'><path d='M21 10c0 7-9 13-9 13s-9-6-9-13a9 9 0 0 1 18 0z'/><circle cx='12' cy='10' r='3'/></svg> IP Address</div>";
  html += "<div style='font-size:15px;font-weight:700;color:#1b6b3d;'>" + htmlEscape(getCurrentIP()) + "</div>";
  html += "</div>";

  // WiFi SSID
  html += "<div style='padding:0 20px;border-right:1px solid #f0f0f0;'>";
  html += "<div style='font-size:11px;color:#999;text-transform:uppercase;letter-spacing:0.5px;margin-bottom:5px;display:flex;align-items:center;gap:5px;'>";
  html += "<svg viewBox='0 0 24 24' width='12' height='12' fill='none' stroke='#bbb' stroke-width='2' xmlns='http://www.w3.org/2000/svg'><path d='M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0'/><line x1='12' y1='20' x2='12.01' y2='20'/></svg> WiFi SSID</div>";
  html += "<div style='font-size:15px;font-weight:700;color:#333;'>" + htmlEscape(getCurrentSSID()) + "</div>";
  html += "</div>";

  // Uptime
  html += "<div style='padding:0 20px;border-right:1px solid #f0f0f0;'>";
  html += "<div style='font-size:11px;color:#999;text-transform:uppercase;letter-spacing:0.5px;margin-bottom:5px;display:flex;align-items:center;gap:5px;'>";
  html += "<svg viewBox='0 0 24 24' width='12' height='12' fill='none' stroke='#bbb' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'><circle cx='12' cy='12' r='10'/><polyline points='12 6 12 12 16 14'/></svg> Uptime</div>";
  html += "<div style='font-size:15px;font-weight:700;color:#333;' id='uptime'>0d 0h 0m 0s</div>";
  html += "</div>";

  html += "</div>"; // end 3-col grid
  html += "</div>"; // end info card

  html += "</section>";

  return html;
}

// ======================================================
// WIFI SECTION
// ======================================================
String getWiFiSectionHTML() {
  String html;

  html += "<section class='section' id='wifi'>";
  html += "<div class='main-header'>";
  html += "<h1>Konfigurasi WiFi</h1>";
  html += "<p>Atur koneksi jaringan untuk perangkat Smart Garden.</p>";
  html += "</div>";

  html += "<div style='display:grid; grid-template-columns:repeat(auto-fit,minmax(300px,1fr)); gap:20px;'>";

  // ── Card 1: Status Koneksi ──────────────────────────
  html += "<div class='control-card'>";

  html += "<div style='display:flex;align-items:center;justify-content:space-between;margin-bottom:20px;'>";
  html += "<div class='control-title' style='margin-bottom:0;'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M5 12.55a11 11 0 0 1 14.08 0M1.42 9a16 16 0 0 1 21.16 0M8.53 16.11a6 6 0 0 1 6.94 0'/><line x1='12' y1='20' x2='12.01' y2='20'/>";
  html += "</svg> Status Koneksi</div>";
  // Dynamic badge - default based on current state
  if (isAPMode()) {
    html += "<span style='padding:4px 10px;border-radius:12px;font-size:12px;font-weight:600;background:#fef3c7;color:#92400e;'>● AP MODE</span>";
  } else {
    html += "<span style='padding:4px 10px;border-radius:12px;font-size:12px;font-weight:600;background:#d4e8df;color:#1b6b3d;'>● CONNECTED</span>";
  }
  html += "</div>";

  // Info rows
  html += "<div style='display:grid;gap:12px;margin-bottom:20px;'>";

  html += "<div style='display:flex;justify-content:space-between;align-items:center;padding:10px 0;border-bottom:1px solid #f0f0f0;'>";
  html += "<span style='font-size:13px;color:#999;'>SSID</span>";
  html += "<span style='font-size:14px;font-weight:600;color:#333;'>" + htmlEscape(getCurrentSSID()) + "</span>";
  html += "</div>";

  html += "<div style='display:flex;justify-content:space-between;align-items:center;padding:10px 0;border-bottom:1px solid #f0f0f0;'>";
  html += "<span style='font-size:13px;color:#999;'>IP Address</span>";
  html += "<span style='font-size:14px;font-weight:600;color:#1b6b3d;'>" + htmlEscape(getCurrentIP()) + "</span>";
  html += "</div>";

  html += "<div style='display:flex;justify-content:space-between;align-items:center;padding:10px 0;'>";
  html += "<span style='font-size:13px;color:#999;'>Uptime</span>";
  html += "<span style='font-size:14px;font-weight:600;color:#333;' id='wifiUptime'>0d 0h 0m 0s</span>";
  html += "</div>";

  html += "</div>"; // end info rows

  // Reset button
  html += "<button onclick='resetWiFi()' class='yellow' "
          "style='width:100%;display:flex;align-items:center;justify-content:center;gap:8px;padding:12px;border-radius:8px;font-size:14px;font-weight:600;'>";
  html += "<svg viewBox='0 0 24 24' width='16' height='16' fill='none' stroke='white' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<polyline points='1 4 1 10 7 10'/><path d='M3.51 15a9 9 0 1 0 .49-4.5'/>";
  html += "</svg>";
  html += "Reset Konfigurasi WiFi</button>";

  html += "</div>"; // end card 1

  // ── Card 2: Jaringan Tersedia ───────────────────────
  html += "<div class='control-card'>";

  html += "<div style='display:flex;align-items:center;justify-content:space-between;margin-bottom:16px;'>";
  html += "<div class='control-title' style='margin-bottom:0;'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<rect x='1' y='6' width='22' height='13' rx='2'/><line x1='1' y1='10' x2='23' y2='10'/>";
  html += "</svg> Jaringan Tersedia</div>";
  html += "<button type='button' onclick='loadNetworks()' "
          "style='display:flex;align-items:center;gap:5px;padding:6px 12px;font-size:12px;background:#1b6b3d;color:white;border:none;border-radius:6px;cursor:pointer;'>";
  html += "<svg viewBox='0 0 24 24' width='13' height='13' fill='none' stroke='white' stroke-width='2' stroke-linecap='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<polyline points='23 4 23 10 17 10'/><polyline points='1 20 1 14 7 14'/>";
  html += "<path d='M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15'/>";
  html += "</svg> Scan</button>";
  html += "</div>";

  html += "<div class='networks-list' id='networksList' style='max-height:200px;overflow-y:auto;display:grid;gap:8px;margin-bottom:16px;'>";
  html += "<div style='padding:20px;text-align:center;color:#999;font-size:13px;'>Loading networks...</div>";
  html += "</div>";

  // Connect form
  html += "<form id='wifiForm' style='display:grid;gap:12px;'>";

  html += "<div>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;display:block;margin-bottom:6px;'>SSID</label>";
  html += "<input type='text' id='wifi_ssid' name='ssid' placeholder='Pilih jaringan atau ketik manual' required>";
  html += "</div>";

  html += "<div>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;display:block;margin-bottom:6px;'>Password</label>";
  html += "<input type='password' id='wifi_password' name='password' placeholder='Masukkan password WiFi'>";
  html += "</div>";

  html += "<button type='submit' "
          "style='width:100%;background:#1b6b3d;display:flex;align-items:center;justify-content:center;gap:8px;padding:13px;border-radius:8px;font-size:14px;font-weight:600;'>";
  html += "<svg viewBox='0 0 24 24' width='16' height='16' fill='none' stroke='white' stroke-width='2.5' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<polyline points='20 6 9 17 4 12'/>";
  html += "</svg>";
  html += "Sambungkan</button>";

  html += "</form>";
  html += "</div>"; // end card 2

  html += "</div>"; // end grid
  html += "</section>";

  return html;
}

// ======================================================
// USER MANAGEMENT SECTION
// ======================================================
String getUserManagementSectionHTML() {
  String html;
  String currentUsername = htmlEscape(getUser(0).username);

  html += "<section class='section' id='users'>";
  html += "<div class='main-header'>";
  html += "<h1>Manajemen User</h1>";
  html += "<p>Kelola akun pengguna dan kredensial login dashboard.</p>";
  html += "</div>";

  html += "<div style='display:grid; grid-template-columns:repeat(auto-fit,minmax(300px,1fr)); gap:20px;'>";

  // ── Card 1: Daftar User ─────────────────────────────
  html += "<div class='control-card'>";
  html += "<div class='control-title'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2'/><circle cx='9' cy='7' r='4'/>";
  html += "<path d='M23 21v-2a4 4 0 0 0-3-3.87'/><path d='M16 3.13a4 4 0 0 1 0 7.75'/>";
  html += "</svg> Daftar User</div>";

  html += "<div id='usersTableContainer' style='margin-bottom:12px;'>";
  html += generateUsersTableHTML();
  html += "</div>";

  // Add user button
  html += "<button type='button' onclick='showAddUserForm()' "
          "style='width:100%;background:#1b6b3d;display:flex;align-items:center;justify-content:center;"
          "gap:8px;padding:12px;border-radius:8px;font-size:14px;font-weight:600;'>";
  html += "<svg viewBox='0 0 24 24' width='16' height='16' fill='none' stroke='white' stroke-width='2.5' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<line x1='12' y1='5' x2='12' y2='19'/><line x1='5' y1='12' x2='19' y2='12'/>";
  html += "</svg>";
  html += "Tambah User Baru</button>";
  html += "</div>"; // end card 1

  // ── Card 2: Form Edit / Add ─────────────────────────
  html += "<div class='control-card'>";
  html += "<div class='control-title' id='userFormTitle'>";
  html += "<svg viewBox='0 0 24 24' width='18' height='18' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<path d='M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7'/>";
  html += "<path d='M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z'/>";
  html += "</svg> Edit Akun</div>";

  html += "<form id='userForm' style='display:grid;gap:14px;'>";
  html += "<input type='hidden' id='user_index' value='0'>";
  html += "<input type='hidden' id='user_action' value='update'>";

  html += "<div>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;display:block;margin-bottom:6px;'>Username</label>";
  html += "<input type='text' id='user_username' value='" + currentUsername + "' placeholder='Masukkan username' required>";
  html += "</div>";

  html += "<div>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;display:block;margin-bottom:6px;'>Password Baru</label>";
  html += "<input type='password' id='user_password' placeholder='••••••••' required>";
  html += "</div>";

  html += "<div>";
  html += "<label style='font-size:13px;font-weight:600;color:#333;display:block;margin-bottom:6px;'>Konfirmasi Password</label>";
  html += "<input type='password' id='user_password_confirm' placeholder='••••••••' required>";
  html += "</div>";

  html += "<div style='display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-top:6px;'>";
  html += "<button type='button' onclick='cancelUserEdit()' class='yellow' "
          "style='padding:12px;border-radius:8px;font-size:14px;font-weight:600;'>Batal</button>";
  html += "<button type='submit' "
          "style='background:#1b6b3d;display:flex;align-items:center;justify-content:center;gap:6px;"
          "padding:12px;border-radius:8px;font-size:14px;font-weight:600;'>";
  html += "<svg viewBox='0 0 24 24' width='15' height='15' fill='none' stroke='white' stroke-width='2.5' stroke-linecap='round' stroke-linejoin='round' xmlns='http://www.w3.org/2000/svg'>";
  html += "<polyline points='20 6 9 17 4 12'/>";
  html += "</svg>";
  html += "Simpan</button>";
  html += "</div>";

  html += "</form>";
  html += "</div>"; // end card 2

  html += "</div>"; // end grid
  html += "</section>";

  return html;
}

// ======================================================
// GLOBAL JAVASCRIPT
// ======================================================
String getGlobalScripts() {
  return R"rawliteral(
<script>
/* ==========================================
   STATUS REFRESH
========================================== */
function getSoilIcon(status) {
      if (status === 'DRY') {
        return `<svg viewBox="0 0 24 24" width="48" height="48" fill="#f59e0b" xmlns="http://www.w3.org/2000/svg"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.32 0z"/></svg>`;
      }
      return `<svg viewBox="0 0 24 24" width="48" height="48" fill="#3b82f6" xmlns="http://www.w3.org/2000/svg"><path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.32 0z"/></svg>`;
    }

    function getRainIcon(status) {
      if (status === 'RAINING') {
        return `<svg viewBox="0 0 24 24" width="48" height="48" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M5 16.5a4 4 0 0 1 0-8 5 5 0 0 1 10 0 4 4 0 0 1 0 8H5z" fill="#bfdbfe" stroke="#3b82f6" stroke-width="2"/><path d="M8 18v3M12 18v3M16 18v3" stroke="#3b82f6" stroke-width="2" stroke-linecap="round"/></svg>`;
      }
      return `<svg viewBox="0 0 24 24" width="48" height="48" fill="#f59e0b" xmlns="http://www.w3.org/2000/svg"><circle cx="12" cy="12" r="5"/><line x1="12" y1="1" x2="12" y2="3" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="12" y1="21" x2="12" y2="23" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="4.22" y1="4.22" x2="5.64" y2="5.64" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="1" y1="12" x2="3" y2="12" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="21" y1="12" x2="23" y2="12" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="4.22" y1="19.78" x2="5.64" y2="18.36" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/><line x1="18.36" y1="5.64" x2="19.78" y2="4.22" stroke="#f59e0b" stroke-width="2" stroke-linecap="round"/></svg>`;
    }

    function formatTimer(seconds) {
      const totalSeconds = Math.max(0, Number(seconds) || 0);
      const minutes = Math.floor(totalSeconds / 60);
      const remainingSeconds = totalSeconds % 60;
      return String(minutes).padStart(2, '0') + ':' +
             String(remainingSeconds).padStart(2, '0');
    }

    async function fetchStatus() {
      try {
        const response = await fetch('/api/status');

        if (!response.ok) return;

        const data = await response.json();

        // Update soil with dynamic status descriptions
        const soilPercent = data.soilPercent || 0;
        let soilStatus = 'DRY';
        let soilDesc = 'Tanah kering, Siram tanaman diperlukan';

        if (soilPercent >= 50) {
          soilStatus = 'WET';
          soilDesc = 'Tanah lembab, Jangan siram tanaman';
        }

        document.getElementById('soilStatus').textContent = soilStatus;
        document.getElementById('soilPercent').textContent = soilPercent + '%';
        const soilTextEl = document.querySelector('#soilCard .status-text');
        if (soilTextEl) soilTextEl.textContent = soilDesc;
        const soilIconEl = document.getElementById('soilIcon');
        if (soilIconEl) soilIconEl.innerHTML = getSoilIcon(soilStatus);
        const soilLabelEl = document.getElementById('soilStatus');
        if (soilLabelEl) soilLabelEl.style.color = soilStatus === 'DRY' ? '#f59e0b' : '#3b82f6';
        const soilCard = document.getElementById('soilCard');
        if (soilCard) {
          const top = soilCard.querySelector('.status-card-top');
          const percent = soilCard.querySelector('.status-percent');
          if (top) top.style.borderTopColor = soilStatus === 'DRY' ? '#f9a850' : '#1b6b3d';
          if (percent) percent.style.borderColor = soilStatus === 'DRY' ? '#f9a850' : '#1b6b3d';
        }

        // Update rain with descriptions
        const rainPercent = data.rainPercent || 0;
        let rainStatus = data.rainStatus || 'CLEAR';
        let rainDesc = 'Tidak ada hujan, Cuaca cerah';

        if (rainStatus === 'RAINING') {
          rainDesc = 'Ada hujan, Jemuran otomatis ditarik';
        }

        document.getElementById('rainStatus').textContent = rainStatus;
        document.getElementById('rainPercent').textContent = rainPercent + '%';
        const rainTextEl = document.querySelector('#rainCard .status-text');
        if (rainTextEl) rainTextEl.textContent = rainDesc;
        const rainIconEl = document.getElementById('rainIcon');
        if (rainIconEl) rainIconEl.innerHTML = getRainIcon(rainStatus);
        const rainLabelEl = document.getElementById('rainStatus');
        if (rainLabelEl) rainLabelEl.style.color = rainStatus === 'RAINING' ? '#2563eb' : '#3b82f6';
        const rainCard = document.getElementById('rainCard');
        if (rainCard) {
          const top = rainCard.querySelector('.status-card-top');
          const percent = rainCard.querySelector('.status-percent');
          if (top) top.style.borderTopColor = rainStatus === 'RAINING' ? '#2563eb' : '#1b6b3d';
          if (percent) percent.style.borderColor = rainStatus === 'RAINING' ? '#2563eb' : '#1b6b3d';
        }

    // Update actuators
    document.getElementById('pumpState').textContent = 'STATUS: ' + (data.pumpState || 'MATI');
    document.getElementById('jemuranState').textContent = 'POSISI: ' + (data.jemuranState || 'KELUAR');

    const timerStatusEl = document.getElementById('pumpTimerStatus');
    const timerRemainingEl = document.getElementById('pumpTimeRemaining');
    if (timerStatusEl) {
      timerStatusEl.textContent = data.pumpTimerActive ? 'Active' : 'Inactive';
      timerStatusEl.style.color = data.pumpTimerActive ? '#1b6b3d' : '#999';
    }
    if (timerRemainingEl) {
      timerRemainingEl.textContent = formatTimer(data.pumpTimeRemaining || 0);
      timerRemainingEl.style.color = data.pumpTimerActive ? '#1b6b3d' : '#999';
    }

    // Update auto mode badge
    const isManual = data.manualMode;
    const modeBadge = document.getElementById('modeBadge');
    const modeDot   = document.getElementById('modeDot');
    const modeLabel = document.getElementById('modeLabel');
    const modeDesc  = document.getElementById('modeDesc');
    const autoModeEl= document.getElementById('autoModeStatus');

    if (modeBadge) {
      modeBadge.style.background = isManual ? '#fef3c7' : '#f0f8f4';
    }
    if (modeDot)   modeDot.style.background  = isManual ? '#f59e0b' : '#1b6b3d';
    if (modeLabel) {
      modeLabel.textContent  = isManual ? 'MANUAL' : 'AUTO';
      modeLabel.style.color  = isManual ? '#92400e' : '#1b6b3d';
    }
    if (modeDesc)  modeDesc.textContent = isManual
      ? 'Mode manual aktif. Kontrol pompa dan jemuran dilakukan secara manual.'
      : 'Sistem mengatur pompa dan jemuran secara otomatis berdasarkan sensor.';
    if (autoModeEl) {
      autoModeEl.textContent = isManual ? 'MANUAL' : 'AUTO';
      autoModeEl.style.color = isManual ? '#f59e0b' : '#1b6b3d';
    }

    // Update uptime
    if (data.uptime) {
      document.getElementById('uptime').textContent = data.uptime;
      const wifiUptime = document.getElementById('wifiUptime');
      if (wifiUptime) wifiUptime.textContent = data.uptime;
    }
  } catch (error) {
    console.log(error);
  }
}

/* ==========================================
   COMMANDS
========================================== */
async function sendCommand(url) {
  try {
    await fetch(url, { method: 'GET' });
    fetchStatus();
  } catch (error) {
    console.log(error);
  }
}

async function sendPumpOn() {
  const durationInput = document.getElementById('pumpDuration');
  let duration = durationInput ? parseInt(durationInput.value, 10) : 0;

  if (!duration || duration < 1) {
    duration = 1;
    if (durationInput) durationInput.value = '1';
  }

  await sendCommand('/pump/on?duration=' + encodeURIComponent(duration));
}

/* ==========================================
   CURRENT TIME & DATE
========================================== */
function updateCurrentTime() {
  const now = new Date();
  const timeStr = now.toLocaleString('id-ID', {
    weekday: 'long',
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit'
  });

  const timeEl = document.getElementById('currentTime');
  if (timeEl) {
    timeEl.textContent = timeStr;
  }
}

/* ==========================================
   NETWORK SELECTION
========================================== */
function selectNetwork(ssid, element) {
  document.getElementById('wifi_ssid').value = ssid;
  document.getElementById('wifi_password').focus();
}

/* ==========================================
   LOAD NETWORKS
========================================== */
async function loadNetworks() {
  try {
    const response = await fetch('/api/networks');
    if (!response.ok) {
      document.getElementById('networksList').innerHTML =
        '<div style="padding:20px;text-align:center;color:#999;">Failed to load networks</div>';
      return;
    }

    const data = await response.json();
    let html = '';

    if (!data.networks || data.networks.length === 0) {
      html = '<div style="padding:20px;text-align:center;color:#999;">No networks found</div>';
    } else {
      data.networks.forEach(network => {
        const bars = getSignalBars(network.rssi);
        const lockIcon = network.secure ?
          '<svg viewBox="0 0 24 24" width="18" height="18" fill="#999" xmlns="http://www.w3.org/2000/svg"><rect x="3" y="11" width="18" height="11" rx="2" fill="none" stroke="#999" stroke-width="2"/><path d="M7 11V7a5 5 0 0 1 10 0v4" fill="none" stroke="#999" stroke-width="2" stroke-linecap="round"/></svg>' :
          '<svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="#999" stroke-width="2" xmlns="http://www.w3.org/2000/svg"><rect x="3" y="11" width="18" height="11" rx="2"/><path d="M7 11V7a5 5 0 0 1 9.9-1"/></svg>';
        const connectedClass = network.connected ? ' connected' : '';
        const statusText = network.connected ? 'CONNECTED' : '●●●';

        html += '<div class="network-item' + connectedClass + '" onclick="selectNetwork(\'' +
                network.ssid.replace(/'/g, "\\'") + '\', this)">';
        html += '<div class="network-icon">';
        html += '<svg viewBox="0 0 24 24" width="20" height="20" fill="none" stroke="#1b6b3d" stroke-width="2" xmlns="http://www.w3.org/2000/svg">';
        for (let i = 1; i <= bars; i++) {
          html += '<path d="M' + (6 + i*3) + ' ' + (20 - i*3) + ' Q' + (6 + i*3) + ' ' + (8 - i*3) + ' ' + (6 + i*3) + ' 4"/>';
        }
        html += '</svg>';
        html += '</div>';
        html += '<div class="network-info">';
        html += '<div class="network-name">' + network.ssid + '</div>';
        html += '<div class="network-status">' + statusText + '</div>';
        html += '</div>';
        html += '<div class="network-lock">' + lockIcon + '</div>';
        html += '</div>';
      });
    }

    document.getElementById('networksList').innerHTML = html;
  } catch (error) {
    console.log('Error loading networks:', error);
    document.getElementById('networksList').innerHTML =
      '<div style="padding:20px;text-align:center;color:#999;">Error loading networks</div>';
  }
}

function getSignalBars(rssi) {
  // RSSI: -30 to -100 dBm
  if (rssi > -50) return 4;
  if (rssi > -70) return 3;
  if (rssi > -85) return 2;
  return 1;
}

/* ==========================================
   WIFI CONFIGURATION
========================================== */
async function saveWiFi(event) {
  event.preventDefault();

  const ssid = document.getElementById('wifi_ssid').value;
  const password = document.getElementById('wifi_password').value;

  if (!ssid) {
    alert('Please enter WiFi SSID');
    return;
  }

  const form = new URLSearchParams();
  form.append('ssid', ssid);
  form.append('password', password);

  try {
    const response = await fetch('/savewifi', {
      method: 'POST',
      body: form
    });

    const text = await response.text();
    alert(text);
    document.getElementById('wifi_ssid').value = '';
    document.getElementById('wifi_password').value = '';
  } catch (error) {
    alert('Error saving WiFi: ' + error);
  }
}

async function resetWiFi() {
  if (!confirm('Are you sure you want to reset WiFi configuration?')) {
    return;
  }

  try {
    const response = await fetch('/resetwifi', {
      method: 'POST'
    });

    const text = await response.text();
    alert(text);
  } catch (error) {
    alert('Error resetting WiFi: ' + error);
  }
}

/* ==========================================
   USER MANAGEMENT
========================================== */
function showAddUserForm() {
  document.getElementById('user_index').value = '-1';
  document.getElementById('user_action').value = 'add';
  document.getElementById('user_username').value = '';
  document.getElementById('user_password').value = '';
  document.getElementById('user_password_confirm').value = '';
  document.getElementById('userFormTitle').innerHTML = `
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
      <line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/>
    </svg> Tambah User Baru`;
}

function editUser(index, username) {
  document.getElementById('user_index').value = index;
  document.getElementById('user_action').value = 'update';
  document.getElementById('user_username').value = username;
  document.getElementById('user_password').value = '';
  document.getElementById('user_password_confirm').value = '';
  document.getElementById('userFormTitle').innerHTML = `
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
      <path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/>
      <path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/>
    </svg> Edit User: ` + username;
}

async function deleteUser(index) {
  if (!confirm('Yakin ingin menghapus user ini?')) return;

  const form = new URLSearchParams();
  form.append('index', index);

  try {
    const response = await fetch('/users/delete', { method: 'POST', body: form });
    const text = await response.text();
    if (response.ok) {
      await refreshUsersTable();
    } else {
      alert(text);
    }
  } catch (error) {
    alert('Error: ' + error);
  }
}

async function refreshUsersTable() {
  try {
    const response = await fetch('/api/users');
    if (response.ok) {
      const html = await response.text();
      const container = document.getElementById('usersTableContainer');
      if (container) container.innerHTML = html;
    }
  } catch (e) {
    console.log('Error refreshing users table:', e);
  }
}

async function saveUser(event) {
  event.preventDefault();

  const username = document.getElementById('user_username').value;
  const password = document.getElementById('user_password').value;
  const passwordConfirm = document.getElementById('user_password_confirm').value;
  const action = document.getElementById('user_action')?.value || 'update';
  const userIndex = document.getElementById('user_index')?.value || '0';

  if (!username || !password) {
    alert('Mohon isi semua kolom');
    return;
  }

  if (password !== passwordConfirm) {
    alert('Password tidak cocok');
    return;
  }

  const form = new URLSearchParams();
  form.append('username', username);
  form.append('password', password);

  let url = '/users/update';
  if (action === 'add') {
    url = '/users/add';
  } else {
    form.append('index', userIndex);
  }

  try {
    const response = await fetch(url, { method: 'POST', body: form });
    const text = await response.text();
    if (response.ok) {
      cancelUserEdit();
      await refreshUsersTable();
    } else {
      alert(text);
    }
  } catch (error) {
    alert('Error: ' + error);
  }
}

function cancelUserEdit() {
  document.getElementById('user_index').value = '0';
  document.getElementById('user_action').value = 'update';
  document.getElementById('user_password').value = '';
  document.getElementById('user_password_confirm').value = '';
  document.getElementById('userFormTitle').innerHTML = `
    <svg viewBox="0 0 24 24" width="18" height="18" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
      <path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/>
      <path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/>
    </svg> Edit Akun`;
}

/* ==========================================
   SIDEBAR NAVIGATION
========================================== */
function switchSection(sectionId) {
  // Hide all sections
  document.querySelectorAll('.section').forEach(section => {
    section.classList.remove('active');
  });

  // Remove active class from all nav links
  document.querySelectorAll('.nav a, .bottom-nav a').forEach(link => {
    link.classList.remove('active');
  });

  // Show selected section
  const section = document.getElementById(sectionId);
  if (section) {
    section.classList.add('active');
  }

  // Add active class to matching nav links
  document.querySelectorAll('.nav a, .bottom-nav a').forEach(link => {
    if (link.getAttribute('href') === '#' + sectionId) {
      link.classList.add('active');
    }
  });
}

document.addEventListener('click', (e) => {
  const anchor = e.target.closest('.nav a, .bottom-nav a');
  if (anchor) {
    e.preventDefault();
    const href = anchor.getAttribute('href');
    const sectionId = href.substring(1); // Remove '#'
    switchSection(sectionId);
  }
});

/* ==========================================
   INITIALIZATION
========================================== */
document.addEventListener('DOMContentLoaded', () => {
  // Set first section as active
  const firstSection = document.querySelector('.section');
  if (firstSection) {
    switchSection(firstSection.id);
  }

  // Status real-time
  fetchStatus();
  setInterval(fetchStatus, 1000);

  // Current time
  updateCurrentTime();
  setInterval(updateCurrentTime, 1000);

  // Load WiFi networks
  loadNetworks();

  // WiFi form
  const wifiForm = document.getElementById('wifiForm');
  if (wifiForm) {
    wifiForm.addEventListener('submit', saveWiFi);
  }

  // User form
  const userForm = document.getElementById('userForm');
  if (userForm) {
    userForm.addEventListener('submit', saveUser);
  }
});
</script>
)rawliteral";
}

// ======================================================
// DASHBOARD PAGE
// ======================================================
String getDashboardPage() {
  String html;

  html += "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' "
          "content='width=device-width, initial-scale=1'>";
  html += "<title>Smart Garden Dashboard</title>";
  html += getGlobalStyles();
  html += "</head><body>";

  html += getHeaderHTML();

  html += "<div class='container'>";

  // Sidebar
  html += getSidebarHTML();

  // Main content
  html += "<main class='main'>";

  // Dashboard
  html += getDashboardSectionHTML();

  // WiFi
  html += getWiFiSectionHTML();

  // User Settings
  html += getUserManagementSectionHTML();

  html += "</main>";
  html += "</div>";

  // Bottom nav for mobile
  html += getBottomNavHTML();

  // Scripts
  html += getGlobalScripts();

  html += "</body></html>";

  return html;
}
