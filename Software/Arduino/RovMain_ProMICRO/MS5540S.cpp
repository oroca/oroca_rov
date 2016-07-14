//----------------------------------------------------------------------------
//    프로그램명 	: MS5540S
//
//    만든이     	:  
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: MS5540S.cpp
//----------------------------------------------------------------------------
////////////////////////////////////////////
// MS5540S
////////////////////////////////////////////
#include "MS5540S.h"

int clock = 6;

const int get_interval = 35;
static int ms5540s_state = 0;


/*---------------------------------------------------------------------------
     TITLE   : ms5540s_reset
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void ms5540s_reset() //this function keeps the sketch a little shorter
{
 SPI.setDataMode(SPI_MODE0);
 SPI.transfer(0x15);
 SPI.transfer(0x55);
 SPI.transfer(0x40);
}

/*---------------------------------------------------------------------------
     TITLE   : ms5540s_setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void ms5540s_setup() {
 SPI.begin(); //see SPI library details on arduino.cc for details
 SPI.setBitOrder(MSBFIRST);
 SPI.setClockDivider(SPI_CLOCK_DIV32); //divide 16 MHz to communicate on 500 kHz
 pinMode(clock, OUTPUT);
 delay(100);
}

/*---------------------------------------------------------------------------
     TITLE   : ms5540s_loop
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void ms5540s_loop()
{
  static uint32_t tTime;

  unsigned int word1 = 0;
  unsigned int word11 = 0;
  unsigned int word2 = 0;
  unsigned int word3 = 0;
  byte word22 = 0;
  byte word33 = 0;
  unsigned int word4 = 0;
  byte word44 = 0;
  static long c1;
  static long c2;
  static long c3;
  static long c4;
  static long c5;
  static long c6;
  unsigned int presMSB = 0; //first byte of value
  unsigned int presLSB =0; //last byte of value
  static unsigned int D1 = 0;
  unsigned int tempMSB = 0; //first byte of value
  unsigned int tempLSB = 0; //last byte of value
  static unsigned int D2 = 0;
  long UT1 = 0;
  long dT = 0;
  long TEMP = 0;
  long OFF = 0;
  long SENS = 0;

  static long PCOMP = 0;
  static float TEMPREAL = 0;

  long dT2 = 0;
  static float TEMPCOMP = 0;

  bool ret = false;

  switch( ms5540s_state )
  {
    case 0:      
      TCCR4B = (TCCR4B & 0xF0) | 1 ; //generates the MCKL signal
      analogWrite (clock, 128) ;
      ms5540s_reset();//resets the sensor - caution: afterwards mode = SPI_MODE0!
      //Calibration word 1
      word1 = 0;
      word11 = 0;
      SPI.transfer(0x1D); //send first byte of command to get calibration word 1
      SPI.transfer(0x50); //send second byte of command to get calibration word 1
      SPI.setDataMode(SPI_MODE1); //change mode in order to listen
      word1 = SPI.transfer(0x00); //send dummy byte to read first byte of word
      word1 = word1 << 8; //shift returned byte
      word11 = SPI.transfer(0x00); //send dummy byte to read second byte of word
      word1 = word1 | word11; //combine first and second byte of word
      ms5540s_reset();//resets the sensor
      //Calibration word 2; see comments on calibration word 1
      word2 = 0;
      word22 = 0;
      SPI.transfer(0x1D);
      SPI.transfer(0x60);
      SPI.setDataMode(SPI_MODE1);
      word2 = SPI.transfer(0x00);
      word2 = word2 <<8;
      word22 = SPI.transfer(0x00);
      word2 = word2 | word22;
      ms5540s_reset();//resets the sensor
      //Calibration word 3; see comments on calibration word 1
      word3 = 0;
      word33 = 0;
      SPI.transfer(0x1D);
      SPI.transfer(0x90);
      SPI.setDataMode(SPI_MODE1);
      word3 = SPI.transfer(0x00);
      word3 = word3 <<8;
      word33 = SPI.transfer(0x00);
      word3 = word3 | word33;
      ms5540s_reset();//resets the sensor
      //Calibration word 4; see comments on calibration word 1
      word4 = 0;
      word44 = 0;
      SPI.transfer(0x1D);
      SPI.transfer(0xA0);
      SPI.setDataMode(SPI_MODE1);
      word4 = SPI.transfer(0x00);
      word4 = word4 <<8;
      word44 = SPI.transfer(0x00);
      word4 = word4 | word44;

      c1 = word1 << 1;
      c2 = ((word3 & 0x3F) >> 6) | ((word4 & 0x3F));
      c3 = (word4 << 6) ;
      c4 = (word3 << 6);
      c5 = (word2 << 6) | ((word1 & 0x1) >> 10);
      c6 = word2 & 0x3F;
      ms5540s_reset();//resets the sensor
      //Temperature:
      SPI.transfer(0x0F); //send first byte of command to get temperature value
      SPI.transfer(0x20); //send second byte of command to get temperature value

      tTime = millis();
      ms5540s_state = 1;
      break;

    case 1:
      if( (millis()-tTime) >= get_interval )
      {
        ms5540s_state = 2;
      }
      break;

    case 2:
      SPI.setDataMode(SPI_MODE1); //change mode in order to listen
      tempMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
      tempMSB = tempMSB << 8; //shift first byte
      tempLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
      D2 = tempMSB | tempLSB; //combine first and second byte of value
      ms5540s_reset();//resets the sensor
     //Pressure:
      SPI.transfer(0x0F); //send first byte of command to get pressure value
      SPI.transfer(0x40); //send second byte of command to get pressure value

      tTime = millis();
      ms5540s_state = 3;
      break;

    case 3:
      if( (millis()-tTime) >= get_interval )
      {
        ms5540s_state = 4;
      }
      break;

    case 4:
      SPI.setDataMode(SPI_MODE1); //change mode in order to listen
      presMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
      presMSB = presMSB << 8; //shift first byte
      presLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
      D1 = presMSB | presLSB;

      UT1 = (c5 * 8) + 20224;
      dT =(D2 - UT1);
      TEMP = 200 + ((dT * (c6 + 50))/1024);
      OFF = (c2*4) + (((c4 - 512) * dT)/4096);
      SENS = c1 + ((c3 * dT)/1024) + 24576;

      PCOMP = ((((SENS * (D1 - 7168))/16384)- OFF)/32)+250;
      TEMPREAL = TEMP/10;
      
      Serial.print("  pressure = ");
      Serial.print(PCOMP);
      Serial.println(" mbar");
      dT2 = dT - ((dT >> 7 * dT >> 7) >> 3);
      TEMPCOMP = (200 + (dT2*(c6+100) >>11))/10;
      Serial.print("  temperature = ");
      Serial.print(TEMPCOMP);
      Serial.println(" *C");
      Serial.println("************************************");

      ret = true;
      ms5540s_state = 0;
      break;

    default:
      ms5540s_state = 0;
      break;
  }
}


