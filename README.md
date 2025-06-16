## ESP32 Relay and Buzzer Control with Web Server

This project enables you to control a relay and buzzer connected to an ESP32 through a simple web interface. The web page acts as a single-page application (SPA) that interacts with the ESP32 over HTTP to toggle the relay and activate the buzzer.

Features

Control relay ON/OFF via a web dashboard.

Buzzer beeps continuously in 500ms intervals when relay is ON.

Real-time status updates on the web page without page reload.

Simple HTTP REST API endpoints for state retrieval and toggle.

Hardware Setup

ESP32 development board.

Relay module connected to GPIO 25.

Buzzer connected to GPIO 26.

Proper power and ground connections for relay and buzzer.

Software Setup

Replace the WiFi credentials (SSID and password) in the source code with your network information.

Upload the code to the ESP32 using the Arduino IDE or compatible platform.

Usage

Open the Serial Monitor to find the IP address assigned to the ESP32.

Open a web browser on the same network and enter the IP address.

Use the toggle button on the page to turn the relay ON or OFF.

When the relay is ON, the buzzer will beep in intervals of 500 milliseconds.

The page updates the status and button label dynamically via JavaScript fetch calls.

Notes

You can modify GPIO pins for relay and buzzer by changing relayPin and buzzerPin variables in the code.

Make sure the relay and buzzer modules are compatible with the ESP32 voltage levels (typically 3.3V logic).

License
This project is open-source and free for personal, educational, and non-commercial use.

