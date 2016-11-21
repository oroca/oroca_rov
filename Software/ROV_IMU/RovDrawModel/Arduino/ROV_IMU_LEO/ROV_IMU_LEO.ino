/*
	IMU For ROV

	Made by Baram ( chcbaram@paran.com )
*/
#include <Servo.h>
#include "LED.h"
#include "IMU.h"

#define RC_MOTOR_PIN_L  9
#define RC_MOTOR_PIN_C  5
#define RC_MOTOR_PIN_R  10

#define RC_MOTOR_L    0
#define RC_MOTOR_C    1
#define RC_MOTOR_R    2
#define RC_LED        3

#define MOTOR_NEUTRAL 1488


Servo   RovMotor[3];
cLED 		LED;
cIMU		IMU;


uint8_t		err_code;


void setup() 
{
	LED.begin();
	
	err_code = IMU.begin();

  LED.off();
  
   RovMotor[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 1000, 2000);
   RovMotor[RC_MOTOR_L].writeMicroseconds(MOTOR_NEUTRAL);
   RovMotor[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 1000, 2000);
   RovMotor[RC_MOTOR_C].writeMicroseconds(MOTOR_NEUTRAL);
   RovMotor[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 1000, 2000);
   RovMotor[RC_MOTOR_R].writeMicroseconds(MOTOR_NEUTRAL);  
}

void loop() 
{
	static uint32_t tTime[2];
  float out_str;
  static uint16_t  rc_pwm = MOTOR_NEUTRAL;

	if( (millis()-tTime[0]) >= 500 )
	{
		tTime[0] = millis();
		//LED.toggle();
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

		if( Ch == 'c' )
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

   if( Ch == '1' )
   {
      digitalWrite(8, !digitalRead(8));
   }


   if( Ch == '2' )
   {
      if( rc_pwm == 1526 )
        rc_pwm = MOTOR_NEUTRAL;
      else
        rc_pwm = 1526;
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
   }


   if( Ch == '3' )
   {
      rc_pwm = MOTOR_NEUTRAL;
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
   }  
	}

}


