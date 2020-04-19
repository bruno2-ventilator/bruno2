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
    int16_t getRaw();
    float getUnfilP();
    float getP();
    void filPresRead();
    void setOffset();
    float getOffset();
    //from datasheet, data ready in average 1ms
    //static const uint32_t sampPeriodUs = 1100; //900Hz
  private:
    //TruStabilityPressureSensor *pres;
    int _minPress = -1;  //Pascals
    int _maxPress =  1;  //Pascals
    const int _minOutCnt = 1638; 
    const int _maxOutCnt = 14745;
    const int _clkFreq    = 500000; //hz - 50~800khz
    char      _units      = 'a';    //pAscal, Psi, mmMercury, mmWater
    uint8_t   _spics;
    float     _calib      = 1.0;
    float     _tempC;
    int16_t   _praw       = 0;
    uint8_t   _status     = 0;
    //float     pout        = 0;
    float     _pOffset    = 0;
    uint8_t   _unitChFlag = 1;
    uint32_t  _offSampFq  = 200; // in HZ
    uint32_t  _offSampN   = 400;
    //const float     _cutoffFreq = 400; //in Hz
    //Filter *fil;

};
#endif
/*-------------------------------------------*/
/*EOF*/
