#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

int sensorValue1 = 0;        
int sensorValue2 = 0;
int destra = 0;
int sinistra = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  sensorValue1 = analogRead(A1);            
  sensorValue2 = analogRead(A2);            
  //if (sensorValue1=HIGH) {
  //  destra++;
  //  delay(100);
  //  lcd.clear();
  //}
  //if (sensorValue2=HIGH) {
  //  sinistra++;
  //  delay(100);
  //  lcd.clear();
  //}
  //lcd.setCursor(0, 0);
  //lcd.print(destra);
  //lcd.setCursor(5, 0);
  //lcd.print(sinistra);
  lcd.setCursor(0, 0);
  lcd.print(sensorValue1);
  lcd.setCursor(5, 0);
  lcd.print(sensorValue2);
  delay(10);
  lcd.clear();
}
