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
uint8_t pinOxFl = 7;
uint8_t pinAiFl = 6;
uint8_t pinInFl = 4;
uint8_t pinExFl = 5;

//pressure sensors
honeywellHsc pMix;
honeywellHsc pInhale;
honeywellHsc pExhale;
uint8_t pinMixP    = 8;
uint8_t pinInhaleP = 2;
uint8_t pinExhaleP = 3;
int pRange60mBa    = 6000;
int pRange1Ba      = 100000;

//valve actuators
pinchValve inhaleValve;
pinchValve exhaleValve;
uint8_t pinInhaleV = 9;
uint8_t pinExhaleV = 10;
uint8_t pinInhOpnV = 14;
uint8_t pinInhCloV = 15;
uint8_t pinExhOpnV = 16;
uint8_t pinExhCloV = 17;

#if RUNTIME
  //setpoints
  int pPeak    = -1; //pascals
  int pPeep    = -1; //pascals
  int respRtUs = -1; //resp/min in millis() period
  int inhExh   = -1; //exhale/inhale float

  //alarm setpoints
  int pPeepHigh    = -1;
  int pPeepLow     = -1;
  int mVentHigh    = -1;
  int mVentLow     = -1;
  int tidVolExHigh = -1;
  int tidVolExLow  = -1;
#endif

//alarm flags
bool pPeepHighFl    = false;
bool pPeepLowFl     = false;
bool mVentHighFl    = false;
bool mVentLowFl     = false;
bool tidVolExHighFl = false;
bool tidVolExLowFl  = false;

//application average variables
float pPeakAv     = 0;
uint32_t pPeakN   = 0;

float pPeepAv     = 0;
uint32_t pPeepN   = 0;

float fOxVolAv    = 0;
float fOxFlowAv   = 0;
uint32_t fOxN     = 0;
uint32_t fOxLastT = micros();
float fOxLastF    = 0;

float fAiVolAv    = 0;
float fAiFlowAv   = 0;
uint32_t fAiN     = 0;
uint32_t fAiLastT = micros();
float fAiLastF    = 0;

float inhVolAv    = 0;
float inhFlowAv   = 0;
uint32_t inhN     = 0;
uint32_t inhLastT = micros();
float inhLastF    = 0;

float exhVolAv    = 0;
float exhFlowAv   = 0;
uint32_t exhN     = 0;
uint32_t exhLastT = micros();
float exhLastF    = 0;


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
  /*______________________________*/
  /*        Init components       */
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
  inhaleValve.init(pinInhaleV, pinInhOpnV, pinInhCloV);
  exhaleValve.init(pinExhaleV, pinExhOpnV, pinExhCloV);


  //led indicators and buzzer indicator
  initStatusIndic();

  //init envoronmental sensors
  initTmpSensor();
  initHumSensor();

  /*______________________________*/
  /*         Init proceses        */
  //zero offset pressure sensors
  //flow meter offset
  oxFlow.setOffset();
  aiFlow.setOffset();
  inFlow.setOffset();
  exFlow.setOffset();

  //pressure sensor offset
  pMix.setOffset();
  pInhale.setOffset();
  pExhale.setOffset();

  //get system to a start-ready state
  exhaleValve.closeValveBlocking();

  //TODO -- start as exhale, use flags for now
  inhaleValveLatch = true;
  exhaleValveLatch = true;
  inhaleValve.closeValveBlocking();
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
    if(moveSteps>currCnt){
      inhaleValve.open1StValve();
      //delayMicroseconds(5);
      exhaleValve.close1StValve();
      currCnt++;
    }else{
      inhaleValveLatch = false;
      exhaleValveLatch = true;
      currCnt = 0;
    }
  }else{
    //computeInhAv();
  }
}
/*-------------------------------------------*/

void exhaleControl(){
  if(exhaleValveLatch){
    if(moveSteps>currCnt){
      exhaleValve.open1StValve();
      //delayMicroseconds(5);
      inhaleValve.close1StValve();
      currCnt++;
    }else{
      exhaleValveLatch = false;
      inhaleValveLatch = true;
      currCnt = 0;
    }
  }else{
    //computeExhAv();
  }
}
/*-------------------------------------------*/



/*********************************************/
/*            AVERAGE SERVICES               */
/*-------------------------------------------*/

