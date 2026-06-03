// ======================================================
// user_manager.h
// Manajemen user untuk login web dashboard
// ======================================================

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <Arduino.h>

// ======================================================
// USER STRUCT
// ======================================================
struct User {
  String username;
  String password;
};

// ======================================================
// SETUP
// ======================================================

// Inisialisasi user manager
// - Membuat user default admin/admin jika belum ada
void userManagerSetup();

// ======================================================
// AUTHENTICATION
// ======================================================

// Validasi username dan password
bool authenticateUser(const String &username, const String &password);

// ======================================================
// USER CRUD
// ======================================================

// Mendapat jumlah user
int getUserCount();

// Mengambil user berdasarkan index
User getUser(int index);

// Menambahkan user baru
// return false jika username sudah ada atau kapasitas penuh
bool addUser(const String &username, const String &password);

// Mengupdate user berdasarkan index
// return false jika index tidak valid
bool updateUser(int index,
                const String &username,
                const String &password);

// Menghapus user berdasarkan index
// return false jika index tidak valid
bool deleteUser(int index);

// ======================================================
// HELPER FUNCTIONS
// ======================================================

// Mengecek apakah username sudah ada
bool userExists(const String &username);

// Mendapat index user berdasarkan username
// return -1 jika tidak ditemukan
int findUserIndex(const String &username);

// ======================================================
// HTML GENERATION
// ======================================================

// Menghasilkan tabel HTML daftar user
String generateUsersTableHTML();

#endif // USER_MANAGER_H