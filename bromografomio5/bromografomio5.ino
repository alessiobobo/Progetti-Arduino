#include <LiquidCrystal.h>

  LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
  
  byte lampOn[8]={0xe,0xe,0xa,0xa,0xa,0xa,0xe,0xe};
  byte lampOff[8]={0xe,0x0,0x0,0x0,0x0,0x0,0x0,0xe};
  byte checkboxNo[8]={0x0,0xe,0x11,0x11,0x11,0xe,0x0,0x0};
  byte checkboxSi[8]={0x0,0xe,0x1f,0x1f,0x1f,0xe,0x0,0x0};
  byte orologio[8]={0x0,0x0,0xe,0x15,0x17,0x11,0xe,0x0};
  byte termo[8]={0xc,0xe,0xc,0xe,0xc,0x1e,0x1e,0xc};
  byte acceso[8]={0xa,0x15,0x0,0xa,0x15,0x0,0xa,0x15};
  byte guasto[8]={0xe,0xe,0xc,0x8,0x2,0x6,0xe,0xe};
  
  char* lamp[3]={"\b","\1","\7"};
  char* checkbox[2]={"\2","\3"};
  
  extern volatile unsigned long timer0_overflow_count;

//---Variabili di stato----------------------------------------------------------

  bool statoLamp[4];
  bool guastoLamp[4];
  int indiceMenu;
  int indiceSottomenu;
  int timer2[6];
  long timer1;
  long rimanente;
  long inizio;

//---Pin input output------------------------------------------------------------

  int pulsanteScorri=A1;
  int pulsanteDx=A2;
  int pulsanteSx=A0;
  int rele[4]={4,5,6,7};
  int fotoresistenza[4]={};
  int soglia=100;
  long attesaCheck=1000;
  int sensoreapertura;
  int buzzer;
  int termometro;
  
//---Funzioni--------------------------------------------------------------------

  void printLamp(){
    for(int ii=0; ii<4; ii++){
      if(guastoLamp[ii]){
        lcd.print(lamp[2]);
      }
      else{
        lcd.print(lamp[statoLamp[ii]]);
      }
    }
  }
  
  void accendiLamp(){
    digitalWrite(rele[0], statoLamp[0]);
    digitalWrite(rele[1], statoLamp[1]);
    digitalWrite(rele[2], statoLamp[2]);
    digitalWrite(rele[3], statoLamp[3]);
  }
  
  void spegniLamp(){
    digitalWrite(rele[0],LOW);
    digitalWrite(rele[1],LOW);
    digitalWrite(rele[2],LOW);
    digitalWrite(rele[3],LOW);
  }
  
  void checkLamp(int i1){
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Attendi!");
    digitalWrite(rele[i1-1], HIGH);
    delay(attesaCheck);
    if(analogRead(fotoresistenza[i1-1]>soglia)){
      guastoLamp[i1-1]=0;
    }
    else{
      guastoLamp[i1-1]=1;
    }
    digitalWrite(rele[i1-1], LOW);
    lcd.clear();
  }
  
  void printTime(long l1){
    if(l1/3600<10){
      lcd.print(0);
    }
    lcd.print(l1/3600);
    lcd.print(":");
    if((l1%3600)/60<10){
      lcd.print(0);
    }
    lcd.print((l1%3600)/60);
    lcd.print(":");
    if((l1%3600)%60<10){
      lcd.print(0);
    }
    lcd.print((l1%3600)%60);
  }
  
  void printTimer2Sel(int i1){
    for(int ii=0; ii<6; ii++){
      if (ii==i1-1){
        lcd.print("[");
      }
      lcd.print (timer2[ii]);
      if (ii==i1-1){
        lcd.print("]");
      }
      if ((((ii+1)%2)==0)&&((ii+1)!=6)){
        lcd.print(":");
      }  
    }
  }

//---Setup-----------------------------------------------------------------------

