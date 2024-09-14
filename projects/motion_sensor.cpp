// www.kuongshun.com
// 2023.6.18

#include <Arduino.h>

int ledPin = 13; // LED on Pin 13 of Arduino
int pirPin = 7;  // Input for HC-S501

int pirValue; // Place to store read PIR Value

void setup()
{

    pinMode(ledPin, OUTPUT);
    pinMode(pirPin, INPUT);
    digitalWrite(ledPin, LOW);
}

void loop()
{
    pirValue = digitalRead(pirPin);
    if (pirValue == 1)
    {
        digitalWrite(ledPin, HIGH);
    }
    else if (pirValue == 0)
    {
        digitalWrite(ledPin, LOW);
    }
}
