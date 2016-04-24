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


	if( (millis()-tTime[0]) >= 500 )
	{
		tTime[0] = millis();
		LED.toggle();
	}


	IMU.update();


	if( (millis()-tTime[1]) >= 100 )
	{
		
		tTime[1] = millis();
		Serial.print(err_code);
		Serial.print(" ");
		Serial.print(IMU.angle[0]/10);
		Serial.print(" ");
		Serial.print(IMU.angle[1]/10);
		Serial.print(" ");
		Serial.println(IMU.angle[2]);
		
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


