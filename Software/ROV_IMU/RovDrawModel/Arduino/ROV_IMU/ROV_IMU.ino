/*
	IMU For ROV

	Made by Baram ( chcbaram@paran.com )
*/

#include "LED.h"
#include "IMU.h"


cLED 		LED;
cIMU		IMU;


uint8_t		err_code;


void setup() 
{
	LED.begin();
	
	err_code = IMU.begin();
}





void loop() 
{
	static uint32_t tTime[2];
  float out_str;


	if( (millis()-tTime[0]) >= 500 )
	{
		tTime[0] = millis();
		LED.toggle();
	}


	IMU.update();


	if( (millis()-tTime[1]) >= 50 )
	{
		
		tTime[1] = millis();
    #if 1
    Serial.print("Orientation: ");
    out_str = IMU.angle[0]/10;
    Serial.print(out_str);
    Serial.print(" ");
    out_str = IMU.angle[1]/10;
    Serial.print(out_str);
    Serial.print(" ");
    out_str = IMU.angle[2];
    Serial.print(out_str);
    Serial.println(" ");
    
    #else
		Serial.print(err_code);
		Serial.print(" ");
		Serial.print(IMU.angle[0]/10);
		Serial.print(" ");
		Serial.print(IMU.angle[1]/10);
		Serial.print(" ");
		Serial.println(IMU.angle[2]);		
    #endif
	}


	if( Serial.available() )
	{
		char Ch = Serial.read();

		if( Ch == '1' )
		{
			Serial.println("ACC Cali Start");

			IMU.SEN.acc_cali_start();
			while( IMU.SEN.acc_cali_get_done() == false )
			{
				IMU.update();
				//Serial.println( IMU.SEN.calibratingA );
			}

			Serial.print("ACC Cali End ");
		}
	}

}


