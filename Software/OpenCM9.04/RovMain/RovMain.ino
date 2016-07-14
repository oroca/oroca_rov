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
#include "Define.h"
#include "IMU.h"
#include "LED.h"

RSP     RovSerial;
Servo   RovServo[3]; 
cIMU    IMU;
cLED    LED;

bool       IsConnected;
uint8_t    err_code;

void rc_usb_test();
void Motor_init();
void process_recv_cmd( void );

void send_cmd_info( void );
void recv_cmd_control( RSP_CMD_OBJ *pCmd );
void sout_IMU_info();
void cal_IMU_acc();

void setup() 
{
  Serial.begin(115200);

  RovSerial.begin(115200); 

  Motor_init();

  LED.Led_init();

  err_code = IMU.begin();

  IsConnected = false;
}


void loop() 
{
  static uint32_t tTime[3];

  if( (millis()-tTime[0]) >= 500 )
  {
    tTime[0] = millis();
    LED.toggle();
  }

  IMU.update();
  //-- 명령어 수신 처리 
  //
  process_recv_cmd();

  //-- 100ms마다 ROV정보 전달
  //
  if( (millis()-tTime[1]) >= 100 )
  {
    tTime[1] = millis();

   sout_IMU_info();
   send_cmd_info();
  }
  cal_IMU_acc();


  if(USB_TEST_AVAILABLE)
  rc_usb_test();
  
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
    Led[0] = (LED_OFF - (pCmd->Data[2]<<8) | (pCmd->Data[1]));   
    Serial.print("Led_L : ");
    Serial.println(Led[0]);
    analogWrite(LED_PIN_L, Led[0]);
  }
  //Led_Right
  if(pCmd->Data[3] == 1)
  {
    Led[1] = (LED_OFF - (pCmd->Data[5]<<8) | (pCmd->Data[4]));
    Serial.print("Led_R : ");
    Serial.println(Led[1]);
    analogWrite(LED_PIN_R, Led[1]);
  }
}


void Motor_init()
{
  RovServo[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 1000, 2000, 0, 255 );
  RovServo[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 1000, 2000, 0, 255 );
  RovServo[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 1000, 2000, 0, 255 );

  RovServo[RC_MOTOR_L].writeMicroseconds(MOTOR_NEUTRAL);
  RovServo[RC_MOTOR_C].writeMicroseconds(MOTOR_NEUTRAL);
  RovServo[RC_MOTOR_R].writeMicroseconds(MOTOR_NEUTRAL);
}

void sout_IMU_info()
{
    Serial.print(err_code);
    Serial.print(" ");
    Serial.print(IMU.angle[0]/10);
    Serial.print(" ");
    Serial.print(IMU.angle[1]/10);
    Serial.print(" ");
    Serial.println(IMU.angle[2]);
}

void cal_IMU_acc()
{
 if( Serial.available() )
  {
    char Ch = Serial.read();

    if( Ch == '1' )
    {
      Serial.println("ACC Cali Start");

      IMU.SEN.acc_cali_start();
      while( IMU.SEN.acc_cali_get_done() == false )
      {
        IMU.update();
        //Serial.println( IMU.SEN.calibratingA );
      }

      Serial.print("ACC Cali End ");
    }
  }
}


//---------RCtest(control Motor & LED by using USB serial)---------

void rc_usb_test()
{
  char ch;
  static uint16_t  rc_pwm = MOTOR_NEUTRAL;
  
  if( Serial.available() )
  {
    ch = Serial.read();

    if( ch == 'p' )
    {
      rc_pwm = MOTOR_NEUTRAL;
      RovServo[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      Serial.println("Stop");
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
