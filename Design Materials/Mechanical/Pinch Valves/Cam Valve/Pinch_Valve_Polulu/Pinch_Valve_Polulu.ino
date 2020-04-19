//Sample code for controlling the cam valve over serial with a Pololu 36v4 stepper driver and Arduino


#include <SPI.h>
#include <HighPowerStepperDriver.h>

const uint8_t CSPin = 10;

// delay between motor steps
const uint16_t StepPeriodUs = 1000;

HighPowerStepperDriver sd;

char user_input;
String inputStr;
int curPos;
int posDifference;
int input;

void setup()
{
  SPI.begin();
  sd.setChipSelectPin(CSPin);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings and clear latched status
  // conditions.
  sd.resetSettings();
  sd.clearStatus();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications, and we find that it usually works well.
  sd.setDecayMode(HPSDDecayMode::AutoMixed);

  // Set the current limit. You should change the number here to an appropriate
  // value for your particular system.
  sd.setCurrentMilliamps36v4(250);

  // Set the number of microsteps that correspond to one full step.
  sd.setStepMode(HPSDStepMode::MicroStep8);

  // Enable the motor outputs.
  sd.enableDriver();

  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter a position between 0 - 100 (0 = closed):");
  Serial.println("Press H to home the stepper");
  Serial.println();
}

void loop()
{
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input =='H')
      {
         Home();
      }
      else if(user_input >= 48 && user_input <= 57)
      {
        inputStr += user_input;
        delay(2);
        while (Serial.available()){
        user_input = Serial.read();  //gets one byte from serial buffer
        inputStr += user_input; //makes the string readString
        delay(2);  //slow looping to allow buffer to fill with next character
        //Serial.println("number read");
        }
        input = inputStr.toInt();
        MoveToPos(input);
        inputStr = "";
      }
      else
      {
        Serial.println("Invalid option entered.");
      }
  }
}

//Home the valve by running against a hard stop
//dir LOW to move clockwise
void Home()
{
  Serial.println("Homing...");
  sd.setDirection(1); //homing in the counterclockwise direction
  for(int i = 1; i<400; i++)  //loop enough to move a quarter rotation at 1/8 microstepping
  {
    sd.step();
    delayMicroseconds(StepPeriodUs);
    curPos = 0;
  }
  Serial.println("Homing complete");
}

//move to the set position
//range is quarter circle split into 100 increments
int MoveToPos(int setPos) {
  
  if (setPos > 100) {
    setPos = 100;
  }

  if (setPos < 0) {
    setPos = 0;
  }
  
  if (setPos > curPos) {
    //move clockwise
    sd.setDirection(0);
    posDifference = setPos - curPos;
    for (int i = 0; i < 4*posDifference; i++) {
      sd.step();
      delayMicroseconds(StepPeriodUs);
    }
    
  }
  if (setPos < curPos) {
    //move counterclockwise
    sd.setDirection(1);
    posDifference = curPos - setPos;
    for (int i = 0; i < 4*posDifference; i++) {
      sd.step();
      delayMicroseconds(StepPeriodUs);
    }
  }
  curPos = setPos;
}
