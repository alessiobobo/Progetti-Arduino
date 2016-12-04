// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4,2,10,11,12,13);

const char hallpin=A4;
double current;
double readhall;
int readdelay=10;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(hallpin, INPUT);
}

void loop() {
  //tempattuale=(float) analogRead(termpin)*500/1023+2;
  readhall=0;
  for(int n=0; n<100; n++){
    readhall=readhall+((analogRead(hallpin))^2);
    delay(readdelay);
  }
  readhall=sqrt(readhall/100);
  current=map(readhall, 0,255,0,5);
  current=(current-2,5)/0,066;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Corrente");
  lcd.setCursor(0, 1);
  lcd.print(current);
  lcd.print(" mA");
}


