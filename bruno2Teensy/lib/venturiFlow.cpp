/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          VENTURI FLOW METER                             */
/*-------------------------------------------------------------------------*/


#include "Arduino.h"
#include "./lib/venturiFlow.h"
#include "./lib/pressureSensor.h"

/*********************************************/
/*                 METHODS                   */
/*-------------------------------------------*/



venturiFlow::venturiFlow(int p1add, int p2add){
  _p1add = p1add;
  _p2add = p2add;
  initSensor();
}
/*-------------------------------------------*/

void venturiFlow::setUnits(char unit){

}
/*-------------------------------------------*/

char venturiFlow::getUnits(){

}
/*-------------------------------------------*/

float venturiFlow::getFlow(){
  //call get pressure senor for both
  //convert using _calib
}
/*-------------------------------------------*/

void venturiFlow::initSensor(){
  //init sensor with addres for p1 and p2
}
/*-------------------------------------------*/



int _p1add;

int _p2add;

int _calib;


};//EOF
#endif
/*-------------------------------------------*/
/*eof*/
