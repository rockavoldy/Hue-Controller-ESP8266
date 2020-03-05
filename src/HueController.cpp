#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERNAL
#include "FastLED.h"

#define USE_SERIAL Serial
#define HOSTNAME "HueController"

// User configurable
#define PORT 8000 // WebSocketServer port, default 8000
#ifndef STASSID
#define STASSID "std.64"    // Router SSID, if you want to connect to your existing network
#define STAPSK "h3ll0w0r1d" // Router Password, change this too
#endif
#define SKIPSTA false          // Change to true if you want to skip connecting to existing router
#define APSSID "HueController" // SSID of Access Point created by ESP8266, if can't connect to Router SSID
#define APPSK "helloworld"     // Password of Access Point created by ESP8266
#define NUM_LEDS 34            // Change to total amount of your leds on this project
#define DATA_PIN 5             // Change to your NEOPIXEL DATA Pin connected. See FastLED Pinout

WebSocketsServer webSocket = WebSocketsServer(PORT);

CRGB leds[NUM_LEDS];

void setColor(uint8_t style, uint8_t h, uint8_t s, uint8_t v)
{
    switch (style)
    {
    case 1: // static
        FastLED.showColor(CHSV(h, s, v));
        break;
    case 2: // running
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CHSV(h, s, v);
            FastLED.show();
            delay(10);
        }
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_ERROR:
        USE_SERIAL.println("Error!");
        break;
    case WStype_DISCONNECTED:
        USE_SERIAL.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        uint8_t style = doc["style"];
        uint8_t h = doc["h"];
        uint8_t s = doc["s"];
        uint8_t v = doc["v"];
        setColor(style, h, s, v);
        break;
    }
}

void setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.printf("Booting up %s\n", HOSTNAME);
    USE_SERIAL.println();

    WiFi.hostname(HOSTNAME);
    USE_SERIAL.printf("Connecting to existing network with SSID: %s\n", STASSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin((const char *)STASSID, (const char *)STAPSK);

    boolean connectedAsClient = false;
    if (SKIPSTA != true)
    {
        for (int i = 0; i < 30; i++)
        {
            USE_SERIAL.print(".");
            if (WiFi.status() == WL_CONNECTED)
            {
                connectedAsClient = true;
                i = 31;
            }
            delay(500);
        }
        if (connectedAsClient == true)
        {
            USE_SERIAL.println();
            USE_SERIAL.printf("Connected to %s !\n", STASSID);
            USE_SERIAL.print("IP Address: ");
            USE_SERIAL.println(WiFi.localIP());
        }
    }

    if (connectedAsClient != true)
    {
        USE_SERIAL.println();
        USE_SERIAL.printf("Connection failed to SSID: %s.\nCreate Access Point !\n", STASSID);
        USE_SERIAL.printf("SSID: %s\nPassword: %s\n", APSSID, APPSK);
        WiFi.softAP((const char *)APSSID, (const char *)APPSK);
        USE_SERIAL.print("IP Address: ");
        USE_SERIAL.println(WiFi.softAPIP());
    }
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            type = "sketch";
        }
        else
        { // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            Serial.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop()
{
    ArduinoOTA.handle();
    webSocket.loop();
}