void computeInhAv(){
  float thisFlow;
  float thisVol;
  
  if(inhLastF==0){
    inhLastF = inFlow.getFlow();
    inhLastT = micros();

    fOxLastF = oxFlow.getFlow();
    fOxLastT = micros();

    fAiLastF = aiFlow.getFlow();
    fAiLastT = micros();

  }else{
    pPeakAv = ((pPeakAv*pPeakN)+pInhale.getP())/(pPeakN+1);
    pPeakN++;

    thisFlow  = inFlow.getFlow();
    inhFlowAv = ((inhFlowAv*inhN)+thisFlow)/(inhN+1);
    thisVol   = ((thisFlow-inhLastF)*1000)/(micros()-inhLastT); //TODO--units
    inhVolAv  = ((inhVolAv*inhN)+thisVol)/(inhN+1);
    inhLastF  = thisFlow;
    inhLastT  = micros();
    inhN++;

    thisFlow  = oxFlow.getFlow();
    fOxFlowAv = ((fOxFlowAv*fOxN)+thisFlow)/(fOxN+1);
    thisVol   = ((thisFlow-fOxLastF)*1000)/(micros()-fOxLastT); //TODO--units
    fOxVolAv  = ((fOxVolAv*fOxN)+thisVol)/(fOxN+1);
    fOxLastF  = thisFlow;
    fOxLastT  = micros();
    fOxN++;

    thisFlow  = aiFlow.getFlow();
    fAiFlowAv = ((fAiFlowAv*fAiN)+thisFlow)/(fAiN+1);
    thisVol   = ((thisFlow-fAiLastF)*1000)/(micros()-fAiLastT); //TODO--units
    fAiVolAv  = ((fAiVolAv*fAiN)+thisVol)/(fAiN+1);
    fAiLastF  = thisFlow;
    fAiLastT  = micros();
    fAiN++;
  }
}
/*-------------------------------------------*/

void computeExhAv(){
  float thisFlow;
  float thisVol;
  
  if(inhLastF==0){
    exhLastF = exFlow.getFlow();
    exhLastT = micros();

    fOxLastF = oxFlow.getFlow();
    fOxLastT = micros();

    fAiLastF = aiFlow.getFlow();
    fAiLastT = micros();

  }else{
    pPeepAv = ((pPeepAv*pPeepN)+pExhale.getP())/(pPeepN+1);
    pPeepN++;

    thisFlow  = exFlow.getFlow();
    exhFlowAv = ((exhFlowAv*exhN)+thisFlow)/(exhN+1);
    thisVol   = ((thisFlow-exhLastF)*1000)/(micros()-exhLastT); //TODO--units
    exhVolAv  = ((exhVolAv*exhN)+thisVol)/(exhN+1);
    exhLastF  = thisFlow;
    exhLastT  = micros();
    exhN++;

    thisFlow  = oxFlow.getFlow();
    fOxFlowAv = ((fOxFlowAv*fOxN)+thisFlow)/(fOxN+1);
    thisVol   = ((thisFlow-fOxLastF)*1000)/(micros()-fOxLastT); //TODO--units
    fOxVolAv  = ((fOxVolAv*fOxN)+thisVol)/(fOxN+1);
    fOxLastF  = thisFlow;
    fOxLastT  = micros();
    fOxN++;

    thisFlow  = aiFlow.getFlow();
    fAiFlowAv = ((fAiFlowAv*fAiN)+thisFlow)/(fAiN+1);
    thisVol   = ((thisFlow-fAiLastF)*1000)/(micros()-fAiLastT); //TODO--units
    fAiVolAv  = ((fAiVolAv*fAiN)+thisVol)/(fAiN+1);
    fAiLastF  = thisFlow;
    fAiLastT  = micros();
    fAiN++;
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
  pPeakAv   = 0;
  pPeakN    = 0;

  pPeepAv   = 0;
  pPeepN    = 0;

  fOxVolAv  = 0;
  fOxFlowAv = 0;
  fOxN      = 0;
  fOxLastT  = micros();
  fOxLastF  = 0;

  fAiVolAv  = 0;
  fAiFlowAv = 0;
  fAiN      = 0;
  fAiLastT  = micros();
  fAiLastF  = 0;

  inhVolAv  = 0;
  inhFlowAv = 0;
  inhN      = 0;
  inhLastT  = micros();
  inhLastF  = 0;

  exhVolAv  = 0;
  exhFlowAv = 0;
  exhN      = 0;
  exhLastT  = micros();
  exhLastF  = 0;
}
/*-------------------------------------------*/

void updateRespSetpoints(){
  exhalePeriodUs = respRtUs/(1+inhExh);
  inhalePeriodUs = respRtUs-exhalePeriodUs;
}
/*-------------------------------------------*/

bool allSetptsReady(){
  if(pPeak==-1||pPeep==-1||respRtUs==-1||inhExh==-1){
     return false;
  }

  return true;
}
/*-------------------------------------------*/

void printPresSensors(){
  char buffer[70];
  sprintf(buffer, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,", 
          pMix.getP(), pInhale.getP(), pExhale.getP(),
          oxFlow.getP(), aiFlow.getP(), inFlow.getP(), exFlow.getP());
  Serial.println(buffer);

}
/*-------------------------------------------*/

void printPresOffset(){
  Serial.print("pmix: ");
  Serial.print(pMix.getOffset());
  Serial.print("\tpin: ");
  Serial.print(pInhale.getOffset());
  Serial.print("\tpex: ");
  Serial.print(pExhale.getOffset());
  Serial.print("\noxf: ");
  Serial.print(oxFlow.getOffset());
  Serial.print("\taif: ");
  Serial.print(aiFlow.getOffset());
  Serial.print("\tinf: ");
  Serial.print(inFlow.getOffset());
  Serial.print("\texf: ");
  Serial.println(exFlow.getOffset());
}
/*-------------------------------------------*/

/*EOF*/
