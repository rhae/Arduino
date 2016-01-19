/**
 * \file GP2Y0A02F.h
 * \author Rüdiger Härtel
 *
 *
 */

#ifndef _GPY20A02F_H_
#define _GPY20A02F_H_

#include <stdint.h>

#ifdef linux
#else
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#endif

/* constant definitions
---------------------------------------------------------------------------*/

/* global defined data types
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

class GP2Y0A02F {
public:
  GP2Y0A02F( int Pin );
  
  uint16_t GetDistanceCm();
  uint16_t GetDistanceCm(uint16_t);
  uint16_t GetDistanceRaw();
  
#ifdef GP2Y0A02F_DOUBLE
  double   GetDistance( );
  double   GetDistance( uint16_t );
#endif
  
private:
  int _pin;
};


#endif
