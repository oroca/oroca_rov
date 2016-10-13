/* 
 * @File: ms5803.h
 * @Brief : MS5803-14BA(Pressure sensor) library
 * @Date : 2016. 05. 13
 * @Version : 0.0
 * @Author : dydwo92@snu.ac.kr
 */

#ifndef __MS5803_H_
#define __MS5803_H_

#define MS5803_RESET 0x1E
#define MS5803_C1 0xA2
#define MS5803_C2 0xA4
#define MS5803_C3 0xA6
#define MS5803_C4 0xA8
#define MS5803_C5 0xAA
#define MS5803_C6 0xAC
#define MS5803_D1 0x48 // OSR = 4096 , resolution 0.2 mbar
#define MS5803_D2 0x58 // OSR = 4096 , resolution 0.002 celcius

#define MS5803_ID  0X76

#define FRESH_WATER	1000.0 // [kg/m^3] 
#define SEA_WATER	1030.0 // [kg/m^3]

extern uint16_t _SENS_T1, _OFF_T1, _TCS, _TCO, _T_REF, _TEMPSENS;
extern uint32_t _D1, _D2;
extern int32_t _dT;
extern int64_t _OFF, _SENS;

void ms5803_reset(void);
void ms5803_init(void);
uint16_t ms5803_read_uint16(int register_id);
uint32_t ms5803_read_uint32(int register_id);
void ms5803_update(int32_t* TEMP, int32_t* P);
float ms5803_depth(float water_type, bool temp_pre);

#endif
