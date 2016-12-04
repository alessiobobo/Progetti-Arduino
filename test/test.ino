#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

//---VARIABILI-------------------------------------------------------------------------
  int taraturaR=255;
  int taraturaG=255;
  int taraturaB=255;



  bool retro=0; //retroilluminazione
  bool rele[4]={0,0,0,0};
  int temp;
  char* temperatura;
  int livello;
  char* testo1="    prova1            ";
  char* testo2="    prova2            ";
  int ora[3]={5,0,0};
  int data[3]={1,1,2000};
  char* giornosett;
  bool oralegale;
  bool bisestile;
  int daa=1;
  long JD;
  int intensitaluce=0;
  int intensitaluceG=0;
  int intensitaluceB=0;
  int intensitaluceW=0;
  float altezza;
  float altezza2;
  float sinaltezza;
  float altezzagrad;
  int sogliatramontoalba=5;

  
  bool errore[]={0,0,0,0,0}; //{termometro, sensore livello, sensore corrente, sensore flusso, orologio}

  int menulivello;
  bool luce[48];
  
void intens(){

  //float delta=(-7.64*sin(daa-2)+9.86*sin(2*daa-160))*3.1416/180;
  //float alfa= acos(-0.2155*cos(1.5707-delta)+0.9765*sin(1.5707-delta)*cos(((ora[0]+(ora[1]/60)-12)*0.2618)));
  float T=0.01*(data[2]-2000+(daa-1)/365.25);
  float a=(0.993133+99.97361*T);
  float M=2*3.1416*(a-(int)a);
  float DL=6893*sin(M)+72*sin(2*M);
  float b=0.7859453+M/6.2832+(6191.2*T+DL)/1296000;
  float L=2*3.1416*(b-(int)b);
  //float X=cos(L);
  //float Y=0.91748*sin(L);
  float Z=0.39778*sin(L);
  float r=sqrt(1-Z*Z);
  float delta;
//  if (r==0&&Z>0) delta=3.1416/2;
//  else if (r==0&&Z<0) delta=3.1416*3/2;
//  else if (r>0&&Z==0) delta=0;
//  else if (r<0&&Z==0) delta=3.1416;
/*  else*/ delta=/*(360/6.2832)*/atan(Z/r);
//  if (r<0&&Z>0) delta=delta+(3.1416/2);
//  if (r<0&&Z<0) delta=delta+(3.1416);
//  if (r>0&&Z<0) delta=delta+(3.1416*3/2);
//  float alfa=/*(48/6.2832)*/atan(Y/(X+r));
  sinaltezza=(-0.2155*sin(delta)+0.9765*cos(delta)*cos(((float)ora[0]-12+(float)ora[1]/60+(float)ora[2]/3600)*3.1416/12));
  altezza=asin(sinaltezza);
  altezzagrad=altezza*180/3.1416;
  if (altezza>0) {
    intensitaluce=(int)(taraturaR*sinaltezza);
    intensitaluceG=(int)(taraturaG*sinaltezza*(altezzagrad/3));
    intensitaluceB=(int)(taraturaB*sinaltezza*(altezzagrad/5)*(altezzagrad/5));
    if (altezza>0.087267) {
      intensitaluceW=(int)(255*pow((sin((altezza-0.087267)*90/85)),2));
      Serial.println(intensitaluceW);
//      intensitaluce=(int)(taraturaR*sinaltezza)/2;
    }
    else intensitaluceW=0;

  }
  else intensitaluceW=0;
}

void loop(){

 // if (ora[2]==59){
    if (ora[1]==59){
      if(ora[0]==23){
        if(daa==365){
        data[2]++;
        daa=1;
        }
        else {
          daa++;
          ora[0]=0;
          ora[1]=0;
//          ora[2]=0;
        }
      }
      else {
        ora[0]++;
        ora[1]=0;
        Serial.print("anno ");Serial.println(data[2]);
        Serial.print("giorno");Serial.println(daa);
        Serial.print("ora ");Serial.print(ora[0]);Serial.print(":");Serial.print(ora[1]);Serial.print(":");Serial.println(ora[2]);
        Serial.println(altezza*180/3.1416);
//        ora[2]=0;
      }
    }
    else {
      ora[1]++;
      delay(500);
//      ora[2]=0;
    }
 // }
//  else ora[2]++;
  
  
  intens();
  
  if (altezzagrad>5){
    strip.setPixelColor(0, intensitaluce,intensitaluce,intensitaluce);
    strip.setPixelColor(1, intensitaluceW,intensitaluceW,intensitaluceW);
  }
  if (altezzagrad<5&&altezzagrad>3){
    strip.setPixelColor(0, intensitaluce,intensitaluce,intensitaluceB);
  }
  if (altezzagrad<3){
    strip.setPixelColor(0, intensitaluce,intensitaluceG,intensitaluceB);
  }  
  strip.show();

//  strip.setPixelColor(0,taraturaR,taraturaG,taraturaB);
//  strip.show();
  

}
