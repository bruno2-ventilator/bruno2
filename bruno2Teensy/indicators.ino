/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                           INDICATOR CONTROL                             */
/*-------------------------------------------------------------------------*/


/*********************************************/
/*           INDICATOR VARIABLES             */
/*-------------------------------------------*/

//LED indicators and buttons
uint8_t pinRedLed = 22;
uint8_t pinGrnLed = 21;

//buzzer indicators
uint8_t pinBuzzer = 23;


/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                         INDICATOR FUNCTIONS                             */
/*-------------------------------------------------------------------------*/

void initStatusIndic(){
  //led indicators
  pinMode(pinRedLed, OUTPUT);
  redLedOnOff(false);
  pinMode(pinGrnLed, OUTPUT);
  grnLedOnOff(false);
  //buzzer indicator
  pinMode(pinBuzzer, OUTPUT);
  buzzerOnOff(false);
  analogWriteFrequency(pinBuzzer, 1000);
}
/*-------------------------------------------*/

void deviceReadySignal(){
  redLedOnOff(true);
  grnLedOnOff(true);
  buzzerOnOff(true);
  delay(500);

  redLedOnOff(false);
  grnLedOnOff(false);
  buzzerOnOff(false);
  delay(300);

  redLedOnOff(true);
  grnLedOnOff(true);
  buzzerOnOff(true);
  delay(500);

  redLedOnOff(false);
  grnLedOnOff(false);
  buzzerOnOff(false);
}
/*-------------------------------------------*/


/*********************************************/
/*                  LEDS                     */
/*-------------------------------------------*/


void redLedOnOff(bool stat){
  if(stat){
    digitalWrite(pinRedLed, LOW);
   }else{
    digitalWrite(pinRedLed, HIGH);
  }
}
/*-------------------------------------------*/

void grnLedOnOff(bool stat){
  if(stat){
    digitalWrite(pinGrnLed, LOW);
   }else{
    digitalWrite(pinGrnLed, HIGH);
  }
}
/*-------------------------------------------*/


/*********************************************/
/*                  BUZZER                   */
/*-------------------------------------------*/

void buzzerOnOff(bool stat){
  if(stat){
    analogWrite(pinBuzzer, 127);
   }else{
    analogWrite(pinBuzzer, 0);
  }
}
/*-------------------------------------------*/

void buzzerOnOff(bool stat, int freq){
  int val = map(freq, 0, 100, 1, 255);

  if(stat){
    analogWrite(pinBuzzer, val);
   }else{
    analogWrite(pinBuzzer, 0);
  }
}
/*-------------------------------------------*/

void sweepSoundBlocking(int durMs, int startFq, int stopFq){
  //TODO -- reeval min and max
  int pwmStart = map(startFq, 0, 100, 0, 255);
  int pwmStop  = map(stopFq,  0, 100, 0, 255);
  int delayTim = durMs/(pwmStop-pwmStart);

  for(int i = pwmStart; i<pwmStop; i++){
    analogWrite(pinBuzzer, i);
    delay(delayTim);
  }
  
  analogWrite(pinBuzzer, 0);
}
/*-------------------------------------------*/

/*EOF*/
