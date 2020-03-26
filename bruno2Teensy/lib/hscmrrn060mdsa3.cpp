
/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                     HSCMRRN060MDSA3 PRESSURE SENSOR                     */
/*-------------------------------------------------------------------------*/

#include <Arduino.h>
#include "bruno2.h"
#include <SPI.h>
#include "hscmrrn060mdsa3"

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
hscmrrn060mdsa3::hscmrrn060mdsa3(uint8_t spics){
  _spics = spics;
}
/*-------------------------------------------*/

void hscmrrn060mdsa3::setUnits(char units){
  if(units=='p' || units=='m' || units=='w')
    _units = units;
  else{
    if(PRINTEN){Serial.println("not a valid unit")}
  }

}
/*-------------------------------------------*/

char hscmrrn060mdsa3::getUnits(){
  if(PRINTEN){
    Serial.print("Sensor units")
    Serial.println((char)_units)
  }
  return _units;
}
/*-------------------------------------------*/

uint16_t hscmrrn060mdsa3::getRaw(){

  SPI.beginTransaction(SPISettings(_clkFreq, MSBFIRST, SPI_MODE1)); //MSB and MODE1
  digitalWrite(selectPin, LOW);       //pull Chipselect Pin to Low

  uint8_t inByte_1 = SPI.transfer(0x00);  // Read first Byte of Pressure
  uint8_t inByte_2 = SPI.transfer(0x00);  // Read second Byte of Pressure
  uint8_t inByte_3 = SPI.transfer(0x00);  // Read first Byte of Temperature
  uint8_t inByte_4 = SPI.transfer(0x00);  // Read second Byte of Temperature

  digitalWrite(selectPin, HIGH);      //pull Chipselect Pin to High
  SPI.endTransaction();               //end SPI Transaction

  Serial.print("Chipselect = "); Serial.print(selectPin, DEC); Serial.print(" ");
  Serial.print("Byte_1 =   "); Serial.print(inByte_1, DEC); Serial.print(" ");
  Serial.print("Byte_2 =   "); Serial.print(inByte_2, DEC); Serial.print(" ");
  Serial.print("Byte_3 =   "); Serial.print(inByte_3, DEC); Serial.print(" ");
  Serial.print("Byte_4 =   "); Serial.print(inByte_4, DEC); Serial.print(" ");

  int16_t pressure_dig = inByte_1 << 8 | inByte_2;

 inByte_3 = inByte_3 << 3; //Shift first Temperature byte 3 left
  
  float realTemp = ((float)inByte_3 * 200 / 2047) - 50; //Convert Digital value to °C
  

  
  
  Serial.print("Temp[C]=       "); Serial.print(realTemp); Serial.print("        ");

  return pressure_dig; //return digital Pressure Value

}
/*-------------------------------------------*/

float hscmrrn060mdsa3::getP(){


}
/*-------------------------------------------*/




const int _clkFreq = 50000; //hz - 50~800khz
char    _units  = 'p';
uint8_t _spics;
float   _calib  = 0.34;

/*-------------------------------------------*/
/*EOF*/




#define OUTPUT_MIN 1638.4        // 1638 counts (10% of 2^14 counts or 0x0666)
#define OUTPUT_MAX 14745.6       // 14745 counts (90% of 2^14 counts or 0x3999)
#define PRESSURE_MIN 0        // min is 0 for sensors that give absolute values
#define PRESSURE_MAX 10.3421   // 1.6bar (I want results in bar)

uint8_t chipSelectPin1 = 10;
uint8_t chipSelectPin2 = 9;




void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(chipSelectPin1, OUTPUT);
  digitalWrite(chipSelectPin1, HIGH);

   pinMode(chipSelectPin2, OUTPUT);
  digitalWrite(chipSelectPin2, HIGH);


}
void loop()
{
  
  
  

 float val1 = 1.0 * ((float)readSensor(chipSelectPin1) - OUTPUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;
  Serial.print("PRESS1 =      "); Serial.println(val1);

  float val2 = 1.0 * ((float)readSensor(chipSelectPin2) - OUTPUT_MIN) * (PRESSURE_MAX - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;
  Serial.print("PRESS2 =                     "); Serial.println(val2);



}

int16_t readSensor (uint8_t selectPin) {

  SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE1)); // Set to 700kHz, MSB and MODE1
  digitalWrite(selectPin, LOW);       //pull Chipselect Pin to Low

  int inByte_1 = SPI.transfer(0x00);  // Read first Byte of Pressure
  int inByte_2 = SPI.transfer(0x00);  // Read second Byte of Pressure
  int inByte_3 = SPI.transfer(0x00);  // Read first Byte of Temperature
  int inByte_4 = SPI.transfer(0x00);  // Read second Byte of Temperature



  digitalWrite(selectPin, HIGH);      //pull Chipselect Pin to High
  SPI.endTransaction();               //end SPI Transaction
  Serial.print("Chipselect = "); Serial.print(selectPin, DEC); Serial.print(" ");
  Serial.print("Byte_1 =   "); Serial.print(inByte_1, DEC); Serial.print(" ");
  Serial.print("Byte_2 =   "); Serial.print(inByte_2, DEC); Serial.print(" ");
  Serial.print("Byte_3 =   "); Serial.print(inByte_3, DEC); Serial.print(" ");
  Serial.print("Byte_4 =   "); Serial.print(inByte_4, DEC); Serial.print(" ");

  int16_t pressure_dig = inByte_1 << 8 | inByte_2;

 inByte_3 = inByte_3 << 3; //Shift first Temperature byte 3 left
  
  float realTemp = ((float)inByte_3 * 200 / 2047) - 50; //Convert Digital value to °C
  

  
  
  Serial.print("Temp[C]=       "); Serial.print(realTemp); Serial.print("        ");

  return pressure_dig; //return digital Pressure Value
}
