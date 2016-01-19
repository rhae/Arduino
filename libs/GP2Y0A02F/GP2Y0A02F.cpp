/**
 * \file GP2Y0A02F.cpp
 * \author Rüdiger Härtel
 *
 */


/* header of standard C - libraries */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* header of common types */

/* shared common header */

/* header of project specific types */

/* project headers */

/* local header */
#include "GP2Y0A02F.h"

/* constant definitions
---------------------------------------------------------------------------*/
#ifndef countof
# define countof( x ) (sizeof(x) / sizeof(x[0]) )
#endif

/* local defined data types
---------------------------------------------------------------------------*/
#ifdef GP2Y0A02F_DOUBLE
struct _LUT_DBL {
  uint16_t ADvalue;
  double   Slope;
  double   Offset;
};
#endif

struct _LUT {
  uint16_t ADvalue;
  int      Slope;
  int      Offset;
};

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/
#ifdef linux
int mock_analogRead();
#endif

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
static uint16_t Map( uint16_t );
static double   MapDbl( uint16_t );

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_FILE_ID
static char _fileid[] = "$Id$";
#endif

#ifdef GP2Y0A02F_DOUBLE
static struct _LUT_DBL _5V_LUT_DBL[] =
{
  { 409, -11.38 , 751 },
  { 307,  -9.31 , 689 },
  { 204,  -4.65 , 498 },
  { 122,  -2.21 , 344 },
  {   0,  -0.68 , 191 },
};
#endif

static struct _LUT _5V_LUT[] =
{
  { 409, (int16_t)(-11.38 * 64), 751 },
  { 307, (int16_t)( -9.31 * 64), 689 },
  { 204, (int16_t)( -4.65 * 64), 498 },
  { 122, (int16_t)( -2.21 * 64), 344 },
  {   0, (int16_t)( -0.68 * 64), 191 },
};



GP2Y0A02F::GP2Y0A02F( int Pin )
{
  _pin = Pin;
}
  
uint16_t GP2Y0A02F::GetDistanceCm()
{
  return GetDistanceCm( GetDistanceRaw() );
}

uint16_t GP2Y0A02F::GetDistanceCm(uint16_t ADValue)
{
  return Map( ADValue );
}

uint16_t GP2Y0A02F::GetDistanceRaw()
{
#ifdef linux
  return 0;
#else
  return (uint16_t) analogRead( _pin );
#endif
}

#ifdef GP2Y0A02F_DOUBLE
double GP2Y0A02F::GetDistance()
{
  return GetDistanceCm( GetDistanceRaw() );
}

double GP2Y0A02F::GetDistance( uint16_t ADValue )
{
  return MapDbl( ADValue );
}
#endif

uint16_t Map( uint16_t ADValue)
{
  uint16_t u;
  int32_t Dist = 0;
  struct _LUT *p = _5V_LUT;
  
  if( ADValue < 100 )
  {
    return 130;
  }
  
  if( ADValue > 512 )
  {
    return 21;
  }
  
  for( u = 0; u < countof(_5V_LUT); u++ )
  {
    if( ADValue >= p->ADvalue )
    {
      Dist = ADValue - p->Offset;
#ifdef GP2YA02F_DUMP
      printf("%u:  Ofs/Slope/Dist: %d/%d/%d\n", u, p->Offset, p->Slope, Dist );
#endif
      Dist = (uint16_t) ( ((Dist<<6) + 32) / p->Slope );
      break;
    }
    p++;
  }
  
  return Dist;
}

#ifdef GP2Y0A02F_DOUBLE
double MapDbl( uint16_t ADValue)
{
  uint16_t u;
  double Dist = 0;
  struct _LUT_DBL *p = _5V_LUT_DBL;
  
  if( ADValue < 100 )
  {
    return 130;
  }
  
  if( ADValue > 512 )
  {
    return 21;
  }
  
  for( u = 0; u < countof(_5V_LUT); u++ )
  {
    if( ADValue >= p->ADvalue )
    {
      Dist = ADValue - p->Offset;
#ifdef GP2YA02F_DUMP
      printf("%u:  Ofs/Slope/Dist: %d/%d/%d\n", u, p->Offset, p->Slope, Dist );
#endif
      Dist = Dist / p->Slope;
      break;
    }
    p++;
  }
  
  return Dist;
}
#endif

/*______________________________________________________________________EOF_*/
