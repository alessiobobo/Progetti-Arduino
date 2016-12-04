// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4,2,10,11,12,13);

const char hallpin=A1;
long current;
double currentdouble;
long readhall;
int readdelay=1;

const char hallvcc=A0;
const char hallgnd=A2;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(hallpin, INPUT);
  pinMode(hallvcc, INPUT);
  pinMode(hallgnd, INPUT);
  digitalWrite(hallvcc, HIGH);
  digitalWrite(hallgnd, LOW);
  Serial.begin(9600);
  
}

void loop() {
  //tempattuale=(float) analogRead(termpin)*500/1023+2;
  current=0;
  for(int n=0; n<1000; n++){
    readhall=(analogRead(hallpin)-510);
    current+=sq(readhall);
    delay(readdelay);
  }
  currentdouble=sqrt(current/1000);
  currentdouble=(currentdouble)*5000000/66/1023;
    Serial.print((int)currentdouble);
    Serial.println(" mA");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Corrente");
  lcd.setCursor(0, 1);
  lcd.print(currentdouble);
  lcd.print(" A");  
}


