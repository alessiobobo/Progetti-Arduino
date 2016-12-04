#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

const int analogInPinsugiu = A3;
const int analogInPindxsx = A2;

int sensorValuesugiu = 0;        // value read from the pot
int sensorValuedxsx = 0;        // value read from the pot
int outputValuesugiu = 0;        // value output to the PWM (analog out)
int outputValuedxsx = 0;        // value output to the PWM (analog out)
int valore1a;
int valore1b;
int valore2a;
int valore2b;

void setup() {
  lcd.begin(16, 2);

}

void loop() {
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  digitalWrite(A0,HIGH);
  digitalWrite(A2,LOW);
  valore1a = analogRead(A1);            
  valore1b = analogRead(A3);

  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  digitalWrite(A1,HIGH);
  digitalWrite(A3,LOW);
  valore2a = analogRead(A0);            
  valore2b = analogRead(A2);  

  lcd.setCursor(0, 0);
  lcd.print(valore1a);
  lcd.setCursor(8, 0);
  lcd.print(valore1b);
  lcd.setCursor(0, 1);
  lcd.print(valore2a);
  lcd.setCursor(8, 1);
  lcd.print(valore2b);
  delay(300);             
  lcd.clear();  
}
