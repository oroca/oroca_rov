//----------------------------------------------------------------------------
//    프로그램명 	: LED
//
//    만든이     	: Made by Baram ( chcbaram@paran.com )
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: LED.h
//----------------------------------------------------------------------------
#ifndef _LED_H_
#define _LED_H_

#include <inttypes.h>
#include <Arduino.h> 




class cLED
{

public:


public:
	cLED();

	void begin( void );

	void on();
	void off();
	void toggle();
private:

};


#endif