#include "WiFiManager.h"

WiFiManager::WiFiManager()
    : pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800), previousMillis(0), inMenu(false) {}

void WiFiManager::begin() {
    Serial.begin(baudRate);
    Serial.println();
    pixels.begin();
    setLEDColor(pixels.Color(255, 255, 0)); // Yellow for initializing
    pixels.setBrightness(10);
    WiFi.disconnect(true); // Clear any stored WiFi credentials
    preferences.begin("wifi", false); // Open preferences in RW mode

    autoReconnect(); // Attempt to reconnect if credentials are stored
}

void WiFiManager::handle() {
    unsigned long currentMillis = millis();

    if (WiFi.status() == WL_CONNECTED) {
        setLEDColor(pixels.Color(0, 255, 0)); // Green
    } else if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (WiFi.status() != WL_CONNECTED && !inMenu) {
            setLEDColor(pixels.Color(255, 0, 0)); // Red
        }
    }
}

void WiFiManager::commandHandler(const String& command) {
    if (command == "/wifi") {
        inMenu = true;
        showMenu();
    } else if (command == "/exit") {
        exitMenu();
    } else if (command == "/back") {
        showMenu();
    } else if (command == "/help") {
        showHelp();
    } else if (inMenu) {
        processMenu();
    }
}

void WiFiManager::scanNetworks() {
    Serial.println("\nScanning for networks...");
    int n = WiFi.scanNetworks();
    Serial.println("Scan complete.");
    if (n == 0) {
        Serial.println("No networks found.");
    } else {
        Serial.print(n);
        Serial.println(" networks found:");
        for (int i = 0; i < n; i++) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.println(")");
            delay(10);
        }
    }
}

void WiFiManager::connectToNetwork() {
    scanNetworks();

    Serial.print("\nEnter the number of the network you want to connect to: ");
    clearSerialBuffer();
    while (Serial.available() == 0) {
        delay(10);
    }
    int networkNumber = Serial.parseInt();
    clearSerialBuffer();
    if (networkNumber < 1 || networkNumber > WiFi.scanComplete()) {
        Serial.println("Invalid choice. Please try again.");
        return;
    }

    String ssid = WiFi.SSID(networkNumber - 1);
    Serial.println("\n==================");
    Serial.print("\nSelected network: ");
    Serial.println(ssid);

    Serial.println("Does the network have a password? (y/n): ");
    while (Serial.available() == 0) {
        delay(10);
    }
    char hasPassword = Serial.read();
    clearSerialBuffer();
    String password = "";

    if (hasPassword == 'y' || hasPassword == 'Y') {
        Serial.print("Enter password: ");
        while (Serial.available() == 0) {
            delay(10);
        }
        password = Serial.readStringUntil('\n');
        password.trim();
    }

    connectToNetwork(ssid.c_str(), password.c_str());
}

void WiFiManager::connectToNetwork(const char* ssid, const char* password) {
  delay(2000);
    if (reconnecting) {
        Serial.print("\nAuto Connecting to ");
    } else {
        Serial.print("\nConnecting to ");
    }
    
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    setLEDColor(pixels.Color(255, 255, 0)); // Yellow for connecting

    // Save credentials
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    // Wait for connection
    int attemptCount = 0;
    const int maxAttempts = 20; // Adjust the number of attempts as needed

    while (WiFi.status() != WL_CONNECTED && attemptCount < maxAttempts) {
        delay(500); // Wait 1 second before checking the connection status again
        attemptCount++;
        Serial.print(".");
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
        delay(2000);
        Serial.print("WiFi Connected To: ");
        Serial.println(ssid);
        Serial.println();
        setLEDColor(pixels.Color(0, 255, 0)); // Green for connected
        reconnecting = false;
    } else {
        delay(2000);
        Serial.print("Failed To Connect To: ");
        Serial.println(ssid);
        Serial.println();
        setLEDColor(pixels.Color(255, 0, 0)); // Red for failed connection
        reconnecting = false;
    }
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    Serial.println("\nDisconnected from WiFi.");
    setLEDColor(pixels.Color(255, 0, 0)); // Red for disconnected
    preferences.clear(); // Clear stored WiFi credentials
}

