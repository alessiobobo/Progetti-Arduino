#include <U8glib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Time.h>  
#include <Wire.h>  

//---Pinout----------------------------------------------------------------------------

#define ONE_WIRE_BUS 2
#define U8G_sck 13
#define U8G_mosi 11
#define pin_retro 4

//-------------------------------------------------------------------------------------

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
U8GLIB_ST7920_128X64_4X u8g(13, 11);


//---VARIABILI-------------------------------------------------------------------------

  bool retro=0; //retroilluminazione
  bool rele[4]={0,0,0,0};
  int temp;
  char* temperatura;
  int livello;
//  char* testo1="    prova1            ";
//  char* testo2="    prova2            ";
  int ora[3];
  int data[3];
  char* giornosett;
  bool oralegale;
  bool bisestile;
  int daa;
  long JD;
  float intensitaluce;

  
  bool errore[]={0,0,0,0,0}; //{termometro, sensore livello, sensore corrente, sensore flusso, orologio}

  int menulivello;
  bool luce[48];

//-------------------------------------------------------------------------------------

//---draw()----------------------------------------------------------------------------

void draw(int i){
  switch(i){
    case(0):
      u8g.setColorIndex(1);
      u8g.setFont(u8g_font_osr26n);
      u8g.drawStr( 3, 29, temperatura);
      u8g.setFont(u8g_font_fur17r);
      u8g.drawCircle(46,14,2);
      u8g.drawStr( 53, 29, "C");
      u8g.drawFrame(110,0,18,64);
      u8g.drawBox(112,20, 14, 42);
      //u8g.drawStr( 0, 54, testo1);
    break;
  }

}

//---lampeggio retroilluminazione------------------------------------------------------

void lampretro(){
  if (ora[2]%2==0){
    retro!=retro;
  }
}

//---intensità illuminazione-----------------------------------------------------------

void intens(){
  switch(data[1]){
    case(1):
      daa=data[0];
      break;
    case(2):
      daa=data[0]+31;
      break;
    case(3):
      daa=data[0]+59;
      if (bisestile) daa=daa+1;
      break;
    case(4):
      daa=data[0]+90;
      if (bisestile) daa=daa+1;
      break;
    case(5):
      daa=data[0]+120;
      if (bisestile) daa=daa+1;
      break;
    case(6):
      daa=data[0]+151;
      if (bisestile) daa=daa+1;
      break;
    case(7):
      daa=data[0]+181;
      if (bisestile) daa=daa+1;
      break;
    case(8):
      daa=data[0]+212;
      if (bisestile) daa=daa+1;
      break;
    case(9):
      daa=data[0]+243;
      if (bisestile) daa=daa+1;
      break;
    case(10):
      daa=data[0]+273;
      if (bisestile) daa=daa+1;
      break;
    case(11):
      daa=data[0]+304;
      if (bisestile) daa=daa+1;
      break;
    case(12):
      daa=data[0]+334;
      if (bisestile) daa=daa+1;
      break;
  }
  //float delta=(-7.64*sin(daa-2)+9.86*sin(2*daa-160))*3.1416/180;
  //float alfa= acos(-0.2155*cos(1.5707-delta)+0.9765*sin(1.5707-delta)*cos(((ora[0]+(ora[1]/60)-12)*0.2618)));
  float T=0.01*(data[2]-2000+(daa-1)/365.25);
  float a=(0.993133+99.97361*T);
  float M=2*3.1416*(a-(int)a);
  float DL=6893*sin(M)+72*sin(2*M);
  float b=0.7859453+M/6.2832+(6191.2*T+DL)/1296000;
  float L=2*3.1416*(b-(int)b);
  float X=cos(L);
  float Y=0.91748*sin(L);
  float Z=0.39778*sin(L);
  float r=sqrt(1-Z*Z);
  float delta;
  if (r==0&&Z>0) delta=3.1416/2;
  else if (r==0&&Z<0) delta=3.1416*3/2;
  else if (r>0&&Z==0) delta=0;
  else if (r<0&&Z==0) delta=3.1416;
  else delta=/*(360/6.2832)*/atan(Z/r);
  if (r<0&&Z>0) delta=delta+(3.1416/2);
  if (r<0&&Z<0) delta=delta+(3.1416);
  if (r>0&&Z<0) delta=delta+(3.1416*3/2);
  float alfa=/*(48/6.2832)*/atan(Y/(X+r));
  float altezza=asin(-0.2155*sin(delta)+0.9765*cos(delta)*cos(((float)ora[0]-12+(float)ora[1]/60)*3.1416/12));
  if (altezza>0) intensitaluce=1023*sin(altezza);
}


//---aggiornamento data e ora----------------------------------------------------------
void aggdo(){
  time_t RTC=now();
  ora[1]=minute(RTC);
  ora[2]=second(RTC);
  if (data[1]=3 && data[0]>=24 && weekday(RTC)==1 && ora[0]==2 && ora[1]==0 && ora[2]==0){
    oralegale=1;
  }
  if (data[1]=10 && data[0]>=25 && weekday(RTC)==1 && ora[0]==2 && ora[1]==0 && ora[2]==0){
    oralegale=0;
  }
  if (oralegale==1){  
    ora[0]=hour(RTC)+1;
  }
  if (oralegale==0){
    ora[0]=hour(RTC);
  }
  if (data[0]!=day(RTC)){
    data[0]=day(RTC);
    switch (weekday(RTC)){
      case 1:
      giornosett="Domenica";
      break;
      case 2:
      giornosett="Lunedi";
      break;
      case 3:
      giornosett="Martedi";
      break;
      case 4:
      giornosett="Mercoledi";
      break;
      case 5:
      giornosett="Giovedi";
      break;
      case 6:
      giornosett="Venerdi";
      break;
      case 7:
      giornosett="Sabato";
      break;
    }
    data[1]=month(RTC);
    if (data[2]!=year(RTC)){
      data[2]=year(RTC);
      if (data[2]%4==0 && (data[2]%100!=0 || data[2]%400==0)){
        bisestile=1;
      }
      else{
        bisestile=0;
      }
    }
    JD=(1461*(data[2]+4800+(data[1]-14)/12))/4+(367*(data[1]-2-12*((data[1]-14)/12)))/12-(3*((data[2]+4900+(data[1]-14)/12)/100))/4+data[0]-32075;
  }

 }


//------------------------------------------------------------------------------------

void setup(void) {
  
  sensors.begin();
  Serial.begin(9600);
  pinMode(pin_retro, OUTPUT);
  pinMode(5,INPUT);  
  pinMode(6,INPUT);  
  pinMode(7,INPUT);  
  pinMode(8,INPUT);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);  
  digitalWrite(6,LOW);  
  digitalWrite(7,LOW);  
  digitalWrite(8,LOW);

}  
    
void loop(void) {
  
  //---Orologio-----------------------------------------------------------------------
  
   
  //---temperatura--------------------------------------------------------------------  

  sensors.requestTemperatures(); // Invia il comando di lettura delle temperatura
  temp=100*sensors.getTempCByIndex(0);
  sprintf(temperatura,"%02d.%01d", temp/100, (temp/10)-((temp/100)*10));

  //---picture loop-------------------------------------------------------------------
  u8g.firstPage();  
  do {
    draw(0);
  } while( u8g.nextPage() );
  delay(500);
  //----------------------------------------------------------------------------------
}

