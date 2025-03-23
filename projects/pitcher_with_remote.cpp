#include <Arduino.h>
#include <IRremote.h>

const int buttonPin = 2;  // Button on digital pin 2
const int servoPin = 9;   // Servo on pin 9
const int RECV_PIN = 3;   // IR receiver on pin 3

bool buttonPressed = false;
int repeatCount = 10;  // Default number of repetitions
int repeatTime = 1000;

IRrecv irrecv(RECV_PIN);
decode_results results;

void moveServoManually(int angle) {
    int pwmValue = map(angle, 0, 180, 544, 2400); // Convert angle to pulse width
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pwmValue);
    digitalWrite(servoPin, LOW);
    delay(20); // 50 Hz frequency
}

void moveServo(int times) {
    for (int i = 0; i < times; i++) {
        moveServoManually(30);  // Move servo to position 30°
        delay(90);
        moveServoManually(60);  // Move servo to position 60°
        delay(repeatTime);
    }
}

void setup() {
    pinMode(buttonPin, INPUT_PULLUP); // Built-in button
    moveServoManually(120);
    Serial.begin(9600);
    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver
    Serial.println("Enabled IRin");
}

void loop() {
    // BUTTON ON ARDUINO
    if (digitalRead(buttonPin) == LOW) {  
        delay(50); // Debounce
        if (digitalRead(buttonPin) == LOW) {
            buttonPressed = true;
        }
    }

    if (buttonPressed) {
        moveServo(repeatCount);
        buttonPressed = false; // After 10 repetitions, wait for a new button press
    }

    // REMOTE CONTROL
    if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX); // Print button code

        switch (results.value) {
            case 0xFF02FD: // Start servo movement repeatCount times
                moveServo(repeatCount);
                break;
            
            case 0xFFE21D: // Start servo movement only once
                moveServo(1);
                break;

            case 0xFF629D: // Increase repeatCount by 1
                repeatCount++;
                Serial.print("repeatCount: ");
                Serial.println(repeatCount);
                break;

            case 0xFFA857: // Decrease repeatCount by 1 (minimum 1)
                if (repeatCount > 1) repeatCount--;
                Serial.print("repeatCount: ");
                Serial.println(repeatCount);
                break;

            case 0xFF906F: // Increase repeatTime by 100ms
                if (repeatTime > 100) repeatTime += 100;
                Serial.print("repeatTime: ");
                Serial.println(repeatTime);
                break;

            case 0xFFE01F: // Decrease repeatTime by 100ms
                if (repeatTime > 100) repeatTime -= 100;
                Serial.print("repeatTime: ");
                Serial.println(repeatTime);
                break;
        }

        irrecv.resume(); // Receive the next signal
    }
}
