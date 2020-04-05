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

/*********************************************/
/*                  LEDS                     */
/*-------------------------------------------*/

void initStatusIndic(){
  //led indicators
  pinMode(pinRedLed, OUTPUT);
  redLedOnOff(false);
  pinMode(pinGrnLed, OUTPUT);
  grnLedOnOff(false);
  //buzzer indicator
  pinMode(pinBuzzer, OUTPUT);
  buzzerOnOff(false);
}
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
    digitalWrite(pinBuzzer, HIGH);
   }else{
    digitalWrite(pinBuzzer, LOW);
  }
}
/*-------------------------------------------*/

/*EOF*/
