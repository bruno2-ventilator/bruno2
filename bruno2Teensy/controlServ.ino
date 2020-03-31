/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                      VENTILATOR CONTROL SERVICES                        */
/*-------------------------------------------------------------------------*/


/*********************************************/
/*            CONTROL VARIABLES              */
/*-------------------------------------------*/


//flow meters
//venturiFlow oxFlow;
//venturiFlow aiFlow;
venturiFlow inFlow;
venturiFlow exFlow;
//uint8_t pinOxFl = 5;
//uint8_t pinAiFl = 5;
uint8_t pinInFl = 5;
uint8_t pinExFl = 6;

//pressure sensors
honeywellHsc pMix;
honeywellHsc pInhale;
honeywellHsc pExhale;
uint8_t pinMixP    = 4;
uint8_t pinInhaleP = 2;
uint8_t pinExhaleP = 3;
int pRange60mBa    = 6000;
int pRange1Ba      = 100000;

//valve actuators
pinchValve inhaleValve;
pinchValve exhaleValve;
uint8_t pinInhale = 14;
uint8_t pinExhale = 15;

//LED indicators and buttons
uint8_t pinRedLed = 22;
uint8_t pinGrnLed = 21;

//buzzer indicators
uint8_t pinBuzzer = 23;

//TODO -- this should be defined here, now on main ino for now
////setpoints
//int pPeak;  //pascals
//int pPeep;  //pascals
//int respR;  //resp/min in millis() period
//int inhExh; //exhale/inhale float
//
////alarm setpoints
//int pPeepHigh;
//int pPeepLow;
//int mVentHigh;
//int mVentLow;
//int tidVolExHigh;
//int tidVolExLow;

//alarm flags
bool pPeepHighFl    = false;
bool pPeepLowFl     = false;
bool mVentHighFl    = false;
bool mVentLowFl     = false;
bool tidVolExHighFl = false;
bool tidVolExLowFl  = false;

//application average variables
uint32_t pPeakAv   = 0; //Pascals
uint32_t pPeakN    = 0; //number val
uint32_t fOxygenAv = 0; //cm^3/s
uint32_t fOxygenN  = 0; //number val
uint32_t fAirAv    = 0; //cm^3/s
uint32_t fAirN     = 0; //number val
uint32_t pPeepAv   = 0; //Pascals
uint32_t pPeepN    = 0; //number val

uint32_t inhVolAv    = 0;
uint32_t inhVolN     = 0;
uint32_t inhLastT    = micros();
uint32_t inhLastF    = 0;
uint32_t exhVolAv    = 0;
uint32_t exhVolN     = 0;
uint32_t exhLastT    = micros();
uint32_t exhLastF    = 0;



//TODO -- this is to bang-bang control the valves
bool inhaleValveLatch;
bool exhaleValveLatch;
int moveSteps = 400; //technically steps, but more like time opening
int currCnt   = 0;

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          CONTROL FUNCTIONS                              */
/*-------------------------------------------------------------------------*/

void initTeensyModules(){
  //start spi interface
  SPI.begin();
}
/*-------------------------------------------*/

void initControl(){
  //init flow meters  
  //oxFlow.init(pinOxFl);
  //aiFlow.init(pinAiFl);
  inFlow.init(pinInFl);
  exFlow.init(pinExFl);
  
  //init pressure sensors
  pMix.init(pinMixP, pRange1Ba);
  pInhale.init(pinInhaleP, pRange60mBa);
  pExhale.init(pinExhaleP, pRange60mBa);

  //valve actuators
  inhaleValve.init(pinInhale);
  exhaleValve.init(pinExhale);

  exhaleValve.closeVlave();

  //led indicators and buzzer indicator
  initStatusIndic();

  //TODO -- start as exhale, use flags for now
  inhaleValveLatch = true;
  exhaleValveLatch = true;
  exhaleValve.openValve();
  inhaleValve.closeValve();
}
/*-------------------------------------------*/


/*********************************************/
/*           RESPIRATION SERVICES            */
/*-------------------------------------------*/

//TODO -- here for now
void reportAvarages(){
  Serial.print("/ms1v");
  Serial.print(pPeakAv);
  Serial.println("t");

  Serial.print("/ms4v");
  Serial.print(pPeepAv);
  Serial.println("t");

  Serial.print("/ms5v");
  Serial.print(inhVolAv);
  Serial.println("t");

  Serial.print("/ms6v");
  Serial.print(exhVolAv);
  Serial.println("t");
}
/*-------------------------------------------*/



