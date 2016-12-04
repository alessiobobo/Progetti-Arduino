#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

char* menu[3][8]=
  {
    {"Lampade","Lampada1","Lampada2","Lampada3","Lampada4"},
    {"Timer","Ore","Minuti","Secondi"},
    {"\177","\176","OK","Avvia","Pausa"}
  };


//---Variabili di stato----------------------------------------------------------
bool statoLamp1;
bool statoLamp2;
bool statoLamp3;
bool statoLamp4;
bool guasto1;
bool guasto2;
bool guasto3;
bool guasto4;

int indiceMenu;
int indiceSottomenu;

int timerOre;
int timerMin1;
int timerMin2;
int timerSec1;
int timerSec2;
int timer;
int Pausa;

//---Pin input output------------------------------------------------------------
int pulsanteScorri=A1;
int pulsanteOk=A0;
int pulsanteIndietro=A2;

int rele1=7;
int rele2=6;
int rele3=5;
int rele4=4;

int fotoresistenza1;
int fotoresistenza2;
int fotoresistenza3;
int fotoresistenza4;

int sensoreapertura;
int buzzer;
int termometro;

//---Variabili di menu-----------------------------------------------------------
int indicemenu;

//---Setup-----------------------------------------------------------------------
void setup() {
 
  lcd.begin(16, 2);
  pinMode(pulsanteScorri, INPUT);
  pinMode(pulsanteOk, INPUT);
  pinMode(pulsanteIndietro, INPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(fotoresistenza1, INPUT);
  pinMode(fotoresistenza2, INPUT);
  pinMode(fotoresistenza3, INPUT);
  pinMode(fotoresistenza4, INPUT);
  pinMode(sensoreapertura, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(termometro, INPUT);
 
}

//---Loop------------------------------------------------------------------------
void loop() {
  switch (indiceMenu){
    case 0:
      if(digitalRead(pulsanteScorri)){
      indiceSottomenu++;
      lcd.clear();
        if(indiceSottomenu>3){
        indiceSottomenu=0;
        }
      delay(300);
      }
      switch (indiceSottomenu){
        case 0:
          if (pulsanteOk){
          statoLamp1=!statoLamp1;
          }
        break;
        case 1:
          if (pulsanteOk){
          statoLamp2=!statoLamp2;
          }
        break;
          case 2:
          if (pulsanteOk){
          statoLamp3=!statoLamp3;
          }
        break;
          case 3:
          if (pulsanteOk){
          statoLamp4=!statoLamp4;
          }
        break;
        
      }
    break;
    
    case 1:
      switch (indiceSottomenu){
        case 0:
          if (pulsanteScorri){
            timerOre++;
            if (timerOre>9){
              timerOre=0;
            }
          }
        break;
        case 1:
          if (pulsanteScorri){
            timerMin1++;
            if (timerMin1>6){
              timerMin1=0;
            }
          }
          case 2:
          if (pulsanteScorri){
            timerMin2++;
            if (timerMin2>9){
              timerMin2=0;
            }
          }
          case 3:
          if (pulsanteScorri){
            timerSec1++;
            if (timerSec1>6){
              timerSec1=0;
            }
          }
          case 4:
          if (pulsanteScorri){
            timerSec1++;
            if (timerSec2>9){
              timerSec2=0;
            }
          }
        break;
        
   
      }
    break;
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
  
  //stampa prima riga
  lcd.setCursor(0,0);
  lcd.print(menu[indiceMenu][0]);
  lcd.print(">");
  
  //stampa seconda riga
  lcd.setCursor(16-strlen(menu[indiceMenu][indiceSottomenu+1]),1);
  //lcd.print(menu[indiceMenu][indiceSottomenu+1]);
  lcd.print(menu[2][0]);

}
