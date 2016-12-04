// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2,3,7,6,5,4);

const int buttonpin1=43;
const int buttonpin2=42;
const char termpin=A1;
const char termpinV0=A2;
const char termpingnd=A0;
const char freddopin=23;
const char freddoVcc=33;
const char caldopin=22;
const char caldoVcc=32;
int buttonstate1=0;
int buttonstate2=0;
int freddo;
int caldo;
float tempattuale;
float temp=EEPROM.read(0);
String sugiu;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(termpin, INPUT);
  pinMode(buttonpin1, INPUT);
  pinMode(buttonpin2, INPUT);
  pinMode(caldopin, OUTPUT);
  pinMode(freddopin, OUTPUT);
  pinMode(freddoVcc, OUTPUT);
  pinMode(caldoVcc, OUTPUT);
  pinMode(termpinV0, OUTPUT);
  pinMode(termpingnd, OUTPUT);
}

void loop() {
  digitalWrite(freddoVcc, LOW);
  digitalWrite(caldoVcc, LOW);
  digitalWrite(termpinV0, HIGH);
  digitalWrite(termpingnd, LOW);
  buttonstate1 = digitalRead(buttonpin1);
  buttonstate2 = digitalRead(buttonpin2);
  tempattuale=(float) analogRead(termpin)*500/1023+2;
    
  if (buttonstate1 == HIGH) {     
    temp=temp-1;
    EEPROM.write(0,temp);
    }
  if (buttonstate2 == HIGH) {     
    temp=temp+1;
    EEPROM.write(0,temp);
    }
  if (tempattuale > (temp+2)) {     
    digitalWrite(freddopin, HIGH);
    digitalWrite(caldopin, LOW);
    sugiu=" >> ";    
    }
  if (tempattuale < (temp-2)) {     
    digitalWrite(freddopin, LOW);
    digitalWrite(caldopin, HIGH);
    sugiu=" << ";    
    }
  if (tempattuale > temp-1 && tempattuale < temp+1) {
    sugiu="    ";
    digitalWrite(caldopin, LOW);
    digitalWrite(freddopin, LOW);
  }
  lcd.setCursor(0, 0);
  lcd.print("Adesso    Ideale.");
  lcd.setCursor(0, 1);
  lcd.print(tempattuale);
  lcd.print("C");
  lcd.print(sugiu);
  lcd.print(temp);
  lcd.print("C");

  delay(200);

}