/*********************************************/
/*              700HZ SERVICES               */
/*-------------------------------------------*/

void inhaleControl(){
  if(inhaleValveLatch){
    if(moveSteps<currCnt){
      inhaleValve.open1StValve();  
      exhaleValve.close1StValve();  
      currCnt++;
    }else{
      inhaleValveLatch = false;
      exhaleValveLatch = true;
      currCnt = 0;
    }
  }else{
    computeAverages();
  }
}
/*-------------------------------------------*/

void exhaleControl(){
  if(exhaleValveLatch){
    if(moveSteps<currCnt){
      exhaleValve.open1StValve();  
      inhaleValve.close1StValve();  
      currCnt++;
    }else{
      exhaleValveLatch = false;
      inhaleValveLatch = true;
      currCnt = 0;
    }
  }else{
    computeAverages();
  }
}
/*-------------------------------------------*/



/*********************************************/
/*              100HZ SERVICES               */
/*-------------------------------------------*/

void computeAverages(){
  int thisFlow;

  if(inhalationFlag && !exhalationFlag){
    pPeakAv = ((pPeakAv*pPeakN)+pInhale.getP())/(pPeakN+1); 
    pPeakN++;
    if(inhLastF==0){
      inhLastF = inFlow.getFlow();
      inhLastT = micros();
    }else{
      thisFlow = (inFlow.getFlow()-inhLastF)/(micros()-inhLastT); 
      inhVolAv = ((inhVolAv*inhVolN)+thisFlow)/(inhVolN+1); 
      inhVolN++;
      inhLastF = inFlow.getFlow();
      inhLastT = micros();
    }
  }
  else if(!inhalationFlag && exhalationFlag){
    pPeepAv = ((pPeepAv*pPeepN)+pExhale.getP())/(pPeepN+1); 
    pPeepN++;
    if(exhLastF==0){
      exhLastF = exFlow.getFlow();
      exhLastT = micros();
    }else{
      thisFlow = (exFlow.getFlow()-exhLastF)/(micros()-exhLastT); 
      exhVolAv = ((exhVolAv*exhVolN)+thisFlow)/(exhVolN+1); 
      exhVolN++;
      exhLastF = exFlow.getFlow();
      exhLastT = micros();
    }
  }

}
/*-------------------------------------------*/



/*********************************************/
/*              ADHOC SERVICES               */
/*-------------------------------------------*/

void ventMixChamber(){}
/*-------------------------------------------*/

void resetAvarages(){
  pPeakAv   = 0; //Pascals
  pPeakN    = 0; //number val
  fOxygenAv = 0; //cm^3/s
  fOxygenN  = 0; //number val
  fAirAv    = 0; //cm^3/s
  fAirN     = 0; //number val
  pPeepAv   = 0; //Pascals
  pPeepN    = 0; //number val
  inhVolAv  = 0;
  inhVolN   = 0;
  exhVolAv  = 0;
  exhVolN   = 0;
  inhLastT  = micros();
  inhLastF  = 0;
  exhVolAv  = 0;
  exhVolN   = 0;
  exhLastT  = micros();
  exhLastF  = 0;
}
/*-------------------------------------------*/

void updateRespSetpoints(){
  exhalePeriod = respR/(1+inhExh);
  inhalePeriod = respR-exhalePeriod;
}
/*-------------------------------------------*/

bool allSetptsReady(){
  if(pPeak==-1||pPeep==-1||respR==-1||inhExh==-1){
     return false;
  }

  return true;
}
/*-------------------------------------------*/


/*********************************************/
/*            INDICATOR SERVICES             */
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

void buzzerOnOff(bool stat){
  if(stat){
    digitalWrite(pinBuzzer, HIGH);
   }else{
    digitalWrite(pinBuzzer, LOW);
  }
}
/*-------------------------------------------*/

void printPresSensors(){
  Serial.print("pMix ");
  Serial.println(pMix.getP());
  Serial.print("pInhale ");
  Serial.println(pInhale.getP());
  Serial.print("pExhale ");
  Serial.println(pExhale.getP());
}
/*-------------------------------------------*/

/*EOF*/