void WiFiManager::showMenu() {
    Serial.println("\n\n===== Main Menu =====");
    Serial.println("Type \"/help\" for more commands");
    Serial.println("1. Connect to WiFi");
    Serial.println("2. Scan for WiFi networks");
    Serial.println("3. Show WiFi/ESP32 info");
    Serial.println("4. Disconnect from WiFi");
    Serial.println("5. Change WiFi mode");
    Serial.println("6. Turn off WiFi");
    Serial.println("7. Turn on WiFi");
    Serial.println("8. Clear WiFi Preferences");
    Serial.println("9. Change Baud Rate");
    Serial.print("Enter your choice: ");
}

void WiFiManager::setMode(WiFiMode_t mode) {
    WiFi.mode(mode);
    Serial.println("WiFi mode set to ");
    switch (mode) {
        case WIFI_MODE_STA: Serial.println("STA"); break;
        case WIFI_MODE_AP: Serial.println("AP"); break;
        case WIFI_MODE_APSTA: Serial.println("APSTA"); break;
        case WIFI_MODE_NULL: Serial.println("NULL"); break;
    }
}

void WiFiManager::turnOn() {
    WiFi.begin();
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    Serial.println("\nWiFi turned on.");
}

void WiFiManager::turnOff() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_MODE_NULL);
    Serial.println("\nWiFi turned off.");
}

void WiFiManager::processMenu() {
    delay(100); // Short delay to ensure serial buffer is ready
    clearSerialBuffer();
    
    while (Serial.available() == 0) {
        // Prevent the loop from running too fast and causing issues
        delay(10);
    }
    int choice = Serial.parseInt();
    clearSerialBuffer();

    switch (choice) {
        case 1:
            connectToNetwork();
            break;
        case 2:
            scanNetworks();
            break;
        case 3:
            Serial.println("\nWiFi/ESP32 info:");
            Serial.print("SSID: ");
            Serial.println(WiFi.SSID());
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            break;
        case 4:
            disconnect();
            break;
        case 5: {
            Serial.println("\nChange WiFi mode:");
            Serial.println("1. Station Mode (STA)");
            Serial.println("2. Access Point Mode (AP)");
            Serial.println("3. Station + Access Point Mode (APSTA)");
            Serial.print("Enter your choice: ");
            while (Serial.available() == 0) {
                delay(10);
            }
            int mode = Serial.parseInt();
            clearSerialBuffer();
            switch (mode) {
                case 1: setMode(WIFI_MODE_STA); break;
                case 2: setMode(WIFI_MODE_AP); break;
                case 3: setMode(WIFI_MODE_APSTA); break;
            }
            break;
        }
        case 6:
            turnOff();
            break;
        case 7:
            turnOn();
            break;
        case 8:
            clearWiFiPreferences();
            break;
        case 9:
            changeBaudRate();
            break;
        default:
            Serial.println("\nInvalid choice. Please try again.");
    }
}

void WiFiManager::exitMenu() {
    inMenu = false;
    Serial.println("\nExiting WiFi Manager...");
}

void WiFiManager::setLEDColor(uint32_t color) {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
}

void WiFiManager::clearSerialBuffer() {
    while (Serial.available()) {
        Serial.read();
    }
}

void WiFiManager::autoReconnect() {
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");

    if (ssid != "") {
        reconnecting = true;
        connectToNetwork(ssid.c_str(), password.c_str());
    } else {
        Serial.println("No stored WiFi credentials found.");
    }
}

void WiFiManager::clearWiFiPreferences() {
    preferences.clear();
    Serial.println("\nWiFi preferences cleared. Device will no longer auto-reconnect.");
}

void WiFiManager::showHelp() {
    Serial.println("\nAvailable Commands:");
    Serial.println("/wifi - Open WiFi manager");
    Serial.println("/exit - Exit WiFi manager");
    Serial.println("/back - Return to previous menu");
    Serial.println("/help - Show available commands");
}

void WiFiManager::changeBaudRate() {
    Serial.println("\nEnter new baud rate (e.g., 9600, 19200, 115200): ");
    clearSerialBuffer();
    while (Serial.available() == 0) {
        delay(10);
    }
    int newBaudRate = Serial.parseInt();
    clearSerialBuffer();

    if (newBaudRate > 0) {
        baudRate = newBaudRate;
        Serial.end();
        Serial.begin(baudRate);
        Serial.print("\nBaud rate changed to: ");
        Serial.println(baudRate);
    } else {
        Serial.println("\nInvalid baud rate. Please try again.");
    }
}
