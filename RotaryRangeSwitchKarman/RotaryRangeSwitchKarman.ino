#include <SoftwareSerial.h>
#include <SimpleTimer.h>

// Declarations for Wireless Module
SoftwareSerial HC12(12, 13); // HC-12 TX Pin, HC-12 RX Pin

// Declarations for Switch
int modeSwitchReading = LOW;           // the current reading from the input pin

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

String starting;
String ending;
String comma;
String allAngles;

// the timer object
SimpleTimer timer;

void setup() {

  // Initialize Values
  starting = String("s");
  ending = String("e");
  comma = String(",");

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
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  
  timer.setInterval(3000, repeatMe); // Repeat Timer Every 2s
}

void loop() {
  modeSwitchReading = digitalRead(3);

  if (modeSwitchReading == HIGH) { // Check if mode switch is on dials OR kinect mode

    // ELBOW SECTION
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

    // SHOULDER SECTION
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

    // TWIST SECTION
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

    // GRIP SECTION
    aStateGrip = digitalRead(10); // Reads the "current" state of the 10
    // If the previous and the current state of the 10 are different, that means a Pulse has occured
    if (aStateGrip != aLastStateGrip) {
      // If the 11 state is different to the 10 state, that means the encoder is rotating clockwise
      if (digitalRead(11) != aStateGrip) {
        if (gripNewAngle < 180) {
          gripNewAngle ++;
        }
      } else {
        if (gripNewAngle > 0) {
          gripNewAngle --;
        }
      }
    }
    aLastStateGrip = aStateGrip; // Updates the previous state of the 10 with the current state

    // TODO: Combine all the different angles to a format like '~0,0,0,0,' and then display to check and finally HC12.write
    allAngles =  elbowNewAngle + comma + shoulderNewAngle + comma + twistNewAngle + comma + gripNewAngle;
    Serial.println(allAngles);
    timer.run();
  } else { // Kinect Mode
    // TODO: Send to HC12 while reading from Serial Kinect stuff and combining it with twist position
  }
}

// a function to be executed periodically
void repeatMe() {
  HC12.print(allAngles);
}

