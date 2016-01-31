/*************************************************************************
* File Name          : Makeblock Ultrasonic.ino
* Author             : Jasen
* Updated            : Jasen
* Version            : V1.0.0
* Date               : 11/6/2013
* Description        : Demo code for Makeblock Starter Robot kit,two motors
                       connect on the M1 and M2 port of baseshield, the Ultrasonic sensor
                       connect on port 3.
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <Makeblock.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

/**
 * Moving Avaerage with integers
 */
class MovingAvg
{
private:
  int _Data[4];
  int _Cnt;
  int _Sum;

  const int SAMPLES = 4;
  
public:
  MovingAvg( );
  int Update( int );
};

MovingAvg::MovingAvg()
{
  memset( _Data, 0, sizeof(_Data) );
  _Cnt = 0;
  _Sum = 0;
}

int MovingAvg::Update( int Value )
{
  int Avg;
  
  _Sum = _Sum - _Data[_Cnt] + Value;
  _Data[_Cnt] = Value;
  
  _Cnt ++;
  if( _Cnt >= SAMPLES )
  {
    _Cnt = 0;
  } 

  if( _Sum < 0 )
  {
    uint16_t Sum = -_Sum;
    Avg = Sum >> 2;
  }
  else
  {
    Avg = _Sum >> 2;
  }

  return Avg;
}
/******************************************************************************/

typedef enum
{
  stNormal,
  stBarrier,
} MovingState_t;

typedef enum
{
  opAuto,
  opRemote
} OperationMode_t;

MeDCMotor MotorL(M1);  
MeDCMotor MotorR(M2);
MeUltrasonicSensor UltrasonicSensor(PORT_3);
/*
Blue module can only be connected to port 3, 4, 5, 6 of base shield.
*/
MeBluetooth bluetooth(PORT_4);

int moveSpeed = 190;
int turnSpeed = 200;
MovingAvg distance;
MovingState_t State;
OperationMode_t Mode;
int randnum = 0;
boolean leftflag,rightflag;

int ForwardWithZeroDist = 0;


void setup()
{
    leftflag=false;
    rightflag=false;
    State = stNormal;
    Mode = opAuto;
    randomSeed(analogRead(0));
    Serial.begin(9600);
    bluetooth.begin(9600);
    Serial.print("UltraSonic Car\n");
}

void loop()
{
  int Distance;
  Distance = distance.Update( UltrasonicSensor.distanceCm() );
  Serial.println(Distance);

  if(bluetooth.available())
  {
    char Buf[32];
    char inDat = bluetooth.read();
    switch( inDat )
    {
      case 'A':
        Mode = opAuto;
        break;
      case 'R':
        Mode = opRemote;
        break;
        
      case 's':
        if (Mode == opRemote) Stop();
        break;
      case 'l':
        if (Mode == opRemote) TurnLeft();
        break;
      case 'r':
        if (Mode == opRemote) TurnRight();
        break;
      case 'f':
        if (Mode == opRemote) Forward();
        break;
      case 'b':
        if (Mode == opRemote) Backward();
        break;
    }
    Serial.println(inDat);

    sprintf( Buf, "%d cm\n", Distance );
    bluetooth.print( Buf);
  }

  if( Mode == opAuto )
  {
    
    if(Distance>2 && Distance<40)
  {
    randnum=random(300);
    if(randnum > 150 && !rightflag)
    {
      Serial.println( "left" );
      leftflag=true;
      TurnLeft();
    }
    else
    {
      Serial.println( "right" );
      rightflag=true;
      TurnRight();  
    }
  }
  else
  {  
    leftflag=false;
    rightflag=false;
    switch( State )
    {
      case stNormal:
        if( Distance == 0 )
        {
          ForwardWithZeroDist++;
        }
        
        if(ForwardWithZeroDist >= 3)
        {
          State = stBarrier;
        }
        else
        {
          Serial.println( "forward" );
          Forward();
        }
        break;
        
      case stBarrier:
        ForwardWithZeroDist--;
        if( ForwardWithZeroDist )
        {
          Serial.println( "backward" );
          Backward();
        }
        else
        {
          State = stNormal;
          Serial.println( "right" );
          rightflag=true;
          TurnRight();
        }
        break;
    }     
  }
  }
  delay(150);
}

void Forward()
{
  MotorL.run(moveSpeed);
  MotorR.run(moveSpeed);
}
void Backward()
{
  MotorL.run(-moveSpeed);
  MotorR.run(-moveSpeed);
}

void Stop()
{
  MotorL.run(0);
  MotorR.run(0);
}

void TurnLeft()
{
  MotorL.run(turnSpeed);
  MotorR.run(-turnSpeed);
}
void TurnRight()
{
  MotorL.run(-turnSpeed);
  MotorR.run(turnSpeed);
}
