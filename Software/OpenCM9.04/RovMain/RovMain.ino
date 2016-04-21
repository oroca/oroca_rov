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
#include <Servo.h>
#include "RSP.h"

RSP     RovSerial;

#define USB_TEST_AVAILABLE 1  // control Motor & LED by using USB serial

#define RC_MOTOR_L    0
#define RC_MOTOR_C    1
#define RC_MOTOR_R    2

#define RC_MOTOR_PIN_L  2
#define RC_MOTOR_PIN_C  3
#define RC_MOTOR_PIN_R  5

#define LED_PIN_L  8 
#define LED_PIN_R  9

bool IsConnected;

void rc_usb_test();
void led_setup();
void process_recv_cmd( void );

void send_cmd_info( void );
void recv_cmd_control( RSP_CMD_OBJ *pCmd );

Servo   RovServo[3];  

void setup() 
{
  Serial.begin(115200);

  RovSerial.begin(115200); 
  RovServo[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 1000, 2000, 0, 255 );
  RovServo[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 1000, 2000, 0, 255 );
  RovServo[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 1000, 2000, 0, 255 );

  RovServo[RC_MOTOR_L].writeMicroseconds(0);
  RovServo[RC_MOTOR_C].writeMicroseconds(0);
  RovServo[RC_MOTOR_R].writeMicroseconds(0);

  pinMode(LED_PIN_L, PWM);
  pinMode(LED_PIN_R, PWM);

  analogWrite(LED_PIN_L, 0);
  analogWrite(LED_PIN_R, 0);

  IsConnected = false;
}


void loop() 
{
  static uint32_t tTime[8];

  //-- 명령어 수신 처리 
  //
  process_recv_cmd();

  if(USB_TEST_AVAILABLE)
  rc_usb_test();

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

  //-- 연결이 끊어진 상태 
  //
/* 
  if( IsConnected == false )
  {
    RovServo[RC_MOTOR_L].write(0);
    RovServo[RC_MOTOR_C].write(0);
    RovServo[RC_MOTOR_R].write(0);    
  }
  */
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
  /*else
  {
    if( (tTime-millis()) >= 1000 )
    {
      IsConnected = false;
    }
    tTime = millis();
  }*/
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
  uint16_t Pwm[3], Led[2];
 
  //-- 모터 제어 
  //Moter_Left
  if(pCmd->Data[6] == 1)
  {
    Pwm[0] = (pCmd->Data[8]<<8) | (pCmd->Data[7]);
    Serial.print("Moter_L : ");
    Serial.println(Pwm[0]);
    RovServo[RC_MOTOR_L].writeMicroseconds(Pwm[0]);
  }
  //Moter_Center
  if(pCmd->Data[9] == 1)
  {
    Pwm[1] = (pCmd->Data[11]<<8) | (pCmd->Data[10]);
    Serial.print("Moter_C : ");
    Serial.println(Pwm[1]);
    RovServo[RC_MOTOR_C].writeMicroseconds(Pwm[1]);
  }
  //Moter_Right
  if(pCmd->Data[12] == 1)
  {
    Pwm[2] = (pCmd->Data[14]<<8) | (pCmd->Data[13]);
    Serial.print("Moter_R : ");
    Serial.println(Pwm[2]);
    RovServo[RC_MOTOR_R].writeMicroseconds(Pwm[2]);
  }
  

  //-- LED 제어 
  //Led_Left
  if(pCmd->Data[0] == 1)
  {
    Led[0] = (pCmd->Data[2]<<8) | (pCmd->Data[1]);
    Serial.print("Led_L : ");
    Serial.println(Led[0]);
    analogWrite(LED_PIN_L, Led[0]);
  }
  //Led_Right
  if(pCmd->Data[3] == 1)
  {
    Led[1] = (pCmd->Data[5]<<8) | (pCmd->Data[4]);
    Serial.print("Led_R : ");
    Serial.println(Led[1]);
    analogWrite(LED_PIN_R, Led[1]);
  }
}


//---------RCtest(control Motor & LED by using USB serial)---------

void rc_usb_test()
{
  char ch;
  static uint16_t  rc_pwm = 1000;
  
  if( Serial.available() )
  {
    ch = Serial.read();

    if( ch == 'p' )
    {
      rc_pwm = 1000;
      RovServo[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      Serial.println("Pwm = 0");
    }

    if( ch == 'w' )
    {
      rc_pwm = constrain(rc_pwm++, 1000, 2000);
      RovServo[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      Serial.println(rc_pwm);
    }

    if( ch == 's' )
    {
      rc_pwm = constrain(rc_pwm--, 1000, 2000);
      RovServo[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      Serial.println(rc_pwm);
    }
  }
}
