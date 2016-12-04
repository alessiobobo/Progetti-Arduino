#include "U8glib.h"

int angle=0;
float angolo;

U8GLIB_ST7920_128X64_4X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

void drawCorCirc(int x0, int y0, int r1, int r2, int a1, int a2){
  int xmax=x0+r2;
  int ymax=y0+r2;
  int xmin=x0-r2;
  int ymin=y0-r2;
  
  
  int x=xmin;
  int y=ymin;

  while(x<=(xmax)&&y<=(ymax)){

    int rq=(x-x0)*(x-x0)+(y-y0)*(y-y0);
    if(x==x0){angolo=90;}
    else if(y==y0&&x>=x0){angolo=180;}
    else{angolo=atan((float(y-y0)/float(x-x0)))/6.28*360;};
    if(angolo<0){angolo=angolo+180;};
    if(y-y0>0){angolo=angolo+180;};
    if((rq<=(r2*r2))&&(rq>=(r1*r1))&&angolo>=a1&&angolo<=(a1+a2)){
      u8g.drawPixel(x,y);
    }
    y++;
    if(y>ymax){
    x++;
    y=ymin;
    }
  }
}



void draw(void) {
  drawCorCirc(32,32,16,20,angle,20);
}


void setup(void) {

  u8g.setColorIndex(1);         // pixel on
  Serial.begin(9600);

}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  angle=angle+10;
}

