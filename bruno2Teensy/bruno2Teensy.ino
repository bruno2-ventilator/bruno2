/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                           BRUNO2 VENTILATOR                             */
/*-------------------------------------------------------------------------*/

#include "Arduino.h"
#include <SPI.h>
#include "libraries/honeywellHsc.h"
#include "libraries/venturiFlow.h"
#include "libraries/pinchValve.h"

#define BUFFSIZE 15
#define SERIALTIMEOUT 200
#define BAUDRATE 115200
#define PRINTDEB true


//timekeeping variables
bool respRateServ        = false;
uint32_t loop700HzTime   = micros();
//uint32_t loop700HzPeriod = 1428;      //700hz main contol
uint32_t loop100HzTime   = micros();
uint32_t loopRaspiPeriod = 20000;     //50hz raspi comm
uint32_t loopRaspiTime   = micros();
uint32_t loop100HzPeriod = 10000;     //100hz average values
uint32_t respStartTime   = micros();
uint32_t inhalePeriod    = 0;         //depends on respR
uint32_t exhalePeriod    = 0;         //depends on respR

//main loop flags
bool offSeqCmpl     = false;
bool inhalationFlag = false;
bool exhalationFlag = false;

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

//TODO -- delete
//setpoints
int pPeak    = 2500; //pascals
int pPeep    = 1200; //pascals
int respR    = (int)(60*1000000/25); //resp/min in millis() period
float inhExh = 2; //exhale/inhale float

//alarm setpoints
int pPeepHigh    = 1500;
int pPeepLow     = 900;
//int mVentHigh    = -1;
//int mVentLow     = -1;
int tidVolExHigh = 1;
int tidVolExLow  = 1;

//TODO -- for now to control loop motor step speed
uint32_t loop700HzPeriod = 1428;      //700hz main contol

/*********************************************/
/*           BRUNO2 VENTILATOR               */
/*-------------------------------------------*/

void setup() {
  //init base teensy modules
  initTeensyModules();

  //start serial comms
  initRaspiCom();

  //init control vars
  initControl();

  //reset timers
  loop700HzTime   = micros();
  loop100HzTime   = micros();
  respStartTime   = micros();
  

  //get all the initial setpoints
  while(!allSetptsReady()){
    if(isRaspiCmdAv()){
      raspiCmdInterp();
      runOneRaspiCmd();
    }
  }

  updateRespSetpoints();
  //TODO -- lisent for msg
  enableRespirator();


}
/*-------------------------------------------*/
/*-------------------------------------------*/


void loop() {

/*____________________________________________*/
/*         RUN RASPI COMMS SERVICES           */
  if(isTimeUp(loopRaspiTime, loopRaspiPeriod)){
    loopRaspiTime = millis();
    //This will only take one cmd at a time
    //otherwse it blocks the code too much
    if(isRaspiCmdAv()){
      raspiCmdInterp();
      runOneRaspiCmd(); 
    }
  }


/*____________________________________________*/
/*               RUN SERVICES                 */
  if(respEnableSt()){
    offSeqCmpl   = true;

    //run 700Hz services
    if(isTimeUp(loop700HzTime, loop700HzPeriod)){
      loop700HzTime   = micros();
      
      //start respiration
      if(!inhalationFlag && !exhalationFlag){
        respStartTime  = micros();
        resetAvarages();
        updateRespSetpoints();
        inhalationFlag = true;
        exhalationFlag = false;
        respRateServ   = true;

      //inhalation control
      }else if(inhalationFlag && !exhalationFlag){
        inhaleControl();
        grnLedOnOff(true);

        if(isTimeUp(respStartTime, inhalePeriod)){
          inhalationFlag = false;
          exhalationFlag = true;
        }

      //exhalation control  
      }else if(!inhalationFlag && exhalationFlag){
        exhaleControl();
        grnLedOnOff(false);

        if(isTimeUp(respStartTime, (exhalePeriod+inhalePeriod))){
          inhalationFlag = false;
          exhalationFlag = false;
        }
      }
    }  
    /*******************************************/

    //run 100Hz services
    if(isTimeUp(loop100HzTime, loop100HzPeriod)){
      loop100HzTime   = micros();
      //computeAverages();
    }  
    /*******************************************/

    //run respiratory Rate Services
    if(respRateServ){
      respRateServ = false;
      updateRespSetpoints();
      //sendAverageVal();
    }  
    /*******************************************/
  
  }
/*____________________________________________*/
/*             TURN OFF SERVICES              */
  else if(!respEnableSt() && offSeqCmpl){
    offSeqCmpl   = true;
  }
  
}
/*-------------------------------------------*/
/*-------------------------------------------*/


bool isTimeUp(uint32_t lastTime, uint32_t period){
  uint32_t lapsed = 0;
  if(micros()<lastTime){
    lapsed = (lastTime - (2^32)) + micros();
  }else{
    lapsed = micros()-lastTime;
  }

  if(lapsed>period){return true;}

  return false;
}
/*-------------------------------------------*/
/*-------------------------------------------*/

/*EOF*/
