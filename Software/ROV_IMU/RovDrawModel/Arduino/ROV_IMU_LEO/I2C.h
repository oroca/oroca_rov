//----------------------------------------------------------------------------
//    프로그램명 	: I2C
//
//    만든이     	: Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: I2C.h
//----------------------------------------------------------------------------
#ifndef _I2C_H_
#define _I2C_H_

#include <inttypes.h>
#include <Arduino.h> 




class cI2C
{

public:	
	uint8_t rawADC[6];
	int16_t i2c_errors_count = 0;

public:
	cI2C();

	void begin( void );

	void rep_start(uint8_t address);
	void stop(void);
	void write(uint8_t data );

	uint8_t read_ack(void);
	uint8_t read_nak(void);

	void read_reg_to_buf(uint8_t add, uint8_t reg, uint8_t *buf, uint8_t size);
	void get_six_raw_adc(uint8_t add, uint8_t reg);
	void write_reg(uint8_t add, uint8_t reg, uint8_t val);

	uint8_t read_reg(uint8_t add, uint8_t reg);

private:
	


};


extern cI2C	I2C;


#endif