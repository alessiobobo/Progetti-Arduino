#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonPin1 = 44;
const int buttonPin2 = 45;

int buttonState1 = 0;
int buttonState2 = 0;

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  lcd.begin(16, 2);  
}

void loop(){

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  lcd.clear();
  delay(100);
  
  if (buttonState1 == HIGH) {     

    lcd.setCursor(0, 0);
    lcd.print("pulsante 1");
    delay(1000);                  // waits for a second    
  
    }

  if (buttonState2 == HIGH) {     

    lcd.setCursor(0, 1);
    lcd.print("pulsante 2");
    delay(1000);                  // waits for a second    
  
    }
    
}
 
