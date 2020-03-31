/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                     RASPBERRY PI COMMUNICATION API                      */
/*-------------------------------------------------------------------------*/


char rcmdbuff[BUFFSIZE];
bool enableOutput = false;

/*********************************************/
/*           BRUNO2 VENTILATOR               */
/*-------------------------------------------*/


void initRaspiCom(){
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIALTIMEOUT);
  buffFlush();
  serialFlush();
}
/*-------------------------------------------*/

bool isRaspiCmdAv(){
  int av = Serial.available();
  if(av>0 && av<BUFFSIZE){
    //size_t tsize = Serial.readBytesUntil('\n', (char *)rcmdbuff, BUFFSIZE);
    Serial.readBytesUntil('\n', (char *)rcmdbuff, BUFFSIZE);
    return true;
  }
  else if(av>=BUFFSIZE){
    Serial.print(av);
    serialFlush();
    return false;
  }
  else{
    return false;
  }
}
/*-------------------------------------------*/

void printBuff(){
  for(int i = 0; i<BUFFSIZE; i++){
    Serial.print(rcmdbuff[i]);
  }
}
/*-------------------------------------------*/

void buffFlush(){
  for(int i = 0; i<BUFFSIZE; i++){
    rcmdbuff[i] = ' ';
  }
}
/*-------------------------------------------*/

void serialFlush(){
  while(Serial.available()>0){
    char tmp = Serial.read();
  }
}
/*-------------------------------------------*/

void runOneRaspiCmd(){}
/*-------------------------------------------*/

void raspiCmdInterp(){
  int i, j, k;
  i = findBuffIdx('/');
  
  int val;
  i++;
  switch(rcmdbuff[i]){
    case 'm':
      //
      break;
    case 'p':
      enableRespirator();
      sendAckMsg();
      break;
    case 's':
      enableRespirator();
      sendAckMsg();
      break;
    case 'c':
      j   = findBuffIdx('v');
      k   = findBuffIdx('t');
      i+=2; j++; k--;
      if(findBuffIdx('.')==-1){
        val = findBuffValue(j,k);
      }else{
        val = findBuffValueFloat(j,k);
      }
      switch(rcmdbuff[i]){
        case '1':
          pPeak        = val;
          sendAckMsg();
          break;
        case '2':
          pPeep        = val;
          sendAckMsg();
          break;
        case '3':
          pPeepHigh    = val;
          sendAckMsg();
          break;
        case '4':
          pPeepLow     = val;
          sendAckMsg();
          break;
        case '5':
          respR = (int)(60*1000000/val);
          sendAckMsg();
          break;
        case '6':
          inhExh       = val;
          sendAckMsg();
          break;
        case '7':
          tidVolExHigh = val;
          sendAckMsg();
          break;
        case '8':
          tidVolExLow  = val;
          sendAckMsg();
          break;
      }
      break;
    default:
      //
      break;
  }
}
/*-------------------------------------------*/

int findBuffIdx(char look){
  //TODO -- there is no error checking
  int ii = 0;
  bool found = false;

  for(ii=0; ii<BUFFSIZE; ii++){
    if(rcmdbuff[ii]==look){
      found = true;
      break;
    }
  }
  
  if(!found){return -1;}

  return ii;
}
/*-------------------------------------------*/

int findBuffValue(int from, int to){
  int size = to-from+1;
  char numb[size];

  for(int ii=0; ii<=size; ii++){
    numb[ii] = rcmdbuff[from+ii];
  }
  return atoi(numb);
}
/*-------------------------------------------*/

int findBuffValueFloat(int from, int to){
  int size = to-from+1;
  char numb[size];

  for(int ii=0; ii<=size; ii++){
    numb[ii] = rcmdbuff[from+ii];
  }
  return atof(numb);
}
/*-------------------------------------------*/

void sendAckMsg(){
  Serial.println("/kt");
}
/*-------------------------------------------*/

bool respEnableSt(){
  return enableOutput;
}
/*-------------------------------------------*/

void enableRespirator(){
  enableOutput = true;
}
/*-------------------------------------------*/

void disableRespirator(){
  enableOutput = false;
}
/*-------------------------------------------*/

/*EOF*/
