#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int analogInPinsugiu = A3;
const int analogInPindxsx = A2;

int sensorValuesugiu = 0;        // value read from the pot
int sensorValuedxsx = 0;        // value read from the pot
float outputValuesugiu = 0;        // value output to the PWM (analog out)
float outputValuedxsx = 0;        // value output to the PWM (analog out)

void setup() {
  lcd.begin(16, 2);  
}

void loop() {
  
  sensorValuesugiu = analogRead(analogInPinsugiu);            
  sensorValuedxsx = analogRead(analogInPindxsx);            
  
  outputValuesugiu = map(sensorValuesugiu, 0, 1023, 0, 5000);
  outputValuedxsx = map(sensorValuedxsx, 0, 1023, 0, 5000);    
  
  lcd.setCursor(0, 0);
  lcd.print("Su Giu ");
  lcd.print(outputValuesugiu);
  lcd.setCursor(0, 1);
  lcd.print("Dx Sx ");
  lcd.print(outputValuedxsx);
  delay(200);             
  lcd.clear();  
}
