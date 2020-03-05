# Wemos D1 Mini

## [WIP] HSL Firmware

---

Branch ini berisi file project PlatformIO dengan firmware yang dibutuhkan untuk diburn ke Wemos D1 Mini

### Dependencies

- esp8266/Arduino board version 2.6.3
- Links2004/arduinoWebSockets version 2.1.4
- bblanchon/ArduinoJson version 6.14.1
- FastLED/FastLED version 3.3.3

### Config

Terdapat beberapa konstanta yang harus dirubah dulu sebelum di flash ke board, diantaranya:

1. PORT server dari WebSocket, disini saya set :8000, tetapi bisa diubah sesuka hati.
2. STASSID !wajib, ubah menjadi SSID dari router WiFi jika ingin terhubung dengan jaringan router.
3. STAPSK !wajib, ubah juga dengan password dari router.
4. SKIPSTA ubah menjadi `true` jika ingin melewati board mencoba terhubung ke jaringan yang sudah ada.
5. APSSID !wajib, ubah menjadi SSID untuk controller terhubung. Disini saya pakai SSID "HueController".
6. APPSK !wajib, sama seperti diatas, untuk set password dari access point yang dibuat oleh board.
7. NUM_LEDS !WAJIB, ubah menjadi jumlah total mata WS2812B/Neopixel yang digunakan.
8. DATA_PIN !WAJIB, ubah menjadi PIN yang digunakan untuk PIN DATA (PIN tengah jika di RGB Strip WS2812B). Disini saya menggunakan PIN D1 di Wemos D1 Mini.

### Panduan

Setelah konfigurasi diatas disesuaikan dengan board, selanjutnya flash script dan lihat Serial Monitor untuk melihat IP dari board.
Jika board membuat Access Point, maka IP default boardnya akan menjadi `192.168.4.1`. Jika board terhubung dengan jaringan WiFi yang sudah ada, maka cek IP di Serial Monitor atau dengan LAN Scanner.

### Roadmap

- [x] Controlled using websocket client
- [ ] Add some style for color changing
- [ ] Add some mode
- [ ] Make settings persistent (board IP, memorize mode, style, color)
- [ ] Make native desktop and mobile app controller
