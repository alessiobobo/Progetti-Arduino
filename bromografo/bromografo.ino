#include "OneWire.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

OneWire ds(11);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//variabili -----------------------------------------------------------------------------------------------------------
extern volatile unsigned long timer0_overflow_count;
long adesso;
long start;                         //da eliminare (perché?)
int mm,ss;
int ml=0,sl=5;                      //default tempo limite
long limite;
long inizioPausa;
bool pausa;                         //da eliminare
long tempoPausa;
long inizioErrori;
bool countdown;
bool countup;
int percentuale;
bool att1=1,att2=1,att3=1,att4=1;   //default tutte le lampade attivate
float temp;
//bool mode=0;                      //0: timer (countdown) - 1: manuale (countup)
bool gL1, gL2, gL3, gL4, gT;        //guasti

//caratteri personalizzati LCD ----------------------------------------------------------------------------------------
const uint8_t charBitmap[][8]=
{
  { 0b00001100, 0b00001110, 0b00001100, 0b00001110, 0b00001100, 0b00011110, 0b00011110, 0b00001100 },   //char termometro
  { 0b00010001, 0b00001010, 0b00000100, 0b00001010, 0b00010001, 0b00000000, 0b00001110, 0b00011111 },   //char lampada guasta
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011011, 0b00000000 },   //char barra 0 di 2
  { 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011011, 0b00000000 },   //char barra 1 di 2
  { 0b00011011, 0b00011011, 0b00011011, 0b00011011, 0b00011011, 0b00011011, 0b00011011, 0b00000000 },   //char barra 2 di 2
  { 0b00010101, 0b00010101, 0b00010101, 0b00010101, 0b00010101, 0b00000000, 0b00001110, 0b00011111 },   //char lampada ON
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00011111 },   //char lampada OFF
  { 0b00000000, 0b00001110, 0b00010101, 0b00010111, 0b00010001, 0b00001110, 0b00000000, 0b00000000 }    //char timer
};

//funzione accensione UV ----------------------------------------------------------------------------------------------
void UVon(bool s1, bool s2, bool s3, bool s4)
{
  digitalWrite(4,s1);
  digitalWrite(5,s2);
  digitalWrite(6,s3);
  digitalWrite(7,s4);

  digitalWrite(8,1);
  digitalWrite(10,0);

  lcd.setCursor(10,2);
  lcd.print("UV");

  lcd.setCursor(12,2);
  if(!gL1) if(att1) lcd.print(char(5)); else lcd.print(char(6));
  
  lcd.setCursor(13,2);
  if(!gL2) if(att2) lcd.print(char(5)); else lcd.print(char(6));
  
  lcd.setCursor(14,2);
  if(!gL3) if(att3) lcd.print(char(5)); else lcd.print(char(6));
  
  lcd.setCursor(15,2);
  if(!gL4) if(att4) lcd.print(char(5)); else lcd.print(char(6));
}

//funzione spegnimento UV ---------------------------------------------------------------------------------------------
void UVoff()
{
  digitalWrite(4,0);
  digitalWrite(5,0);
  digitalWrite(6,0);
  digitalWrite(7,0);

  digitalWrite(8,0);
  digitalWrite(10,1);

  lcd.setCursor(10,2);
  lcd.print("UV");
  
  lcd.setCursor(12,2);
  if(!gL1) lcd.print(char(6));
  
  lcd.setCursor(13,2);
  if(!gL2) lcd.print(char(6));
  
  lcd.setCursor(14,2);
  if(!gL3) lcd.print(char(6));
  
  lcd.setCursor(15,2);
  if(!gL4) lcd.print(char(6));
}

//funzione lettura temperatura ----------------------------------------------------------------------------------------
float getTemp()
{
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr))
  {
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7])
  {
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28)
  {
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);
 
  for (int i = 0; i < 9; i++)
  {
    data[i] = ds.read();
  }
  
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB);
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
}

