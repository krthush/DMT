//MeArmMeCon-A04.ino

//Use With MeCon.exe Ver0.4 Windows Software for MeArm Motion Control

 

 

#include <Servo.h>

 

//MeArm HAS 4 SERVOS

Servo xServo;  // create servo object, arm base servo - left right motion

Servo yServo;  // create servo object, left side servo - forward backwards motion

Servo zServo;  // create servo object, right side servo - forward backwards motion

Servo clawServo;  // create servo object, end of arm srevo - open,close the claw hand

 

//servo positions values, expects 1-180 deg.

int xPos;

int yPos;

int zPos;

int clawPos;

 

//*************** INIT AT STARTUP *******************************************************************

 

void setup() {        // the setup function runs once when you press reset or power the board

 

  // assign servo to pin numbers

  xServo.attach(11,780,2180);  // attaches the servo on pin 11 to the servo object

  yServo.attach(10,780,2180);  // attaches the servo on pin 10 to the servo object

  zServo.attach(9,575,2460);  // attaches the servo on pin 9 to the servo object

  clawServo.attach(6);  // attaches the servo on pin 6 to the servo object

 

  // initialize serial port

  Serial.begin(9600);

 

  // Debug only send serial message to host com port terminal window in Arduino IDE

  //Serial.print("*** MeCom Test V04 ***.");   // send program name, uncomment for debug connection test

 

 

 

}

 

// ******************************************************************************************************

// ********************************** MAIN PROGRAM LOOP START *******************************************

// ******************************************************************************************************

 

void loop() {

 

 

  //serial in packet patern = xVal,yVal,zVal,clawVal + end of packet char 'x'

  while (Serial.available() > 0) {

    xPos = Serial.parseInt();

    yPos = Serial.parseInt();

    zPos = Serial.parseInt();

    clawPos = Serial.parseInt();

 

    if (Serial.read() == 'x') { // Detect end of packet char 'x', go ahead and update servo positions

 

 

      // UPDATE SERVO POSITIONS

      xServo.write(xPos);

      yServo.write(yPos);

      zServo.write(zPos);

      clawServo.write(clawPos);

 

 

    }

 

  }

 

}

 
