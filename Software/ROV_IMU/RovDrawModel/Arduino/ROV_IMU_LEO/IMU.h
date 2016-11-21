//----------------------------------------------------------------------------
//    프로그램명 	: IMU
//
//    만든이     	: Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: IMU.h
//----------------------------------------------------------------------------
#ifndef _IMU_H_
#define _IMU_H_

#include <inttypes.h>
#include <Arduino.h> 

#include "MPU6050.h"


#define IMU_OK			0x00
#define IMU_ERR_I2C		0x01










class cIMU
{

public:
	cMPU6050 SEN;

	int16_t accZ;
	int16_t angle[3];

public:
	cIMU();

	uint8_t begin( void );
	uint8_t update( void );

	void computeIMU( void );
	void getEstimatedAttitude( void );

private:	

};


#endif