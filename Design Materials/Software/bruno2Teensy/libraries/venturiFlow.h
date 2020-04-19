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
    void filFlowRead();
    float getP();
    float getUnfilP();
    void setOffset();
    float getOffset();
  private:
    const int _pRangeSense = 6000; //using hscmrrn060mdsa3
    char    _units   = 'm'; //cm^3/s
    float   _calib   = 1;
    uint8_t _unitChFlag = 1;
    honeywellHsc *pSen;

};
#endif
/*-------------------------------------------*/
/*EOF*/
