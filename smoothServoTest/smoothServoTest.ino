#include "ServoCos.h"

ServoCos HeadServo;
int SweepDirection = 1;

void setup() {
  HeadServo.attach(5, 780, 2180);
}

void loop() {
  if (HeadServo.hasStopped()) { // there is also .isMoving()
    if (SweepDirection == 1) {
      SweepDirection = -1;
      HeadServo.setupEase(10, 500);
    }
    else
    {
      SweepDirection = 1;
      HeadServo.setupEase(10, 500);
    }
  }
  HeadServo.updatePosition();

  if (HeadServo.hasStopped()) { // there is also .isMoving()
    if (SweepDirection == 1) {
      SweepDirection = -1;
      HeadServo.setupEase(30, 500);
    }
    else
    {
      SweepDirection = 1;
      HeadServo.setupEase(30, 500);
    }
  }
  HeadServo.updatePosition();
}



