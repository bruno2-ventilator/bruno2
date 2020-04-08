/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                    HONEYWELL HSC SPI PRESSURE SENSOR                    */
/*-------------------------------------------------------------------------*/

#ifndef honeywellHsc_h
#define honeywellHsc_h

#include "Arduino.h"
#include <SPI.h>
#include <HoneywellTruStabilitySPI.h>
#include <filters.h>

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class honeywellHsc
{

  public:
    void init(uint8_t spics, int pRange); //in pascals
    void setUnits(char units);
    char getUnits();
    float getP();
    void filPresRead();
    int getRaw();
    void setOffset();
    const uint32_t _samplingFreq = 2800; //in Hz
  private:
    TruStabilityPressureSensor *pres;
    int _minPress;  //Pascals
    int _maxPress;  //Pascals
    const int _clkFreq    = 500000; //hz - 50~800khz
    char      _units      = 'a';    //pAscal, Psi, mmMercury, mmWater
    uint8_t   _spics;
    float     _calib      = 1;
    float     _tempC;
    uint16_t  _praw       = 0;
    float     pout        = 0;
    int32_t   _pOffset    = 0;
    uint8_t   _unitChFlag = 1;
    uint32_t  _cutoffFreq = 700; //in Hz
    Filter fil(_cutoffFreq, (float)(1/_samplingFreq), IIR::ORDER::ODER2, IIR::TYPE::LOWPASS);

};
#endif
/*-------------------------------------------*/
/*EOF*/
