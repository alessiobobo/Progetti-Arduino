#include "U8glib.h"

int angolo=0;
int angle=0;
int v=0;
char* vel;

U8GLIB_ST7920_128X64_4X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

void drawCorCirc(int x0, int y0, int r1, int r2, int a1, int a2){
  angolo=a1;
  float a1rad= float(a1)*6.28/360;
  float a1a2rad= float (a1+a2)*6.28/360;
  float radius=r1;

  while(angolo<=(a1+a2)){
    float anglerad=angolo*6.28/360;
    int x= x0-cos(anglerad)*radius;
    int y= y0-sin(anglerad)*radius;
    u8g.drawPixel(x,y);
    radius++;
    if (radius>r2){
      radius=r1;
      angolo++;
      if (angolo%12==0){
        angolo+=3;
      }
    }
  }
}



void draw(void) {
  drawCorCirc(64,64,55,63,0,angle);
  u8g.setFont(u8g_font_10x20r);
  u8g.setScale2x2();
  dtostrf(v, 2, 0, vel);
  //sprintf(vel, "%d",v);
  u8g.drawStr( 22 , 27, vel);
  u8g.undoScale();	
  u8g.setFont(u8g_font_5x8r);
  u8g.drawStr(86,54,"km/h");

}


void setup(void) {

  u8g.setColorIndex(1);         // pixel on
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  angle+=12;
  v+=6;
      if (angle>180){
      angle=0;
      v=0;
    }

}

