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
  int sign = 1;
  tpSen = pSen->getP();
  
  if(tpSen<0){sign = -1;}

  if(_unitChFlag==1){
    if(_units=='l'){      //Psi
      _calib = 1;
    }
    else if(_units=='m'){ //mmMercury
      _calib = 1000;
    }
    else if(_units=='i'){ //mmWater
      _calib = 61;
    }
    _unitChFlag = 0;
  }

  return (float)(sign*0.442*sqrt(abs(tpSen)*1000)*_calib); //in l/s
}
/*-------------------------------------------*/

float venturiFlow::getP(){
  return pSen->getP();
}
/*-------------------------------------------*/

float venturiFlow::getUnfilP(){
  return pSen->getUnfilP();
}
/*-------------------------------------------*/

void venturiFlow::filFlowRead(){
  pSen->filPresRead();
}
/*-------------------------------------------*/

void venturiFlow::setOffset(){
  pSen->setOffset();
}
/*-------------------------------------------*/

float venturiFlow::getOffset(){
  return pSen->getOffset();
}
/*-------------------------------------------*/

/*EOF*/
