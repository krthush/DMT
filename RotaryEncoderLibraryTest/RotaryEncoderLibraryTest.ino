#include <TimerOne.h>

#include <ClickEncoder.h>

ClickEncoder *elbowEncoder;
ClickEncoder *shoulderEncoder;
ClickEncoder *twistEncoder;
ClickEncoder *gripEncoder;

int elbowLastValue = -1;
int shoulderLastValue = -1;
int twistLastValue = -1;
int gripLastValue = -1;

int elbowValue = 0;
int shoulderValue = 0;
int twistValue = 0;
int gripValue = 0;

int elbowNewAngle = 0;
int shoulderNewAngle = 0;
int twistNewAngle = 0;
int gripNewAngle = 0;

String comma;
String allAngles;

void timerIsr() {
  elbowEncoder->service();
  shoulderEncoder->service();
  twistEncoder->service();
  gripEncoder->service();
}

void setup() {
  Serial.begin(9600);

  // Initialize Values
  comma = String(",");
  allAngles = String();

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
  gripEncoder = new ClickEncoder(A11, A12, A13, 1);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}

void loop() {

  /* Elbow Encoder Code */
  elbowValue += elbowEncoder->getValue();
  if (elbowValue != elbowLastValue) {
    elbowLastValue = elbowValue;
  }
  if (elbowValue >= 0 && elbowValue <= 180) {
    elbowNewAngle = elbowValue;
  }

  /* Shoulder Encoder Code */
  shoulderValue += shoulderEncoder->getValue();
  if (shoulderValue != shoulderLastValue) {
    shoulderLastValue = shoulderValue;
  }
  if (shoulderValue >= 0 && shoulderValue <= 180) {
    shoulderNewAngle = shoulderValue;
  }

  /* Twist Encoder Code */
  twistValue += twistEncoder->getValue();
  if (twistValue != twistLastValue) {
    twistLastValue = twistValue;
  }
  if (twistValue >= 0 && twistValue <= 180) {
    twistNewAngle = twistValue;
  }

  /* Grip Encoder Code */
  gripValue += gripEncoder->getValue();
  if (gripValue != gripLastValue) {
    gripLastValue = gripValue;
  }
  if (gripValue >= 0 && gripValue <= 180) {
    gripNewAngle = gripValue;
  }

  // Strings are combined to allAngles to be sent through radio/serial
  allAngles = elbowNewAngle + comma + shoulderNewAngle + comma + twistNewAngle + comma + gripNewAngle;
  Serial.println(allAngles);

}
