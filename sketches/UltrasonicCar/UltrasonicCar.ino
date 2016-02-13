/**
   \file   UltrasonicCar.ino
   \brief  Makeblock car with sonic distance measurement

   This file was based on the original Makeblock UltrasonicCar
   test but is enhanced with other features now.
*/

#include <MeOrion.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define _TASK_SLEEP_ON_IDLE_RUN 1
#include <TaskScheduler.h>


/**
   Moving Avaerage with integers
*/
class MovingAvg
{
  private:
    int _Data[16];
    int _Cnt;
    int _Sum;

    const int SAMPLES = 16;

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
  if ( _Cnt >= SAMPLES )
  {
    _Cnt = 0;
  }

  if ( _Sum < 0 )
  {
    uint16_t Sum = -_Sum;
    Avg = Sum >> 4;
    Avg = -Avg;
  }
  else
  {
    Avg = _Sum >> 4;
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

/*** local defined functions *******************************************************/
void ReadSensors( void );
void Control( void );
void Remote( void );
const char *FormatState( int );

/*** variabled *********************************************************************/

MeDCMotor          MotorL(M2);
MeDCMotor          MotorR(M1);
MeUltrasonicSensor UltrasonicSensor(PORT_3);
MeBluetooth        bluetooth(PORT_4);

int              moveSpeed = 190;
int              turnSpeed = 200;
MovingAvg        DistAvg;
int              Distance;
MovingState_t    State;
OperationMode_t  Mode;
int              randnum = 0;
boolean          leftflag, rightflag;

int ForwardWithZeroDist = 0;

Scheduler Runner;
Task      TaskSensor(    10, TASK_FOREVER, ReadSensors, &Runner, true );
Task      TaskControl(  100, TASK_FOREVER, Control,     &Runner, true );
Task      TaskRemote(   200, TASK_FOREVER, Remote,      &Runner, true );

/*** setup *************************************************************/
void setup()
{
  leftflag = false;
  rightflag = false;
  State = stNormal;
  Mode = opAuto;
  randomSeed(analogRead(0));
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.print("UltraSonic Car 1.0\n");
}

/*** loop *************************************************************/
void loop()
{
  Runner.execute();
}

/*** ReadSensors *************************************************************/
void ReadSensors( void )
{
  Distance = DistAvg.Update( UltrasonicSensor.distanceCm() );
}

/*** Control ****************************************************************/
void Control( void )
{
  static int LastState = -1;
  if ( Mode == opAuto )
  {
    char Buf[16];

    sprintf( Buf, "%d cm\n", Distance );
    Serial.print( Buf );
    if ( Distance < 2 && Distance > 80 )
    {
      randnum = random(300);
      if (randnum > 150 && !rightflag)
      {
        Serial.println( "left" );
        leftflag = true;
        TurnLeft();
      }
      else
      {
        Serial.println( "right" );
        rightflag = true;
        TurnRight();
      }
    }
    else
    {
      leftflag = false;
      rightflag = false;
      switch ( State )
      {
        case stNormal:
          if ( Distance == 0 )
          {
            ForwardWithZeroDist++;
          }

          if (ForwardWithZeroDist >= 3)
          {
            State = stBarrier;
            ForwardWithZeroDist += 10;
          }
          else
          {
            Serial.println( "forward" );
            Forward();
          }
          break;

        case stBarrier:
          ForwardWithZeroDist--;
          if ( ForwardWithZeroDist )
          {
            Serial.println( "backward" );
            Backward();
          }
          else
          {
            State = stNormal;
            Serial.println( "right" );
            rightflag = true;
            TurnRight();
          }
          break;
      }
    }

    if( State != LastState )
    {
      Serial.println( FormatState( State ) );
      LastState = State;
    }
  }
}

/*** Remote *****************************************************************/
void Remote( void )
{
  if (bluetooth.available())
  {
    char Buf[32];
    char inDat = bluetooth.read();
    switch ( inDat )
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
}

const char *FormatState( int )
{
  if( State == stNormal )
  {
    return "stNormal";
  }
  return "stBarrier";
}

/*** ************************************************************************/
void Forward()
{
  MotorL.run(moveSpeed);
  MotorR.run(moveSpeed);
}

/*** ************************************************************************/
void Backward()
{
  MotorL.run(-moveSpeed);
  MotorR.run(-moveSpeed);
}

/*** ************************************************************************/
void Stop()
{
  MotorL.run(0);
  MotorR.run(0);
}

/*** ************************************************************************/
void TurnLeft()
{
  MotorL.run(turnSpeed);
  MotorR.run(-turnSpeed);
  delay( 400 );
}

/*** ************************************************************************/
void TurnRight()
{
  MotorL.run(-turnSpeed);
  MotorR.run(turnSpeed);
  delay( 400 );
}

/*** EOF *******************************************************************/


