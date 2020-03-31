/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          VENTURI FLOW METER                             */
/*-------------------------------------------------------------------------*/


#include "Arduino.h"
#include "libraries/venturiFlow.h"
#include "libraries/honeywellHsc.h"

#define PRINTDEB false

/*********************************************/
/*                 METHODS                   */
/*-------------------------------------------*/

void venturiFlow::init(uint8_t pAdd){
  //init sensor with addres for p1 and p2
  pSen = new honeywellHsc();

  pSen->init(pAdd, _pRangeSense);
  pSen->setUnits('a');
  if(PRINTDEB){Serial.println("m: init flow");}
}
/*-------------------------------------------*/

void venturiFlow::setUnits(char units){
  if(units=='l' || units=='m' || units=='i'){
    _units = units;
    _unitChFlag = 1;
  }
  else{
    if(PRINTDEB){Serial.println("m: not a valid unit");}
  }
}
/*-------------------------------------------*/

char venturiFlow::getUnits(){
  if(PRINTDEB){
    Serial.print("m: Flow sensor units ");
  }
  return _units;
}
/*-------------------------------------------*/

float venturiFlow::getFlow(){
  float tpSen;
  tpSen = pSen->getP();

  return tpSen*_calib;
}
/*-------------------------------------------*/

/*EOF*/
