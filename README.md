# Techno Low Money : ESP8266 Servo Control with Blynk

This project demonstrates how to control a servo motor using the ESP8266, Blynk app, and the ESP8266_ISR_Servo library. It includes functionality to control the servo's position via both the Serial Monitor and the Blynk app.
Our official website for this project is https://deweysworld.app/technolowmoney

## Features:
- Control servo position from the Blynk app (Virtual Pin `V1`).
- Servo position updates via Serial Monitor input.
- LED blink indication when new data is received.
- Automatically resets servo to 0 degrees after reaching the target position.

## Components:
- **ESP8266 board**
- **Servo motor**
- **Blynk app** (for controlling the servo remotely)

## Setup:

### 1. Clone or Download the Project
- Clone or download this repository to your local machine.

### 2. Install Required Libraries
- Ensure the following libraries are installed:
  - **ESP8266WiFi**: For connecting to WiFi.
  - **Blynk**: For connecting the ESP8266 to the Blynk app.
  - **ESP8266_ISR_Servo**: For controlling the servo motor.

### 3. Update WiFi Credentials
In the code, replace the following placeholders with your WiFi credentials, BLYNK TOKEN:
```cpp
const char ssid[] = "Your_WiFi_SSID";       // Your WiFi SSID
const char pass[] = "Your_WiFi_Password";   // Your WiFi Password
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"  // Your Blynk Auth Token
#define BLYNK_TEMPLATE_ID "Your_Blynk_Template_ID"  // Your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "Your_Blynk_Template_Name"  // Your Blynk Template Name


