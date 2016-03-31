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
#include "RSP.h"
#include <Servo.h> 
 

#define USE_RC_SETUP  1


#define RC_MOTOR_L    0
#define RC_MOTOR_C    1
#define RC_MOTOR_R    2

#define RC_MOTOR_PIN_L  2
#define RC_MOTOR_PIN_C  3
#define RC_MOTOR_PIN_R  5

RSP     RovSerial;
Servo   RovServo[3];  

bool IsConnected;

void rc_setup( void );
void process_recv_cmd( void );

void send_cmd_info( void );
void recv_cmd_control( RSP_CMD_OBJ *pCmd );

void setup() 
{
  Serial.begin(115200);

  RovSerial.begin(115200); 
  RovServo[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 0, 2000, 0, 255 );
  RovServo[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 0, 2000, 0, 255 );
  RovServo[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 0, 2000, 0, 255 );

  RovServo[RC_MOTOR_L].write(0);
  RovServo[RC_MOTOR_C].write(0);
  RovServo[RC_MOTOR_R].write(0);


  IsConnected = false;
}



void loop() 
{
  static uint32_t tTime[8];


  //-- 명령어 수신 처리 
  //
  process_recv_cmd();


  #if USE_RC_SETUP ==  1
  rc_setup();
  #endif


  //-- 100ms마다 ROV정보 전달
  //
  if( (tTime[0] - millis()) >= 100 )
  {
    tTime[0] = millis();

    send_cmd_info();

    if( IsConnected == true )
    {
      //Serial.println("Connected");
    }
  }

/*
  //-- 연결이 끊어진 상태 
  //
  if( IsConnected == false )
  {
    RovServo[RC_MOTOR_L].write(0);
    RovServo[RC_MOTOR_C].write(0);
    RovServo[RC_MOTOR_R].write(0);    
  }
*/
 
}



void rc_setup( void )
{
  volatile char ch;
  static int  rc_pwm_L = 100, rc_pwm_R = 100, rc_pwm_T = 50;

  if( Serial.available() )
  {
    ch = Serial.read();
    //시동 
    if( ch == 'o' )
    {
      RovServo[0].write(rc_pwm_L);
      RovServo[1].write(rc_pwm_R);
      RovServo[2].write(rc_pwm_T);
      Serial.println("Start");
    }
    //정지 (시동끄기)
    if( ch == 'p' )
    {
      RovServo[0].write(0);
      RovServo[1].write(0);
      RovServo[2].write(0);
      Serial.println("Stop");
    }
    //전진 모터 가속
    if( ch == 'w' )
    {
      rc_pwm_L = constrain(rc_pwm_L++, 0, 2000);
      rc_pwm_R = constrain(rc_pwm_R++, 0, 2000);
      RovServo[0].write(rc_pwm_L);
      RovServo[2].write(rc_pwm_R);
      Serial.println("Left=");
      Serial.println(rc_pwm_L);
      Serial.println("Right=");
      Serial.println(rc_pwm_R);
    }
    //전진 모터 감속
    if( ch == 's' )
    {
      rc_pwm_L = constrain(rc_pwm_L--, 0, 2000);
      rc_pwm_R = constrain(rc_pwm_R--, 0, 2000);
      RovServo[0].write(rc_pwm_L);
      RovServo[2].write(rc_pwm_R);
      Serial.println("Left=");
      Serial.println(rc_pwm_L);
      Serial.println("Right=");
      Serial.println(rc_pwm_R);
    }
    //좌회전
    if( ch == 'a' )
    {
      rc_pwm_L = constrain(rc_pwm_L--, 0, 2000);      
      rc_pwm_R = constrain(rc_pwm_R++, 0, 2000);
      RovServo[0].write(rc_pwm_L);    //좌측모터 출력 감소
      RovServo[2].write(rc_pwm_R);    //우측모터 출력 증가
      Serial.println("Left=");
      Serial.println(rc_pwm_L);
      Serial.println("Right=");
      Serial.println(rc_pwm_R);
    }
    //우회전
    if( ch == 'd' )
    {
      rc_pwm_L = constrain(rc_pwm_L++, 0, 2000);
      rc_pwm_R = constrain(rc_pwm_R--, 0, 2000);
      RovServo[0].write(rc_pwm_L);   // 좌측모터 출력 증가
      RovServo[2].write(rc_pwm_R);    //우측모터 출력 감소
      Serial.println("Left=");
      Serial.println(rc_pwm_L);
      Serial.println("Right=");
      Serial.println(rc_pwm_R);
    }
    //상단모터 가속
    if( ch == 'q' )
    {
      rc_pwm_T = constrain(rc_pwm_T++, 0, 2000);
      RovServo[1].write(rc_pwm_T);  
      Serial.println("Top=");
      Serial.println(rc_pwm_T);
    }
    //상단모터 감속
    if( ch == 'e' )
    {
      rc_pwm_T = constrain(rc_pwm_T--, 0, 2000);
      RovServo[1].write(rc_pwm_T);   
      Serial.println("Top=");
      Serial.println(rc_pwm_T);
    }
  }

}

void process_recv_cmd( void )
{
  bool Ret;
  uint8_t i;
  RSP_CMD_OBJ    *pCmd;
  static uint32_t tTime;


  Ret = RovSerial.update();

  if( Ret == true )
  {
        pCmd  = RovSerial.Get_CmdPtr(); 

    /*  
    Serial.print("Cmd : ");
    Serial.print( pCmd->Cmd, HEX);
    Serial.print(" Size : ");
    Serial.print( pCmd->Length, DEC);
    Serial.print(" ");
    for( i=0; i<pCmd->Length; i++ )
    {
      Serial.print(pCmd->Data[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    */

    switch( pCmd->Cmd )
    {
      case 0x01:
        IsConnected = true;
        recv_cmd_control( pCmd );
        break;

      case 0xFE:
        IsConnected = true;
        break;
    } 
  }
  else
  {
    if( (tTime-millis()) >= 1000 )
    {
      IsConnected = false;
    }
    tTime = millis();
  }
}



void send_cmd_info( void )
{
  RSP_CMD_OBJ Cmd;

  Cmd.Cmd     = 0xFF;
  Cmd.Length  = 1;
  Cmd.Data[0] = 100;

  RovSerial.SendCmd( &Cmd );
}



void recv_cmd_control( RSP_CMD_OBJ *pCmd )
{
  uint16_t Pwm[3];

  //-- 모터 제어 
  //
  Pwm[0] = (pCmd->Data[8]<<8) | (pCmd->Data[7]);
  Serial.print("RC : ");
  Serial.println(Pwm[0]);
  RovServo[RC_MOTOR_L].write(Pwm[0]);


  //-- LED 제어 
  //
}
