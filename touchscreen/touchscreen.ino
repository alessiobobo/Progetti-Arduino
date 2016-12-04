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
  digitalWrite
  sensorValue1 = analogRead(A8);            
  sensorValue2 = analogRead(A9);            
  lcd.setCursor(0, 0);
 // lcd.print(map(sensorValue1,640,740, 1, 10));
  lcd.print(sensorValue1);
  lcd.setCursor(7, 0);
 // lcd.print(map(sensorValue2,600,800, 1, 10));
  lcd.print(sensorValue2);
  delay(300);             
  lcd.clear();  
}
