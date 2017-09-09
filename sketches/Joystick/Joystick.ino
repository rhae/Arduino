/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/Verdana9pt7.h>
//#include <Fonts/Picopixel.h>

#ifndef countof
# define countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#include <Themes/Themes1/48x48/n.h>
#include <Themes/Themes1/48x48/s.h>
#include <Themes/Themes1/48x48/e.h>
#include <Themes/Themes1/48x48/w.h>
#include <Themes/Themes1/48x48/nw.h>
#include <Themes/Themes1/48x48/ne.h>
#include <Themes/Themes1/48x48/sw.h>
#include <Themes/Themes1/48x48/se.h>

#include <Themes/Themes1/48x48/home.h>
#include <Themes/Themes1/48x48/pressed.h>


typedef struct _IconSpec {
  int16_t X;
  int16_t Y;
  uint8_t const * Data;
  int16_t W;
  int16_t H;
  uint16_t Colors;
  char     Text[16];
} IconSpec;

const IconSpec PROGMEM IconTab[] = {
  { 0, 0, home,    48, 48, 1, "Home" },
  { 0, 0, pressed, 48, 48, 1, "Pressed" },
  { 0, 0, n,       48, 48, 1, "North" },
  { 0, 0, s,       48, 48, 1, "South" },
  { 0, 0, e,       48, 48, 1, "East"},
  { 0, 0, w,       48, 48, 1, "West"},
  { 0, 0, ne,      48, 48, 1, "NorthEast"},
  { 0, 0, nw,      48, 48, 1, "NorthWest"},
  { 0, 0, se,      48, 48, 1, "SouthEast"},
  { 0, 0, sw,      48, 48, 1, "SouthWest"}
};


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

class ThumbJoystick {
  public:
  static const int HOME     = 0;
  static const int PRESSED  = 1;
  static const int DIR_N    = 2;
  static const int DIR_S    = 3;
  static const int DIR_E    = 4;
  static const int DIR_W    = 5;
  static const int DIR_NE   = 6;
  static const int DIR_NW   = 7;
  static const int DIR_SE   = 8;
  static const int DIR_SW   = 9;
  
  ThumbJoystick( int PinX, int PinY );

  int getXDir();
  int getYDir();
  
  int getXYDir();

  bool isPressed();

  void update();

  private:
  
  int _PinX;
  int _PinY;

  int _X;
  int _Y;
};

ThumbJoystick::ThumbJoystick( int PinX, int PinY )
{
  _PinX = PinX;
  _PinY = PinY; 
}

int ThumbJoystick::getXDir()
{
  return _X;
}

int ThumbJoystick::getYDir()
{
  return _Y;
}

int ThumbJoystick::getXYDir()
{
  if( isPressed() ) return PRESSED;
  
  if( _X > 530 )
  {
    if( _Y < 500 ) return DIR_SE;
    if( _Y > 530 ) return DIR_NE;
    return DIR_E;
  }

  if( _X < 500 )
  {
    if( _Y < 500 ) return DIR_SW;
    if( _Y > 530 ) return DIR_NW;
    return DIR_W;
  }

  if( _Y < 500 ) return DIR_S;
  if( _Y > 530 ) return DIR_N;
  return HOME;
}

bool ThumbJoystick::isPressed()
{
  return ( _X > 1020 ) ? true : false;
}

void ThumbJoystick::update()
{
  _X = analogRead( _PinX );
  _Y = analogRead( _PinY );
}

ThumbJoystick Joystick( A0, A1 );

const IconSpec* getIcon( int IcoIdx )
{
  return &IconTab[ IcoIdx ];
}

void drawIcon( int XY )
{
  IconSpec Icon;
  memcpy_P( &Icon, &IconTab[XY], sizeof(IconSpec));
  display.drawBitmap( Icon.X, Icon.Y, Icon.Data, Icon.W, Icon.H, Icon.Colors );
  display.setCursor( 5, 62 );
  //display.setFont( &Verdana9pt7b );
  //display.setTextSize(1);
  display.print( Icon.Text );
  #if 0
  switch (XY)
  {
    case ThumbJoystick::HOME:
      display.drawBitmap( 10, 10, home, 16, 16, 1 );
      break;
    case ThumbJoystick::PRESSED:
      display.drawBitmap( 10, 10, pressed, 16, 16, 1 );
      break;
    case ThumbJoystick::DIR_N:
      display.drawBitmap( 0, 0, n, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_S:
      display.drawBitmap( 0, 0, s, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_E:
      display.drawBitmap( 0, 0, e, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_W:
      display.drawBitmap( 0, 0, w, 48, 48, 1 );
      break;

    case ThumbJoystick::DIR_NE:
      display.drawBitmap( 0, 0, ne, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_NW:
      display.drawBitmap( 0, 0, nw, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_SE:
      display.drawBitmap( 0, 0, se, 48, 48, 1 );
      break;
    case ThumbJoystick::DIR_SW:
      display.drawBitmap( 0, 0, sw, 48, 48, 1 );
      break;
  }
  #endif
}

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  int n = sizeof( w );
  Serial.print( "1234 sizeof(w) = " );
  Serial.println( n );

#if 0
  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0,  nw, 48, 48, 1);
  display.display();
  delay(5000);
#endif

  for( int i = 0; i < countof(IconTab); i++ )
  {
    char Buf[32];
    sprintf( Buf, "%d %p %s", i, IconTab[i].Data, IconTab[i].Text );
    Serial.println( Buf );
  }

  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setFont( &Verdana9pt7b );
}

void loop() {
  
  Joystick.update();

#if 0
  char Buf[16] = {};
  int X = Joystick.getXDir();
  int Y = Joystick.getYDir();
  sprintf( Buf, "%d %d", X, Y );
  Serial.println( Buf );
#endif

  display.clearDisplay();
  int XY = Joystick.getXYDir();
  drawIcon( XY );

  display.display();
}


