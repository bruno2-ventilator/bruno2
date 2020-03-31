/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                              PINCH VALVE                                */
/*-------------------------------------------------------------------------*/

#ifndef pinchvalve_h
#define pinchvalve_h

#include "Arduino.h"
#include <SPI.h>
#include <HighPowerStepperDriver.h>

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class pinchValve 
{

  public:
    void init(uint8_t spics);
    void closeValve();
    void openValve();
    void close1StValve();
    void open1StValve();
    void moveToPosRel(int pos);
  private:
    uint8_t _spics;
    bool _direction    = false; //change to true if rotating is opposite
    int _stepsPerRev   = 200; //from motor datasheet
    int _mStPerRev;
    int _currentSet    = 250;
    uint16_t _stepMode = 8;
    //uint16_t _stepMode = HPSDStepMode::MicroStep8;
    HighPowerStepperDriver *mot;
    int _stPeriodUs    = 1000;

};
#endif
/*-------------------------------------------*/
/*EOF*/
