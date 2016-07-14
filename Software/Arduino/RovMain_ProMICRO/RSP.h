//----------------------------------------------------------------------------
//    프로그램명 	: RSP(Rov Serial Protocol)
//
//    만든이     	:  
//
//    날  짜     : 
//    
//    최종 수정  	: 
//
//    MPU_Type	: 
//
//    파일명     	: RSP.h
//----------------------------------------------------------------------------
#ifndef _RSP_H_
#define _RSP_H_

#include <inttypes.h>
#include <Arduino.h> 

#define RSP_SERIAL	Serial1

#define RSP_CMD_MAX_LENGTH			128

typedef struct 
{
	uint8_t	Cmd;
	uint8_t	Length;
	uint8_t CheckSum;
	uint8_t CheckSumRecv;	
	uint8_t	Data[RSP_CMD_MAX_LENGTH];	
} RSP_CMD_OBJ;

typedef struct 
{
	uint8_t	Cmd;
	uint8_t	Length;
	uint8_t CheckSum;
	uint8_t	Data[RSP_CMD_MAX_LENGTH];	
} RSP_RESP_OBJ;

class RSP
{
public:
	RSP();

	RSP_CMD_OBJ		Cmd;
	RSP_RESP_OBJ	Resp;

	void begin( uint32_t Baud );
	bool update( void );

	RSP_CMD_OBJ  *Get_CmdPtr( void );
	RSP_RESP_OBJ *Get_RespPtr( void );

	void SendCmd( RSP_CMD_OBJ *pCmd );
	void SendResp( RSP_RESP_OBJ *pResp );

private:
	uint8_t  Cmd_State;
	uint8_t  Index_Data;

	uint32_t CurrentTime;
	uint32_t PreviousTime;
};

#endif
