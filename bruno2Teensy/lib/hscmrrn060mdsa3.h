/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                     HSCMRRN060MDSA3 PRESSURE SENSOR                     */
/*-------------------------------------------------------------------------*/

#ifndef hscmrrn060mdsa3_h
#define hscmrrn060mdsa3_h

#include <Arduino.h>
#include <SPI.h>

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class hscmrrn060mdsa3
{

  public:
    hscmrrn060mdsa3(uint8_t spics);
    void setUnits(char units);
    char getUnits();
    uint16_t getRaw();
    float getP();
  private:
    const int _clkFreq = 50000; //hz - 50~800khz
    char      _units   = 'p';
    uint8_t   _spics;
    float     _calib   = 0.34;
    float     _tempC;

};
#endif
/*-------------------------------------------*/
/*EOF*/
