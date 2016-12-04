#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 7, 6, 5, 4);

int buttonStateok = 0;
int buttonStateindietro = 0;
int sensorValuesugiu = 0;
int sensorValuedxsx = 0; 
int outputValuesugiu = 0;
int outputValuedxsx = 0;
int level=0;

int indicemenu;
int indicesotto;
int indicesottosotto;
int indicesottosottosotto;

String menu[10]={"menu1","menu2","menu3","menu4","menu5","menu6","menu7","menu8","menu9","menu10"};
String sotto[10]={"smenu1","smenu2","smenu3","smenu4","smenu5","smenu6","smenu7","smenu8","smenu9","smenu10",};
String sottosotto[10]={"ssmenu1","ssmenu2","ssmenu3","ssmenu4","ssmenu5","ssmenu6","ssmenu7","ssmenu8","ssmenu9","ssmenu10",};
String sottosottosotto[10]={"sssmenu1","sssmenu2","sssmenu3","sssmenu4","sssmenu5","sssmenu6","sssmenu7","sssmenu8","sssmenu9","sssmenu10",};

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

    if (outputValuesugiu>2 && outputValuesugiu>abs(outputValuedxsx) ){
      if (level==0){
      indicemenu++;
      indicesotto=0;
      }
      if (level==1){
      indicesotto++;
      indicesottosotto=0;
      }
      if (level==2){
      indicesottosotto++;
      indicesottosottosotto=0;
      }
      delay(300);
      lcd.clear();
    }
    if (outputValuesugiu<-2&& outputValuesugiu<-abs(outputValuedxsx)){
      if (level==0){
      indicemenu--;
      indicesotto=0;
      }
      if (level==1){
      indicesotto--;
      indicesottosotto=0;
      }
      if (level==2){
      indicesottosotto--;
      indicesottosottosotto=0;
      }
      delay(300);
      lcd.clear();
    }
    if (outputValuedxsx>2 && outputValuedxsx>abs(outputValuesugiu)){
      if (level==0){
      indicesotto++;
      }
      if (level==1){
      indicesottosotto++;
      }
      if (level==2){
      indicesottosottosotto++;
      }
      delay(300);
      lcd.clear();
    }
    if (outputValuedxsx<-2 && outputValuedxsx<-abs(outputValuesugiu)){
      if (level==0){
      indicesotto--;
      }
      if (level==1){
      indicesottosotto--;
      }
      if (level==2){
      indicesottosottosotto--;
      }
      delay(300);
      lcd.clear();
    }
    if (indicemenu<0) {
      indicemenu=9;
    }
    if (indicemenu>9){
      indicemenu=0;
    }
    if(indicesotto<0){
      indicesotto=9;
    }
    if(indicesotto>9) {
      indicesotto=0;
    }
    if(indicesottosotto<0){
      indicesottosotto=9;
    }
    if(indicesottosotto>9) {
      indicesottosotto=0;
    }
    if(indicesottosottosotto<0){
      indicesottosottosotto=9;
    }
    if(indicesottosottosotto>9) {
      indicesottosottosotto=0;
    }
    
    
    if(buttonStateok == HIGH){
    level++;
    delay(300);
    lcd.clear();
    }
    
    if(buttonStateindietro == HIGH){
    level--;
    delay(300);
    lcd.clear();
    }    
    
    level=constrain(level,0,2);
        
    if(level==0){
      lcd.setCursor(0,0);
      lcd.print(menu[indicemenu]);
      lcd.setCursor(0,1);
      lcd.print(sotto[indicesotto]);
    }
    
    if(level==1){
      lcd.setCursor(0,0);
      lcd.print(sotto[indicesotto]);
      lcd.setCursor(0,1);
      lcd.print(sottosotto[indicesottosotto]);
    }
    
    if(level==2){
      lcd.setCursor(0,0);
      lcd.print(sottosotto[indicesottosotto]);
      lcd.setCursor(0,1);
      lcd.print(sottosottosotto[indicesottosottosotto]);
    }
 
}
