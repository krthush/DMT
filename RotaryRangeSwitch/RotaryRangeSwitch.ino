#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Declarations for Wireless Module
RF24 radio(12, 13); // CE, CSN
const byte address[6] = "00001";

// Declarations for Switch
int modeSwitchReading; // the current reading from 'mode' switch the input pin
int onSwitchReading; // the current reading from the 'on' switch input pin
int onState = LOW; // the current state of 'on'
int modeState = LOW;      // the current state of 'mode'

// Declarations for encoders
int aStateElbow;
int aLastStateElbow;

int aStateShoulder;
int aLastStateShoulder;

int aStateTwist;
int aLastStateTwist;

int aStateGrip;
int aLastStateGrip;

int elbowNewAngle = 0;
int shoulderNewAngle = 0;
int twistNewAngle = 0;
int gripNewAngle = 0;

String comma;
String allAngles;
char sendText[20];

void setup() {

  Serial.begin(9600);             // Serial port to computer

  // Initialize Values
  comma = String(",");
  allAngles = String();

  //Switch Pin Mode
  pinMode(2, INPUT); // Set pin 2 as input for onSwitch
  pinMode(3, INPUT); // Set pin 3 as input for modeSwitch

  pinMode(4, INPUT);
  pinMode(5, INPUT); // Set pin 4,5 as input for elbow encoder

  pinMode(6, INPUT);
  pinMode(7, INPUT); // Set pin 6,7 as input for shoulder encoder

  pinMode(8, INPUT);
  pinMode(9, INPUT); // Set pin 8,9 as input for twist encoder

  pinMode(10, INPUT);
  pinMode(11, INPUT); // Set pin 10,11 as input for grip encoder

  //Wireless Port Setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  // Check if 'on' switch is in HIGH or LOW
  onSwitchReading = digitalRead(2);
  if (onSwitchReading == HIGH) {
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
      Serial.println("Dials");

      /* ELBOW SECTION */
      aStateElbow = digitalRead(4); // Reads the "current" state of the 4
      // If the previous and the current state of the 4 are different, that means a Pulse has occured
      if (aStateElbow != aLastStateElbow) {
        // If the 5 state is different to the 4 state, that means the encoder is rotating clockwise
        if (digitalRead(5) != aStateElbow) {
          if (elbowNewAngle < 180) {
            elbowNewAngle ++;
          }
        } else {
          if (elbowNewAngle > 0) {
            elbowNewAngle --;
          }
        }
      }
      aLastStateElbow = aStateElbow; // Updates the previous state of the 6 with the current state

      /* SHOULDER SECTION */
      aStateShoulder = digitalRead(6); // Reads the "current" state of the 6
      // If the previous and the current state of the 6 are different, that means a Pulse has occured
      if (aStateShoulder != aLastStateShoulder) {
        // If the 7 state is different to the 6 state, that means the encoder is rotating clockwise
        if (digitalRead(7) != aStateShoulder) {
          if (shoulderNewAngle < 180) {
            shoulderNewAngle ++;
          }
        } else {
          if (shoulderNewAngle > 0) {
            shoulderNewAngle --;
          }
        }
      }
      aLastStateShoulder = aStateShoulder; // Updates the previous state of the 6 with the current state

      /* TWIST SECTION */
      aStateTwist = digitalRead(8); // Reads the "current" state of the 8
      // If the previous and the current state of the 8 are different, that means a Pulse has occured
      if (aStateTwist != aLastStateTwist) {
        // If the 9 state is different to the 8 state, that means the encoder is rotating clockwise
        if (digitalRead(9) != aStateTwist) {
          if (twistNewAngle < 180) {
            twistNewAngle ++;
          }
        } else {
          if (twistNewAngle > 0) {
            twistNewAngle --;
          }
        }
      }
      aLastStateTwist = aStateTwist; // Updates the previous state of the 8 with the current state

      /* GRIP SECTION */
      aStateGrip = digitalRead(10); // Reads the "current" state of the 10
      // If the previous and the current state of the 10 are different, that means a Pulse has occured
      if (aStateGrip != aLastStateGrip) {
        // If the 11 state is different to the 10 state, that means the encoder is rotating clockwise
        if (digitalRead(11) != aStateGrip) {
          if (gripNewAngle < 180) {
            gripNewAngle = gripNewAngle + 5;
          }
        } else {
          if (gripNewAngle > 0) {
            gripNewAngle = gripNewAngle - 5;
          }
        }
      }
      aLastStateGrip = aStateGrip; // Updates the previous state of the 10 with the current state

      // Strings are combined to allAngles to be sent through radio/serial
      allAngles = elbowNewAngle + comma + shoulderNewAngle + comma + twistNewAngle + comma + gripNewAngle;
      Serial.println(allAngles);

      // Convert to char array to be sent through radio.write
      allAngles.toCharArray(sendText, sizeof(sendText));
      radio.write(&sendText, sizeof(sendText));

    } else { // Kinect Mode

      Serial.println("Kinect");
      Serial.println(allAngles);

      // N.B. Dials seem to stop working here because of the sampling rate issue?

      //    /* TWIST SECTION */
      //    aStateTwist = digitalRead(8); // Reads the "current" state of the 8
      //    // If the previous and the current state of the 8 are different, that means a Pulse has occured
      //    if (aStateTwist != aLastStateTwist) {
      //      // If the 9 state is different to the 8 state, that means the encoder is rotating clockwise
      //      if (digitalRead(9) != aStateTwist) {
      //        if (twistNewAngle < 180) {
      //          twistNewAngle ++;
      //        }
      //      } else {
      //        if (twistNewAngle > 0) {
      //          twistNewAngle --;
      //        }
      //      }
      //    }
      //    aLastStateTwist = aStateTwist; // Updates the previous state of the 8 with the current state
      //
      //    /* GRIP SECTION */
      //    aStateGrip = digitalRead(10); // Reads the "current" state of the 10
      //    // If the previous and the current state of the 10 are different, that means a Pulse has occured
      //    if (aStateGrip != aLastStateGrip) {
      //      // If the 11 state is different to the 10 state, that means the encoder is rotating clockwise
      //      if (digitalRead(11) != aStateGrip) {
      //        if (gripNewAngle < 180) {
      //          gripNewAngle ++;
      //        }
      //      } else {
      //        if (gripNewAngle > 0) {
      //          gripNewAngle --;
      //        }
      //      }
      //    }
      //    aLastStateGrip = aStateGrip; // Updates the previous state of the 10 with the current state

      /* SHOULDER AND ELBOW obtained from serial which comes from Kinect code */
      elbowNewAngle = Serial.parseInt();
      shoulderNewAngle = Serial.parseInt();

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