void setup() {

  lcd.createChar(0, lampOff);
  lcd.createChar(1, lampOn);
  lcd.createChar(2, checkboxNo);
  lcd.createChar(3, checkboxSi);
  lcd.createChar(4, orologio);
  lcd.createChar(5, termo);
  lcd.createChar(6, acceso);
  lcd.createChar(7, guasto);

  lcd.begin(16, 2);

  pinMode(pulsanteScorri, INPUT);
  pinMode(pulsanteSx, INPUT);
  pinMode(pulsanteDx, INPUT);
  pinMode(rele[0], OUTPUT);
  pinMode(rele[1], OUTPUT);
  pinMode(rele[2], OUTPUT);
  pinMode(rele[3], OUTPUT);
  pinMode(fotoresistenza[0], INPUT);
  pinMode(fotoresistenza[1], INPUT);
  pinMode(fotoresistenza[2], INPUT);
  pinMode(fotoresistenza[3], INPUT);
  pinMode(sensoreapertura, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(termometro, INPUT);
 
}

//---Loop------------------------------------------------------------------------
void loop() {
  switch (indiceMenu){
    case 0:
      lcd.setCursor(0,0);
      lcd.print("\5");
      lcd.print("20\337C");
      lcd.setCursor(8,0);
      printTime(timer1);
      lcd.setCursor(0,1);
      lcd.print("Menu");
      lcd.setCursor(6,1);
      printLamp();
      lcd.setCursor(11,1);
      lcd.print("Avvia");
      if(digitalRead(pulsanteSx)){
        indiceMenu++;
        delay(300);
        lcd.clear();
      }
      if(digitalRead(pulsanteDx)){
        if((!statoLamp[0]||guastoLamp[0])&&(!statoLamp[1]||guastoLamp[1])&&(!statoLamp[2]||guastoLamp[2])&&(!statoLamp[3]||guastoLamp[3])){
          indiceMenu=8;
        }
        else if(timer1==0){
          indiceMenu=9;
        }
        else{
          indiceMenu=3;
        }
        delay(300);
        lcd.clear();
      }
      
            
      
    break;
    
    case 1:                                        //lampade
         switch (indiceSottomenu){
            case 0:
              lcd.setCursor(0,0);
              lcd.print("Lampade");
              lcd.setCursor(0,1);
              lcd.print("\177        Imposta");
              if(digitalRead(pulsanteSx)){
                indiceMenu--;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteDx)){
                indiceSottomenu++;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteScorri)){
                indiceMenu++;
                delay(300);
                lcd.clear();
              }
          break;
          default:
              lcd.setCursor(0,0);
              lcd.print("Lampada ");
              lcd.print(indiceSottomenu);
              lcd.setCursor(0,1);
              lcd.print("\177  ");
              printLamp();
              if(guastoLamp[indiceSottomenu-1]){
                lcd.setCursor(10,0);
                lcd.print("Guasta");
                lcd.setCursor(9,1);
                lcd.print("Riprova");
                if(digitalRead(pulsanteDx)){
                  checkLamp(indiceSottomenu);
                }
              }
              else{
              lcd.setCursor(15,0);
              lcd.print(checkbox[statoLamp[indiceSottomenu-1]]);
              lcd.setCursor(10,1);
              lcd.print("On/Off");
              if(digitalRead(pulsanteDx)){
                statoLamp[indiceSottomenu-1]=!statoLamp[indiceSottomenu-1];
                delay(300);
                lcd.clear();
              }
              }
              if(digitalRead(pulsanteSx)){
                indiceSottomenu=0;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteScorri)){
                indiceSottomenu++;
                if(indiceSottomenu>4){
                  indiceSottomenu=1;
                }
                delay(300);
                lcd.clear();
              }
            break;
          }
        break;          
        case 2:
          switch(indiceSottomenu){
            case 0:
              lcd.setCursor(0,0);
              lcd.print("Timer");
              lcd.setCursor(0,1);
              lcd.print("\177        Imposta");
              if(digitalRead(pulsanteSx)){
                indiceMenu=0;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteDx)){
                indiceSottomenu++;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteScorri)){
                indiceMenu--;
                delay(300);
                lcd.clear();
              }
            break;
            
            default:
              lcd.setCursor(3,0);
              printTimer2Sel(indiceSottomenu);
              lcd.setCursor(0,1);
              if(indiceSottomenu==6){
                lcd.print("\74             OK");
                if(digitalRead(pulsanteDx)){
                  indiceSottomenu=0;
                  delay(300);
                  lcd.clear();
                }
              }
              else{
                if (indiceSottomenu==1){
                  lcd.print("\177              \76");
                }
                else {
                  lcd.print("\74              \76");
                }
                if(digitalRead(pulsanteDx)){
                  indiceSottomenu++;
                  delay(300);
                  lcd.clear();

                } 
              }
              if(digitalRead(pulsanteSx)){
                indiceSottomenu--;
                rimanente=timer1;
                delay(300);
                lcd.clear();
              }
              if(digitalRead(pulsanteScorri)){
                timer2[indiceSottomenu-1]++;
                if(((indiceSottomenu==3)||(indiceSottomenu==5))&&timer2[indiceSottomenu-1]>5){
                  timer2[indiceSottomenu-1]=0;
                }
                if(timer2[indiceSottomenu-1]>9){
                  timer2[indiceSottomenu-1]=0;
                }
                timer1=36000*timer2[0]+3600*timer2[1]+600*timer2[2]+60*timer2[3]+10*timer2[4]+timer2[5];
                delay(300);
                lcd.clear();
              }
            break;
          }            
        break;
        case 3:
          lcd.setCursor(0,0);
          lcd.print("    Sicuro?     ");
          lcd.setCursor(0,1);
          lcd.print("\177             SI");
          if(digitalRead(pulsanteDx)){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Tieni premuto  ");
            delay(100);
            if(digitalRead(pulsanteDx)){
              delay(300);
              if(digitalRead(pulsanteDx)){
                delay(300);
                if(digitalRead(pulsanteDx)){
                  while(digitalRead(pulsanteDx)){
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("    Rilascia    ");
                    delay(100);
                  }
                  indiceMenu=4;
                  inizio=timer0_overflow_count;
                  lcd.clear();
                }
              }
            }
          }
          if(digitalRead(pulsanteSx)){
            indiceMenu=0;
            delay(300);
          lcd.clear();
          }
        break;
        case 4:
          lcd.setCursor(0,1);
          lcd.print(" \6");
          printLamp();
          lcd.print("\6    Pausa");
          accendiLamp();
          rimanente=timer1+inizio/1000-timer0_overflow_count/1000;
          lcd.setCursor(0,0);
          printTime(rimanente);
          if(digitalRead(pulsanteDx)){
            indiceMenu=5;
            delay(300);
            lcd.clear();
          }
          lcd.setCursor(15-((String)(100-rimanente*100/timer1)).length(),0);
          lcd.print(100-rimanente*100/timer1);
          lcd.print("%");
          if(rimanente<=0){
            indiceMenu=6;
            spegniLamp();
            lcd.clear();
          };
          if(statoLamp[0] && analogRead(fotoresistenza[0])<soglia){
          guastoLamp[0]=1;
          statoLamp[0]=0;
          indiceMenu=7;
          lcd.clear();
          }
          if(statoLamp[1] && analogRead(fotoresistenza[0])<soglia){
          guastoLamp[1]=1;
          statoLamp[1]=0;
          indiceMenu=7;
          lcd.clear();
          }
          if(statoLamp[2] && analogRead(fotoresistenza[0])<soglia){
          guastoLamp[2]=1;
          statoLamp[2]=0;
          indiceMenu=7;
          lcd.clear();
          }
          if(statoLamp[3] && analogRead(fotoresistenza[0])<soglia){
          guastoLamp[3]=1;
          statoLamp[3]=0;
          indiceMenu=7;
          lcd.clear();
          }
        break;
        case 5:
          spegniLamp();
          lcd.setCursor(0,0);
          lcd.print("    In Pausa    ");
          lcd.setCursor(0,1);
          lcd.print("Ferma   Riprendi");
          if(digitalRead(pulsanteDx)){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Tieni premuto  ");
            delay(100);
            if(digitalRead(pulsanteDx)){
              delay(300);
              if(digitalRead(pulsanteDx)){
                delay(300);
                if(digitalRead(pulsanteDx)){
                  while(digitalRead(pulsanteDx)){
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("    Rilascia    ");
                    delay(100);
                  }
                  indiceMenu=4;
                  lcd.clear();
                }
              }
            }
          }
          if(digitalRead(pulsanteSx)){
          indiceMenu=0;
          delay(300);
          lcd.clear();
          }
        break;
        case 6:
        lcd.setCursor(0,0);
        lcd.print("      Fine       ");
        lcd.setCursor(0,1);
        lcd.print("\177");
        if(digitalRead(pulsanteSx)){
          indiceMenu=0;
          delay(300);
          lcd.clear();
        }
        break;
        case 7:
        spegniLamp();
        lcd.setCursor(0,0);
        lcd.print("Guasto! ");
        printTime(rimanente);
        lcd.setCursor(0,1);
        lcd.print("\177       ");
        lcd.setCursor(6,1);
        printLamp();
        if(digitalRead(pulsanteSx)){
          indiceMenu=0;
          delay(300);
          lcd.clear();
        }
        break;
        
        case 8:
        lcd.setCursor(0,0);
        lcd.print("Nessuna lampada!");
        lcd.setCursor(0,1);
        lcd.print("\177  ");
        printLamp();
        lcd.print("  Imposta");
        if(digitalRead(pulsanteSx)){
          indiceMenu=0;
          delay(300);
          lcd.clear();
        }
        if(digitalRead(pulsanteDx)){
          indiceMenu=1;
          indiceSottomenu=1;
          delay(300);
          lcd.clear();
        }
        break;
        case 9:
        lcd.setCursor(2,0);
        lcd.print("Timer nullo!");
        lcd.setCursor(0,1);
        lcd.print("\177        Imposta");
        if(digitalRead(pulsanteSx)){
          indiceMenu=0;
          delay(300);
          lcd.clear();
        }
        if(digitalRead(pulsanteDx)){
          indiceMenu=2;
          indiceSottomenu=1;
          delay(300);
          lcd.clear();
        }
        break;
        }
  
  }

