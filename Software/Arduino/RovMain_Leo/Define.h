//----------------------------------------------------------------------------
//    프로그램명 	: Define
//
//    만든이     	:  
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: Define.h
//----------------------------------------------------------------------------
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <inttypes.h>


//--모터 관련 상수, 변수
#define RC_MOTOR_L    0
#define RC_MOTOR_C    1
#define RC_MOTOR_R    2
#define RC_LED        3

#define RC_MOTOR_PIN_L  9
#define RC_MOTOR_PIN_C  5
#define RC_MOTOR_PIN_R  10

#define MOTOR_NEUTRAL 1488
#define USB_TEST_AVAILABLE 1

////////////////////////////////

static bool    IsConnected;
static uint8_t   err_code;
static uint16_t Motor_pwm[3] = {MOTOR_NEUTRAL,MOTOR_NEUTRAL,MOTOR_NEUTRAL};




#endif _DEFINE_H_
