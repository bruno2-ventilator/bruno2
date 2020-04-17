/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                           BRUNO2 VENTILATOR                             */
/*-------------------------------------------------------------------------*/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "libraries/honeywellHsc.h"
#include "libraries/venturiFlow.h"
#include "libraries/pinchValve.h"
#include <Adafruit_BME280.h>
#include <LibTempTMP421.h>

#define BUFFSIZE 15
#define SERIALTIMEOUT 200
#define BAUDRATE 115200
#define PRINTDEB true
#define RUNTIME false


//timekeeping variables
bool respRateServ          = false;
uint32_t loop700HzTime     = micros();
uint32_t loop700HzPeriodUs = 1428;      //700hz main contol
uint32_t loop100HzTime     = micros();
uint32_t loopRaspiPeriodUs = 20000;     //50hz raspi comm
uint32_t loopRaspiTime     = micros();
uint32_t loop100HzPeriodUs = 10000;     //100hz average values
uint32_t respStartTime     = micros();
uint32_t inhalePeriodUs    = 0;         //depends on respR
uint32_t exhalePeriodUs    = 0;         //depends on respR
uint32_t presSensorTime    = micros();
uint32_t loopPrintPeriodUs = 50000;//20Hz
uint32_t loopPrintTime     = micros();
float loopErr              = 1.03;

//main loop flags
bool offSeqCmpl     = false;
bool inhalationFlag = false;
bool exhalationFlag = false;

#if !(RUNTIME)
  //setpoints
  int pPeak    = 2500; //pascals
  int pPeep    = 1200; //pascals
  int respRtUs = (int)(60*1000000/25); //resp/min in millis() period
  float inhExh = 4; //exhale/inhale float

  //alarm setpoints
  int pPeepHigh    = 1500;
  int pPeepLow     = 900;
  int mVentHigh    = -1;
  int mVentLow     = -1;
  int tidVolExHigh = 1;
  int tidVolExLow  = 1;
#endif


//TODO -- delete, for debugging
uint32_t debugtime = micros();


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

  #if !(RUNTIME)
    enableRespirator();
  #endif

  //get all the initial setpoints
  while(!allSetptsReady() && !respEnableSt()){
    if(isRaspiCmdAv()){
      raspiCmdInterp();
      runOneRaspiCmd();
    }
  }

  updateRespSetpoints();
  deviceReadySignal();

  //reset timers
  loop700HzTime   = micros();
  loop100HzTime   = micros();
  loopRaspiTime   = micros();
  respStartTime   = micros();
  presSensorTime  = micros();

}
/*-------------------------------------------*/
/*-------------------------------------------*/


void loop() {


/*____________________________________________*/
/*         RUN RASPI COMMS SERVICES           */
  if(isTimeUp(loopRaspiTime, loopRaspiPeriodUs)){
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
    if(isTimeUp(loop700HzTime, loop700HzPeriodUs)){
      
      if(PRINTDEB){
        int tmp;
        tmp = micros()-loop700HzTime;
        if(tmp>(loop700HzPeriodUs*loopErr)){Serial.print("700Hz: ");Serial.println(tmp);}
      }
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

        if(isTimeUp(respStartTime, inhalePeriodUs)){
          inhalationFlag = false;
          exhalationFlag = true;
        }

      //exhalation control
      }else if(!inhalationFlag && exhalationFlag){
        exhaleControl();
        grnLedOnOff(false);

        if(isTimeUp(respStartTime, (exhalePeriodUs+inhalePeriodUs))){
          inhalationFlag = false;
          exhalationFlag = false;
        }
      }

    }
    /*******************************************/

//    //run pressure reads for filtering
//    if(isTimeUp(presSensorTime, honeywellHsc::sampPeriodUs)){
//      presSensorTime  = micros();
//      updatePresSensors();
//    }
//    /*******************************************/

    //run report P sensors services
    if(isTimeUp(loopPrintTime, loopPrintPeriodUs)){
      int tmp;
      int tmpTime;

      tmpTime = loop100HzTime;
      loop100HzTime = micros();

      if(PRINTDEB){
        tmp = micros()-tmpTime;
        if(tmp>(loopPrintPeriodUs*loopErr)){Serial.print("50hz: "); Serial.println(tmp);}
        tmp = micros();
      }

      printPresSensors();

      if(PRINTDEB){
        tmp = micros()-tmp;
        if(tmp>150){Serial.print("print: "); Serial.println(tmp);}
      }

    }
    /*******************************************/

//    //run 100Hz services
//    if(isTimeUp(loop100HzTime, loop100HzPeriodUs)){
//      loop100HzTime = micros();
//      computeAverages();
//    }
//    /*******************************************/

    //run respiratory Rate Services
    if(respRateServ){
      respRateServ = false;
      updateRespSetpoints();
      //reportAvarages();
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


bool isTimeUp(uint32_t lastTime, uint32_t periodUs){
  uint32_t lapsed = 0;;

  if(micros()<lastTime){
    lapsed = (lastTime - (2^32)) + micros();
  }else{
    lapsed = micros()-lastTime;
  }

  if(lapsed>periodUs){return true;}

  return false;
}
/*-------------------------------------------*/
/*-------------------------------------------*/

/*EOF*/
