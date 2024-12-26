#ifndef ESP8266
#error This code is designed to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1

#include "ESP8266_ISR_Servo.h"

#define MIN_MICROS      800
#define MAX_MICROS      2450

int servoIndex1 = -1;
int targetPosition = 0;  // Target position for the servo
int currentPosition = 0; // Current position of the servo
bool moveRequested = false;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Replace these with your WiFi and Blynk credentials
const char ssid[] = "Your_WiFi_SSID";       // Your WiFi SSID
const char pass[] = "Your_WiFi_Password";   // Your WiFi Password
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"  // Your Blynk Auth Token

// Blynk template settings (replace with your actual template details)
#define BLYNK_TEMPLATE_ID "Your_Blynk_Template_ID"  // Your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "Your_Blynk_Template_Name"  // Your Blynk Template Name

// Define Virtual Pin for controlling the servo
#define VIRTUAL_PIN V1

// Define LED pin (D4 on ESP8266 is GPIO2)
#define LED_PIN 2

void blinkLED() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, LOW);   // Turn the LED on
    delay(100);                   // Wait 100ms
    digitalWrite(LED_PIN, HIGH);  // Turn the LED off
    delay(100);                   // Wait 100ms
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting ESP8266_ISR_MultiServos on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP8266_ISR_SERVO_VERSION);

  pinMode(LED_PIN, OUTPUT);       // Initialize LED pin as an output
  digitalWrite(LED_PIN, HIGH);    // Turn off LED initially

  servoIndex1 = ISR_Servo.setupServo(5, MIN_MICROS, MAX_MICROS);

  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();  // Process Blynk events

  // Read from Serial Monitor
  if (Serial.available() > 0)
  {
    int received = Serial.parseInt();

    if (received >= 0 && received <= 180)
    {
      targetPosition = received;
      moveRequested = true;
      Serial.print(F("Received new position from Serial Monitor: "));
      Serial.println(targetPosition);

      // Blink LED when data is received from Serial Monitor
      blinkLED();

      // Update Blynk with the new position
      Blynk.virtualWrite(VIRTUAL_PIN, targetPosition);
    }
    else
    {
      Serial.println(F("Position out of range (0-180)."));
    }
  }

  if (moveRequested && servoIndex1 != -1)
  {
    currentPosition = ISR_Servo.getPosition(servoIndex1);

    if (currentPosition != targetPosition)
    {
      Serial.print(F("Moving servo from "));
      Serial.print(currentPosition);
      Serial.print(F(" to "));
      Serial.println(targetPosition);

      // Move servo to the target position
      if (currentPosition < targetPosition)
      {
        for (int position = currentPosition; position <= targetPosition; position++)
        {
          ISR_Servo.setPosition(servoIndex1, position);
          delay(10); // Adjust delay as needed
        }
      }
      else
      {
        for (int position = currentPosition; position >= targetPosition; position--)
        {
          ISR_Servo.setPosition(servoIndex1, position);
          delay(10); // Adjust delay as needed
        }
      }

      // Move servo back to 0 degrees
      Serial.println(F("Returning servo to 0 degrees"));
      if (targetPosition != 0)
      {
        for (int position = targetPosition; position >= 0; position--)
        {
          ISR_Servo.setPosition(servoIndex1, position);
          delay(10); // Adjust delay as needed
        }
        Blynk.virtualWrite(VIRTUAL_PIN, 0);
      }

      moveRequested = false;
    }
  }

  delay(2000); // Adjust delay as needed
}

// Blynk virtual pin handler
BLYNK_WRITE(VIRTUAL_PIN)
{
  int received = param.asInt(); // Read integer value from Blynk app

  if (received >= 0 && received <= 180) // Check if within valid range
  {
    targetPosition = received;
    moveRequested = true;
    Serial.print(F("Received new position from Blynk: "));
    Serial.println(targetPosition);

    // Blink LED when data is received from Blynk
    blinkLED();
  }
  else
  {
    Serial.println(F("Position out of range (0-180)."));
  }
}
