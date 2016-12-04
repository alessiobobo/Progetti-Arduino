#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

int sensorValue1 = 0;        
int sensorValue2 = 0;        
int sensorValue3 = 0;        
int sensorValue4 = 0;        
float outputValue = 0;       

void setup() {
  lcd.begin(16, 2);  
}

void loop() {
  sensorValue1 = analogRead(A8);            
  sensorValue2 = analogRead(A9);            
  sensorValue3 = analogRead(A10);            
  sensorValue4 = analogRead(A11);            
  lcd.setCursor(0, 0);
  lcd.print(sensorValue1);
  lcd.setCursor(7, 0);
  lcd.print(sensorValue2);
  lcd.setCursor(0, 1);
  lcd.print(sensorValue3);
  lcd.setCursor(7, 1);
  lcd.print(sensorValue4);
  delay(100);             
  lcd.clear();  
}
