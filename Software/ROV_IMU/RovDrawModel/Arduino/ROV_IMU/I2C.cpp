//----------------------------------------------------------------------------
//    프로그램명 	: 
//
//    만든이     	: Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: LED.ino
//----------------------------------------------------------------------------




#include <Arduino.h> 
#include "I2C.h"




cI2C I2C;



#define I2C_PULLUPS_ENABLE         PORTD |=  (1<<0); PORTD |=  (1<<1);
#define I2C_PULLUPS_DISABLE        PORTD &= ~(1<<0); PORTD &= ~(1<<1);





void __attribute__ ((noinline)) waitTransmissionI2C(uint8_t twcr) 
{
     uint8_t count = 255;

     TWCR = twcr;
     
     while (!(TWCR & (1<<TWINT))) 
     {
          count--;
          if (count==0) 
          {              //we are in a blocking state => we don't insist
               TWCR = 0;                  //and we force a reset on TWINT register
               I2C.i2c_errors_count++;
               break;
          }
     }
}





/*---------------------------------------------------------------------------
     TITLE   : I2C
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
cI2C::cI2C()
{	
}




/*---------------------------------------------------------------------------
     TITLE   : begin
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::begin( void )
{
     I2C_PULLUPS_DISABLE
 
     TWSR = 0;                                    // no prescaler => prescaler = 1
     TWBR = ((F_CPU / 400000) - 16) / 2;          // set the I2C clock rate to 400kHz
     TWCR = 1<<TWEN;                              // enable twi module, no interrupt
     
     i2c_errors_count = 0;
}




/*---------------------------------------------------------------------------
     TITLE   : rep_start
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::rep_start(uint8_t address) 
{
     waitTransmissionI2C((1<<TWINT) | (1<<TWSTA) | (1<<TWEN)); // send REPEAT START condition and wait until transmission completed
     TWDR = address;                                           // send device address
     waitTransmissionI2C((1<<TWINT) | (1<<TWEN));              // wail until transmission completed
}





/*---------------------------------------------------------------------------
     TITLE   : stop
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::stop(void) 
{
     TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
     //  while(TWCR & (1<<TWSTO));                // <- can produce a blocking state with some WMP clones
}





/*---------------------------------------------------------------------------
     TITLE   : stop
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::write(uint8_t data ) 
{
     TWDR = data;                                 // send data to the previously addressed device
     waitTransmissionI2C((1<<TWINT) | (1<<TWEN));
}





/*---------------------------------------------------------------------------
     TITLE   : read_ack
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint8_t cI2C::read_ack(void) 
{
     waitTransmissionI2C((1<<TWINT) | (1<<TWEN) | (1<<TWEA));
     return TWDR;
}





/*---------------------------------------------------------------------------
     TITLE   : read_ack
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint8_t cI2C::read_nak(void) 
{
     waitTransmissionI2C((1<<TWINT) | (1<<TWEN));
     uint8_t r = TWDR;
     stop();
     return r;
}




/*---------------------------------------------------------------------------
     TITLE   : read_reg_to_buf
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::read_reg_to_buf(uint8_t add, uint8_t reg, uint8_t *buf, uint8_t size) 
{
     rep_start(add<<1); // I2C write direction
     write(reg);        // register selection
     rep_start((add<<1) | 1);  // I2C read direction
     
     uint8_t *b = buf;
     while (--size) *b++ = read_ack(); // acknowledge all but the final byte

     *b = read_nak();
}





/*---------------------------------------------------------------------------
     TITLE   : get_six_raw_adc
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::get_six_raw_adc(uint8_t add, uint8_t reg) 
{
     read_reg_to_buf(add, reg, rawADC, 6);
}





/*---------------------------------------------------------------------------
     TITLE   : write_reg
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cI2C::write_reg(uint8_t add, uint8_t reg, uint8_t val) 
{
     rep_start(add<<1); // I2C write direction
     write(reg);        // register selection
     write(val);        // value to write in register
     stop();
}





/*---------------------------------------------------------------------------
     TITLE   : read_reg
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
uint8_t cI2C::read_reg(uint8_t add, uint8_t reg) 
{
     uint8_t val;

     read_reg_to_buf(add, reg, &val, 1);

     return val;
}