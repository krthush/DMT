#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Declarations for Servos
// Creating the servo objects
Servo elbowservo;
Servo shoulderservo;
Servo twistservo;
Servo gripservo;

// intial positions
int elbowangle = 0;
int shoulderangle = 0;
int twistangle = 0;
int gripangle = 0;

int elbownewangle = 0;
int shouldernewangle = 0;
int twistnewangle = 0;
int gripnewangle = 0;

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
  elbowservo.attach(5, 780, 2180);
  shoulderservo.attach(6, 780, 2180);
  twistservo.attach(9, 780, 2180);
  gripservo.attach(10, 780, 2180);

  //start positions
  elbowangle = 0;
  shoulderangle = 0;
  twistangle = 0;
  gripnewangle = 0;
}

void loop() {
  if (radio.available()) {
    char receiveText[32] = "";
    radio.read(&receiveText, sizeof(receiveText));
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
    elbownewangle = firstValue.toInt();
    shouldernewangle = secondValue.toInt();
    twistnewangle = thirdValue.toInt();
    gripnewangle = fourthValue.toInt();

    moveangle(elbownewangle, elbowangle, elbowservo);
    moveangle(shouldernewangle, shoulderangle, shoulderservo);
    moveangle(twistnewangle, twistangle, twistservo);
    moveangle(gripnewangle, gripangle, gripservo);

    elbowangle = elbownewangle;
    shoulderangle = shouldernewangle;
    twistangle = twistnewangle;
    gripangle = gripnewangle;
  }
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
      for (; older < newer; older += 1)
      {
        servotype.write(older);
      }
    }
  }
}

