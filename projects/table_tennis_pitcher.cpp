#include <Arduino.h>
#include <Servo.h>

Servo myservo;
const int buttonPin = 2;  // Button on digital pin 2
const int servoPin = 9;   // Servo on pin 9

bool buttonPressed = false;
int repeatCount = 10;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP); // Set up the button with an internal pull-up resistor
    myservo.attach(servoPin);
    myservo.write(120);  // Default servo position
}

void loop() {
    // Waiting for the button press
    if (digitalRead(buttonPin) == LOW) {  
        delay(50); // Debounce to remove false presses
        if (digitalRead(buttonPin) == LOW) { // Confirm button press
            buttonPressed = true;
        }
    }

    if (buttonPressed) {
        for (int i = 0; i < repeatCount; i++) {
            myservo.write(120); // Move servo to position 120°
            delay(1000);
            myservo.write(30);  // Move servo to position 30°
            delay(90);
        }
        myservo.write(120);
        buttonPressed = false; // After 10 repetitions, wait for a new button press
    }
}
