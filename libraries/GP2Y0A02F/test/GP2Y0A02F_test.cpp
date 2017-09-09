/**
 * \file GP2Y0A02F.cpp
 * \author Rüdiger Härtel
 *
 */


/* header of standard C - libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* header of common types */

/* shared common header */

/* header of project specific types */

/* project headers */
#include "GP2Y0A02F.h"

/* local header */

/* constant definitions
---------------------------------------------------------------------------*/
#ifndef countof
# define countof( x ) (sizeof(x) / sizeof(x[0]) )
#endif

/* local defined data types
---------------------------------------------------------------------------*/
struct _LUT_DBL {
  uint16_t ADvalue;
  double   Slope;
  double   Offset;
};

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
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


static struct _LUT_DBL _5V_LUT_DBL[] =
{
  { 409, -11.38 , 751 },
  { 307,  -9.31 , 689 },
  { 204,  -4.65 , 498 },
  { 122,  -2.21 , 344 },
  {   0,  -0.68 , 191 },
};


uint16_t s_TestVec[] = 
{ 
  512, 501, 489, 466, 421, 410, 363, 307,
  289, 265, 242, 219, 205, 189, 167, 145,
  123, 116, 109, 102
};

/***************************************************************************/
/**
 *  M A I N
 */
int main(
      int argc, /**< */
      char **argv
      )
{
  GP2Y0A02F Sen1( 0 );
  Sen1.begin();
  
  if( argc == 2 )
  {
    uint16_t ADvalue = (uint16_t) strtoul( argv[1], 0, 0 );
    uint16_t Distance = Sen1.getDistanceCm( ADvalue );
    printf( "%u;%u\n", ADvalue, Distance );
  }
  else
  {
  
    for( uint16_t u = 0; u < countof(s_TestVec); u++ )
    {
      uint16_t Distance  = Sen1.getDistanceCm( s_TestVec[u] );
      double   Distance2 = MapDbl( s_TestVec[u] );
      double   Diff = Distance2 - Distance;
      
      printf( "%u;%u;%.3f;%.3f\n", s_TestVec[u], Distance, Distance2, Diff );
    }
  }
  
  
  return 0;
}

/*** MapDbl ****************************************************************/
/**
 *  Calculate the distance with double precision
 * 
 *  \param ADValue    Value from analog to digital converter
 * 
 *  \return distance in cm
 */
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
  
  for( u = 0; u < countof(_5V_LUT_DBL); u++ )
  {
    if( ADValue >= p->ADvalue )
    {
      Dist = ADValue - p->Offset;
#ifdef DUMP
      printf("%u:  Ofs/Slope/Dist: %d/%d/%d\n", u, p->Offset, p->Slope, Dist );
#endif
      Dist = Dist / p->Slope;
      break;
    }
    p++;
  }
  
  return Dist;
}

/*______________________________________________________________________EOF_*/
