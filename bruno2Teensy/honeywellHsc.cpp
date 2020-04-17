/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                    HONEYWELL HSC SPI PRESSURE SENSOR                    */
/*-------------------------------------------------------------------------*/

#include <Arduino.h>
#include <SPI.h>
#include "libraries/honeywellHsc.h"

#define PRINTDEB false
//TODO -- define with file name for printdebug

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/

//Filter filt(400, 0.0011, IIR::ORDER::OD2, IIR::TYPE::LOWPASS);
//Filter filt(_cutoffFreq, (1.0*sampPeriodUs/1000000), IIR::ORDER::OD2, IIR::TYPE::LOWPASS);

void honeywellHsc::init(uint8_t spics, int pRange){
  _spics = spics;
  _minPress = -pRange; //Pascals, from datasheet
  _maxPress = pRange;  //Pascals, from datasheet

  //pres = new TruStabilityPressureSensor(_spics, _minPress, _maxPress);
  //pres->begin();
  pinMode(_spics, OUTPUT);
  digitalWrite(_spics, HIGH); //CS high to disable


  //fil = new Filter(_cutoffFreq, ((float)(sampPeriodUs/1000000)), IIR::ORDER::OD2, IIR::TYPE::LOWPASS);
  if(PRINTDEB){Serial.println("m: init press");}

}
/*-------------------------------------------*/

void honeywellHsc::setUnits(char units){
  if(units=='p' || units=='m' || units=='w' || units=='a'){
    _units = units;
    _unitChFlag = 1;
  }
  else{
    if(PRINTDEB){Serial.println("m: not a valid unit");}
  }
}
/*-------------------------------------------*/

char honeywellHsc::getUnits(){
  if(PRINTDEB){
    Serial.print("m: Pressure sensor units ");
  }
  return _units;
}
/*-------------------------------------------*/

int16_t honeywellHsc::getRaw(){
  SPI.beginTransaction(SPISettings(_clkFreq, MSBFIRST, SPI_MODE0)); //MSB and MODE0
  digitalWrite(_spics, LOW);       //pull Chipselect Pin to Low

  uint8_t inByte_1 = SPI.transfer(0x00);  // Read first Byte of Pressure
  uint8_t inByte_2 = SPI.transfer(0x00);  // Read second Byte of Pressure
  //uint8_t inByte_3 = SPI.transfer(0x00);  // Read first Byte of Temperature
  //uint8_t inByte_4 = SPI.transfer(0x00);  // Read second Byte of Temperature

  digitalWrite(_spics, HIGH);      //pull Chipselect Pin to High
  SPI.endTransaction();            //end SPI Transaction

  _praw   = ((inByte_1 & 0x3F)<<8) | inByte_2;
  _status = ((inByte_1 & 0xC0)>>6);
  
  //TODO -- delete, just for error checking
  if(PRINTDEB){
    if(_status != 0){
    Serial.print("stat: ");
    Serial.println(_status,BIN);
    }
  }
  return _praw;
}
/*-------------------------------------------*/

float honeywellHsc::getUnfilP(){
  float tmpVal;
  tmpVal = (_maxPress-_minPress)*1.0/(_maxOutCnt-_minOutCnt);

  if(_unitChFlag==1){
    //fil->flush();
    if(_units=='p'){      //Psi
      _calib = 0.000145038;
    }
    else if(_units=='m'){ //mmMercury
      _calib = 0.0075006;
    }
    else if(_units=='w'){ //mmWater
      _calib = 0.1019716;
    }
    else if(_units=='a'){ //pAscals (def)
      _calib = 1;
    }
    _unitChFlag = 0;
  }

  //from datasheet
  return _calib*((getRaw()-_minOutCnt)*tmpVal + _minPress - _pOffset);
}
/*-------------------------------------------*/

float honeywellHsc::getP(){
  return getUnfilP();
  //return pout;
}
/*-------------------------------------------*/

void honeywellHsc::filPresRead(){
//  if(_status==0){
//    pout = fil->filterIn(getUnfilP());
//  }
//  else{
//    //TODO -- how to hande error, now filling with average
//    pout = fil->filterIn(pout);
//  }
}
/*-------------------------------------------*/

void honeywellHsc::setOffset(){
  bool cont = true;
  uint32_t lastTime = micros();
  uint32_t cnt = 0;
  uint32_t period = 1000000/_offSampFq;
  float tmpOff = 0;

  while(cont){
    //TODO -- this will not work at wrap up
    if((micros()-lastTime)>period){
      tmpOff = (getUnfilP()+tmpOff*cnt)/(cnt+1);
      lastTime = micros();
      cnt++;
    }

    if(cnt > _offSampN){cont = false;}
  }

  _pOffset = tmpOff;
}
/*-------------------------------------------*/

float honeywellHsc::getOffset(){
  return _pOffset;
}
/*-------------------------------------------*/

/*EOF*/
