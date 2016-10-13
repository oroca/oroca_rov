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
#include "ROV_LED.h"
#include "Define.h"
#include "mpu.h"
#include "MS5540S.h"

Servo   RovMotor[3];
RSP RSP;
MPU6050 mpu;

float water_depth;

void setup() 
{
  Serial.begin(115200);
  RSP.begin(115200);
  //while (!Serial);
  
  RovMotor_init();
  Led_setup();
  //dmpDataReady();
  //dmp_setup();
 // ms5540s_setup();
 
  IsConnected = false;
}

void loop() 
{
  static uint32_t tTime[4];
 
  process_recv_cmd();
  
  //-- 100ms마다 ROV정보 전달
  if( (millis() - tTime[1]) >= 100 )
  {
    tTime[1] = millis();
    //send_cmd_info();
   // send_rov_info();
  }
  
  //-- 200ms마다 IMU정보 전달
  if( (millis() - tTime[2]) >= 200 )
  {
    tTime[2] = millis();
    //dmp_loop();
  }

   //-- 201ms마다 Pressure정보 전달
  if( (millis() - tTime[3]) >= 201 )
  {
    tTime[3] = millis();
  //  ms5540s_loop();
  }
  
 if(USB_TEST_AVAILABLE) rc_usb_test();

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




////////////////////////////////////////////
// FUNTION
////////////////////////////////////////////
/*---------------------------------------------------------------------------
     TITLE   : process_recv_cmd
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void process_recv_cmd( void )
{
  uint32_t tTime;
  RSP_CMD_OBJ  *pCmd;
  bool Ret;
  
  Ret = RSP.update();

  if( Ret == true )
  {
        pCmd  = RSP.Get_CmdPtr(); 

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

/*---------------------------------------------------------------------------
     TITLE   : send_cmd_info
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void send_cmd_info( void )
{
  RSP_CMD_OBJ  Cmd;
  
  Cmd.Cmd     = 0xFF;
  Cmd.Length  = 1;
  Cmd.Data[0] = 100;

  RSP.SendCmd( &Cmd );
}

/*---------------------------------------------------------------------------
     TITLE   : send_rov_info
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void send_rov_info( void )
{
  RSP_CMD_OBJ  Cmd;
  
  Cmd.Cmd     = 0x81;
  Cmd.Length  = 16;
  Cmd.Data[0] = (char)(*((unsigned long*)&roll) >> 24);
  Cmd.Data[1] = (char)(*((unsigned long*)&roll) >> 16);
  Cmd.Data[2] = (char)(*((unsigned long*)&roll) >> 8);
  Cmd.Data[3] = (char)(*((unsigned long*)&roll));
  Cmd.Data[4] = (char)(*((unsigned long*)&pitch) >> 24);
  Cmd.Data[5] = (char)(*((unsigned long*)&pitch) >> 16);
  Cmd.Data[6] = (char)(*((unsigned long*)&pitch) >> 8);
  Cmd.Data[7] = (char)(*((unsigned long*)&pitch));
  Cmd.Data[8] = (char)(*((unsigned long*)&yaw) >> 24);
  Cmd.Data[9] = (char)(*((unsigned long*)&yaw) >> 16);
  Cmd.Data[10] = (char)(*((unsigned long*)&yaw) >> 8);
  Cmd.Data[11] = (char)(*((unsigned long*)&yaw));
  Cmd.Data[12] = (char)(*((unsigned long*)&water_depth) >> 24);
  Cmd.Data[13] = (char)(*((unsigned long*)&water_depth) >> 16);
  Cmd.Data[14] = (char)(*((unsigned long*)&water_depth) >> 8);
  Cmd.Data[15] = (char)(*((unsigned long*)&water_depth));
    
  RSP.SendCmd( &Cmd );
}



/*---------------------------------------------------------------------------
     TITLE   : recv_cmd_control
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void recv_cmd_control( RSP_CMD_OBJ *pCmd )
{
  //-- 모터 제어 
  //Moter_Left
  if(pCmd->Data[6] == 1)
  {
    Motor_pwm[RC_MOTOR_L] = ((uint16_t)(pCmd->Data[8]<<8) & 0xff00) | ((uint16_t)(pCmd->Data[7]) & 0xff);
    Serial.print("Moter_L : ");
    Serial.println(Motor_pwm[RC_MOTOR_L]);
    RovMotor[RC_MOTOR_L].writeMicroseconds(Motor_pwm[RC_MOTOR_L]);
  }
  //Moter_Center
  if(pCmd->Data[9] == 1)
  {
    Motor_pwm[RC_MOTOR_C] = ((uint16_t)(pCmd->Data[11]<<8) & 0xff00) | ((uint16_t)(pCmd->Data[10]) & 0xff);
    Serial.print("Moter_C : ");
    Serial.println(Motor_pwm[RC_MOTOR_C]);
    RovMotor[RC_MOTOR_C].writeMicroseconds(Motor_pwm[RC_MOTOR_C]);
  }
  //Moter_Right
  if(pCmd->Data[12] == 1)
  {
    Motor_pwm[RC_MOTOR_R] = ((uint16_t)(pCmd->Data[14]<<8) & 0xff00) | ((uint16_t)(pCmd->Data[13]) & 0xff);
    Serial.print("Moter_R : ");
    Serial.println(Motor_pwm[RC_MOTOR_R]);
    RovMotor[RC_MOTOR_R].writeMicroseconds(Motor_pwm[RC_MOTOR_R]);
  }

  //-- LED 제어 
  //Led_Left
  if(pCmd->Data[0] == 1)
  {
    LED_pwm[LED_L] = ((uint16_t)(pCmd->Data[2]<<8) & 0xff00) | ((uint16_t)(pCmd->Data[1]) & 0xff);  
    Serial.print("Led_L : ");
    Serial.println(LED_pwm[LED_L]);
    digitalWrite(LED_PIN_L, LED_pwm[LED_L]);
  }
 /*
  //Led_Right
  if(pCmd->Data[3] == 1)
  {
    LED_pwm[LED_R] = (LED_OFF - (pCmd->Data[5]<<8) | (pCmd->Data[4]));
    Serial.print("Led_R : ");
    Serial.println(LED_pwm[LED_R]);
    analogWrite(LED_PIN_R, LED_pwm[LED_R]);
  }
  */
}

