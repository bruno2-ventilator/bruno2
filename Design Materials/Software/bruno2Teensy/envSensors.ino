/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                         ENVIRONMENTAL SENSORS                           */
/*-------------------------------------------------------------------------*/


/*********************************************/
/*             ENV SEN VARIABLES             */
/*-------------------------------------------*/

//tem/hum sensors BME280
//#define PINBME280 10
uint8_t pinBme280 = 20;
Adafruit_BME280 *bme;
//Adafruit_BME280 bme(PINBME280);
int bmeDelayTime = 1000000;  // in microseconds

//temp sensor TMP421
uint8_t tempI2cAdd = 0x2A;
LibTempTMP421 temp = LibTempTMP421(0, tempI2cAdd);

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*                          ENV SENS FUNCTIONS                             */
/*-------------------------------------------------------------------------*/

/*********************************************/
/*               TEMPERATURE                 */
/*-------------------------------------------*/

void initTmpSensor(){
  temp.Init();
}
/*-------------------------------------------*/

float getTemp(){
  return temp.GetTemperature();
}
/*-------------------------------------------*/

float getTempF(){
  return (temp.GetTemperature() * 9.0) / 5.0 + 32.0;
}
/*-------------------------------------------*/


/*********************************************/
/*                 HUMIDITY                  */
/*-------------------------------------------*/
/* There are many ways to work with this sens*/
/* wired with spi pins, direct(forced) hum   */
/* reading. Later could try normal mode a/o  */
/* using reading filtering.                  */


void initHumSensor(){
  //bme.begin();
  bme = new Adafruit_BME280(pinBme280);

  bme->init();
  bme->setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1,   // temperature
                  Adafruit_BME280::SAMPLING_NONE, // pressure
                  Adafruit_BME280::SAMPLING_X1,   // humidity
                  Adafruit_BME280::FILTER_OFF );  //TODO -- try to filter
                    
}
/*-------------------------------------------*/

float getRelHum(){
  bme->takeForcedMeasurement();
  return bme->readHumidity();
}
/*-------------------------------------------*/

/*EOF*/
