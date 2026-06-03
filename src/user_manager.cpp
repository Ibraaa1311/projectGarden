// ======================================================
// user_manager.cpp
// Implementasi manajemen user
// ======================================================

#include "user_manager.h"
#include "config.h"

#include <Preferences.h>

// ======================================================
// GLOBAL OBJECT
// ======================================================
Preferences userPrefs;

// ======================================================
// INTERNAL HELPERS
// ======================================================
static String makeUsernameKey(int index) {
  return "u" + String(index);
}

static String makePasswordKey(int index) {
  return "p" + String(index);
}

static void saveUserCount(int count) {
  userPrefs.putInt("count", count);
}

static int loadUserCount() {
  return userPrefs.getInt("count", 0);
}

// ======================================================
// SETUP
// ======================================================
void userManagerSetup() {
  userPrefs.begin(PREF_USERS_NAMESPACE, false);

  int count = loadUserCount();

  // Jika belum ada user, buat admin/admin
  if (count == 0) {
    userPrefs.putString(makeUsernameKey(0).c_str(), DEFAULT_USERNAME);
    userPrefs.putString(makePasswordKey(0).c_str(), DEFAULT_PASSWORD);
    saveUserCount(1);

    Serial.println("Default user created:");
    Serial.print("Username: ");
    Serial.println(DEFAULT_USERNAME);
    Serial.print("Password: ");
    Serial.println(DEFAULT_PASSWORD);
  }

  userPrefs.end();
}

// ======================================================
// AUTHENTICATION
// ======================================================
bool authenticateUser(const String &username, const String &password) {
  int count = getUserCount();

  for (int i = 0; i < count; i++) {
    User user = getUser(i);

    if (user.username == username &&
        user.password == password) {
      return true;
    }
  }

  return false;
}

// ======================================================
// USER CRUD
// ======================================================
int getUserCount() {
  userPrefs.begin(PREF_USERS_NAMESPACE, true);
  int count = loadUserCount();
  userPrefs.end();

  return count;
}

User getUser(int index) {
  User user;
  user.username = "";
  user.password = "";

  int count = getUserCount();

  if (index < 0 || index >= count) {
    return user;
  }

  userPrefs.begin(PREF_USERS_NAMESPACE, true);

  user.username =
      userPrefs.getString(makeUsernameKey(index).c_str(), "");

  user.password =
      userPrefs.getString(makePasswordKey(index).c_str(), "");

  userPrefs.end();

  return user;
}

bool addUser(const String &username, const String &password) {
  if (username.length() == 0) return false;
  if (username.length() > MAX_USERNAME_LENGTH) return false;
  if (password.length() > MAX_PASSWORD_LENGTH) return false;

  if (userExists(username)) {
    return false;
  }

  userPrefs.begin(PREF_USERS_NAMESPACE, false);

  int count = loadUserCount();

  if (count >= MAX_USERS) {
    userPrefs.end();
    return false;
  }

  userPrefs.putString(
      makeUsernameKey(count).c_str(),
      username);

  userPrefs.putString(
      makePasswordKey(count).c_str(),
      password);

  saveUserCount(count + 1);

  userPrefs.end();

  Serial.print("User added: ");
  Serial.println(username);

  return true;
}

bool updateUser(int index,
                const String &username,
                const String &password) {
  int count = getUserCount();

  if (index < 0 || index >= count) {
    return false;
  }

  if (username.length() == 0) {
    return false;
  }

  // Cek apakah username dipakai user lain
  for (int i = 0; i < count; i++) {
    if (i == index) continue;

    User u = getUser(i);
    if (u.username == username) {
      return false;
    }
  }

  userPrefs.begin(PREF_USERS_NAMESPACE, false);

  userPrefs.putString(
      makeUsernameKey(index).c_str(),
      username);

  userPrefs.putString(
      makePasswordKey(index).c_str(),
      password);

  userPrefs.end();

  Serial.print("User updated: ");
  Serial.println(username);

  return true;
}

