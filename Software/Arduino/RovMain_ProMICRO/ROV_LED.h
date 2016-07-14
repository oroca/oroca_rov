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
//    파일명     	: ROV_LED.h
//----------------------------------------------------------------------------
#ifndef _ROV_LED_H_
#define _ROV_LED_H_

#include <inttypes.h>

#define LED_PIN_L  8
//#define LED_PIN_R  9

#define LED_L 0
// #define LED_R 1

#define LED_OFF 0xFFFF

static uint16_t LED_pwm[1] = {0};

void Led_setup();


#endif
