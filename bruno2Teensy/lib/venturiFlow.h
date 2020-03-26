/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          VENTURI FLOW METER                             */
/*-------------------------------------------------------------------------*/

#ifndef venturiFlow_h
#define venturiFlow_h

#include "Arduino.h"
#include "./lib/pressureSensor.h"

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class venturiFlow
{

  public:
    venturiFlow(int p1add, int p2add);
    void setUnits(char unit);
    char getUnits();
    float getFlow();
  private:
    void initSensor();
    int _p1add;
    int _p2add;
    int _calib = 0.34;
    //pressureSensor p1
    //pressureSensor p2

};
#endif
/*-------------------------------------------*/
/*EOF*/
