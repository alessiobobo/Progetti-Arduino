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

  float etal;       //età della luna
  float etalfrac;   //frazione visibile della luna

  float zhors;
  float altezzas;
  float altezzal;
  bool retro=0; //retroilluminazione
  bool rele[4]={0,0,0,0};
  int temp;
  String temperatura;
  int livello;
  String testo1="    prova1            ";
  String testo2="    prova2            ";
  int ora[3]={5,0,0};
  long data[3]={17,8,2014};
  String giornosett;
  bool oralegale;
  bool bisestile;
  int daa=1;
  int intensitaluce=0;
  int intensitaluceG=0;
  int intensitaluceB=0;
  int intensitaluceW=0;
  int sogliatramontoalba=5;

  
  bool errore[]={0,0,0,0,0}; //{termometro, sensore livello, sensore corrente, sensore flusso, orologio}

  int menulivello;
  bool luce[48];
  
void intens(){


long d;  //numero del giorno
float dfloat;
float ecl;  //inclinazione eclittica in gradi
float GMST0;
float GMST;
float LST;
#define longitudine 34.468
#define latitudine 12.445

//---sole-----------------------------------
#define Ns 0 //longitudine del nodo ascendente
#define is 0 //inclinazione rispetto all'eclittica
float ws; //argomento del perielio
#define as 1 //semiasse maggiore (UA)
float es; //eccentricità
float Ms; //anomalia media
float Es; //anomalia eccentricità
float xs;
float ys;
float xvs;  //x
float yvs;  //y
float vs; //anomalia reale
float rs; //distanza
float lons; //longitudine vera
float xes;  //x equatoriale
float yes;  //y equatoriale
float zes;  //z equatoriale
float RAs;  //ascensione retta
float DECs; //declinazione
float Ls; //longitudine media
float HAs;
float x2s;
float y2s;
float z2s;
float xhors;
float yhors;

float azimuts;
//------------------------------------------

//---luna-----------------------------------
float Nl;         //longitudine del nodo ascendente
#define il 5.1454*3.1416/180 //inclinazione rispetto all'eclittica
float wl;         //argomento del perielio
#define al 60.2666     //semiasse maggiore (raggi terrestri)
#define el 0.0549      //eccentricità
float Ml;         //anomalia media
float El;         //anomalia eccentricità
float E2l;          //doppione per iterazione
float xvl;
float yvl;
float xhl;
float yhl;
float zhl;
float vl;
float rl;
float xgl;          //x eclittica
float ygl;          //y eclittica
float zgl;          //z eclittica
float xel;          //x equatoriale
float yel;          //y equatoriale
float zel;          //z equatoriale
float lonecl;       //longitudine eclittica
float latecl;       //latitudine eclittica
float RAl;          //ascensione retta
float DECl;         //declinazione
float Ll;         //longitudine media
float HAl;
float xl;
float yl;
float zl;
float xhorl;
float yhorl;
float zhorl;
float azimutl;

//------------------------------------------

float D;
float F;


  d=367*data[2]-7*(data[2]+(data[1]+9)/12)/4+275*data[1]/9+data[0]-730530;
  dfloat=d+(float)ora[0]/24+(float)ora[1]/1440+(float)ora[2]/86400;
  
  ecl=(23.4393-3.563E-7*dfloat)*3.1416/180;
  //---sole-----------------------------------
  ws=(282.9404+4.70935E-5*dfloat)*3.1416/180;
  es=0.016709-1.151E-9*dfloat;
  Ms=(356.0470+0.9856002585*dfloat)*3.1416/180;
  while(Ms>6.2832) Ms=Ms-6.2832;
  while(Ms<0) Ms=Ms+6.2832;
  Ls=Ms+ws; //longitudine media del sole (Ns=0)
 
  //---tempi----------------------------------
  GMST0=Ls+3.1416;
  GMST=GMST0+((((float)ora[0])+((float)ora[1])/60+((float)ora[2])/3600)*6.2832/24);
  LST=GMST+longitudine*3.1416/180;




  Es=Ms+es*sin(Ms)*(1+es*cos(Ms));
  xvs=cos(Es)-es;
  yvs=sqrt(1-es*es)*sin(Es);
  vs=atan2(yvs,xvs);
  rs=sqrt(xvs*xvs+yvs*yvs);
  lons=vs+ws;
  xs=rs*cos(lons);
  ys=rs*sin(lons);
  xes=xs;
  yes=ys*cos(ecl);
  zes=ys*sin(ecl);
  RAs=atan2(yes,xes);
  DECs=atan2(zes,sqrt(xes*xes+yes*yes));
  Ls=Ms+ws;

  HAs=LST-RAs;
  //Serial.print(GMST0,4);

  x2s=cos(HAs)*cos(DECs);
  y2s=sin(HAs)*cos(DECs);
  z2s=sin(DECs);
  xhors=x2s*sin(latitudine*3.1416/180)-z2s*cos(latitudine*3.1416/180);
  yhors=y2s;
  zhors=x2s*cos(latitudine*3.1416/180)+z2s*sin(latitudine*3.1416/180);
  azimuts=atan2(yhors,xhors)+3.1416;
  altezzas=asin(zhors);
  /*Serial.print(HAs);
  Serial.print(" ");
  Serial.print(GMST0);
  Serial.print(" ");
  Serial.print(ws);
  Serial.print(" ");
  Serial.println(RAs);*/

  //---luna-----------------------------------
  Nl=(125.1228-0.0529538083*dfloat)*3.1416/180;
  wl=(318.0634+0.1643573223*dfloat)*3.1416/180;
  Ml=(115.3654+13.0649929509*dfloat)*3.1416/180;
  while(Ml>6.2832) {int frazintera;frazintera=(int)(Ml/6.2832);Ml=Ml-6.2832*(float)frazintera;}
  while(Ml<0) {int frazintera;frazintera=(int)(Ml/6.2832);Ml=Ml-6.2832*(float)frazintera;}
  Ll=Ml+wl+Nl;  //longitudine media della luna
  El=Ml+el*sin(Ml)*(1+el*cos(Ml));
  E2l=El-(El-el*sin(El)-Ml)/(1-el*cos(El));
  while (abs(El-E2l)>(0.001*3.1416/180)){El=E2l;E2l=El-(El-el*sin(El)-Ml)/(1-el*cos(El));Serial.println("while3");}
  xvl=al*(cos(El)-el);
  yvl=al*(sqrt(1-el*el)*sin(El));
  vl=atan2(xvl,yvl);
  rl=sqrt(xvl*xvl+yvl*yvl);

  xhl=rl*(cos(Nl)*cos(vl+wl)-sin(Nl)*sin(vl+wl)*cos(il));
    yhl=rl*(sin(Nl)*cos(vl+wl)+cos(Nl)*sin(vl+wl)*cos(il));
    zhl=rl*(sin(vl+wl)*sin(il));

  D=Ll-Ls;  //elongazione media luna
  F=Ll-Nl;  //argomento della latitudine della luna

  lonecl=atan2(yhl,xhl);
  lonecl=lonecl+(3.82394E-5*(365.2422*(data[2]-2000.0)-d))*3.1416/180;  //correzione per la precessione
  lonecl=lonecl+(-1.274*sin(Ml-2*D)-1.274*sin(Ml-2*D)-0.186*sin(Ms)-0.059*sin(2*Ml-2*D)-0.057*sin(Ml-2*D+Ms)+0.053*sin(Ml+2*D)+0.046*sin(2*D-Ms)+0.041*sin(Ml-Ms)-0.035*sin(D)-0.031*sin(Ml+Ms)-0.015*sin(2*F-2*D)+0.011*sin(Ml-4*D))*3.1416/180; //correzioni agli ordini superiori: precisione 2 arcominuti
  latecl=atan2(zhl,sqrt(xhl*xhl+yhl*yhl));
  latecl=latecl+(-0.173*sin(F-2*D)-0.055*sin(Ml-F-2*D)-0.046*sin(Ml+F-2*D)+0.033*sin(F+2*D)+0.017*sin(2*Ml+F))*3.1416/180;  //correzioni agli ordini superiori: precisione 2 arcominuti
  rl=rl+(-0.58*cos(Ml-2*D)-0.46*cos(2*D))*3.1416/180; //correzioni agli ordini superiori: precisione 2 arcominuti

  //---coordinate corrette--------------------
  xgl=rl*cos(lonecl)*cos(latecl);
  ygl=rl*sin(lonecl)*cos(latecl);
  zgl=rl*sin(latecl);

  //---coordinate corrette equatoriali--------
  xel=xgl;
  yel=ygl*cos(ecl)-zgl*sin(ecl);
  zel=ygl*sin(ecl)+zgl*cos(ecl);
  RAl=atan2(yel,xel);
  DECl=atan2(zel, sqrt(xel*xel+yel*yel));

  //---coordinate azimutali-------------------
  HAl=LST-RAl;
  xl=cos(HAl)*cos(DECl);
  yl=sin(HAl)*cos(DECl);
  zl=sin(DECl);
  xhorl=xl*sin(latitudine*3.1416/180)-zl*cos(latitudine*3.1416/180);
  yhorl=yl;
  zhorl=xl*cos(latitudine*3.1416/180)+zl*sin(latitudine*3.1416/180);
  azimutl=atan2(yhorl,xhorl)+3.1416;
  altezzal=atan2(zhorl,sqrt(xhorl*xhorl+yhorl*yhorl));

  //---correzione coordinate tropocentriche---
  altezzal=altezzal-asin(1/rl)*cos(altezzal);
  //------------------------------------------

  //---età------------------------------------
  etal=(dfloat/29.530588);
  etal=etal-(int)etal;
  etal=etal-0.33333333;
  if (etal<0) etal=etal+1;
  etalfrac=1-((abs(etal-0.5))/0.5); //0=luna nuova, 1= luna piena
  //------------------------------------------


  if (abs(altezzal)<0.1) {
    Serial.print("alba/tramonto");
    Serial.print(ora[0]);
    Serial.print(":");
    Serial.print(ora[1]);
    Serial.print(":");
    Serial.println(ora[2]);
  }


  if (altezzas>0) {
    intensitaluce=(int)(taraturaR*zhors);
    intensitaluceG=(int)(taraturaG*zhors*((zhors*180/3.1416)/3));
    intensitaluceB=(int)(taraturaB*zhors*((zhors*180/3.1416)/5)*((zhors*180/3.1416)/5));
    if (altezzas>0.087267) {
      intensitaluceW=(int)(255*pow((sin((altezzas-0.087267)*90/85)),2));
//      intensitaluce=(int)(taraturaR*zhors)/2;
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

//        ora[2]=0;
      }
    }
    else {
      ora[1]++;
      Serial.print("anno ");Serial.println(data[2]);
      Serial.print("giorno");Serial.println(daa);
      Serial.print("ora ");Serial.print(ora[0]);Serial.print(":");Serial.print(ora[1]);Serial.print(":");Serial.println(ora[2]);
      Serial.println(altezzal*180/3.1416);
      Serial.print("Frazione visibile: ");
      Serial.println(etalfrac,6);
      delay(200);
//      ora[2]=0;
    }
 // }
//  else ora[2]++;
  
  
  intens();
  
  if ((zhors*180/3.1416)>5){
    strip.setPixelColor(0, intensitaluce,intensitaluce,intensitaluce);
    strip.setPixelColor(1, intensitaluceW,intensitaluceW,intensitaluceW);
  }
  if ((zhors*180/3.1416)<5&&(zhors*180/3.1416)>3){
    strip.setPixelColor(0, intensitaluce,intensitaluce,intensitaluceB);
  }
  if ((zhors*180/3.1416)<3){
    strip.setPixelColor(0, intensitaluce,intensitaluceG,intensitaluceB);
  }  
  strip.show();

//  strip.setPixelColor(0,taraturaR,taraturaG,taraturaB);
//  strip.show();
  

}
