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
//    파일명     	: RSP.cpp
//----------------------------------------------------------------------------
#include <Arduino.h> 

#include "RSP.h"

#define RSP_CMD_STX							0x02
#define RSP_CMD_ETX							0x03

#define RSP_CMD_STATE_WAIT_STX				0
#define RSP_CMD_STATE_WAIT_CMD				1
#define RSP_CMD_STATE_WAIT_SIZE				2
#define RSP_CMD_STATE_WAIT_DATA				3
#define RSP_CMD_STATE_WAIT_CHECKSUM			4
#define RSP_CMD_STATE_WAIT_ETX				5

// #define _USE_DEBUG	

/*---------------------------------------------------------------------------
     TITLE   : RSP
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
RSP::RSP()
{
	Cmd_State = RSP_CMD_STATE_WAIT_STX;
}

/*---------------------------------------------------------------------------
     TITLE   : begin
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void RSP::begin( uint32_t Baud )
{
	RSP_SERIAL.begin(Baud);	
}

/*---------------------------------------------------------------------------
     TITLE   : update
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
bool RSP::update( void )
{
	bool    Ret = false;
	uint8_t ch;

	//-- 명령어 수신
	//
	if( RSP_SERIAL.available() )
	{
		ch = RSP_SERIAL.read();
		#ifdef _USE_DEBUG
		Serial.println(ch, HEX);
		#endif
	}
	else
	{
		return false;
	}


	//-- 바이트간 타임아웃 설정(50ms)
	//
	CurrentTime = millis();

	if( (CurrentTime - PreviousTime) > 1000 )
	{
		Cmd_State    = RSP_CMD_STATE_WAIT_STX;
		PreviousTime = CurrentTime;
	}	

	//-- 명령어 상태
	//
	switch( Cmd_State )
	{

		//-- STX 문자 기다리는 상태 
		//
		case RSP_CMD_STATE_WAIT_STX:

			// 시작 문자를 기다림
			if( ch == RSP_CMD_STX )
			{
				Cmd_State    = RSP_CMD_STATE_WAIT_CMD;
				Cmd.CheckSum = 0x00;
				Cmd.Length   = 0;		
				//Serial.println("STX");		
			}
			break;

		//-- 명령어 기다리는 상태 
		//
		case RSP_CMD_STATE_WAIT_CMD:
			Cmd.Cmd       = ch;
			Cmd.CheckSum ^= ch;
			Cmd_State = RSP_CMD_STATE_WAIT_SIZE;
			//Serial.println("CMD");
			break;

		//-- 데이터 사이즈 기다리는 상태(128까지)
		//
		case RSP_CMD_STATE_WAIT_SIZE:
			//Serial.println("SIZE");
			if( ch <= RSP_CMD_MAX_LENGTH )
			{
				Cmd.Length    = ch;
				Index_Data    = 0;
				Cmd.CheckSum ^= ch;
				if( Cmd.Length > 0 )
				{
					Cmd_State = RSP_CMD_STATE_WAIT_DATA;
				}
				else
				{
					Cmd_State = RSP_CMD_STATE_WAIT_CHECKSUM;	
				}
			}
			else
			{
				Cmd_State = RSP_CMD_STATE_WAIT_STX;	
			}
			break;

		//-- 데이터를 기다리는 상태
		//
		case RSP_CMD_STATE_WAIT_DATA:
			
			//Serial.println("DATA");

			Cmd.CheckSum          ^= ch;
			Cmd.Data[ Index_Data ] = ch;

			Index_Data++;

			if( Index_Data >= Cmd.Length )
			{
				Cmd_State     = RSP_CMD_STATE_WAIT_CHECKSUM;
			} 
			break;

		//-- 체크섬을 기다리는 상태
		//
		case RSP_CMD_STATE_WAIT_CHECKSUM:

			//Serial.println("CHECKSUM");

			Cmd.CheckSumRecv = ch;
			Cmd_State        = RSP_CMD_STATE_WAIT_ETX;		
			break;

		//-- ETX 기다리는 상태
		//
		case RSP_CMD_STATE_WAIT_ETX:

			//Serial.println("ETX");
			#ifdef _USE_DEBUG
			Serial.print(Cmd.CheckSumRecv);
			Serial.print(" ");
			Serial.println(Cmd.CheckSum);
			#endif

			if( ch == RSP_CMD_ETX )
			{				
				if( Cmd.CheckSumRecv == Cmd.CheckSum )
				{
					Ret = true;
				}
			}
			Cmd_State = RSP_CMD_STATE_WAIT_STX;		
			break;
	}

	return Ret;
}


/*---------------------------------------------------------------------------
     TITLE   : Get_CmdPtr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
RSP_CMD_OBJ *RSP::Get_CmdPtr( void )
{
	return &Cmd;
}

/*---------------------------------------------------------------------------
     TITLE   : Get_RespPtr
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
RSP_RESP_OBJ *RSP::Get_RespPtr( void )
{
	return &Resp;
}

/*---------------------------------------------------------------------------
     TITLE   : SendReap
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void RSP::SendResp( RSP_RESP_OBJ *pResp )
{
	uint8_t i;
	uint8_t CheckSum = 0;

/*
	RSP_SERIAL.write( RSP_CMD_STX );

	RSP_SERIAL.write( pResp->Cmd );			CheckSum ^= pResp->Cmd;
	RSP_SERIAL.write( pResp->Length );		CheckSum ^= pResp->Length;

	for( i=0; i<pResp->Length; i++ )
	{
		RSP_SERIAL.write( pResp->Data[i] );
		CheckSum ^= pResp->Data[i];		
	}

	RSP_SERIAL.write( CheckSum );
	RSP_SERIAL.write( RSP_CMD_ETX );
  */
}





/*---------------------------------------------------------------------------
     TITLE   : SendCmd
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void RSP::SendCmd( RSP_CMD_OBJ *pCmd )
{
	uint8_t i;
	uint8_t CheckSum = 0;


	RSP_SERIAL.write( RSP_CMD_STX );	
	RSP_SERIAL.write( pCmd->Cmd );			CheckSum ^= pCmd->Cmd;
	RSP_SERIAL.write( pCmd->Length );		CheckSum ^= pCmd->Length;

	for( i=0; i<pCmd->Length; i++ )
	{
		RSP_SERIAL.write( pCmd->Data[i] );
		CheckSum ^= pCmd->Data[i];		
	}

	RSP_SERIAL.write( CheckSum );
	RSP_SERIAL.write( RSP_CMD_ETX );
}
