//----------------------------------------------------------------------------
//    프로그램명 	: ROV_LED
//
//    만든이     	:  
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: ROV_LED.cpp
//----------------------------------------------------------------------------

#include <Arduino.h> 
#include "ROV_LED.h"



/*---------------------------------------------------------------------------
     TITLE   : Motor_usb_test
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Led_setup()
{
  pinMode(LED_PIN_L, OUTPUT);
 // pinMode(LED_PIN_R, OUTPUT);

  analogWrite(LED_PIN_L, LED_pwm[LED_L]);
 // analogWrite(LED_PIN_R, LED_pwm[LED_R]);
  Serial.println("LED setup");
}

