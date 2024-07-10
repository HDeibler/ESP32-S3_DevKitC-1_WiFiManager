#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

#define PIN 48
#define NUMPIXELS 1

class WiFiManager {
public:
    WiFiManager();
    void begin();
    void handle();
    void commandHandler(const String& command);
    void processMenu();

private:
    Adafruit_NeoPixel pixels;
    Preferences preferences;
    unsigned long previousMillis;
    const long interval = 10000; // Interval to check WiFi status
    bool inMenu;
    bool reconnecting = false;
    int baudRate = 19200;

    void scanNetworks();
    void connectToNetwork();
    void connectToNetwork(const char* ssid, const char* password);
    void disconnect();
    void showMenu();
    void setMode(WiFiMode_t mode);
    void turnOn();
    void turnOff();
    void setLEDColor(uint32_t color);
    void exitMenu();
    void clearSerialBuffer();
    void autoReconnect();
    void clearWiFiPreferences();
    void showHelp();
    void changeBaudRate();
};

#endif
