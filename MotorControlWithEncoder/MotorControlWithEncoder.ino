#include <Servo.h>
// Creating the servo objects
Servo twistservo;
// intial positions
int twistPos;
int twistangle = 0;
int twistnewangle = 0;

// Declarations for encoders
#define outputA 6
#define outputB 7
int aState;
int aLastState;

void setup()
{
  twistservo.attach(11, 780, 2180);

  //start positions
  twistangle = 0;

  // Initialization for Encoder
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);

  Serial.begin (9600);
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);
}

void loop()
{
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      if (twistnewangle <= 180) {
        twistnewangle ++;
      }
    } else {
      if (twistnewangle >= 0) {
        twistnewangle --;
      }
    }
    moveangle(twistnewangle, twistangle, twistservo);
    twistangle = twistnewangle;
    Serial.print("Position: ");
    Serial.println(twistangle);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void moveangle(int newer, int older, Servo servotype) {
  if (newer >= 0 && newer <= 180)
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
      for (; older < newer; older += 1)
      {
        servotype.write(older);
      }
    }
  }
}

