#include <GP2Y0A02F.h>

GP2Y0A02F Dist(1);
uint16_t distance;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  distance = Dist.GetDistanceCm();
  Serial.print("\nDistance in centimers: ");
  Serial.print(distance);  
  delay(500); //make it readable
}