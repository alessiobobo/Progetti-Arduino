#include "U8glib.h"

U8GLIB_ST7920_128X64_4X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

char* dIsplay[6]={"prima riga","seconda riga","terza riga","quarta riga","quinta riga","sesta riga"};

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, dIsplay[0]);
}

void Push(char** a, char* b){
  a[0]=a[1];
  a[1]=a[2];
  a[2]=a[3];
  a[3]=a[4];
  a[4]=a[5];
  a[5]=b;
}

void setup(void) {
    u8g.setColorIndex(1);         // pixel on
}

void loop(void) {
Push(dIsplay,"ciao");
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(500);
}

