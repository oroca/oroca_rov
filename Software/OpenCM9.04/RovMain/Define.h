//----------------------------------------------------------------------------
//    프로그램명 	: DEFINE
//
//    만든이     	: Made by Baram ( chcbaram@paran.com )
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: DEFINE.h
//----------------------------------------------------------------------------
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <inttypes.h>
#include <Arduino.h> 



#define ROLL		0
#define PITCH		1
#define YAW			2

#define USB_TEST_AVAILABLE 1  // control Motor & LED by using USB serial

#define RC_MOTOR_L    0
#define RC_MOTOR_C    1
#define RC_MOTOR_R    2

#define RC_MOTOR_PIN_L  2
#define RC_MOTOR_PIN_C  3
#define RC_MOTOR_PIN_R  5

#define LED_PIN_L  8 
#define LED_PIN_R  9

#define MOTOR_NEUTRAL 1488
#define LED_OFF 0xFFFF


#endif
