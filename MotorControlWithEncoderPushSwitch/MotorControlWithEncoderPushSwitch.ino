#include <Servo.h>
// Creating the servo objects
Servo twistservo;
// intial positions
int twistPos;
int twistangle = 0;
int twistnewangle = 0;

// Encoder Switch Declarations
int inPin = 2;         // the number of the input pin
int state = HIGH;      // the current state of the grip
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 500;   // the debounce time, increase if the output flickers

void setup()
{
  // Servo Setup
  twistservo.attach(11, 780, 2180);
  twistangle = 0;  //start positions

  // Encoder Pin Setup
  pinMode(inPin, INPUT);
}

void loop()
{
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();    
  }

  if (state == HIGH) {
    twistnewangle=20;
  } else {
    twistnewangle=0;
  }
  moveangle(twistnewangle, twistangle, twistservo);  
  twistangle = twistnewangle;
  previous = reading;
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
