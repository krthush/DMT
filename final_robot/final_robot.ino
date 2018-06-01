#include <Servo.h>
// Creating the servo objects
Servo elbowservo;
Servo shoulderservo;
// intial positions

int elbowPos;
int shoulderPos;

int elbowangle = 0;
int shoulderangle = 0;

int elbownewangle = 0;
int shouldernewangle = 0;

float readin = 0;
const char startchar = '~'; //the start character of the serial readings.
char ch = ' '; //the char being read in

void setup()
{
  Serial.begin(9600);
  elbowservo.attach(9, 780, 2180);
  shoulderservo.attach(10, 780, 2180);

  //start positions
  elbowangle = 0;
  shoulderangle = 0;
}

void loop()
{
}



void serialEvent()
{
  while (Serial.available() or ch != startchar)
  {
    ch = Serial.read();
  }
  elbownewangle = Serial.parseInt();
  shouldernewangle = Serial.parseInt();

  moveangle(elbownewangle, elbowangle, elbowservo);
  moveangle(shouldernewangle, shoulderangle, shoulderservo);

  elbownewangle=elbowangle;
  shouldernewangle=shoulderangle;
  
}

// Function to move selected servo to new angle
void moveangle(int newer, int older, Servo servotype) {
  if (newer > 0 && newer < 180)
  {
    if (newer < older)
    {
      for (; older > newer; older -= 1)
      {
        servotype.write(older);
      }
    }
    else
    {
      for (; older <= newer; older += 1)
      {
        servotype.write(older);
      }
    }
  }
}

