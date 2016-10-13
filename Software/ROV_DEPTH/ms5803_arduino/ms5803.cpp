/* 
 * @File: ms5803.c
 * @Brief : MS5803-14BA(Pressure sensor) library
 * @Date : 2016. 05. 13
 * @Version : 0.0
 * @Author : dydwo92@snu.ac.kr
 */

#include "Arduino.h"
#include <stdint.h>
#include <stdbool.h>
#include <Wire.h>
#include "ms5803.h"

uint16_t _SENS_T1, _OFF_T1, _TCS, _TCO, _T_REF, _TEMP_SENS;
uint32_t _D1, _D2;
int32_t _dT;
int64_t _OFF, _SENS;

/*******************************************************************************
* Function Name  : ms5803_reset
* Description    : Rest ms5803-14ba sensor
* Input          : None.
* Output         : None.
* Reference      : Datasheet p.11
*******************************************************************************/
void ms5803_reset(void){
  Wire.beginTransmission(MS5803_ID);
  Wire.write(MS5803_RESET);
  Wire.endTransmission(true);
}

/*******************************************************************************
* Function Name  : ms5803_init
* Description    : Save factory calibrated data
* Input          : None.
* Output         : None.
* Reference      : Datasheet p.7 figure.3
*******************************************************************************/
void ms5803_init(void){
  ms5803_reset();

  _SENS_T1 = ms5803_read_uint16(MS5803_C1);
  _OFF_T1 = ms5803_read_uint16(MS5803_C2);
  _TCS = ms5803_read_uint16(MS5803_C3);
  _TCO = ms5803_read_uint16(MS5803_C4);
  _T_REF = ms5803_read_uint16(MS5803_C5);
  _TEMP_SENS = ms5803_read_uint16(MS5803_C6);  
}

/*******************************************************************************
* Function Name  : ms5803_read_uint16
* Description    : Read PROM data from sensor
* Input          : Register ID to read
* Output         : 16bit unsinged int
* Reference      : Datasheet p.12
*******************************************************************************/
uint16_t ms5803_read_uint16(int register_id){
  delay(10);
  uint16_t data;
  Wire.beginTransmission(MS5803_ID);
  Wire.write(register_id);
  Wire.endTransmission(false);
  Wire.requestFrom(MS5803_ID,2,true);
  data=Wire.read()<<8|Wire.read();
  Wire.endTransmission(true);

  return data;
}

/*******************************************************************************
* Function Name  : ms5803_read_uint32
* Description    : Read digital pressure and temperature data
* Input          : Register ID to read
* Output         : 32bit unsigned int
* Reference      : Datasheet p.11
*******************************************************************************/
uint32_t ms5803_read_uint32(int register_id){
  uint32_t data;
  Wire.beginTransmission(MS5803_ID);
  Wire.write(register_id);
  Wire.endTransmission(true);

  delay(10);
  Wire.beginTransmission(MS5803_ID);
  Wire.write(0x00);
  Wire.endTransmission(false);
  Wire.requestFrom(MS5803_ID,3,true);
  data = (int32_t)Wire.read()<<16|(int32_t)Wire.read()<<8|(int32_t)Wire.read();
  Wire.endTransmission(true);

  return data;
}

/*******************************************************************************
* Function Name  : ms5803_update
* Description    : Update temperature and pressure
* Input          : address of temperature and pressure variables
* Output         : None.
* Reference      : Datasheet p.7 figure.3
*******************************************************************************/
void ms5803_update(int32_t* TEMP,int32_t* P){
  _D1 = ms5803_read_uint32(MS5803_D1);
  _D2 = ms5803_read_uint32(MS5803_D2);

  _dT = (int32_t)_D2 - (int32_t)_T_REF*256;
  *TEMP = 2000 + _dT*(int32_t)_TEMP_SENS/8388608;

  _OFF = (int64_t)_OFF_T1*65536 + (int64_t)_TCO*(int64_t)_dT/128;
  _SENS = (int64_t)_SENS_T1*32768 + (int64_t)_TCS*(int64_t)_dT/256;
  *P = (int32_t)((int64_t)_D1*_SENS/2097152 - _OFF)/32768;  
}

/*******************************************************************************
* Function Name  : ms5803_depth
* Description    : Change pressure to water depth
* Input          : Water type(fresh water, sea water)
* Output         : water depth [m]
* Reference      : None.
*******************************************************************************/
float ms5803_depth(float water_type, bool temp_pre){
  int32_t TEMP, P;
  float depth, p;

  ms5803_update(&TEMP, &P);

  // smaller than 1atm
  if(P < 10132.5){
    depth = 0;
  }else{
    p = (float)P - 10132.5; 
    p *= 10.0; // [Pa]
    depth = p/water_type/9.81; // [m]
  }
  if(temp_pre){
    Serial.print(TEMP/100);
    Serial.print(":");
    Serial.print(TEMP/100);
    Serial.print(":");
    Serial.print(P);
    Serial.print(":");
    Serial.print(P);
    Serial.print(":");    
    Serial.print(depth*10);
    Serial.print(":");
    Serial.print(depth*10);
    Serial.println();       
  }
  
  return depth;
}
