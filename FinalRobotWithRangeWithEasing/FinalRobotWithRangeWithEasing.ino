#include <ServoCos.h>

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// This is the time in ms for movements to be made
int moveTime = 100; // Increase if movements are not smooth!

// Declarations for Servos with ServoCos Library
// Creating the servo objects
ServoCos elbowServo;
ServoCos shoulderServo;
ServoCos twistServo;
ServoCos gripServo;
int sweepDirection = 1;

// intial positions
int elbowNewAngle = 0;
int shoulderNewAngle = 0;
int twistNewAngle = 0;
int gripNewAngle = 0;

// Declarations for Switch
RF24 radio(2, 4); // CE, CSN
const byte address[6] = "00001";

void setup() {

  // Wireless module Serial Setup
  Serial.begin(9600);             // Serial port to computer
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Attachment and startup for Servos
  elbowServo.attach(5, 780, 2180);
  shoulderServo.attach(6, 780, 2180);
  twistServo.attach(9, 780, 2180);
  gripServo.attach(10, 780, 2180);
}

void loop() {
  if (radio.available()) {
    char receiveText[32] = "";
    radio.read(&receiveText, sizeof(receiveText));
    delay(15);

    String allAngles(receiveText);
    Serial.println(allAngles);

    // N.B. Splitting of this string can be done more efficiently saving dynamic memory usage (see arduino stack exchange) - using functions such as strchr(), strtok(), atoi() and pointers.
    int commaIndex = allAngles.indexOf(',');
    int secondCommaIndex = allAngles.indexOf(',', commaIndex + 1); //  Search for the next comma just after the first
    int thisCommaIndex = allAngles.indexOf(',', secondCommaIndex + 1);

    String firstValue = allAngles.substring(0, commaIndex);
    String secondValue = allAngles.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = allAngles.substring(secondCommaIndex + 1); // To the end of the string
    String fourthValue = allAngles.substring(thisCommaIndex + 1);

    // If Start Char is passed, then read all values and move servo to given angle
    elbowNewAngle = firstValue.toInt();
    shoulderNewAngle = secondValue.toInt();
    twistNewAngle = thirdValue.toInt();
    gripNewAngle = fourthValue.toInt();

    // TODO: make function so that there is less repeated code (SEE BELOW)
    //    moveAngle(elbowNewAngle, elbowServo);
    //    moveAngle(shoulderNewAngle, shoulderServo);
    //    moveAngle(twistNewAngle, twistServo);
    //    moveAngle(gripNewAngle, gripServo);

    /* ELBOW */
    if (elbowServo.hasStopped()) { // there is also .isMoving()
      if (sweepDirection == 1) {
        sweepDirection = -1;
        elbowServo.setupEase(elbowNewAngle, moveTime);
      }
      else
      {
        sweepDirection = 1;
        elbowServo.setupEase(elbowNewAngle, moveTime);
      }
    }
    elbowServo.updatePosition();

    /* SHOULDER */
    if (shoulderServo.hasStopped()) { // there is also .isMoving()
      if (sweepDirection == 1) {
        sweepDirection = -1;
        shoulderServo.setupEase(shoulderNewAngle, moveTime);
      }
      else
      {
        sweepDirection = 1;
        shoulderServo.setupEase(shoulderNewAngle, moveTime);
      }
    }
    shoulderServo.updatePosition();

    /* TWIST */
    if (twistServo.hasStopped()) { // there is also .isMoving()
      if (sweepDirection == 1) {
        sweepDirection = -1;
        twistServo.setupEase(twistNewAngle, moveTime);
      }
      else
      {
        sweepDirection = 1;
        twistServo.setupEase(twistNewAngle, moveTime);
      }
    }
    twistServo.updatePosition();

    /* GRIP */
    if (gripServo.hasStopped()) { // there is also .isMoving()
      if (sweepDirection == 1) {
        sweepDirection = -1;
        gripServo.setupEase(gripNewAngle, moveTime);
      }
      else
      {
        sweepDirection = 1;
        gripServo.setupEase(twistNewAngle, moveTime);
      }
    }
    gripServo.updatePosition();

  }
}

// TODO: This function needs to be made instead of repeating code from above
//void moveAngle(int newAngle, ServoCos servoType) {
//  if (servoType.hasStopped()) { // there is also .isMoving()
//    if (sweepDirection == 1) {
//      sweepDirection = -1;
//      servoType.setupEase(newAngle, moveTime);
//    }
//    else
//    {
//      sweepDirection = 1;
//      servoType.setupEase(newAngle, moveTime);
//    }
//  }
//  servoType.updatePosition();
//}
