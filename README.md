# Smart Garden IoT System

## Deskripsi

Smart Garden IoT System adalah sistem otomasi taman berbasis ESP32 yang dirancang untuk memantau kondisi lingkungan dan mengendalikan perangkat taman secara otomatis maupun manual melalui web dashboard lokal dan MQTT.

Sistem ini mampu memonitor kelembapan tanah dan kondisi hujan, mengontrol pompa penyiraman serta jemuran otomatis, dan menyediakan antarmuka manajemen pengguna serta konfigurasi jaringan WiFi melalui browser.

---

## Fitur Utama

### Monitoring Sensor

* Monitoring kelembapan tanah (Soil Moisture Sensor)
* Monitoring kondisi hujan (Rain Sensor)
* Tampilan persentase sensor secara realtime
* Status sensor:

  * DRY / WET
  * RAINING / CLEAR
* Deteksi sensor tidak terhubung atau rusak

### Kontrol Aktuator

#### Pompa Air

* Mode otomatis berdasarkan kondisi tanah
* Mode manual melalui dashboard
* Mode manual melalui MQTT
* Timer pompa manual yang dapat diatur pengguna
* Countdown timer realtime
* Auto OFF ketika timer selesai

#### Jemuran Otomatis

* Masuk otomatis saat hujan
* Keluar otomatis saat cuaca cerah
* Kontrol manual melalui dashboard
* Kontrol manual melalui MQTT

### Dashboard Web Lokal

* Login authentication
* Monitoring sensor realtime
* Monitoring status aktuator
* Monitoring status koneksi sensor
* Monitoring mode sistem
* Manual control pompa
* Manual control jemuran
* Timer pompa manual
* AJAX realtime update tanpa refresh halaman

### Manajemen WiFi

* Konfigurasi WiFi melalui web dashboard
* Penyimpanan kredensial menggunakan Preferences
* Fallback ke Access Point apabila koneksi WiFi gagal
* Reset konfigurasi WiFi

### Manajemen User

* Login multi-user
* Tambah user
* Edit user
* Hapus user
* Session management menggunakan cookie

### MQTT Integration

Publish:

* smarthome/pagi/taman/tanah/state
* smarthome/pagi/taman/hujan/state
* smarthome/pagi/taman/tanah/health
* smarthome/pagi/taman/hujan/health
* smarthome/pagi/taman/pompa/state
* smarthome/pagi/taman/jemuran/state

Subscribe:

* smarthome/pagi/taman/pompa/set
* smarthome/pagi/taman/jemuran/set
* smarthome/pagi/dapur/api/state

---

## Teknologi yang Digunakan

### Hardware

* ESP32
* Soil Moisture Sensor
* Rain Sensor
* Relay Module
* Water Pump
* Servo Motor
* MB102 Power Supply
* Kabel Jumper

### Software

* PlatformIO
* Arduino Framework
* ESP32Servo
* PubSubClient
* WebServer
* Preferences

---

## Struktur Project

```text
src/
│
├── main.cpp
├── config.h
│
├── hardware.cpp
├── hardware.h
│
├── mqtt_manager.cpp
├── mqtt_manager.h
│
├── wifi_manager.cpp
├── wifi_manager.h
│
├── web_server.cpp
├── web_server.h
│
├── web_pages.cpp
├── web_pages.h
│
├── user_manager.cpp
└── user_manager.h
```

---

## Mode Operasi

### AUTO Mode

Pada mode otomatis:

* Pompa dikendalikan berdasarkan kelembapan tanah
* Jemuran dikendalikan berdasarkan kondisi hujan
* Monitoring sensor berjalan secara otomatis

### MANUAL Mode

Pada mode manual:

* User mengontrol perangkat secara langsung
* Pompa dapat dijalankan menggunakan timer
* Jemuran dapat dikendalikan secara manual
* Automation dinonaktifkan sementara

---

## Default Login

```text
Username : admin
Password : admin
```

---

## MQTT Broker

```text
Broker   : 154.19.38.42
Port     : 1883
Username : iot-pagi
Password : iot-pagi-pass
```

---

## Akses Dashboard

### Mode Access Point

```text
SSID : ESP32-TAMAN
URL  : http://192.168.4.1
```

### Mode Station

```text
http://<IP-ESP32>
```

---

## Author

Kelompok 3 👨🏼‍💻

Teknik Informatika – Computer Networking
