#include <SoftwareSerial.h>

// Declarations for Wireless Module
SoftwareSerial HC12(12, 13); // HC-12 TX Pin, HC-12 RX Pin

// Declarations for Switch
int inPin = 2;         // the number of the input pin
int switchReading;           // the current reading from the input pin

void setup() {
  
  //Switch Pin Mode
  pinMode(inPin, INPUT);
  
  //Wireless Port Setup
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}

void loop() {
  switchReading = digitalRead(inPin);

  // Check if Switch is active, if so then send signal through
  if (switchReading == HIGH) {
    while (HC12.available()) {        // If HC-12 has data
      Serial.write(HC12.read());      // Send the data to Serial monitor
    }
    while (Serial.available()) {      // If Serial monitor has data
      HC12.write(Serial.read());      // Send that data to HC-12
    }
  }
}