//setup ---------------------------------------------------------------------------------------------------------------
void setup()
{
  //inizializzazione seriale ----------------------------------------------------------------------
  Serial.begin(9600);

  //inizializzazione LCD --------------------------------------------------------------------------
  lcd.begin(16,4);
  lcd.backlight();
  
  //inserimento caratteri personalizzati in memoria LCD -------------------------------------------
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
  for ( int i = 0; i < charBitmapSize; i++ )
  {
    lcd.createChar(i, (uint8_t *)charBitmap[i]);
  }

  //pinMode ---------------------------------------------------------------------------------------
  pinMode(0, INPUT);    //pulsante -
  pinMode(1, INPUT);    //pulsante +
  pinMode(2, INPUT);    //pulsante OK
  pinMode(3, INPUT);    //pulsante UV start/stop
  pinMode(4, OUTPUT);   //relé lampada 1
  pinMode(5, OUTPUT);   //relé lampada 2
  pinMode(6, OUTPUT);   //relé lampada 3
  pinMode(7, OUTPUT);   //relé lampada 4
  pinMode(8,  OUTPUT);  //led UV ON
  pinMode(9,  OUTPUT);  //led ERROR
  pinMode(10, OUTPUT);  //led UV OFF
  //pin 11              //termometro one-wire
  //pin 12
  pinMode(13, OUTPUT);  //cicalino
  pinMode(A0,INPUT);    //fotoresistenza lampada 1
  pinMode(A1,INPUT);    //fotoresistenza lampada 2
  pinMode(A2,INPUT);    //fotoresistenza lampada 3
  pinMode(A3,INPUT);    //fotoresistenza lampada 4
  //pin A4              //SDA
  //pin A5              //SCL

  //led UV OFF acceso all'inizio ------------------------------------------------------------------
  digitalWrite(10,1);

  //impostazione timer iniziale (= limite) --------------------------------------------------------
  mm=ml;
  ss=sl;
}