bool deleteUser(int index) {
  int count = getUserCount();

  if (index < 0 || index >= count) {
    return false;
  }

  // Minimal harus ada 1 user
  if (count <= 1) {
    return false;
  }

  userPrefs.begin(PREF_USERS_NAMESPACE, false);

  // Geser semua user setelah index ke depan
  for (int i = index; i < count - 1; i++) {
    String nextUsername =
        userPrefs.getString(makeUsernameKey(i + 1).c_str(), "");

    String nextPassword =
        userPrefs.getString(makePasswordKey(i + 1).c_str(), "");

    userPrefs.putString(
        makeUsernameKey(i).c_str(),
        nextUsername);

    userPrefs.putString(
        makePasswordKey(i).c_str(),
        nextPassword);
  }

  // Hapus user terakhir
  userPrefs.remove(makeUsernameKey(count - 1).c_str());
  userPrefs.remove(makePasswordKey(count - 1).c_str());

  // Update jumlah user
  saveUserCount(count - 1);

  userPrefs.end();

  Serial.print("User deleted at index: ");
  Serial.println(index);

  return true;
}

// ======================================================
// HELPER FUNCTIONS
// ======================================================
bool userExists(const String &username) {
  return findUserIndex(username) != -1;
}

int findUserIndex(const String &username) {
  int count = getUserCount();

  for (int i = 0; i < count; i++) {
    User user = getUser(i);

    if (user.username == username) {
      return i;
    }
  }

  return -1;
}

// ======================================================
// HTML GENERATION
// ======================================================
String generateUsersTableHTML() {

  String html;

  html += R"rawliteral(

  <style>

    .user-table {
      width: 100%;
      border-collapse: collapse;
    }

    .user-table th,
    .user-table td {
      padding: 16px;
      border-bottom: 1px solid #e5e7eb;
    }

    .user-table th {
      text-align: left;
      font-size: 13px;
      font-weight: 700;
      color: #6b7280;
      background: #f8fafc;
    }

    .user-table td {
      font-size: 14px;
      color: #111827;
    }

    .user-table tr:hover {
      background: #f9fafb;
    }

    .action-buttons {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 14px;
    }

    .action-btn {
      border: none !important;
      background: transparent !important;
      padding: 0 !important;
      margin: 0 !important;
      outline: none !important;
      box-shadow: none !important;
      cursor: pointer;
      line-height: 0;
    }

    .action-btn svg {
      width: 20px;
      height: 20px;
    }

    .edit-btn {
      color: #1b6b3d;
    }

    .delete-btn {
      color: #d97706;
    }

    .edit-btn:hover {
      color: #1d4ed8;
      transform: scale(1.1);
    }

    .delete-btn:hover {
      color: #b91c1c;
      transform: scale(1.1);
    }

  </style>

  )rawliteral";

  html += "<table class='user-table'>";

  html += "<thead>";
  html += "<tr>";
  html += "<th style='width:70px'>NO</th>";
  html += "<th>USERNAME</th>";
  html += "<th style='width:80px;text-align:center'>ACTION</th>";
  html += "</tr>";
  html += "</thead>";

  html += "<tbody>";

  int count = getUserCount();

  for (int i = 0; i < count; i++) {

    User user = getUser(i);

    html += "<tr>";

    // NO
    html += "<td>";
    html += String(i + 1);
    html += "</td>";

    // USERNAME
    html += "<td style='font-weight:600'>";
    html += user.username;
    html += "</td>";

    // ACTION
    html += "<td>";

    html += "<div class='action-buttons'>";

    // EDIT
    html += "<button ";
    html += "class='action-btn edit-btn' ";
    html += "onclick='editUser(" + String(i) + ",\"" + user.username + "\")'>";

    html += R"rawliteral(
      <svg viewBox="0 0 24 24"
      fill="none"
      stroke="currentColor"
      stroke-width="2"
      stroke-linecap="round"
      stroke-linejoin="round">

        <path d="M4 4h10v2H6v12h12v-8h2v10H4V4z"/>
        <path d="M20.7 7.3l-4-4L9 11v4h4l7.7-7.7z"/>

      </svg>
    )rawliteral";

    html += "</button>";

    // DELETE
    html += "<button ";
    html += "class='action-btn delete-btn' ";
    html += "onclick='deleteUser(" + String(i) + ")'>";

    html += R"rawliteral(
      <svg viewBox="0 0 24 24"
      fill="none"
      stroke="currentColor"
      stroke-width="2"
      stroke-linecap="round"
      stroke-linejoin="round">

        <path d="M9 3h6l1 2h4v2H4V5h4l1-2zm-1 6h2v9H8V9zm6 0h2v9h-2V9zM6 7h12l-1 13a2 2 0 01-2 2H9a2 2 0 01-2-2L6 7z"/>

      </svg>
    )rawliteral";

    html += "</button>";

    html += "</div>";

    html += "</td>";

    html += "</tr>";
  }

  html += "</tbody>";
  html += "</table>";

  return html;
}