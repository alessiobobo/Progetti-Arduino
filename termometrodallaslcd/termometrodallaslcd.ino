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
  LiquidCrystal lcd(A0,A1,12,11,10,9);

void setup(void)
{
  lcd.begin(16, 2);
  // Start up the library
  sensors.begin();
  // Imposta il valore di righe e colonne del display LCD 
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop(void)
{ 
  
    // locate devices on the bus
  sensors.begin();
  sensors.requestTemperatures(); // Invia il comando di lettura delle temperatura
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Found ");
  lcd.print(sensors.getDeviceCount(), DEC);
  lcd.print(" devices");
  lcd.setCursor(0,0);
  lcd.setCursor(0,1);
  lcd.print("Temperatura ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print ("C"); 
  delay(500);
}
