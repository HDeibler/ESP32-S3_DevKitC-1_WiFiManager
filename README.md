# ESP32-S3_DevKitC-1_WiFiManager

WiFiManager is a GUI middleman for the ESP32 WiFi library, specifically designed for the ESP32-S3-DevKitC-1. This library provides a user-friendly interface for managing WiFi connections directly from the Serial Monitor. With WiFiManager, you can scan for available networks, connect to a WiFi network, disconnect, and view connection details with ease. Additionally, it offers functionality to change the baud rate, clear stored WiFi credentials, and more, all via simple serial commands.
Features

    User-Friendly Interface: Manage WiFi connections through a simple menu displayed in the Serial Monitor.
    
    Scan for Networks: Easily scan and list available WiFi networks.
    
    Connect to WiFi: Connect to a selected WiFi network with or without a password.
    
    Disconnect: Disconnect from the current WiFi network and clear stored credentials.
    
    Auto-Reconnect: Automatically reconnect to a previously saved network upon reboot.
    
    LED Indicators: Use onboard NeoPixel LED to indicate connection status (Red for disconnected, Yellow for connecting, Green for connected).
    
    Change Baud Rate: Dynamically change the serial communication baud rate.
    
    Clear Preferences: Clear stored WiFi credentials from the device.
    
    Help Command: Display all available commands and their usage.

Commands

    /wifi: Open the WiFi manager menu.
    
    /exit: Exit the WiFi manager menu.
    
    /back: Return to the previous menu.
    
    /help: Display all available commands.
    

Menu Options

    1.) Connect to WiFi: Scan for networks and connect to the selected network.
    
    2.) Scan for WiFi networks: Display available WiFi networks.
    
    3.) Show WiFi/ESP32 info: Show current WiFi connection details and ESP32 IP address.
    
    4.) Disconnect from WiFi: Disconnect from the current WiFi network.
    
    5.) Change WiFi mode: Switch between Station, Access Point, and Station+AP modes.
    
    6.) Turn off WiFi: Turn off the WiFi module.

    7.) Turn on WiFi: Turn on the WiFi module.
    
    8.) Clear WiFi Preferences: Clear stored WiFi credentials.
    
    9.) Change Baud Rate: Change the serial communication baud rate.

Download the ZIP file from GitHub and extract it, or clone the repository using:
    
    git clone https://github.com/yourusername/WiFiManager.git

Copy to Arduino Libraries Folder:

    Copy the WiFiManager folder to your Arduino libraries folder. The default location is usually:
    
        Windows: C:\Users\YourUsername\Documents\Arduino\libraries
        
        macOS: ~/Documents/Arduino/libraries
        
        Linux: ~/Arduino/libraries

Restart Arduino IDE:

    Close and reopen the Arduino IDE to refresh the list of available libraries.

Use the following include statement in your Arduino sketch:

    #include <WiFiManager.h>
