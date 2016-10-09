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
#include "MPU6050.h"




#define MPU6050_ADDRESS     0x68 // address pin AD0 low (GND), default for FreeIMU v0.4 and InvenSense evaluation board
//#define MPU6050_ADDRESS     0x69 // address pin AD0 high (VCC)


#define ACC_ORIENTATION(X, Y, Z)  {accADC[PITCH]  = -X; accADC[ROLL]  =  Y; accADC[YAW]  =   Z;}
#define GYRO_ORIENTATION(X, Y, Z) {gyroADC[PITCH] =  Y; gyroADC[ROLL] =  X; gyroADC[YAW] =  -Z;}



#define GYRO_DLPF_CFG   0     // GYRO_LPF_256HZ






/*---------------------------------------------------------------------------
     TITLE   : cMPU6050
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
cMPU6050::cMPU6050()
{	
     calibratingG = 0;
     calibratingA = 0;
}




/*---------------------------------------------------------------------------
     TITLE   : gyro_init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::gyro_init( void )
{
     uint8_t i;


     for( i=0; i<3; i++ )
     {
          gyroZero[i] = 0;
          accZero[i]  = 0;
     }

     I2C.write_reg(MPU6050_ADDRESS, 0x6B, 0x80);             //PWR_MGMT_1    -- DEVICE_RESET 1
     delay(50);
     I2C.write_reg(MPU6050_ADDRESS, 0x6B, 0x03);             //PWR_MGMT_1    -- SLEEP 0; CYCLE 0; TEMP_DIS 0; CLKSEL 3 (PLL with Z Gyro reference)
     I2C.write_reg(MPU6050_ADDRESS, 0x1A, GYRO_DLPF_CFG);    //CONFIG        -- EXT_SYNC_SET 0 (disable input pin for data sync) ; default DLPF_CFG = 0 => ACC bandwidth = 260Hz  GYRO bandwidth = 256Hz)
     I2C.write_reg(MPU6050_ADDRESS, 0x1B, 0x18);             //GYRO_CONFIG   -- FS_SEL = 3: Full scale set to 2000 deg/sec

     calibratingG = 512;     
}



/*---------------------------------------------------------------------------
     TITLE   : gyro_get_adc
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::gyro_get_adc( void ) 
{
     I2C.get_six_raw_adc(MPU6050_ADDRESS, 0x43);

     GYRO_ORIENTATION(   ((I2C.rawADC[0]<<8) | I2C.rawADC[1])>>2 , // range: +/- 8192; +/- 2000 deg/sec
                         ((I2C.rawADC[2]<<8) | I2C.rawADC[3])>>2 ,
                         ((I2C.rawADC[4]<<8) | I2C.rawADC[5])>>2 );
     gyro_common();
}





/*---------------------------------------------------------------------------
     TITLE   : gyro_cali_start
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::gyro_cali_start() 
{
     calibratingG = 512;
}




/*---------------------------------------------------------------------------
     TITLE   : acc_init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::acc_init( void ) 
{
     I2C.write_reg(MPU6050_ADDRESS, 0x1C, 0x10);             //ACCEL_CONFIG  -- AFS_SEL=2 (Full Scale = +/-8G)  ; ACCELL_HPF=0   //note something is wrong in the spec.
     //note: something seems to be wrong in the spec here. With AFS=2 1G = 4096 but according to my measurement: 1G=2048 (and 2048/8 = 256)
     //confirmed here: http://www.multiwii.com/forum/viewtopic.php?f=8&t=1080&start=10#p7480
}




/*---------------------------------------------------------------------------
     TITLE   : acc_get_adc
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::acc_get_adc( void ) 
{
     I2C.get_six_raw_adc(MPU6050_ADDRESS, 0x3B);
     ACC_ORIENTATION(    ((I2C.rawADC[0]<<8) | I2C.rawADC[1])>>3 ,
                         ((I2C.rawADC[2]<<8) | I2C.rawADC[3])>>3 ,
                         ((I2C.rawADC[4]<<8) | I2C.rawADC[5])>>3 );
     acc_common();
}





/*---------------------------------------------------------------------------
     TITLE   : gyro_common
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::gyro_common() 
{
     static int16_t previousGyroADC[3] = {0,0,0};
     static int32_t g[3];
     uint8_t axis, tilt=0;

     if (calibratingG>0) 
     {
          for (axis = 0; axis < 3; axis++) 
          {
               if (calibratingG == 512) 
               { // Reset g[axis] at start of calibration
                    g[axis]=0;                    
                    previousGyroADC[axis] = gyroADC[axis];
               }
               if (calibratingG % 10 == 0) 
               {
                    if(abs(gyroADC[axis] - previousGyroADC[axis]) > 8) tilt=1;
                    previousGyroADC[axis] = gyroADC[axis];
               }
               g[axis] += gyroADC[axis]; // Sum up 512 readings
               gyroZero[axis]=g[axis]>>9;

               if (calibratingG == 1) 
               {
                    //SET_ALARM_BUZZER(ALRM_FAC_CONFIRM, ALRM_LVL_CONFIRM_ELSE);
               }
          }
    
          if(tilt) 
          {
               calibratingG=1000;
          } 
          else 
          {
               calibratingG--;
          }
          return;
     }


     for (axis = 0; axis < 3; axis++) 
     {
          gyroADC[axis] -= gyroZero[axis];
          //anti gyro glitch, limit the variation between two consecutive readings
          gyroADC[axis] = constrain(gyroADC[axis],previousGyroADC[axis]-800,previousGyroADC[axis]+800);
          previousGyroADC[axis] = gyroADC[axis];
     }
}





/*---------------------------------------------------------------------------
     TITLE   : acc_common
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::acc_common() 
{
     static int32_t a[3];

     if (calibratingA>0) 
     {
         calibratingA--;
         for (uint8_t axis = 0; axis < 3; axis++) 
         {
           if (calibratingA == 511) a[axis]=0;   // Reset a[axis] at start of calibration
           a[axis] += accADC[axis];           // Sum up 512 readings
           accZero[axis] = a[axis]>>9; // Calculate average, only the last itteration where (calibratingA == 0) is relevant
         }
         if (calibratingA == 0) 
         {
           accZero[YAW] -= ACC_1G;   // shift Z down by ACC_1G and store values in EEPROM at end of calibration
         }
     }


     accADC[ROLL]  -=  accZero[ROLL] ;
     accADC[PITCH] -=  accZero[PITCH];
     accADC[YAW]   -=  accZero[YAW] ;
}





/*---------------------------------------------------------------------------
     TITLE   : acc_cali_start
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void cMPU6050::acc_cali_start() 
{
     calibratingA = 512;
}





/*---------------------------------------------------------------------------
     TITLE   : acc_cali_get_done
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
bool cMPU6050::acc_cali_get_done() 
{
     if( calibratingA == 0 ) return true;
     else                    return false;
}




/*---------------------------------------------------------------------------
     TITLE   : gyro_cali_get_done
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
bool cMPU6050::gyro_cali_get_done() 
{
     if( calibratingG == 0 ) return true;
     else                    return false;
}