//Sample code for controlling cam valve over computer serial with a Schamlzhaus EasyDriver and Arduino


//Declare pin functions on Redboard
#define stp 3
#define dir 2
#define MS1 4
#define MS2 5
#define EN  6

//Declare variables for functions
char user_input;
String inputStr;
int curPos;
int posDifference;
long delayVal = 5000; //microseconds delay between toggling step pin
int i; //for loops
int input;

bool turnDir = true; //Change this if the direction is wrong

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(Trigger, OUTPUT);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter a position between 0 - 100 (0 = closed):");
  Serial.println("Press H to home the stepper");
  Serial.println();

  digitalWrite(MS1, HIGH); //Pull MS1 high and MS2 high to set logic to 1/8 microstep resolution
  digitalWrite(MS2, HIGH);
  digitalWrite(EN, LOW); //enable the stepper motor
}

//Main loop
void loop() {
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
      //resetEDPins();
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, LOW);
}

//Home the valve by running against a hard stop
//dir LOW to move clockwise
void Home()
{
  Serial.println("Homing...");
  digitalWrite(dir, turnDir); //homing in the counterclockwise direction
  for(i = 1; i<400; i++)  //loop enough to move a quarter rotation at 1/8 microstepping
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
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
    digitalWrite(dir, !turnDir);
    posDifference = setPos - curPos;
    for (i = 0; i < 4*posDifference; i++) {
      digitalWrite(stp,HIGH); //Trigger one step forward
      delayMicroseconds(delayVal);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(delayVal);
    }
    
  }
  if (setPos < curPos) {
    //move counterclockwise
    digitalWrite(dir, turnDir);
    posDifference = curPos - setPos;
    for (i = 0; i < 4*posDifference; i++) {
      digitalWrite(stp,HIGH); //Trigger one step forward
      delayMicroseconds(delayVal);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(delayVal);
    }
  }
  curPos = setPos;
}
