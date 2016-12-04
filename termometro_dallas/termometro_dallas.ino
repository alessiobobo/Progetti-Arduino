#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Il terminale data del sensore è connesso
// alla porta 2 di Arduino
#define ONE_WIRE_BUS 2

// Imposta la comunicazione oneWire per comunicare
// con un dispositivo compatibile
OneWire oneWire(ONE_WIRE_BUS);

// Passaggio oneWire reference alla Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// RS EN D4 D5 D6 D7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup(void)
{
  // Start up the library
  sensors.begin();
  // Imposta il valore di righe e colonne del display LCD 
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop(void)
{ 
  
    // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  sensors.requestTemperatures(); // Invia il comando di lettura delle temperatura
  lcd.clear();
  lcd.setCursor(0, 0); // bottom left
  lcd.print("Temperatura di: ");
  lcd.setCursor(0, 1); // bottom left
  Serial.println (sensors.getTempCByIndex(0));
  lcd.print (" C"); 
  delay(500);
}
