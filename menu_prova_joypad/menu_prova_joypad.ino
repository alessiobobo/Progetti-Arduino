#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

int buttonStateok = 0;
int buttonStateindietro = 0;
int sensorValuesugiu = 0;
int sensorValuedxsx = 0; 
int outputValuesugiu = 0;
int outputValuedxsx = 0;

int indicemenu;
int indicesotto;

void setup() {
 
  lcd.begin(16, 2);
  pinMode(42, INPUT);
  pinMode(43, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);
  pinMode(52, OUTPUT);
  
}

void loop() {
  
  buttonStateok = digitalRead(42);
  buttonStateindietro = digitalRead(43);
  sensorValuesugiu = analogRead(A14);            
  sensorValuedxsx = analogRead(A15);

  outputValuesugiu = (68 - map(sensorValuesugiu, 0, 1023, 0, 140))/10;
  outputValuedxsx = (74 - map(sensorValuedxsx, 0, 1023, 0, 140))/10;

        lcd.setCursor(12,0);
        lcd.print(indicemenu);
        lcd.setCursor(12,1);
        lcd.print(indicesotto);  

  
    if (outputValuesugiu>2 && outputValuesugiu>abs(outputValuedxsx) ){
      indicemenu++;
      indicesotto=0;
      delay(300);
      lcd.clear();
    }
    if (outputValuesugiu<-2&& outputValuesugiu<-abs(outputValuedxsx)){
      indicemenu--;
      indicesotto=0;
      delay(300);
      lcd.clear();
    }
      if (outputValuedxsx>2 && outputValuedxsx>abs(outputValuesugiu)){
      indicesotto++;
      delay(300);
      lcd.clear();
    }
      if (outputValuedxsx<-2 && outputValuedxsx<-abs(outputValuesugiu)){
      indicesotto--;
      delay(300);
      lcd.clear();
    }
    
  //indicemenu=constrain(indicemenu,0,1);
  //indicesotto=constrain(indicesotto,0,1);
  
  switch (indicemenu){
    case 0:
      switch (indicesotto){
        case 0:
        lcd.setCursor(0,0);
        lcd.print("Led");
        lcd.setCursor(0,1);
        lcd.print("accendi");
          if (buttonStateok==HIGH){
          digitalWrite(52, HIGH);
          }
        break;
        case 1:
        lcd.setCursor(0,0);
        lcd.print("Led");
        lcd.setCursor(0,1);
        lcd.print("spegni");
          if (buttonStateok==HIGH){
          digitalWrite(52, LOW);
          }
        break;
        default:
        if(indicesotto<0){
          indicesotto=1;
          }
        else {
          indicesotto=0;
          }
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
        if(indicesotto<0){
          indicesotto=1;
          }
        else {
          indicesotto=0;
          }
        break;
      }
    break;
    
    default:
      if (indicemenu<0) {
      indicemenu=1;
      }
      else {
      indicemenu=0;
      }
    break;
    
  }
}
