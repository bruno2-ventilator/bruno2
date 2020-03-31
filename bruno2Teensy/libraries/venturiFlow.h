/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          VENTURI FLOW METER                             */
/*-------------------------------------------------------------------------*/

#ifndef venturiFlow_h
#define venturiFlow_h

#include "Arduino.h"
#include "honeywellHsc.h"

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class venturiFlow
{

  public:
    void init(uint8_t pAdd);
    void setUnits(char units);
    char getUnits();
    float getFlow();
  private:
    const int _pRangeSense = 6000; //using hscmrrn060mdsa3
    float  _calib = 0.34;
    char  _units = 'l';
    uint8_t _unitChFlag = 1;
    honeywellHsc *pSen;

};
#endif
/*-------------------------------------------*/
/*EOF*/
