/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                    HONEYWELL HSC SPI PRESSURE SENSOR                    */
/*-------------------------------------------------------------------------*/

#include <Arduino.h>
#include <SPI.h>
#include "libraries/honeywellHsc.h"

#define PRINTDEB false

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
//honeywellHsc::honeywellHsc(int spics){
//  _spics = spics;
//  init();
//  if(PRINTDEB){Serial.println("constructor");}
//}
///*-------------------------------------------*/

void honeywellHsc::init(uint8_t spics, int pRange){
  _spics = spics;
  _minPress = -pRange; //Pascals, from datasheet
  _maxPress = pRange;  //Pascals, from datasheet

  pres = new TruStabilityPressureSensor(_spics, _minPress, _maxPress);
  pres->begin();
  //pinMode(_spics, OUTPUT);
  //digitalWrite(_spics, HIGH); //CS high to disable

  _lastTime = micros();

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

int honeywellHsc::getRaw(){
  SPI.beginTransaction(SPISettings(_clkFreq, MSBFIRST, SPI_MODE1)); //MSB and MODE1
  digitalWrite(_spics, LOW);       //pull Chipselect Pin to Low

  uint8_t inByte_1 = SPI.transfer(0x00);  // Read first Byte of Pressure
  uint8_t inByte_2 = SPI.transfer(0x00);  // Read second Byte of Pressure
  //uint8_t inByte_3 = SPI.transfer(0x00);  // Read first Byte of Temperature
  //uint8_t inByte_4 = SPI.transfer(0x00);  // Read second Byte of Temperature

  digitalWrite(_spics, HIGH);      //pull Chipselect Pin to High
  SPI.endTransaction();            //end SPI Transaction

  _praw = (inByte_1 & 0x3F) << 8 | inByte_2;
  return _praw;
}
/*-------------------------------------------*/

float honeywellHsc::getP(){
  return pout;
}
/*-------------------------------------------*/

void honeywellHsc::setOffset(){
  //TODO -- read many samples and set offset
}
/*-------------------------------------------*/

void honeywellHsc::filPresRead(){

  if(_unitChFlag==1){
    fil.flush();
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

  if(pres->readSensor()==0){
    pout = fil.filterIn((float)((pres->pressure()-_pOffset)*_calib));
  }
  else{
    //TODO -- how to hande error
    Serial.println("sensor read error");
    }
  }
}
/*-------------------------------------------*/
/*EOF*/
