//----------------------------------------------------------------------------
//    프로그램명 	: MS5540S
//
//    만든이     	:  
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: MS5540S.h
//----------------------------------------------------------------------------
#ifndef _MS5540S_H_
#define _MS5540S_H_

#include <inttypes.h>
#include <SPI.h>

void ms5540s_reset();
void ms5540s_setup();
void ms5540s_loop();

//extern static long PCOMP;


#endif
