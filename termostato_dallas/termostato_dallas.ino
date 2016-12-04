// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8,7,12,11,10,9);

const char buttonpin1=5;
const char buttonpin2=6;
const char termpin=5;
const char termpinVcc=1;
const char termpinGnd=3;
const char freddopin=A5;
const char caldopin=A4;

int freddo;
int caldo;
float tempattuale;
int temp=EEPROM.read(0);
String sugiu;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  pinMode(buttonpin1, INPUT);
  pinMode(buttonpin2, INPUT);
  pinMode(caldopin, OUTPUT);
  pinMode(freddopin, OUTPUT);
  pinMode(termpinVcc, OUTPUT);
  pinMode(termpinGnd, OUTPUT);
  digitalWrite(termpinVcc, HIGH);
  digitalWrite(termpinGnd, LOW);
  digitalWrite(caldopin, LOW);
  digitalWrite(freddopin, LOW);
  sensors.begin();
}

void loop() {

  sensors.requestTemperatures(); // Send the command to get temperatures
  tempattuale=sensors.getTempCByIndex(0);

  if (digitalRead(buttonpin1)) {
    temp=temp-1;
    EEPROM.write(0,temp);
    delay(100);
  }
  if (digitalRead(buttonpin2)) {
    temp=temp+1;
    EEPROM.write(0,temp);
    delay(100);
  }


  sugiu="    ";
  if (tempattuale > (temp+2)) {     
    digitalWrite(freddopin, HIGH);
    digitalWrite(caldopin, LOW);
    sugiu=">>";    
    }
  if (tempattuale < (temp-2)) {     
    digitalWrite(freddopin, LOW);
    digitalWrite(caldopin, HIGH);
    sugiu="<<";    
    }
  if (tempattuale > temp-1 && tempattuale < temp+1) {
    sugiu="  ";
    digitalWrite(caldopin, LOW);
    digitalWrite(freddopin, LOW);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Attuale    Impostata");
  lcd.setCursor(0, 1);
  lcd.print(tempattuale);
  lcd.print("\337C");
  lcd.setCursor(9, 1);
  lcd.print(sugiu);
  lcd.setCursor(16, 1);
  lcd.print(temp);
  lcd.print("\337C");
  delay(200);
}


