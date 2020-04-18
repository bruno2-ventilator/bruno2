/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                               PINCH VALVE                               */
/*-------------------------------------------------------------------------*/


#include <Arduino.h>
#include <SPI.h>
#include "libraries/pinchValve.h"

#define PRINTDEB false

/*********************************************/
/*                 METHODS                   */
/*-------------------------------------------*/

void pinchValve::init(uint8_t spics, uint8_t pinOpen, uint8_t pinClose){
  //init sensor with addres for p1 and p2
  _spics = spics;

  mot = new HighPowerStepperDriver();
  mot->setChipSelectPin(_spics);

  // Give the driver some time to power up.
  delay(5);

  // Reset the driver to its default settings and clear latched status
  mot->resetSettings();
  mot->clearStatus();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  mot->setDecayMode(HPSDDecayMode::AutoMixed);

  // Set the current limit. You should change the number in h file
  mot->setCurrentMilliamps36v4(_currentSet);

  // Set the number of microsteps that correspond to one full step.
  mot->setStepMode(_stepMode);
  _mStPerRev = _stepsPerRev*_stepMode;

  // Enable the motor outputs.
  mot->enableDriver();

  //enable limit Sw
  _pinOp = pinOpen;
  _pinCl = pinClose;
  pinMode(_pinOp, INPUT);
  pinMode(_pinCl, INPUT);

  if(PRINTDEB){Serial.println("m: init pinch valve motor");}
}
/*-------------------------------------------*/

void pinchValve::closeValveBlocking(){
  if(!isValveFullClose()){
    mot->setDirection(_direction);
    do{
      mot->step();
      delayMicroseconds(_stPeriodUs);
    }while(!isValveFullClose());
  }
}
/*-------------------------------------------*/

void pinchValve::openValveBlocking(){
  if(!isValveFullOpen()){
    mot->setDirection(!_direction);
    do{
      mot->step();
      delayMicroseconds(_stPeriodUs);
    }while(!isValveFullOpen());
  }
}
/*-------------------------------------------*/

void pinchValve::close1StValve(){
  if(!isValveFullClose()){
    mot->setDirection(_direction);
    mot->step();
  }
}
/*-------------------------------------------*/

void pinchValve::open1StValve(){
  if(!isValveFullOpen()){
    mot->setDirection(!_direction);
    mot->step();
  }
}
/*-------------------------------------------*/

void pinchValve::moveToPosRel(int pos){
  if(pos==0|| abs(pos)>=_mStPerRev){return;}
  if(pos>0){mot->setDirection(_direction);}
  else {mot->setDirection(!_direction);}

  for (int i = 0; i < pos; i++) {
    mot->step();

    //TODO -- delaying here as patch, idealy would pos control in main loop
    delayMicroseconds(_stPeriodUs);
  }
}
/*-------------------------------------------*/

bool pinchValve::isValveFullClose(){
  return digitalRead(_pinCl);
}
/*-------------------------------------------*/

bool pinchValve::isValveFullOpen(){
  return digitalRead(_pinOp);
}
/*-------------------------------------------*/

/*EOF*/
