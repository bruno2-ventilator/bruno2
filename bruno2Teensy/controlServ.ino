/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                      VENTILATOR CONTROL SERVICES                        */
/*-------------------------------------------------------------------------*/


/*********************************************/
/*            CONTROL VARIABLES              */
/*-------------------------------------------*/

//flow meters
venturiFlow oxFlow;
venturiFlow aiFlow;
venturiFlow inFlow;
venturiFlow exFlow;
uint8_t pinOxFl = 9;
uint8_t pinAiFl = 7;
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


//TODO -- this should be defined here, now on main ino for now
////setpoints
//int pPeak  = -1; //pascals
//int pPeep  = -1; //pascals
//int respR  = -1; //resp/min in millis() period
//int inhExh = -1; //exhale/inhale float
//
////alarm setpoints
//int pPeepHigh    = -1;
//int pPeepLow     = -1;
////int mVentHigh    = -1;
////int mVentLow     = -1;
//int tidVolExHigh = -1;
//int tidVolExLow  = -1;

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

uint32_t inhVolAv  = 0;
uint32_t inhVolN   = 0;
uint32_t inhLastT  = micros();
uint32_t inhLastF  = 0;
uint32_t exhVolAv  = 0;
uint32_t exhVolN   = 0;
uint32_t exhLastT  = micros();
uint32_t exhLastF  = 0;


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
  oxFlow.init(pinOxFl);
  aiFlow.init(pinAiFl);
  inFlow.init(pinInFl);
  exFlow.init(pinExFl);

  //init pressure sensors
  pMix.init(pinMixP, pRange1Ba);
  pInhale.init(pinInhaleP, pRange60mBa);
  pExhale.init(pinExhaleP, pRange60mBa);

  //valve actuators
  inhaleValve.init(pinInhale);
  exhaleValve.init(pinExhale);

  //led indicators and buzzer indicator
  initStatusIndic();

  //init envoronmental sensors
  initTmpSensor();
  initHumSensor();

  //get system to a start-ready state
  exhaleValve.closeValve();
  //TODO -- start as exhale, use flags for now
  inhaleValveLatch = true;
  exhaleValveLatch = true;
  inhaleValve.closeValve();
}
/*-------------------------------------------*/


/*********************************************/
/*           RESPIRATION SERVICES            */
/*-------------------------------------------*/

//TODO -- here for now
void reportAvarages(){
  Serial.print(pPeakAv);
  Serial.print(",");
  Serial.print(pPeepAv);
  Serial.print(",");
  Serial.print(inhVolAv);
  Serial.print(",");
  Serial.print(exhVolAv);
  Serial.println(",");
//  Serial.print("/ms1v");
//  Serial.print(pPeakAv);
//  Serial.println("t");
//
//  Serial.print("/ms4v");
//  Serial.print(pPeepAv);
//  Serial.println("t");
//
//  Serial.print("/ms5v");
//  Serial.print(inhVolAv);
//  Serial.println("t");
//
//  Serial.print("/ms6v");
//  Serial.print(exhVolAv);
//  Serial.println("t");
}
/*-------------------------------------------*/



/*********************************************/
/*              700HZ SERVICES               */
/*-------------------------------------------*/

void inhaleControl(){
  if(inhaleValveLatch){
    if(moveSteps>currCnt){
      inhaleValve.open1StValve();
      delayMicroseconds(5);
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
    if(moveSteps>currCnt){
      exhaleValve.open1StValve();
      delayMicroseconds(5);
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
  float thisFlow;

  if(inhalationFlag && !exhalationFlag){
    pPeakAv = (int)((pPeakAv*pPeakN)+pInhale.getP())/(pPeakN+1);
    pPeakN++;
    if(inhLastF==0){
      inhLastF = inFlow.getFlow();
      inhLastT = micros();
    }else{
      thisFlow = ((inFlow.getFlow()-inhLastF)*1000)/(micros()-inhLastT);
      inhVolAv = (int)((inhVolAv*inhVolN)+thisFlow)/(inhVolN+1);
      inhVolN++;
      inhLastF = inFlow.getFlow();
      inhLastT = micros();
    }
  }
  else if(!inhalationFlag && exhalationFlag){
    pPeepAv = (int)((pPeepAv*pPeepN)+pExhale.getP())/(pPeepN+1);
    pPeepN++;
    if(exhLastF==0){
      exhLastF = exFlow.getFlow();
      exhLastT = micros();
    }else{
      thisFlow = ((exFlow.getFlow()-exhLastF)*1000)/(micros()-exhLastT);
      exhVolAv = (int)((exhVolAv*exhVolN)+thisFlow)/(exhVolN+1);
      exhVolN++;
      exhLastF = exFlow.getFlow();
      exhLastT = micros();
    }
  }
}
/*-------------------------------------------*/



/*********************************************/
/*              SENSOR SERVICES              */
/*-------------------------------------------*/

void updatePresSensors(){
  //flow sensors
  oxFlow.filFlowRead();
  aiFlow.filFlowRead();
  inFlow.filFlowRead();
  exFlow.filFlowRead();

  //pressure sensors
  pMix.filPresRead();
  pInhale.filPresRead();
  pExhale.filPresRead();
  
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

void printPresSensors(){
  Serial.print("pmix: ");
  Serial.print(pMix.getP());
  Serial.print("\tpin: ");
  Serial.print(pInhale.getP());
  Serial.print("\tpex: ");
  Serial.print(pExhale.getP());
  Serial.print("\noxf: ");
  Serial.print(oxFlow.getFlow());
  Serial.print("\taif: ");
  Serial.print(aiFlow.getFlow());
  Serial.print("\tinf: ");
  Serial.print(inFlow.getFlow());
  Serial.print("\texf: ");
  Serial.println(exFlow.getFlow());
}
/*-------------------------------------------*/

int thisthis(){
  return pMix.getRaw();
}

/*EOF*/
