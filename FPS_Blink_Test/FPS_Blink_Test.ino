#include "FPS_GT511C3.h"
#include "ESP_SoftwareSerial.h"

// Hardware setup - FPS connected to:
//	  digital pin 4(arduino rx, fps tx)
//	  digital pin 5(arduino tx - 560ohm resistor - fps tx - 1000ohm resistor - ground)
//		this voltage divider brings the 5v tx line down to about 3.2v so we dont fry our fps

FPS_GT511C3 fps(D8, D7);

void setup()
{
	Serial.begin(115200);
	fps.UseSerialDebug = true; // so you can see the messages in the serial debug screen

  for(int i=0; i<=4; i++){
    Serial.println("Loading "+5-i);
    delay(1000); 
  }
	
	fps.Open();
  fps.ChangeBaudRate(115200);
}


void loop()
{
	// FPS Blink LED Test
 
	fps.AcquireTemplateToSerial();
	delay(3000);
}
