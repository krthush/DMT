#include <Servo.h>

Servo elbowServo;
Servo shoulderServo;

void setup() {
  elbowServo.attach(5, 780, 2180);
  shoulderServo.attach(6, 780, 2180);
}

void loop() {
  elbowServo.write(155);
  delay(100);
  shoulderServo.write(150);
  delay(100);
}

