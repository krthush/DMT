#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <TimerOne.h>

#include <ClickEncoder.h>

// Declarations for Wireless Module
RF24 radio(12, 13); // CE, CSN
const byte address[6] = "00001";

// Declarations for Switch
int modeSwitchReading; // the current reading from 'mode' switch the input pin
int onSwitchReading; // the current reading from the 'on' switch input pin
int onState = LOW; // the current state of 'on'
int modeState = LOW;      // the current state of 'mode'

// Declarations for encoders
ClickEncoder *elbowEncoder;
ClickEncoder *shoulderEncoder;
ClickEncoder *twistEncoder;
ClickEncoder *gripEncoder;

int elbowLastValue = -1;
int shoulderLastValue = -1;
int twistLastValue = -1;
int gripLastValue = -1;

int elbowValue = 120;
int shoulderValue = 170;
int twistValue = 73;
int gripValue = 70;

int elbowNewAngle = 120;
int shoulderNewAngle = 170;
int twistNewAngle = 73;
int gripNewAngle = 70;

String comma;
String allAngles;
char sendText[20];

void timerIsr() {
  elbowEncoder->service();
  shoulderEncoder->service();
  twistEncoder->service();
  gripEncoder->service();
}

void setup() {

  Serial.begin(9600);             // Serial port to computer

  // Initialize Values
  comma = String(",");
  allAngles = String();

  //Switch Pin Mode
  pinMode(2, INPUT); // Set pin 2 as input for onSwitch
  pinMode(3, INPUT); // Set pin 3 as input for modeSwitch

  /* Elbow Pins */
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT); // Button
  elbowEncoder = new ClickEncoder(A0, A1, A2, 4);
  elbowEncoder->setAccelerationEnabled(true); // Acceleration enabled

  /* Shoulder Pins */
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT); // Button
  shoulderEncoder = new ClickEncoder(A3, A4, A5, 4);
  shoulderEncoder->setAccelerationEnabled(true); // Acceleration enabled

  /* Twist Pins */
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT); // Button
  twistEncoder = new ClickEncoder(A8, A9, A10, 4);
  twistEncoder->setAccelerationEnabled(true); // Acceleration enabled

  /* Grip Pins */
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT); // Button
  gripEncoder = new ClickEncoder(A11, A12, A13, 4);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  //Wireless Port Setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  // Check if 'on' switch is in HIGH or LOW
  onSwitchReading = digitalRead(2);
  if (onSwitchReading == LOW) { // Why LOW? seems odd, needs investigation.
    onState = HIGH;
  } else {
    onState = LOW;
  }

  // Check if 'mode' switch is in HIGH or LOW
  modeSwitchReading = digitalRead(3);
  if (modeSwitchReading == HIGH) {
    modeState = HIGH;
  } else {
    modeState = LOW;
  }

  if (onState == HIGH) {
    if (modeState == HIGH) { // Check if mode switch is on dials OR kinect mode
      Serial.print("Dials ");

      /* Elbow Encoder Code */
      elbowValue += elbowEncoder->getValue();
      if (elbowValue != elbowLastValue) {
        elbowLastValue = elbowValue;
      }
      if (elbowValue > 0 && elbowValue < 180) {
        elbowNewAngle = elbowValue;
      } else if (elbowValue <= 0) {
        elbowValue = 1;
      } else if (elbowValue >= 180) {
        elbowValue = 179;
      }

      /* Shoulder Encoder Code */
      shoulderValue += shoulderEncoder->getValue();
      if (shoulderValue != shoulderLastValue) {
        shoulderLastValue = shoulderValue;
      }
      if (shoulderValue > 0 && shoulderValue < 180) {
        shoulderNewAngle = shoulderValue;
      } else if (shoulderValue <= 0) {
        shoulderValue = 1;
      } else if (shoulderValue >= 180) {
        shoulderValue = 179;
      }

      /* Twist Encoder Code */
      twistValue += twistEncoder->getValue();
      if (twistValue != twistLastValue) {
        twistLastValue = twistValue;
      }
      if (twistValue > 0 && twistValue < 180) {
        twistNewAngle = twistValue;
      } else if (twistValue <= 0) {
        twistValue = 1;
      } else if (twistValue >= 180) {
        twistValue = 179;
      }

      /* Grip Encoder Code */
      gripValue += gripEncoder->getValue();
      if (gripValue != gripLastValue) {
        gripLastValue = gripValue;
      }
      if (gripValue > 0 && gripValue <= 90) {
        gripNewAngle = gripValue;
      } else if (gripValue <= 0) {
        gripValue = 1;
      } else if (gripValue >= 180) {
        gripValue = 179;
      }

      // Strings are combined to allAngles to be sent through radio/serial
      allAngles = elbowNewAngle + comma + shoulderNewAngle + comma + twistNewAngle + comma + gripNewAngle;
      Serial.println(allAngles);

      // Convert to char array to be sent through radio.write
      allAngles.toCharArray(sendText, sizeof(sendText));
      radio.write(&sendText, sizeof(sendText));

    } else { // Kinect Mode
      Serial.print("Kinect ");

      /* SHOULDER AND ELBOW obtained from serial which comes from Kinect code */
      elbowNewAngle = 180-Serial.parseInt(); // Flip for calibration
      shoulderNewAngle = 180-Serial.parseInt(); // Flip for calibration

      // strings are combined to allAngles to be sent through radio/serial
      allAngles = elbowNewAngle + comma + shoulderNewAngle + comma + twistNewAngle + comma + gripNewAngle;
      Serial.println(allAngles);

      // Convert to char array to be sent through radio.write
      allAngles.toCharArray(sendText, sizeof(sendText));
      radio.write(&sendText, sizeof(sendText));
      delay(15);
    }
  }
}

