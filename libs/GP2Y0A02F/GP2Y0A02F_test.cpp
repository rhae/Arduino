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

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_FILE_ID
static char _fileid[] = "$Id$";
#endif

uint16_t s_TestVec[] = 
{ 512,
501,
489,
466,
421,
410,
363,
307,
289,
265,
242,
219,
205,
189,
167,
145,
123,
116,
109,
102
};

/***************************************************************************/
/**
 *
 */
int main(
      int argc, /**< */
      char **argv
      )
{
  GP2Y0A02F Sen1( 0 );
  
  if( argc == 2 )
  {
    uint16_t ADvalue = (uint16_t) strtoul( argv[1], 0, 0 );
    uint16_t Distance = Sen1.GetDistanceCm( ADvalue );
    printf( "%u;%u\n", ADvalue, Distance );
  }
  else
  {
  
    for( uint16_t u = 0; u < countof(s_TestVec); u++ )
    {
      uint16_t Distance  = Sen1.GetDistanceCm( s_TestVec[u] );
      double   Distance2 = Sen1.GetDistance( s_TestVec[u] );
      double   Diff = Distance2 - Distance;
      
      printf( "%u;%u;%.3f;%.3f\n", s_TestVec[u], Distance, Distance2, Diff );
    }
  }
  
  
  return 0;
}


/*______________________________________________________________________EOF_*/
