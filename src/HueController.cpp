#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
// #include <WebSocketsServer.h>
#include <PubSubClient.h>
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
#define STASSID "Fuu"           // Router SSID, if you want to connect to your existing network
#define STAPSK "UL6jucYSz4bdeV" // Router Password, change this too
#endif
#define SKIPSTA false             // Change to true if you want to skip connecting to existing router
#define APSSID "HueController"    // SSID of Access Point created by ESP8266, if can't connect to Router SSID
#define APPSK "helloworld"        // Password of Access Point created by ESP8266
#define NUM_LEDS 34               // Change to total amount of your leds on this project
#define DATA_PIN 5                // Change to your NEOPIXEL DATA Pin connected. See FastLED Pinout
#define MQTT_SERVER "172.20.10.2" // MQTT Server

// WebSocketsServer webSocket = WebSocketsServer(PORT);

WiFiClient espClient;
PubSubClient client(espClient);
CRGB leds[NUM_LEDS];

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

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

void callback(char *topic, uint8_t *payload, unsigned int length)
{
    // Serial.print("Message arrived [");
    // Serial.print(topic);
    // Serial.print("] ");
    // for (int i = 0; i < length; i++) {
    //     Serial.print((char)payload[i]);
    // }
    // Serial.println();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);
    uint8_t style = doc["style"];
    uint8_t h = doc["h"];
    uint8_t s = doc["s"];
    uint8_t v = doc["v"];
    setColor(style, h, s, v);
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "HueController-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            //   client.publish("presence", "hello world");
            client.subscribe("changeColor");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
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
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
    // webSocket.begin();
    // webSocket.onEvent(webSocketEvent);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
    ArduinoOTA.handle();
    // webSocket.loop();
}
