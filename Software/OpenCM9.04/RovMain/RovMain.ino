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
Servo   RovMotor[3]; 
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
  Serial.begin(115200);  //USB시리얼 통신 초기화

  RovSerial.begin(115200); //UART1 시리얼 통신 초기화

  Motor_init(); //모터 초기화

  LED.Led_init(); //LED초기화

  err_code = IMU.begin(); // IMU 에러여부 확인 리턴 변수

  IsConnected = false;
}


void loop() 
{
  static uint32_t tTime[3];

  //-- 500ms마다 LED 토글
  if( (millis()-tTime[0]) >= 500 )
  {
    tTime[0] = millis();
    LED.toggle();
  }

  //-- IMU 갱신
  IMU.update();
  
  //-- 명령어 수신 처리 
  process_recv_cmd();

  //-- 100ms마다 ROV정보 전달
  if( (millis()-tTime[1]) >= 100 )
  {
    tTime[1] = millis();

   sout_IMU_info(); //IMU 정보 USB시리얼창에 출력
   send_cmd_info(); //커맨드 정보 USB시리얼창에 출력
  }
  cal_IMU_acc();


  if(USB_TEST_AVAILABLE)  //USB시리얼로 테스트를 한다면,
  rc_usb_test();
  
  //-- 연결이 끊어진 상태 
/************************************ 
  if( IsConnected == false )
  {
    RovMotor[RC_MOTOR_L].writeMicroseconds(MOTOR_NEUTRAL);
    RovMotor[RC_MOTOR_C].writeMicroseconds(MOTOR_NEUTRAL);
    RovMotor[RC_MOTOR_R].writeMicroseconds(MOTOR_NEUTRAL);    
  }
 ************************************/
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

  // HeartBit
  /**********************************
  else
  {
    if( (tTime-millis()) >= 1000 )
    {
      IsConnected = false;
    }
    tTime = millis();
  }
  ***********************************/
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
    RovMotor[RC_MOTOR_L].writeMicroseconds(Pwm[0]);
  }
  //Moter_Center
  if(pCmd->Data[9] == 1)
  {
    Pwm[1] = (pCmd->Data[11]<<8) | (pCmd->Data[10]);
    Serial.print("Moter_C : ");
    Serial.println(Pwm[1]);
    RovMotor[RC_MOTOR_C].writeMicroseconds(Pwm[1]);
  }
  //Moter_Right
  if(pCmd->Data[12] == 1)
  {
    Pwm[2] = (pCmd->Data[14]<<8) | (pCmd->Data[13]);
    Serial.print("Moter_R : ");
    Serial.println(Pwm[2]);
    RovMotor[RC_MOTOR_R].writeMicroseconds(Pwm[2]);
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
/*********************************************
  //Led_Right
  if(pCmd->Data[3] == 1)
  {
    Led[1] = (LED_OFF - (pCmd->Data[5]<<8) | (pCmd->Data[4]));
    Serial.print("Led_R : ");
    Serial.println(Led[1]);
    analogWrite(LED_PIN_R, Led[1]);
  }
**********************************************/
}


void Motor_init()
{
  RovMotor[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 1000, 2000, 0, 255 );
  RovMotor[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 1000, 2000, 0, 255 );
  RovMotor[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 1000, 2000, 0, 255 );

  RovMotor[RC_MOTOR_L].writeMicroseconds(MOTOR_NEUTRAL);
  RovMotor[RC_MOTOR_C].writeMicroseconds(MOTOR_NEUTRAL);
  RovMotor[RC_MOTOR_R].writeMicroseconds(MOTOR_NEUTRAL);
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

    if( ch =='p' )     // 모터 정지
    {
      rc_pwm = MOTOR_NEUTRAL;
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println("Stop");
    }

    if( ch == 'w' )     // 모터 가속
    {
      rc_pwm = constrain(rc_pwm++, 1000, 2000);
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println(rc_pwm);
    }

    if( ch == 's' )     // 모터 감속
    {
      rc_pwm = constrain(rc_pwm--, 1000, 2000);
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println(rc_pwm);
    }
    
    if( ch == 'q' )  // LED OFF
    {
      digitalWrite(LED_PIN_L, 1);
      Serial.println("                                               usb_LED : OFF");
    }
    if( ch == 'e' )   // LED ON
    {
      digitalWrite(LED_PIN_L, 0);
      Serial.println("                                               usb_LED : ON");
    }
    
    
  }
}
