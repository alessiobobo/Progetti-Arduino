#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int buttonState1 = 0;
int buttonState2 = 0;

void setup() {
 
  lcd.begin(16, 2);
  pinMode(44, INPUT);
  pinMode(45, INPUT);
  
}

int indicemenu;
int indicesotto;

void loop() {
  
  buttonState1 = digitalRead(44);
  buttonState2 = digitalRead(45);
  
    if (buttonState1 == HIGH){
      indicemenu++;
      indicesotto=0;
      delay(300);
      lcd.clear();
    }
    if (buttonState2 == HIGH) {
      indicesotto++;
      delay(300);
      lcd.clear();
    }
    
  switch (indicemenu){
    case 0:
      switch (indicesotto){
        
        case 0:
        lcd.setCursor(0,0);
        lcd.print("menu 1");
        lcd.setCursor(0,1);
        lcd.print("sottomenu 1");
        break;
        
        case 1:
        lcd.setCursor(0,0);
        lcd.print("menu 1");
        lcd.setCursor(0,1);
        lcd.print("sottomenu 2");
        break;
        
        default:
        indicesotto=0;
        break;
      }
    break;
    
    case 1:
      switch (indicesotto){
      
        case 0:
        lcd.setCursor(0,0);
        lcd.print("menu 2");
        lcd.setCursor(0,1);
        lcd.print("sottomenu 1");
        break;
      
        case 1:
        lcd.setCursor(0,0);
        lcd.print("menu 2");
        lcd.setCursor(0,1);
        lcd.print("sottomenu 2");
        break;
      
        default:
        indicesotto=0;
        break;
      }
    break;
    
    default:
    indicemenu=0;  
    break;
    
    }
    
}
