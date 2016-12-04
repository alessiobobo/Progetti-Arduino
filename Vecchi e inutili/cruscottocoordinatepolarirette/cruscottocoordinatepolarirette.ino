#include "U8glib.h"

float angolo=0;
int angle=0;

U8GLIB_ST7920_128X64_4X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

void drawCorCirc(int x0, int y0, int r1, int r2, int a1, int a2){
  angolo=a1;
  float a1rad= float(a1)*6.28/360;
  float a1a2rad= float (a1+a2)*6.28/360;
  float radius=r1;

  while(angolo<=(a1+a2)){
    float anglerad=angolo*6.28/360;
    int x= x0-cos(anglerad)*r1;
    int y= y0-sin(anglerad)*r1;
    int xf= x0-cos(anglerad)*r2;
    int yf= y0-sin(anglerad)*r2;

    u8g.drawLine(x,y,xf,yf);
    radius++;
    if (radius>r2){
      radius=r1;
      angolo+=1;
    }
  }
}



void draw(void) {
  drawCorCirc(64,64,55,63,0,angle);
}


void setup(void) {

  u8g.setColorIndex(1);         // pixel on

}

void loop(void) {
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  angle=angle+10;
      if (angle>180){
      angle=0;
    }

}

