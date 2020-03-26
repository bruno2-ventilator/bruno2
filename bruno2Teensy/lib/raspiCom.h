/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                     RASPBERRY PI COMMUNICATION API                      */
/*-------------------------------------------------------------------------*/

#ifndef raspiCom_h
#define raspiCom_h

#include "Arduino.h"

/*********************************************/
/*                CLASS DEF                  */
/*-------------------------------------------*/
class raspiCom
{

  public:
    raspiCom(int baudrate);
  private:
    int _baud;

};//EOF
#endif
/*-------------------------------------------*/
/*eof*/
