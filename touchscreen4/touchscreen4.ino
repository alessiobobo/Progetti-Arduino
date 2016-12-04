#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

int touchX;
int touchY;

void setup() {
  lcd.begin(16, 2);
}

void loop() {
 
  // set L = ground
 pinMode(30, OUTPUT);
 digitalWrite(30, LOW); 
 // set R = Vcc
 pinMode(35, OUTPUT);
 digitalWrite(35, HIGH); 
 // T e B high impedance (input mode)
 pinMode(34, INPUT);
 pinMode(31, INPUT);
 // wait a bit, then read from Top
 delay(10); 
 touchX = analogRead(A0); 

 // Set B = gnd
 pinMode(31, OUTPUT);
 digitalWrite(31, LOW); 
 // set T = Vcc
 pinMode(34, OUTPUT);
 digitalWrite(34, HIGH); 
 // R e L high impedance (input mode)
 pinMode(30, INPUT);
 pinMode(35, INPUT); 
 // wait a bit, then read from Right
 delay(10); 
 touchY = analogRead(A3);
 
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print(touchX);
  lcd.setCursor(8, 0);
  lcd.print(touchY);
  delay(300);             

}
