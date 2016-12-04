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
  if (sensorValue1>100){
    lcd.print(1);
  }
  else lcd.print(0);
  lcd.setCursor(8, 0);
  if (sensorValue2>100){
    lcd.print(1);
  }
  else lcd.print(0);
  lcd.setCursor(0, 1);
    if (sensorValue3>100){
    lcd.print(1);
  }
  else lcd.print(0);
  lcd.setCursor(8, 1);
    if (sensorValue4>100){
    lcd.print(1);
  }
  else lcd.print(0);
  delay(100);             
  lcd.clear();  
}
