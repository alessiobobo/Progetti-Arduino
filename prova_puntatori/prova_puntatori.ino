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

// stringa nomemenu,dimensione,nomemenu,dimensione

String menu[8]={"menu1","2","menu2","3","menu3","3","menu4","2"};

String sotto[10]={"smenu11","2","smenu12","2","smenu21","1","smenu22","1","smenu23","1","smenu31","1","smenu32","1","smenu33","1","smenu41","1","smenu42","1",};

String sottosotto[10]={"ssmenu111","ssmenu112","ssmenu121","ssmenu122","ssmenu211","ssmenu221","ssmenu231","ssmenu311","ssmenu321","ssmenu331","ssmenu411","ssmenu422"};

void setup() {
 
  lcd.begin(16, 2);
  pinMode(42, INPUT);
  pinMode(43, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);
  pinMode(52, OUTPUT);
  
}

  String *a=&menu[0];
  String *b=&sotto[0];


void loop() {

  String c[2]={*a,*b};
  String d;
    
    a=a+2;

      lcd.setCursor(0,0);
      lcd.print(c[0]);
      lcd.setCursor(0,1);
      lcd.print(c[1]);
      delay(10000);

}



