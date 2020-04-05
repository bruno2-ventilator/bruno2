/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                    HONEYWELL HSC SPI PRESSURE SENSOR                    */
/*-------------------------------------------------------------------------*/

#include "Arduino.h"
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

void honeywellHsc::getRaw(){
  SPI.beginTransaction(SPISettings(_clkFreq, MSBFIRST, SPI_MODE1)); //MSB and MODE1
  digitalWrite(_spics, LOW);       //pull Chipselect Pin to Low

  uint8_t inByte_1 = SPI.transfer(0x00);  // Read first Byte of Pressure
  uint8_t inByte_2 = SPI.transfer(0x00);  // Read second Byte of Pressure
  uint8_t inByte_3 = SPI.transfer(0x00);  // Read first Byte of Temperature
  uint8_t inByte_4 = SPI.transfer(0x00);  // Read second Byte of Temperature

  digitalWrite(_spics, HIGH);      //pull Chipselect Pin to High
  SPI.endTransaction();            //end SPI Transaction

  _praw = (inByte_1 & 0x3F) << 8 | inByte_2;
  //Serial.print(inByte_1,BIN);
  //Serial.print("  ");
  //Serial.print(inByte_2,BIN);
  //Serial.print("  ");
  //Serial.print(inByte_3,BIN);
  //Serial.print("  ");
  //Serial.println(inByte_4,BIN);
  //Serial.println(_praw, BIN);
  //inByte_3 = inByte_3 << 3; //Shift first Temperature byte 3 left
  //_tempC = inByte_4;
  //_tempC = ((float)inByte_3 * 200 / 2047) - 50; //Convert Digital value to °C
}
/*-------------------------------------------*/

float honeywellHsc::getP(){
  float pout = 0;

  if(_unitChFlag==1){
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
    pout = pres->pressure()*_calib;
    if(PRINTDEB){
      Serial.print("m: Pressure: ");
      Serial.print(pout);
      Serial.print(" ");
      Serial.println((char)_units);
    }
  }
  else{
    if(PRINTDEB){
      Serial.println("m: Pressure: No new readout");
    }
  }

  //pout = _praw*_calib;
  //pout = 1.0 * ((float)readSensor(_spics) - OUTPUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;
  //pout = 1.0 * ((float)readSensor(_spics) - OUTPUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;
  //#define OUTPUT_MIN 1638.4        // 1638 counts (10% of 2^14 counts or 0x0666)
  //#define OUTPUT_MAX 14745.6       // 14745 counts (90% of 2^14 counts or 0x3999)
  //#define PRESSURE_MIN 0        // min is 0 for sensors that give absolute values
  //#define PRESSURE_MAX 10.3421   // 1.6bar (I want results in bar)
  //float val2 = 1.0 * ((float)readSensor(chipSelectPin2) - OUTPUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;

  return pout;
}
/*-------------------------------------------*/
/*EOF*/