//loop ----------------------------------------------------------------------------------------------------------------
void loop()
{
  //test in seriale -------------------------------------------------------------------------------
  Serial.print(adesso);
  Serial.print("\t");
  Serial.print(start);
  Serial.print("\t");
  Serial.print(pausa);
  Serial.print("\t");
  Serial.print(limite);
  Serial.print("\t");
  Serial.println(countdown);
  
  //impostazione adesso (in ms) -------------------------------------------------------------------
  adesso = timer0_overflow_count;
  
  //impostazione limite (in ms) -------------------------------------------------------------------
  limite = ml*60.*1000.+sl*1000.;
  
  //scrittura timer su LCD ------------------------------------------------------------------------
  lcd.setCursor(0,0);
  lcd.print(char(7));
  lcd.setCursor(3,0);
  if(mm<10) lcd.print(0);
  lcd.print(mm);
  lcd.print(":");
  if(ss<10) lcd.print(0);
  lcd.print(ss);
  lcd.print(" / ");
  if(ml<10) lcd.print(0);
  lcd.print(ml);
  lcd.print(":");
  if(sl<10) lcd.print(0);
  lcd.print(sl);

  //scrittura percentuale su LCD ------------------------------------------------------------------
  lcd.setCursor(0,1);
  if(percentuale<10) lcd.print("  ");
  else if(percentuale<100) lcd.print(" ");
  lcd.print(percentuale);
  lcd.print("%");
  
  //scrittura barra di stato su LCD ---------------------------------------------------------------
  for(int i=0; i<10; i++)
  {
    lcd.setCursor(6+i,1);
    if(10*(i+1)<=percentuale) lcd.print(char(4));
    else if(10*(i+1)-5<=percentuale) lcd.print(char(3));
    else lcd.print(char(2));
  }

  //lettura pulsante UV start/stop ----------------------------------------------------------------
  if(digitalRead(3))
  {
    delay(200);
    if(countdown)
    {
      UVoff();
      countdown=0;
      inizioPausa=adesso;
      pausa=1;
      lcd.setCursor(1,0);
      lcd.print(char(4));
    }
    else
    {
      UVon(att1,att2,att3,att4);
      //if(start<0) start=adesso;
      countdown=1;
      tempoPausa+=adesso-inizioPausa;
      pausa=0;
      lcd.setCursor(1,0);
      lcd.print(" ");
    }
  }

  //countdown -------------------------------------------------------------------------------------
  if(countdown)
  {

    mm=(int)(ceil((limite+tempoPausa-(adesso-start))/1000.))/60;
    ss=(int)(ceil((limite+tempoPausa-(adesso-start))/1000.))%60;
    
    percentuale=(int)((double)(adesso-start-tempoPausa)/(double)limite*100.);

    UVon(att1, att2, att3, att4);
  }
  
  //fine countdown --------------------------------------------------------------------------------
  if(adesso-start>=limite+tempoPausa)
  {
    countdown=0;
    start=-1;
    tempoPausa=0;
    inizioPausa=0;
    UVoff();
  }

  //scrittura temperatura su LCD ------------------------------------------------------------------
  lcd.setCursor(0,2);
  lcd.print(char(0));
  temp=getTemp();
  if(temp==-1000) lcd.print("--.-");
  else if(temp<10) lcd.print(" ");
  if(temp>-1000) lcd.print(temp,1);
  lcd.send(0xDF, DATA);
  lcd.print("C");
  
  //controllo guasti lampade ----------------------------------------------------------------------
  if(countdown || countup)
  {
    if(adesso-tempoPausa-start>1000)
    {
      if(analogRead(A0)<100)
      {
        if(att1) gL1=1;
      }
      else gL1=0;

      if(analogRead(A1)<100)
      {
        if(att2) gL2=1;
      }
      else gL2=0;

      if(analogRead(A2)<100)
      {
        if(att3) gL3=1;
      }
      else gL3=0;

      if(analogRead(A3)<100)
      {
        if(att4) gL4=1;
      }
      else gL4=0;
    }
    else
    {
      gL1=0;
      gL2=0;
      gL3=0;
      gL4=0;
    }
  }

  //controllo guasto termometro -------------------------------------------------------------------
  if(temp==-1000) gT=1;
  else gT=0;

  //scrittura guasti lampade su LCD ---------------------------------------------------------------
  if(gL1 || gL2 || gL3 || gL4)
  {
    if(inizioErrori<0) inizioErrori=adesso;
    
    if((adesso-inizioErrori)%1500<750)
    {
      lcd.setCursor(0,3);
      lcd.print("GUASTO LAMP ");

      if(gL1)
      {
        lcd.setCursor(12,3);
        lcd.print(1);
        lcd.setCursor(12,2);
        lcd.print(char(1));
      }
      else
      {
        lcd.setCursor(12,3);
        lcd.print(" ");
      }

      if(gL2)
      {
        lcd.setCursor(13,3);
        lcd.print(2);
        lcd.setCursor(13,2);
        lcd.print(char(1));
      }
      else
      {
        lcd.setCursor(13,3);
        lcd.print(" ");
      }

      if(gL3)
      {
        lcd.setCursor(14,3);
        lcd.print(3);
        lcd.setCursor(14,2);
        lcd.print(char(1));
      }
      else
      {
        lcd.setCursor(14,3);
        lcd.print(" ");
      }

      if(gL4)
      {
        lcd.setCursor(15,3);
        lcd.print(4);
        lcd.setCursor(15,2);
        lcd.print(char(1));
      }
      else
      {
        lcd.setCursor(15,3);
        lcd.print(" ");
      }
    }
  }

  //scrittura guasto termometro su LCD ------------------------------------------------------------
  if(gT)
  {
    if(inizioErrori<0) inizioErrori=adesso;

    if((adesso-inizioErrori)%1500>750)
    {
      lcd.setCursor(0,3);
      lcd.print("GUASTO TERMOM.  ");
    }
  }
  
  //scrittura "nessun guasto" su LCD --------------------------------------------------------------
  if(!(gL1 || gL2 || gL3 || gL4 || gT))
  {
    inizioErrori=-1;
    lcd.setCursor(0,3);
    lcd.print("NESSUN GUASTO   ");
  }

  //blocco se TUTTE le lampade guaste -------------------------------------------------------------
  if((countdown || countup) && gL1 && gL2 && gL3 && gL4)
  {
    UVoff();
    countdown=0;
    inizioPausa=adesso;
    pausa=1;
    lcd.setCursor(1,0);
    lcd.print(char(4));
  }
}