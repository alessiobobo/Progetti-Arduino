#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int analogInPinsugiu = A3;
const int analogInPindxsx = A2;

int sensorValuesugiu = 0;        // value read from the pot
int sensorValuedxsx = 0;        // value read from the pot
int outputValuesugiu = 0;        // value output to the PWM (analog out)
int outputValuedxsx = 0;        // value output to the PWM (analog out)

void setup() {
  lcd.begin(16, 2);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  
  sensorValuesugiu = analogRead(analogInPinsugiu);            
  sensorValuedxsx = analogRead(analogInPindxsx);            
  
  outputValuesugiu = 68 - map(sensorValuesugiu, 0, 1023, 0, 140);
  outputValuedxsx = map(sensorValuedxsx, 0, 1023, 0, 140)-74;    
  
  lcd.setCursor(0, 0);
  lcd.print("Su Giu ");
  lcd.print(outputValuesugiu/10);
  lcd.setCursor(0, 1);
  lcd.print("Dx Sx ");
  lcd.print(outputValuedxsx/10);
  delay(200);             
  lcd.clear();  
}
