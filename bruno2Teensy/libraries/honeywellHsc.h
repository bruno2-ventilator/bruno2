/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                    HONEYWELL HSC SPI PRESSURE SENSOR                    */
/*-------------------------------------------------------------------------*/

#ifndef honeywellHsc_h
#define honeywellHsc_h

#include "Arduino.h"
#include <SPI.h>
#include "HoneywellTruStabilitySPI.h"

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
    void getRaw();
  private:
    TruStabilityPressureSensor *pres;
    int _minPress;  //Pascals
    int _maxPress;  //Pascals
    const int _clkFreq    = 100000; //hz - 50~800khz
    char      _units      = 'a';    //pAscal, Psi, mmMercury, mmWater
    uint8_t   _spics;
    float     _calib      = 1;
    float     _tempC;
    uint16_t  _praw;
    uint8_t   _unitChFlag = 1;

};
#endif
/*-------------------------------------------*/
/*EOF*/
