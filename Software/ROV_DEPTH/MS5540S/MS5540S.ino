//----------------------------------------------------------------------------
//    프로그램명   : RovMain
//
//    만든이       :  
//
//    날  짜     : 
//    
//    최종 수정   : 
//
//    MPU_Type  : 
//
//    파일명       : RovMain.ino
//----------------------------------------------------------------------------

#include "MS5540S.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  ms5540s_setup();
 }

void loop() 
{
  static uint32_t tTime;
  
  ms5540s_loop(SEA_WATER);  // or FRESH_WATER

  if( (millis() - tTime) >= 200 )
  {
    print_data();
    tTime = millis();
  }
}

void print_data()
{
  Serial.print("Pressure : ");      
  Serial.print(pressure);
  Serial.println(" mb(millibar)");   
  Serial.print("Temperature : ");      
  Serial.print(temperature);
  Serial.println(" 'C");   
  Serial.print("WaterDepth : ");      
  Serial.print(waterdepth);   
  Serial.println(" m\r\n");
}

