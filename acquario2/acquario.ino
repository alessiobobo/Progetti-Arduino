#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


//#define ONE_WIRE_BUS 2
//OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire(10);
DallasTemperature sensors(&oneWire);

// RS EN D4 D5 D6 D7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const char hallpin=A0;
unsigned long current;
int readdelay=1;

void setup(void)
{
  sensors.begin();
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(hallpin, INPUT);
}

void loop(void)
{ 
  
//Lettura temperatura-----------------------------------------------------------------------
  sensors.begin();
  if(sensors.getDeviceCount()==1){
    Serial.print("Trovato ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" sensore.");
  }
  else{
    Serial.print("Trovati ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" sensori.");
  }
  sensors.requestTemperatures(); // Invia il comando di lettura delle temperatura
  Serial.print (sensors.getTempCByIndex(0));
  Serial.println(" C");
//------------------------------------------------------------------------------------------

//Lettura corrente--------------------------------------------------------------------------
  current=0;
  for(int n=0; n<1000; n++){
    current+=sq((long)(analogRead(hallpin)-511));
    delay(readdelay);
  }
  current=sqrt(current*10);
  current=(current)*750/1023;
  Serial.println("Corrente");
  Serial.print(current);
  Serial.println(" mA");
//------------------------------------------------------------------------------------------
}
