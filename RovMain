//----------------------------------------------------------------------------
//    프로그램명   : RovMain
//
//    만든이       :  
//
//    날  짜     : 
//    
//    최종 수정   : 
//
//    MPU_Type  : 
//
//    파일명       : RovMain.ino
//----------------------------------------------------------------------------

#include "Command.h"

Command RovCommand;

void setup() 
{
  Serial.begin(115200);
  RovCommand.RovSerial_begin(115200);

  RovCommand.RovMotor_init(RC_MOTOR_PIN_L, RC_MOTOR_L);
  RovCommand.RovMotor_init(RC_MOTOR_PIN_C, RC_MOTOR_C);
  RovCommand.RovMotor_init(RC_MOTOR_PIN_R, RC_MOTOR_R);
  led_setup();
  
  IsConnected = false;
}

void loop() 
{
  static uint32_t tTime[8];

  //-- 명령어 수신 처리 
  RovCommand.process_recv_cmd();

  //-- 100ms마다 ROV정보 전달
  if( (tTime[0] - millis()) >= 100 )
  {
    tTime[0] = millis();

    RovCommand.send_cmd_info();

    if( IsConnected == true )
    {
      Serial.println("Connected");
    }
  }

  //-- 연결이 끊어진 상태 
/* 
  if( IsConnected == false )
  {
    RovServo[RC_MOTOR_L].writeMicrosecond(MOTOR_NEUTRAL);
    RovServo[RC_MOTOR_C].writeMicrosecond(MOTOR_NEUTRAL);
    RovServo[RC_MOTOR_R].writeMicrosecond(MOTOR_NEUTRAL);    
  }
  */
}