/*---------------------------------------------------------------------------
     TITLE   : RovMotor_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void RovMotor_init()
{
   RovMotor[RC_MOTOR_L].attach(RC_MOTOR_PIN_L, 1000, 2000);
   RovMotor[RC_MOTOR_L].writeMicroseconds(MOTOR_NEUTRAL);
   RovMotor[RC_MOTOR_C].attach(RC_MOTOR_PIN_C, 1000, 2000);
   RovMotor[RC_MOTOR_C].writeMicroseconds(MOTOR_NEUTRAL);
   RovMotor[RC_MOTOR_R].attach(RC_MOTOR_PIN_R, 1000, 2000);
   RovMotor[RC_MOTOR_R].writeMicroseconds(MOTOR_NEUTRAL);   
   Serial.println("Motor setup");
}


////////////////////////////////////////
//        IMU
/////////////////////////////////////////
/*---------------------------------------------------------------------------
     TITLE   : dmpDataReady
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void dmpDataReady() {
    mpuInterrupt = true;
}

/*---------------------------------------------------------------------------
     TITLE   : dmp_setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void dmp_setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
 
    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
  //  while (Serial.available() && Serial.read()); // empty buffer
  //  while (!Serial.available());                 // wait for data
  //  while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

/*---------------------------------------------------------------------------
     TITLE   : dmp_loop
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void dmp_loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
      //  Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

    
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            // store roll, pitch, yaw
            yaw = ypr[0] * 180/M_PI;
            roll = ypr[1] * 180/M_PI;
            pitch = ypr[2] * 180/M_PI;
                        
            Serial.print("  roll : ");
            Serial.print(roll);
            Serial.print("  pitch : ");
            Serial.print(pitch);
            Serial.print("  yaw : ");
            Serial.println(yaw);

    }
}


////////////////////////////////////////////
// TEST using only USB Serial
////////////////////////////////////////////
/*---------------------------------------------------------------------------
     TITLE   : rc_usb_test
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void rc_usb_test()
{
  char ch;
  static uint16_t  rc_pwm = MOTOR_NEUTRAL;
  static uint16_t  rc_led = 0;
 
  
  
  if( Serial.available() )
  {
    ch = Serial.read();

    if( ch =='p' )
    {
      rc_pwm = MOTOR_NEUTRAL;
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println("Stop");
    }

    if( ch == 'w' )
    {
      rc_pwm = constrain(rc_pwm++, 1000, 2000);
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println(rc_pwm);
    }

    if( ch == 's' )
    {
      rc_pwm = constrain(rc_pwm--, 1000, 2000);
      RovMotor[RC_MOTOR_C].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_L].writeMicroseconds(rc_pwm);
      RovMotor[RC_MOTOR_R].writeMicroseconds(rc_pwm);
      Serial.print("                                               usb_motor : ");
      Serial.println(rc_pwm);
    }

   if( ch == 'q' )
   {
      digitalWrite(LED_PIN_L, 1);
      Serial.println("                                               usb_LED : OFF");
   }
    if( ch == 'e' )
   {
      digitalWrite(LED_PIN_L, 0);
      Serial.println("                                               usb_LED : ON");
   }
  }    
}


