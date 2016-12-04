// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4,2,10,11,12,13);

const char displayrw=3;
const char buttonpin1=5;
const char buttonpin2=6;
const char termpin=A4;
const char termpinV0=A5;
const char termpingnd=A3;
const char freddopin=A2;

const char caldopin=A1;

int buttonstate1=0;
int buttonstate2=0;
int freddo;
int caldo;
float tempread;
float tempattuale;
int temp=EEPROM.read(0);
String sugiu;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(displayrw, OUTPUT);
  pinMode(termpin, INPUT);
  pinMode(buttonpin1, INPUT);
  pinMode(buttonpin2, INPUT);
  pinMode(caldopin, OUTPUT);
  pinMode(freddopin, OUTPUT);

  pinMode(termpinV0, OUTPUT);
  pinMode(termpingnd, OUTPUT);
}

void loop() {
  digitalWrite(displayrw, HIGH);
  digitalWrite(termpinV0, HIGH);
  digitalWrite(termpingnd, LOW);
  buttonstate1 = digitalRead(buttonpin1);
  buttonstate2 = digitalRead(buttonpin2);
  if (buttonstate1 == HIGH) {
  temp=temp-1;
  EEPROM.write(0,temp);
  delay(100);
  goto fine;
  }
  if (buttonstate2 == HIGH) {
  temp=temp+1;
  EEPROM.write(0,temp);
  delay(100);
  goto fine;
  }
  //tempattuale=(float) analogRead(termpin)*500/1023+2;
  tempread=analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempread=tempread+analogRead(termpin);
  delay(2);
  tempattuale=tempread/1023*25+2;

  sugiu="    ";
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adesso    Ideale.");
  lcd.setCursor(0, 1);
  lcd.print(tempattuale);
  lcd.print("C");
  lcd.print(sugiu);
  lcd.print(temp);
  lcd.print("C");
  delay(200);
  fine:;
}


