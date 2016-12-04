#include <U8glib.h>
#include <avr/pgmspace.h>
#include "optiLoader.h"

U8GLIB_ST7920_128X64_4X u8g(2,3,4);

char* dIsplay[6]={"                   ","                   ","     OptiLoader     ","2011 by B. Westfield","                   ","                   "};

void draw(void) {
  u8g.setFont(u8g_font_6x10r);
  
  u8g.drawStr( 4, 10, dIsplay[0]);
//  Serial.println(dIsplay[0]);
  u8g.drawStr( 4, 20, dIsplay[1]);
//  Serial.println(dIsplay[1]);
  u8g.drawStr( 4, 30, dIsplay[2]);
//  Serial.println(dIsplay[2]);
  u8g.drawStr( 4, 40, dIsplay[3]);
//  Serial.println(dIsplay[3]);
  u8g.drawStr( 4, 50, dIsplay[4]);
//  Serial.println(dIsplay[4]);
  u8g.drawStr( 4, 60, dIsplay[5]);
//  Serial.println(dIsplay[5]);
}

void Clear(char** a){
  for(int i=0; i<6; i++){
    a[i]="";
  }
}

void Push(char** a, char* b){
  for(int i=0; i<6; i++){ 
    if(a[i]==""){
      a[i]=b;
      break;
    }
    else if(i==5){
      for(int ii=0; ii<5; ii++){
        a[ii]=a[ii+1];
      }
      a[i]=b;
      break;
    }
  }
//}

/*void Push(char** a, char* b){
a[0]=a[1];
a[1]=a[2];
a[2]=a[3];
a[3]=a[4];
a[4]=a[5];
a[5]=b;*/

    u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

delay(300);

}

char Arduino_preprocessor_hint;

/*
 * Pins to target
 */
#define SCK 52
#define MISO 50
#define MOSI 51
#define RESET 10
#define POWER 7

#define pinButton 5

// STK Definitions; we can still use these as return codes
#define STK_OK 0x10
#define STK_FAILED 0x11


// Useful message printing definitions
#define fp(string) flashprint(PSTR(string));
#define debug(string) // flashprint(PSTR(string));
#define error(string) flashprint(PSTR(string));

// Forward references
void pulse(int pin, int times);
void read_image(image_t *ip);

// Global Variables

/*
 * Table of defined images
 */
image_t *images[] = {
  &image_328p, 0
};

int pmode=0;
// address for reading and writing, set by 'U' command
int here;

uint16_t target_type = 0;		/* type of target_cpu */
uint16_t target_startaddr;
uint8_t target_pagesize;       /* Page size for flash programming (bytes) */
uint8_t *buff;

image_t *target_flashptr; 	       /* pointer to target info in flash */
uint8_t target_code[512];	       /* The whole code */


void setup () {
  Serial.begin(19200); 			/* Initialize serial for status msgs */
  pinMode(13, OUTPUT); 			/* Blink the pin13 LED a few times */
  pulse(13,20);

  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  delay(1000);
  
  /*for(int i=0; i<6 ;i++){
    Push(dIsplay,"");
  }*/
  Clear(dIsplay);
  
  pinMode(pinButton, INPUT);

}

void loop (void) {
fp("\nOptiLoader Bootstrap programmer.\n2011 by Bill Westfield (WestfW)\n\n");
  if (target_poweron()) {		/* Turn on target power */
    do {
      if (!target_identify()) 		/* Figure out what kind of CPU */
        break;
      if (!target_findimage())		/* look for an image */
        break;
      if (!target_progfuses())		/* get fuses ready to program */
        break;
      if (!target_program()) 		/* Program the image */
        break;
      (void) target_normfuses(); 	/* reset fuses to normal mode */
    } 
    while (0);
  } 
  else {
    Serial.println();
  }
  target_poweroff(); 			/* turn power off */

  fp ("\nPress Button or hit RESET for next chip\n")
  Push (dIsplay,"Press Button for next");

    while (1) {
      if (digitalRead(pinButton))
        break;
    }

}

/*
 * Low level support functions
 */

/*
 * flashprint
 * print a text string direct from flash memory to Serial
 */
void flashprint (const char p[])
{
  uint8_t c;
  while (0 != (c = pgm_read_byte(p++))) {
    Serial.write(c);
  }
}

/*
 * hexton
 * Turn a Hex digit (0..9, A..F) into the equivalent binary value (0-16)
 */
uint8_t hexton (uint8_t h)
{
  if (h >= '0' && h <= '9')
    return(h - '0');
  if (h >= 'A' && h <= 'F')
    return((h - 'A') + 10);
  error("Bad hex digit!");
}

/*
 * pulse
 * turn a pin on and off a few times; indicates life via LED
 */
#define PTIME 30
void pulse (int pin, int times) {
  do {
    digitalWrite(pin, HIGH);
    delay(PTIME);
    digitalWrite(pin, LOW);
    delay(PTIME);
  } 
  while (times--);
}

/*
 * spi_init
 * initialize the AVR SPI peripheral
 */
void spi_init () {
  uint8_t x;
  SPCR = 0x53;  // SPIE | MSTR | SPR1 | SPR0
  x=SPSR;
  x=SPDR;
}

/*
 * spi_wait
 * wait for SPI transfer to complete
 */
void spi_wait () {
  debug("spi_wait");
  Push (dIsplay,"spi wait");
  do {
  } 
  while (!(SPSR & (1 << SPIF)));
}

/*
 * spi_send
 * send a byte via SPI, wait for the transfer.
 */
uint8_t spi_send (uint8_t b) {
  uint8_t reply;
  SPDR=b;
  spi_wait();
  reply = SPDR;
  return reply;
}


/*
 * Functions specific to ISP programming of an AVR
 */

/*
 * target_identify
 * read the signature bytes (if possible) and check whether it's
 * a legal value (atmega8, atmega168, atmega328)
 */

boolean target_identify ()
{
  boolean result;
  target_type = 0;
  fp("\nReading signature:");
  Push (dIsplay,"Reading signature");
  target_type = read_signature();
  if (target_type == 0 || target_type == 0xFFFF) {
    fp(" Bad value: ");
    result = false;
  } 
  else {
    result = true;
  }
  Serial.println(target_type, HEX);
  if (target_type == 0) {
    fp("  (no target attached?)\n");
    Push (dIsplay,"no target attached?");
  }
  return result;
}

unsigned long spi_transaction (uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  uint8_t n, m;
  spi_send(a); 
  n=spi_send(b);
  //if (n != a) error = -1;
  m=spi_send(c);
  return 0xFFFFFF & ((n<<16)+(m<<8) + spi_send(d));
}

uint16_t start_pmode () {
  uint16_t result;

  pinMode(13, INPUT); // restore to default
  spi_init();
  debug("...spi_init done");
  Push (dIsplay,"...spi init done");
  // following delays may not work on all targets...
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, LOW);
  delay(50);
  digitalWrite(RESET, LOW);
  delay(50);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  debug("...spi_transaction");
  Push (dIsplay,"...spi transaction");
  result = spi_transaction(0xAC, 0x53, 0x00, 0x00);
  debug("...Done");
  Push (dIsplay,"...Done!");
  pmode = 1;
  return result;
}

void end_pmode () {
  SPCR = 0; 				/* reset SPI */
  digitalWrite(MISO, 0); 		/* Make sure pullups are off too */
  pinMode(MISO, INPUT);
  digitalWrite(MOSI, 0);
  pinMode(MOSI, INPUT);
  digitalWrite(SCK, 0);
  pinMode(SCK, INPUT);
  digitalWrite(RESET, 0);
  pinMode(RESET, INPUT);
  pmode = 0;
}

/*
 * read_image
 *
 * Read an intel hex image from a string in pgm memory.
 * We assume that the image does not exceed the 512 bytes that we have
 * allowed for it to have.  that would be bad.
 * Also read other data from the image, such as fuse and protecttion byte
 * values during programming, and for after we're done.
 */
void read_image (image_t *ip)
{
  uint16_t len, totlen=0, addr;
  char *hextext = &ip->image_hexcode[0];
  target_startaddr = 0;
  target_pagesize = pgm_read_byte(&ip->image_pagesize);
  uint8_t b, cksum = 0;

  while (1) {
    if (pgm_read_byte(hextext++) != ':') {
      error("No colon");
      Push (dIsplay, "No colon");
      break;
    }
    len = hexton(pgm_read_byte(hextext++));
    len = (len<<4) + hexton(pgm_read_byte(hextext++));
    cksum = len;

    b = hexton(pgm_read_byte(hextext++)); /* record type */
    b = (b<<4) + hexton(pgm_read_byte(hextext++));
    cksum += b;
    addr = b;
    b = hexton(pgm_read_byte(hextext++)); /* record type */
    b = (b<<4) + hexton(pgm_read_byte(hextext++));
    cksum += b;
    addr = (addr << 8) + b;
    if (target_startaddr == 0) {
      target_startaddr = addr;
      fp("  Start address at ");
      Serial.println(addr, HEX);
      
      char* temp;
      sprintf(temp,"%h", (addr, HEX));
      Push (dIsplay, strcat ("Start address at ", temp));
      
    } 
    else if (addr == 0) {
      break;
    }

    b = hexton(pgm_read_byte(hextext++)); /* record type */
    b = (b<<4) + hexton(pgm_read_byte(hextext++));
    cksum += b;

    for (uint8_t i=0; i < len; i++) {
      b = hexton(pgm_read_byte(hextext++));
      b = (b<<4) + hexton(pgm_read_byte(hextext++));
      if (addr - target_startaddr >= sizeof(target_code)) {
        error("Code extends beyond allowed range");
        Push (dIsplay,"Code extends beyond");
        Push (dIsplay,"allowed range");
        break;
      }
      target_code[addr++ - target_startaddr] = b;
      cksum += b;
#if VERBOSE
      Serial.print(b, HEX);
      char* temp;
      sprintf(temp,"%h", (b,HEX));
      Push (dIsplay,temp);
      Serial.write(' ');
#endif
      totlen++;
      if (totlen >= sizeof(target_code)) {
        error("Too much code");
        Push (dIsplay,"Too much code");
        break;
      }
    }
    b = hexton(pgm_read_byte(hextext++)); /* checksum */
    b = (b<<4) + hexton(pgm_read_byte(hextext++));
    cksum += b;
    if (cksum != 0) {
      error("Bad checksum: ");
      Serial.print(cksum, HEX);
      
      char* temp;
      sprintf(temp,"%h", (cksum, HEX));
      Push (dIsplay, strcat ("Bad checksum: ", temp));
      
    }
    if (pgm_read_byte(hextext++) != '\n') {
      error("No end of line");
      Push (dIsplay, "No end of line");
      break;
    }
#if VERBOSE
    Serial.println();
#endif
  }
  fp("  Total bytes read: ");
  Serial.println(totlen);
  
  char* temp;
  sprintf(temp,"%d", totlen);
  Push (dIsplay, strcat ("Total bytes read: ", temp));

  
}

/*
 * target_findimage
 *
 * given target_type loaded with the relevant part of the device signature,
 * search the hex images that we have programmed in flash, looking for one
 * that matches.
 */

boolean target_findimage ()
{
  image_t *ip;
  fp("Searching for image...\n");
  Push(dIsplay,"Searching for image");
  for (uint8_t i=0; i < sizeof(images)/sizeof(images[0]); i++) {
    target_flashptr = ip = images[i];
    if (ip && (pgm_read_word(&ip->image_chipsig) == target_type)) {
      fp("  Found \"");
      Push(dIsplay,"Found");
      flashprint(&ip->image_name[0]);
      Push(dIsplay,&ip->image_name[0]);
      fp("\" for ");
      flashprint(&ip->image_chipname[0]);
      Push (dIsplay, strcat("for ",&ip->image_chipname[0]));
      fp("\n");
      read_image(ip);
      return true;
    }
  }
  fp(" Not Found\n");
  Push (dIsplay,"Not found");
  return(false);
}

/*
 * target_progfuses
 * given initialized target image data, re-program the fuses to allow
 * the optiboot image to be programmed.
 */

boolean target_progfuses ()
{
  uint8_t f;
  fp("\nSetting fuses for programming");
  Push (dIsplay,"Setting fuses");

  f = pgm_read_byte(&target_flashptr->image_progfuses[FUSE_PROT]);
  if (f) {
    fp("\n  Lock: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xE0, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h", (f,HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xE0, 0x00, f), HEX));
    temp=strcat("Lock: ",temp);
    strcat(temp," ");
    strcat(temp,temp2);
    Push (dIsplay, temp);
  }
  f = pgm_read_byte(&target_flashptr->image_progfuses[FUSE_LOW]);
  if (f) {
    fp("  Low: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA0, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f,HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA0, 0x00, f), HEX));
    temp=strcat("Low: ",temp);
    strcat(temp, " ");
    strcat(temp,temp2);
    Push (dIsplay, temp);
    
  }
  f = pgm_read_byte(&target_flashptr->image_progfuses[FUSE_HIGH]);
  if (f) {
    fp("  High: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA8, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f,HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA8, 0x00, f), HEX));
    temp=strcat("High: ",temp);
    strcat(temp, " ");
    strcat(temp,temp2);
    Push (dIsplay, temp);
  }
  f = pgm_read_byte(&target_flashptr->image_progfuses[FUSE_EXT]);
  if (f) {
    fp("  Ext: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA4, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f,HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA4, 0x00, f), HEX));
    temp=strcat("Ext: ",temp);
    strcat(temp, " ");
    strcat(temp,temp2);
    Push (dIsplay, temp);
  }
  Serial.println();
  return true; 			/* */
}

/*
 * target_program
 * Actually program the image into the target chip
 */

boolean target_program ()
{
  int l; 				/* actual length */

  fp("\nProgramming bootloader: ");
  Push (dIsplay,"Programming bootloader");
  here = target_startaddr>>1; 		/* word address */
  buff = target_code;
  l = 512;
  Serial.print(l, DEC);
  fp(" bytes at 0x");
  Serial.println(here, HEX);
  char* temp;
  sprintf(temp,"%d",(l, DEC));
  char* temp2;
  sprintf(temp2,"%h",(here, HEX));
  strcat(temp," bytes at ");
  strcat(temp, temp2);
  Push (dIsplay, temp);


  spi_transaction(0xAC, 0x80, 0, 0); 	/* chip erase */
  delay(1000);
  if (write_flash(l) != STK_OK) {
    error("\nFlash Write Failed");
    Push (dIsplay,"Flash write FAILED");
    return false;
  }
  return true; 			/*  */
}

/*
 * target_normfuses
 * reprogram the fuses to the state they should be in for bootloader
 * based programming
 */
boolean target_normfuses ()
{
  uint8_t f;
  fp("\nRestoring normal fuses");
  Push(dIsplay,"Restoring fuses");

  f = pgm_read_byte(&target_flashptr->image_normfuses[FUSE_PROT]);
  if (f) {
    fp("\n  Lock: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xE0, 0x00, f), HEX);
    
    
  }
  f = pgm_read_byte(&target_flashptr->image_normfuses[FUSE_LOW]);
  if (f) {
    fp("  Low: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA0, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f, HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA0, 0x00, f), HEX));
    temp=strcat("Low: ",temp);
    strcat(temp," ");
    strcat(temp, temp2);
    Push (dIsplay, temp);

  }
  f = pgm_read_byte(&target_flashptr->image_normfuses[FUSE_HIGH]);
  if (f) {
    fp("  High: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA8, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f, HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA8, 0x00, f), HEX));
    temp=strcat("High: ",temp);
    strcat(temp," ");
    strcat(temp, temp2);
    Push (dIsplay, temp);

  }
  f = pgm_read_byte(&target_flashptr->image_normfuses[FUSE_EXT]);
  if (f) {
    fp("  Ext: ");
    Serial.print(f, HEX);
    fp(" ");
    Serial.print(spi_transaction(0xAC, 0xA4, 0x00, f), HEX);
    char* temp;
    sprintf(temp,"%h",(f, HEX));
    char* temp2;
    sprintf(temp2,"%h",(spi_transaction(0xAC, 0xA4, 0x00, f), HEX));
    temp=strcat("Ext: ",temp);
    strcat(temp," ");
    strcat(temp, temp2);
    Push (dIsplay, temp);

  }
  Serial.println();
  return true; 			/* */
}

/*
 * target_poweron
 * Turn on power to the target chip (assuming that it is powered through
 * the relevant IO pin of THIS arduino.)
 */
boolean target_poweron ()
{
  uint16_t result;

  fp("Target power on! ...");
  Push(dIsplay,"Target power on!...");
  digitalWrite(POWER, LOW);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
  digitalWrite(RESET, LOW);  // reset it right away.
  pinMode(RESET, OUTPUT);
  /*
   * Check if the target is pulling RESET HIGH by reverting to input
   */
  delay(5);
  pinMode(RESET, INPUT);
  delay(1);
  if (digitalRead(RESET) != HIGH) {
    fp("No RESET pullup detected! - no target?");
    Push(dIsplay,"No reset pullup");
    Push(dIsplay,"No target?");
    return false;
  }
  pinMode(RESET, OUTPUT);

  delay(200);
  fp("\nStarting Program Mode");
  Push(dIsplay,"Start program mode");
  result = start_pmode();
  if ((result & 0xFF00) != 0x5300) {
    fp(" - Failed, result = 0x");
    Serial.print(result, HEX);
    char* temp;
    sprintf (temp, "%h", (result,HEX));
    temp=strcat("Failed, result = 0x",temp);
    Push(dIsplay,temp);
    return false;
  }
  fp(" [OK]\n");
  Push (dIsplay, "OK");
  return true;
}

boolean target_poweroff ()
{
  end_pmode();
  digitalWrite(POWER, LOW);
  delay(200);
  pinMode(POWER, INPUT);
  fp("\nTarget power OFF!\n");
  Push (dIsplay, "Target power OFF");
  return true;
}

void flash (uint8_t hilo, int addr, uint8_t data) {
#if VERBOSE
  Serial.print(data, HEX);
  fp(":");
  Serial.print(spi_transaction(0x40+8*hilo, 
  addr>>8 & 0xFF, 
  addr & 0xFF,
  data), HEX);
  fp(" ");
  char* temp;
  sprintf(temp, "%h", (data, HEX);
  char* temp2;
  sprintf(temp2, "%h", (spi_transaction(0x40+8*hilo, addr>>8 & 0xFF, addr & 0xFF, data), HEX));
  strcat(temp, ":");
  strcat(temp, temp2);
  Push (dIsplay, temp);
  
  
#else
  (void) spi_transaction(0x40+8*hilo, 
  addr>>8 & 0xFF, 
  addr & 0xFF,
  data);
#endif
}

void commit (int addr) {
  fp("  Commit Page: ");
  Serial.print(addr, HEX);
  fp(":");
  Serial.println(spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0), HEX);
  char* temp;
  sprintf(temp, "%h", (addr, HEX));
  char* temp2;
  sprintf(temp2, "%h", (spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0), HEX));
  temp=strcat("Commit page: ",temp);
  strcat(temp, ":");
  strcat(temp, temp2);
  Push(dIsplay, temp);
  delay(100);
}

//#define _current_page(x) (here & 0xFFFFE0)
int current_page (int addr) {
  if (target_pagesize == 32) return here & 0xFFFFFFF0;
  if (target_pagesize == 64) return here & 0xFFFFFFE0;
  if (target_pagesize == 128) return here & 0xFFFFFFC0;
  return here;
}

uint8_t write_flash (int length) {
  if (target_pagesize < 1) return STK_FAILED;
  //if (target_pagesize != 64) return STK_FAILED;
  int page = current_page(here);
  int x = 0;
  while (x < length) {
    if (page != current_page(here)) {
      commit(page);
      page = current_page(here);
    }
    flash(LOW, here, buff[x]);
    flash(HIGH, here, buff[x+1]);
    x+=2;
    here++;
  }

  commit(page);

  return STK_OK;
}

uint16_t read_signature () {
  uint8_t sig_middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
  uint8_t sig_low = spi_transaction(0x30, 0x00, 0x02, 0x00);
  return ((sig_middle << 8) + sig_low);
}

/*
 * Firmware images.
 * These are the intel Hex files produced by the optiboot makefile,
 * with a small amount of automatic editing to turn them into C strings,
 * and a header attched to identify them
 */


image_t PROGMEM image_328p = {
  {
    "optiboot_atmega328.hex"    }
  ,
  {
    "atmega328P"    }
  ,
  0x950F,				/* Signature bytes for 328P */
  {
    0x3F,0xE2,0xDF,0x07,0    }
  ,
  {
    0x2F,0,0,0,0    }
  ,
  128,
  {
    ":100000000C9463000C948B000C948B000C948B006C\n"
    ":100010000C948B000C948B000C948B000C94E30BD1\n"
    ":100020000C948B000C948B000C948B000C948B0024\n"
    ":100030000C948B000C948B000C948B000C948B0014\n"
    ":100040000C94A00C0C948B000C948B000C948B00E3\n"
    ":100050000C948B000C948B000C948B000C948B00F4\n"
    ":100060000C948B000C948B000000000024002700EF\n"
    ":100070002A0000000000250028002B0000000000DE\n"
    ":1000800023002600290004040404040404040202DA\n"
    ":100090000202020203030303030301020408102007\n"
    ":1000A0004080010204081020010204081020000012\n"
    ":1000B0000007000201000003040600000000000029\n"
    ":1000C0000000C700F60911241FBECFEFD8E0DEBF45\n"
    ":1000D000CDBF11E0A0E0B1E0E6E0F0E202C00590A3\n"
    ":1000E0000D92A037B107D9F712E0A0E7B1E001C047\n"
    ":1000F0001D92A03BB107E1F710E0C6ECD0E004C0D0\n"
    ":100100002297FE010E94FD0FC23CD107C9F70E9451\n"
    ":10011000420E0C9401100C940000549A08951F9301\n"
    ":10012000CF93DF93382F492F8091B2018F5F877073\n"
    ":100130008093B20180FD02C020E001C020E881FD73\n"
    ":10014000206482FD2062122F105F832F942F9C0168\n"
    ":10015000F901C0E0D0E06285662331F080E091E0F3\n"
    ":1001600070E00E94AB0F6F5F6783212F269F212DC8\n"
    ":1001700011242483662311F0615067832196319600\n"
    ":10018000C330D10541F7DF91CF911F91089588E8E1\n"
    ":1001900091E061E070E00E94FD0A08958EE4809392\n"
    ":1001A0007C0062E070E080E090E00E941C0D809195\n"
    ":1001B0007A00806480937A0080917A0086FDFCCF7B\n"
    ":1001C0008091780060917900362F20E090E0282B14\n"
    ":1001D000392B442737FD4095542F64EB7BE281E1B6\n"
    ":1001E00090E00E94E00FCA01B9010895E0E7F1E054\n"
    ":1001F000108211821282339681E0E838F807C1F745\n"
    ":10020000E0916E01F0916F01CF0160810E944D0A73\n"
    ":1002100008952F923F924F925F926F927F928F92AA\n"
    ":100220009F92AF92BF92CF92DF92EF92FF920F9385\n"
    ":100230001F93DF93CF93CDB7DEB764970FB6F894D3\n"
    ":10024000DEBF0FBECDBF9A8B898B809160019091EC\n"
    ":1002500061010196889F9001899F300D989F300D14\n"
    ":100260001124E989FA89EE0FFF1F49895A89E40FA1\n"
    ":10027000F51FEE0FFF1F8F0100501F4FD8018D910A\n"
    ":100280009C91AC01249FC001259F900D349F900D3F\n"
    ":10029000112469E170E00E94AB0F6B01EE5FFE4F2D\n"
    ":1002A00080819181DC012A9FC0012B9F900D3A9F94\n"
    ":1002B000900D112469E170E00E94AB0F7B01F80101\n"
    ":1002C00084819581AC01249FC001259F900D349FAE\n"
    ":1002D000900D112469E170E00E94AB0F6B8B7C8B59\n"
    ":1002E0002091890130918A0180919301909194012C\n"
    ":1002F0000196889F8001899F100D989F100D1124F1\n"
    ":10030000F901EE0FFF1FE20FF31FEE0FFF1F9F011A\n"
    ":1003100020503F4FD9018D919C91AC01049FC001A9\n"
    ":10032000059F900D149F900D112469E170E00E94CB\n"
    ":10033000AB0F3B01EE5FFE4F80819181DC010A9F94\n"
    ":10034000C0010B9F900D1A9F900D112469E170E080\n"
    ":100350000E94AB0F4B01F901848195819C01029FA2\n"
    ":10036000C001039F900D129F900D112469E170E070\n"
    ":100370000E94AB0F5B0140916E0150916F01588B51\n"
    ":100380004F878EEC9FEFC89ED001C99EB00DD89EBE\n"
    ":10039000B00D1124BE87AD87E89EF001E99EF00DF7\n"
    ":1003A000F89EF00D1124FC87EB874B895C89489FF0\n"
    ":1003B0009001499F300D589F300D11243A872987AD\n"
    ":1003C00000E010E01A8219821C821B8218861F82AC\n"
    ":1003D0001E821D822224332444245524209193011B\n"
    ":1003E0003091940180916001909161012817390743\n"
    ":1003F00009F08EC0023311050CF05AC080918901BA\n"
    ":1004000090918A01892B11F18D819E816EEC7FEF95\n"
    ":100410000E94AB0F660D771D709396016093950156\n"
    ":10042000C1016EEC7FEF0E94AB0F680D791D7093D8\n"
    ":10043000980160939701C2016EEC7FEF0E94AB0FB1\n"
    ":100440006A0D7B1D70939A0160939901209195012B\n"
    ":1004500030919601442737FD4095542FA901332749\n"
    ":1004600022278091970190919801AA2797FDA09546\n"
    ":10047000BA2FBA2FA92F982F8827282B392B4A2B30\n"
    ":100480005B2B8091990190919A01AA2797FDA095E5\n"
    ":10049000BA2F282B392B4A2B5B2BCA01AA27BB2743\n"
    ":1004A000682FBB27A52F942F832FE0E7F1E053C0DF\n"
    ":1004B0000233110569F41092960110929501109281\n"
    ":1004C00098011092970110929A0110929901BECF53\n"
    ":1004D00089899A89892B09F4B9CF8D859E8562E334\n"
    ":1004E00070E00E94AB0F70939601609395018B852D\n"
    ":1004F0009C8562E370E00E94AB0F7093980160935B\n"
    ":10050000970189859A8562E370E00E94AB0F9ACFCC\n"
    ":1005100089819A8162E370E00E94AB0F709396012B\n"
    ":10052000609395018B819C8162E370E00E94AB0F28\n"
    ":1005300070939801609397018F81988562E370E0D2\n"
    ":100540000E94AB0F70939A016093990102331105D9\n"
    ":1005500009F07CCF4AC0608381832283339691E087\n"
    ":10056000E838F907C1F78F859889DC016C910E9402\n"
    ":100570004D0A6AE070E080E090E00E941C0D0F5F81\n"
    ":100580001F4FE981FA81EC0DFD1DFA83E9832B8170\n"
    ":100590003C812E0D3F1D3C832B834F8158858B89D9\n"
    ":1005A0009C89480F591F58874F83AD81BE81A60D86\n"
    ":1005B000B71DBE83AD83280C391C4A0C5B1CED852E\n"
    ":1005C000FE85EC0DFD1DFE87ED872B853C852E0DF0\n"
    ":1005D0003F1D3C872B8749855A85480F591F5A87ED\n"
    ":1005E00049870436110509F0F9CE89899A899093D3\n"
    ":1005F0008A018093890180916001909161019093BB\n"
    ":1006000094018093930164960FB6F894DEBF0FBEF9\n"
    ":10061000CDBFCF91DF911F910F91FF90EF90DF90B1\n"
    ":10062000CF90BF90AF909F908F907F906F905F9092\n"
    ":100630004F903F902F9008958F929F92AF92BF92CC\n"
    ":10064000CF92DF92EF92FF920F931F93CF93DF939E\n"
    ":100650000E94E80C28EB3BE040E050E00E94BE0F17\n"
    ":100660006E3171058105910530F40E94CE007093C2\n"
    ":10067000900160938F0180918F0190919001815C36\n"
    ":10068000924124F081E08093A10108C08091A101F2\n"
    ":10069000882321F01092A1010E94F6008091A1010F\n"
    ":1006A000882309F446C380E00E94EF0D892B09F4EA\n"
    ":1006B00040C381E08093A20180918F0190919001CD\n"
    ":1006C00084559B400CF415C28091A101882309F048\n"
    ":1006D00010C28FEF9FEFAFEFBFEF80938B0190932E\n"
    ":1006E0008C01A0938D01B0938E0188E891E064EABB\n"
    ":1006F00071E00E94C10B892B49F12091A801309132\n"
    ":10070000A9014091AA015091AB012F3FFFEF3F0794\n"
    ":10071000FFEF4F07FFEF5F07A9F080918B019091EA\n"
    ":100720008C01A0918D01B0918E01281739074A07DD\n"
    ":100730005B0741F020938B0130938C0140938D0136\n"
    ":1007400050938E0188E891E00E94020B8091A301F2\n"
    ":100750008823D1F08FEF9FEFAFEFBFEF80938B0136\n"
    ":1007600090938C01A0938D01B0938E011092A30100\n"
    ":1007700010929C0110929B0110929E0110929D017B\n"
    ":100780001092A00110929F0180918B0190918C0199\n"
    ":10079000A0918D01B0918E01853A2AE592072FEF45\n"
    ":1007A000A20720E0B20709F4B8C0863AEAE59E073E\n"
    ":1007B000EFEFAE07E0E0BE070CF043C0873CF8E384\n"
    ":1007C0009F07FFEFAF07F0E0BF0709F4A3C0883C25\n"
    ":1007D00028E392072FEFA20720E0B207CCF4873E70\n"
    ":1007E000E8E19E07EFEFAE07E0E0BE0709F489C03D\n"
    ":1007F0008F3CF0E39F07FFEFAF07F0E0BF0709F47E\n"
    ":100800007DC08F5E9041AF4FB04009F095C27FC070\n"
    ":10081000853BEAE49E07EFEFAE07E0E0BE0709F490\n"
    ":1008200082C08D3AF2E59F07FFEFAF07F0E0BF0708\n"
    ":1008300009F47CC08D5B9244AF4FB04009F07CC29C\n"
    ":100840006FC08D35E2EA9E07EFEFAE07E0E0BE072E\n"
    ":1008500009F446C08E35F2EA9F07FFEFAF07F0E0DC\n"
    ":10086000BF07C4F485382AE792072FEFA20720E0DC\n"
    ":10087000B20709F449C08736E8E99E07EFEFAE07F3\n"
    ":10088000E0E0BE07A9F187599846AF4FB04009F0A4\n"
    ":1008900053C229C08F3420EB92072FEFA20720E02C\n"
    ":1008A000B20749F18035E0EB9E07EFEFAE07E0E0DD\n"
    ":1008B000BE073CF48755984AAF4FB04009F03CC2A0\n"
    ":1008C00042C08F3120EE92072FEFA20720E0B2073F\n"
    ":1008D00081F18D51924EAF4FB04009F02DC245C00D\n"
    ":1008E00080E090E002C087E090E00E94090124C20D\n"
    ":1008F00088E090E0FACF89E090E0F7CF81E090E0E7\n"
    ":10090000F4CF82E090E0F1CF83E090E0EECF84E09E\n"
    ":1009100090E0EBCF85E090E0E8CF86E090E0E5CF97\n"
    ":100920008AE090E0E2CF8BE090E0DFCF8CE090E0D7\n"
    ":10093000DCCF8091600190916101823091050CF4CF\n"
    ":10094000FBC1019709C080916001909161018430E1\n"
    ":1009500091050CF0F1C101969093610180936001C3\n"
    ":100960008091890190918A01C0CF0E94F600209168\n"
    ":100970008F0130919001C901A3E0880F991FAA95BA\n"
    ":10098000E1F7821B930B8058974568EE73E00E9455\n"
    ":10099000AB0F6F5F7F4F70939201609391011616BA\n"
    ":1009A000170634F081E090E09093920180939101DA\n"
    ":1009B0008091910190919201099734F088E090E044\n"
    ":1009C0009093920180939101A0906E01B0906F017D\n"
    ":1009D000CC24DD24F9EFEF2EF6E0FF2EE9EF8E2E8A\n"
    ":1009E000E6E09E2E65C0C70167E070E00E94AB0F95\n"
    ":1009F0007093960160939501C4018E199F0967E079\n"
    ":100A000070E00E94AB0F7093980160939701109271\n"
    ":100A10009A0110929901C0E7D1E000E010E0209126\n"
    ":100A2000950130919601442737FD4095542FA90137\n"
    ":100A3000332722278091970190919801AA2797FD4B\n"
    ":100A4000A095BA2FBA2FA92F982F8827282B392B9A\n"
    ":100A50004A2B5B2B8091990190919A01AA2797FDCF\n"
    ":100A6000A095BA2F282B392B4A2B5B2BCA01AA271A\n"
    ":100A7000BB278883BB27A52F942F832F89832A83A5\n"
    ":100A8000C501F50160810E944D0A0F5F1F4F23963B\n"
    ":100A9000C016D10624F66FE370E080E090E00E947B\n"
    ":100AA0001C0D0894C11CD11C81E09FEFE80EF91EBB\n"
    ":100AB0008091910190919201C816D9060CF493CFC0\n"
    ":100AC00060ED77E080E090E00E941C0D0E94F6004F\n"
    ":100AD0001092960110929501109298011092970130\n"
    ":100AE00010929A011092990110928A011092890134\n"
    ":100AF00023C18091A101882309F492C08091A201B1\n"
    ":100B0000882309F08DC081E08093A30110928A01AF\n"
    ":100B100010928901109296011092950110929801FD\n"
    ":100B20001092970110929A011092990100916E0112\n"
    ":100B300010916F01C0E7D1E02091060130910701CB\n"
    ":100B4000442737FD4095542FA90133272227809150\n"
    ":100B5000080190910901AA2797FDA095BA2FBA2FF5\n"
    ":100B6000A92F982F8827282B392B4A2B5B2B809174\n"
    ":100B70000A0190910B01AA2797FDA095BA2F282B67\n"
    ":100B8000392B4A2B5B2BCA01AA27BB278883BB279B\n"
    ":100B9000A52F942F832F89832A83C801F8016081B0\n"
    ":100BA0000E944D0A2091000130910101442737FD38\n"
    ":100BB0004095542FA901332722278091020190915B\n"
    ":100BC0000301AA2797FDA095BA2FBA2FA92F982F16\n"
    ":100BD0008827282B392B4A2B5B2B8091040190917D\n"
    ":100BE0000501AA2797FDA095BA2F282B392B4A2B50\n"
    ":100BF0005B2BCA01AA27BB278883BB27A52F942F6D\n"
    ":100C0000832F89832A8364E670E080E090E00E946D\n"
    ":100C10001C0D2396F1E0C838DF0709F08DCF8CC09A\n"
    ":100C20008091A201882309F47DC081E08093A30113\n"
    ":100C300010928A01109289011092960110929501EA\n"
    ":100C4000109298011092970110929A0110929901B6\n"
    ":100C5000E0906E01F0906F01C0E0D0E000E010E0A5\n"
    ":100C6000AFEFCA2ED12CC233D10554F410929C019F\n"
    ":100C700010929B01C80162E370E00E94AB0F19C0A3\n"
    ":100C8000C233D10549F410929C0110929B01D0927D\n"
    ":100C90009E01C0929D0111C010929C0110929B0177\n"
    ":100CA000C8018E5C91436EEC7FEF0E94AB0F6150E8\n"
    ":100CB0007F4F70939E0160939D011092A00110924E\n"
    ":100CC0009F0180919D0190919E01AA2797FDA0957B\n"
    ":100CD000BA2FBA2FA92F982F88279D01442755276F\n"
    ":100CE000622F292F3A2F4B2F5527E0E7F1E0608341\n"
    ":100CF00021838283339691E0E838F907C1F7C70171\n"
    ":100D0000F70160810E944D0A6AE070E080E090E0A7\n"
    ":100D10000E941C0D219601501F4FC436D10509F0C9\n"
    ":100D2000A2CF0AC010928A01109289010E94F60097\n"
    ":100D300003C01092A201C0CCDF91CF911F910F91FF\n"
    ":100D4000FF90EF90DF90CF90BF90AF909F908F90EB\n"
    ":100D50000895EF92FF920F931F93CF93DF93EC01CF\n"
    ":100D60008B017A0180919702882309F04DC087E9B1\n"
    ":100D700092E00E94510E892B09F446C01092A20203\n"
    ":100D80001092A1028FEF8093A5028093A602809318\n"
    ":100D9000A7028093A8028093A9028093AA0281E00F\n"
    ":100DA0008093AB021092AD021092AC021092A4029A\n"
    ":100DB0001092A3028091B6019091B701892B31F472\n"
    ":100DC0008FE992E09093B7018093B601E091B8016A\n"
    ":100DD000F091B901309721F08FE992E095838483F7\n"
    ":100DE0008FE992E09093B9018093B8011092AF021D\n"
    ":100DF0001092AE0286E691E09093A00280939F024B\n"
    ":100E000087E992E00E945B0E8FE992E0BE01A801A3\n"
    ":100E100097010E94C509DF91CF911F910F91FF901B\n"
    ":100E2000EF90089580E791E068E070E040E050E0E6\n"
    ":100E30000E94A90680E062E00E94B00D0E94F600C8\n"
    ":100E400088E891E00E94850C81E090E09093A501F4\n"
    ":100E50008093A4010E94CE007093900160938F0153\n"
    ":100E600008955F926F927F928F929F92AF92BF92FE\n"
    ":100E7000CF92DF92EF92FF920F931F93DF93CF9366\n"
    ":100E8000CDB7DEB72E970FB6F894DEBF0FBECDBF3D\n"
    ":100E90005C014A01F9011E2DDC011C969C917A83AC\n"
    ":100EA00069835C834B8380818B8781818C8782817E\n"
    ":100EB0008D87913029F4CE0101960E948F0006C0E3\n"
    ":100EC0001A86198618861F821E821D8289819A8140\n"
    ":100ED000810F911D9A838983002311F480E002C061\n"
    ":100EE000812F8D5F8E870E94F60CF5012785308952\n"
    ":100EF000621B730B62337105B0F3F89439814A8138\n"
    ":100F00008BB15C9819858E81181B1E83E981FA814B\n"
    ":100F10008181882311F420E004C0282F210F08F4D8\n"
    ":100F20002FEF832F942FFC01CC84022F2C2D029FB6\n"
    ":100F3000012D1124DE84AB81BC81FB84ED847D8195\n"
    ":100F40006F81798468845A8450E040E030E020E08A\n"
    ":100F500080C1872D811B182F862D871B782F852D0B\n"
    ":100F6000861B682F9BB190618BB18F7E11979BB9C7\n"
    ":100F700000C007FF8BB920813327889400C000C0D0\n"
    ":100F8000002C8BB92111270F00C0002C9BB900C089\n"
    ":100F900006FF8BB908F42FEF889400C000C000C092\n"
    ":100FA0008BB9F0FC320F00C0002C9BB900C005FFCC\n"
    ":100FB0008BB937958894F1FC320F00C000C0002C2B\n"
    ":100FC0008BB93795889400C0002C9BB900C004FFF2\n"
    ":100FD0008BB9F2FC320F3795889400C000C0002C0A\n"
    ":100FE0008BB9F3FC320F00C0002C9BB900C003FF8B\n"
    ":100FF0008BB937958894F4FC320F00C000C0002CE8\n"
    ":101000008BB93795889400C0002C9BB900C002FFB3\n"
    ":101010008BB9F5FC320F3795889400C000C0002CC6\n"
    ":101020008BB9F6FC320F00C0002C9BB900C001FF49\n"
    ":101030008BB937958894F7FC320F00C000C0002CA4\n"
    ":101040008BB93795889400C0002C9BB900C000FF75\n"
    ":101050008BB900C000C0002C00C000C08BB900C01C\n"
    ":1010600000C0002C9BB900C037FF8BB922814427F8\n"
    ":10107000889400C000C0002C8BB92111260F00C03D\n"
    ":10108000002C9BB900C036FF8BB908F42FEF889471\n"
    ":1010900000C000C000C08BB9E0FC420F00C0002CB3\n"
    ":1010A0009BB900C035FF8BB947958894E1FC420F8E\n"
    ":1010B00000C000C0002C8BB94795889400C0002C5C\n"
    ":1010C0009BB900C034FF8BB9E2FC420F479588946E\n"
    ":1010D00000C000C0002C8BB9E3FC420F00C0002C04\n"
    ":1010E0009BB900C033FF8BB947958894E4FC420F4D\n"
    ":1010F00000C000C0002C8BB94795889400C0002C1C\n"
    ":101100009BB900C032FF8BB9E5FC420F479588942C\n"
    ":1011100000C000C0002C8BB9E6FC420F00C0002CC0\n"
    ":101120009BB900C031FF8BB947958894E7FC420F0B\n"
    ":1011300000C000C0002C8BB94795889400C0002CDB\n"
    ":101140009BB900C030FF8BB9ED0DF11D889400C034\n"
    ":1011500000C000C08BB900C000C0002C9BB900C00B\n"
    ":1011600047FF8BB921810027889400C000C0002C64\n"
    ":101170008BB92111210F00C0002C9BB900C046FF84\n"
    ":101180008BB908F42FEF889400C000C000C08BB961\n"
    ":10119000C0FC020F00C0002C9BB900C045FF8BB9FA\n"
    ":1011A00007958894C1FC020F00C000C0002C8BB9C9\n"
    ":1011B0000795889400C0002C9BB900C044FF8BB9F0\n"
    ":1011C000C2FC020F0795889400C000C0002C8BB9A8\n"
    ":1011D000C3FC020F00C0002C9BB900C043FF8BB9B9\n"
    ":1011E00007958894C4FC020F00C000C0002C8BB986\n"
    ":1011F0000795889400C0002C9BB900C042FF8BB9B2\n"
    ":10120000C5FC020F0795889400C000C0002C8BB964\n"
    ":10121000C6FC020F00C0002C9BB900C041FF8BB977\n"
    ":1012200007958894C7FC020F00C000C0002C8BB942\n"
    ":101230000795889400C0002C9BB900C040FF8BB973\n"
    ":1012400000C000C0002C00C000C08BB900C000C0AE\n"
    ":10125000002C109709F07DCEB401882777FD80958A\n"
    ":10126000982F20EE31E040E050E00E94780F7B01A3\n"
    ":101270008C01DC01CB0124E0B695A7959795879565\n"
    ":101280002A95D1F781509440A040B040B0F0209111\n"
    ":101290009202309193024091940250919502D801AC\n"
    ":1012A000C7011EE0B695A795979587951A95D1F732\n"
    ":1012B000820F931FA41FB51F0BC0809192029091C3\n"
    ":1012C0009302A0919402B09195020196A11DB11DC7\n"
    ":1012D0008093920290939302A0939402B09395020C\n"
    ":1012E00078940E94F60CD50150967C936E931F97CC\n"
    ":1012F0002E960FB6F894DEBF0FBECDBFCF91DF9113\n"
    ":101300001F910F91FF90EF90DF90CF90BF90AF9023\n"
    ":101310009F908F907F906F905F900895EF92FF92D3\n"
    ":101320000F93DF93CF9300D00F92CDB7DEB7AB0111\n"
    ":1013300019821A821B829E012F5F3F4FB90100E084\n"
    ":10134000EE24FF240E9431070F900F900F90CF9151\n"
    ":10135000DF910F91FF90EF900895EF92FF920F931E\n"
    ":1013600001E0EE24FF240E9431070F91FF90EF90DF\n"
    ":101370000895EF92FF920F9300E0EE24FF240E9465\n"
    ":1013800031070F91FF90EF900895CF92DF92EF9287\n"
    ":10139000FF920F931F93CF93DF938C017B01121663\n"
    ":1013A000130624F06A01C0E0D0E002C06901EA013E\n"
    ":1013B000D801ED91FC910480F581E02DC8010995DB\n"
    ":1013C000CE01880F991F8C0F9D1F8E0D9F1DF80158\n"
    ":1013D00093838283C586D686C801DF91CF911F9102\n"
    ":1013E0000F91FF90EF90DF90CF9008958FEF809353\n"
    ":1013F000B3011092B5011092B4010895EF92FF92DB\n"
    ":101400000F931F93CF93DF93EC018091BE019091D6\n"
    ":10141000BF0101969093BF018093BE01019786178B\n"
    ":101420009707A4F10E94E80C9B01AC016091BE01FA\n"
    ":101430007091BF0188EE93E0FB01E89FB001E99F46\n"
    ":10144000700DF89F700D1124882777FD8095982FD7\n"
    ":10145000E090BA01F090BB010091BC011091BD0178\n"
    ":101460002E193F09400B510B0E94BE0F3A8329836E\n"
    ":101470001092BF011092BE010E94E80C6093BA0165\n"
    ":101480007093BB018093BC019093BD01DF91CF911C\n"
    ":101490001F910F91FF90EF9008952F923F924F92DE\n"
    ":1014A0005F926F927F928F929F92AF92BF92CF92F4\n"
    ":1014B000DF92EF92FF920F931F93DF93CF93CDB7FD\n"
    ":1014C000DEB729970FB6F894DEBF0FBECDBF1C0163\n"
    ":1014D0006F830091B6011091B701862E9924AA243A\n"
    ":1014E000BB24CE0104969987888765C0D8011C96D5\n"
    ":1014F000BC91BE83F101818192818436910518F4FB\n"
    ":10150000D8011C961C92D801ED91FC911197828113\n"
    ":1015100093819D838C8312966D907C9013971D967A\n"
    ":101520004D905C901E9719821A821B829F8199238D\n"
    ":1015300099F168017E010894E11CF11CD601169610\n"
    ":101540002C91169719966C912223D9F06623C9F035\n"
    ":1015500070E080E090E06F5F7F4F8F4F9F4F30E0F3\n"
    ":1015600040E050E02F5F3F4F4F4F5F4F0E94780F9A\n"
    ":10157000A50194010E94780FBC0188279927F701E3\n"
    ":1015800060830894C11CD11C0894E11CF11C88855F\n"
    ":101590009985E816F90691F6C801B301A2019E01EA\n"
    ":1015A0002F5F3F4FAC81BD81FD0109958E81F80110\n"
    ":1015B0008487048115810115110509F097CFC101B8\n"
    ":1015C00069E170E00E94FE0929960FB6F894DEBF2B\n"
    ":1015D0000FBECDBFCF91DF911F910F91FF90EF9084\n"
    ":1015E000DF90CF90BF90AF909F908F907F906F9043\n"
    ":1015F0005F904F903F902F9008956093C00110929C\n"
    ":10160000C201089582E08093C10110928D02089575\n"
    ":101610002F923F924F925F926F927F928F929F9202\n"
    ":10162000AF92BF92CF92DF92EF92FF920F931F93F0\n"
    ":10163000DF93CF9300D0CDB7DEB71B01FB018485CC\n"
    ":101640009585863091052CF420E030E040E050E0B4\n"
    ":1016500081C0F5ECAF2EFDE9BF2EFCE1CF2EF1E805\n"
    ":10166000DF2EE6E04E2E512C9C01220F331F3A83D1\n"
    ":10167000298359C0F101A285B385F2013497EA0F9D\n"
    ":10168000FB1FE080F180A40DB51D6D917C9180E081\n"
    ":1016900090E00E94630E3B014C0100E010E0C801A5\n"
    ":1016A000B7010E94630E7B018C012DEC3CEC4CE4F5\n"
    ":1016B0005FE30E94150F9B01AC01C401B3010E94BE\n"
    ":1016C0005F0E88231CF400E010E018C0C401B301D1\n"
    ":1016D0002DEC3CEC4CE45FE30E94150F9B01AC0148\n"
    ":1016E000C801B7010E945F0E082F112707FD109552\n"
    ":1016F0000027110F001F11270F5F1F4FC601B501F3\n"
    ":1017000023E931E040E051E00E94780F5801CC24F9\n"
    ":10171000B7FCC094DC2CA626B726C826D92622E022\n"
    ":1017200030E0420E531E89819A814816590609F00D\n"
    ":10173000A1CFF101A482B582C682D78280E290E077\n"
    ":10174000918780878FEF9FEF9183808321E030E046\n"
    ":1017500040E050E0B901CA010F900F90CF91DF91A6\n"
    ":101760001F910F91FF90EF90DF90CF90BF90AF90BF\n"
    ":101770009F908F907F906F905F904F903F902F90B1\n"
    ":1017800008959C01FB0185EC91E09387828780910D\n"
    ":101790008D02848715868091C101853081F4C9014D\n"
    ":1017A0000E94080B611571058105910519F021E072\n"
    ":1017B00030E007C082E08093C10110928D0220E0EA\n"
    ":1017C00030E0C90108951F920F920FB60F921124B5\n"
    ":1017D0002F933F934F935F936F937F938F939F9339\n"
    ":1017E000AF93BF93EF93FF938091C0010E94EF0DE1\n"
    ":1017F000482F8091C3019091C40101969093C40138\n"
    ":101800008093C30180918D02843618F085E0809327\n"
    ":10181000C1018091C1018330D1F0843020F4823045\n"
    ":1018200009F059C006C0843071F1853009F053C009\n"
    ":101830004CC0442309F04FC08091C3019091C40172\n"
    ":101840008436910508F443C010928D021EC04130C9\n"
    ":1018500009F041C080918D02E82FF0E02091C30192\n"
    ":101860003091C401EE0FFF1FE054FE4F36832583F5\n"
    ":101870008F5F80938D021092C4011092C30184E0A7\n"
    ":101880008093C10128C04423B9F480918D02E82FD0\n"
    ":10189000F0E02091C3013091C401EE0FFF1FE0542E\n"
    ":1018A000FE4F368325838F5F80938D021092C40193\n"
    ":1018B0001092C30183E0E4CF8091C3019091C401F1\n"
    ":1018C0008536910540F085E0DBCF442321F410926A\n"
    ":1018D000C4011092C3018091C201882329F04423DE\n"
    ":1018E00011F42D9A01C02D98FF91EF91BF91AF9106\n"
    ":1018F0009F918F917F916F915F914F913F912F9128\n"
    ":101900000F900FBE0F901F901895F89492E090934F\n"
    ":10191000B0009093B10084E68093B300E2EBF0E076\n"
    ":101920001082E08190937000789482E08093C101EE\n"
    ":1019300010928D028091C00160E00E94B00D089568\n"
    ":101940001F920F920FB60F9211242F933F938F93F4\n"
    ":101950009F93AF93BF938091920290919302A09135\n"
    ":101960009402B0919502309196020196A11DB11D8D\n"
    ":10197000232F2D5F2D3720F02D570196A11DB11D6E\n"
    ":10198000209396028093920290939302A0939402E4\n"
    ":10199000B093950280918E0290918F02A091900257\n"
    ":1019A000B09191020196A11DB11D80938E0290937A\n"
    ":1019B0008F02A0939002B0939102BF91AF919F913B\n"
    ":1019C0008F913F912F910F900FBE0F901F90189500\n"
    ":1019D0008FB7F89420919202309193024091940233\n"
    ":1019E000509195028FBFB901CA0108959FB7F8942D\n"
    ":1019F00020918E0230918F0240919002509191027D\n"
    ":101A000086B5A89B06C08F3F21F02F5F3F4F4F4FF9\n"
    ":101A10005F4F9FBF542F432F322F2227280F311D96\n"
    ":101A2000411D511D82E0220F331F441F551F8A950F\n"
    ":101A3000D1F7B901CA0108959B01AC017FB7F894B1\n"
    ":101A400080918E0290918F02A0919002B0919102AC\n"
    ":101A500066B5A89B05C06F3F19F00196A11DB11D89\n"
    ":101A60007FBFBA2FA92F982F8827860F911DA11D00\n"
    ":101A7000B11D62E0880F991FAA1FBB1F6A95D1F79D\n"
    ":101A8000BC012DC0FFB7F89480918E0290918F0217\n"
    ":101A9000A0919002B0919102E6B5A89B05C0EF3FDE\n"
    ":101AA00019F00196A11DB11DFFBFBA2FA92F982FC4\n"
    ":101AB00088278E0F911DA11DB11DE2E0880F991F8F\n"
    ":101AC000AA1FBB1FEA95D1F7861B970B885E934030\n"
    ":101AD000C8F2215030404040504068517C4F2115A1\n"
    ":101AE00031054105510571F60895789484B58260F9\n"
    ":101AF00084BD84B5816084BD85B5826085BD85B5B2\n"
    ":101B0000816085BDEEE6F0E0808181608083E1E860\n"
    ":101B1000F0E0108280818260808380818160808398\n"
    ":101B2000E0E8F0E0808181608083E1EBF0E080819B\n"
    ":101B300084608083E0EBF0E0808181608083EAE76D\n"
    ":101B4000F0E08081846080838081826080838081F6\n"
    ":101B5000816080838081806880831092C1000895B5\n"
    ":101B6000CF93DF93482F50E0CA0186569F4FFC0168\n"
    ":101B700034914A575F4FFA018491882369F190E0CC\n"
    ":101B8000880F991FFC01E859FF4FA591B491FC0102\n"
    ":101B9000EE58FF4FC591D491662351F42FB7F894B6\n"
    ":101BA0008C91932F909589238C93888189230BC0E6\n"
    ":101BB000623061F42FB7F8948C91932F909589231C\n"
    ":101BC0008C938881832B88832FBF06C09FB7F8949E\n"
    ":101BD0008C91832B8C939FBFDF91CF910895682FB9\n"
    ":101BE00070E0CB0182559F4FFC012491CB018656BA\n"
    ":101BF0009F4FFC0144916A577F4FFB0194919923B9\n"
    ":101C000019F420E030E03CC0222351F1233071F080\n"
    ":101C1000243028F42130A1F0223011F514C02630F0\n"
    ":101C2000B1F02730C1F02430D9F404C08091800095\n"
    ":101C30008F7703C0809180008F7D8093800010C0DB\n"
    ":101C400084B58F7702C084B58F7D84BD09C0809133\n"
    ":101C5000B0008F7703C08091B0008F7D8093B0007B\n"
    ":101C6000892F90E0880F991F84589F4FFC01A59100\n"
    ":101C7000B4918C9120E030E0842311F021E030E039\n"
    ":101C8000C9010895CF93DF930E94750D0E9412073A\n"
    ":101C9000C0E0D0E00E941C032097E1F30E94000006\n"
    ":101CA000F9CFFC0120E030E08081882311F421E0AD\n"
    ":101CB00030E0C9010895FC0181E08083089540D09F\n"
    ":101CC00008F481E00895E89409C097FB3EF49095EC\n"
    ":101CD0008095709561957F4F8F4F9F4F9923A9F005\n"
    ":101CE000F92F96E9BB279395F69587957795679594\n"
    ":101CF000B795F111F8CFFAF4BB0F11F460FF1BC0D8\n"
    ":101D00006F5F7F4F8F4F9F4F16C0882311F096E96A\n"
    ":101D100011C0772321F09EE8872F762F05C0662318\n"
    ":101D200071F096E8862F70E060E02AF09A95660FD1\n"
    ":101D3000771F881FDAF7880F9695879597F908958A\n"
    ":101D4000990F0008550FAA0BE0E8FEEF16161706CC\n"
    ":101D5000E807F907C0F012161306E407F50798F034\n"
    ":101D6000621B730B840B950B39F40A2661F0232B4D\n"
    ":101D7000242B252B21F408950A2609F4A140A695C9\n"
    ":101D80008FEF811D811D089597F99F6780E870E0AE\n"
    ":101D900060E008959FEF80EC089500240A941616E1\n"
    ":101DA000170618060906089500240A941216130649\n"
    ":101DB000140605060895092E0394000C11F48823D7\n"
    ":101DC00052F0BB0F40F4BF2B11F460FF04C06F5FF3\n"
    ":101DD0007F4F8F4F9F4F089557FD9058440F551FC9\n"
    ":101DE00059F05F3F71F04795880F97FB991F61F09D\n"
    ":101DF0009F3F79F087950895121613061406551F14\n"
    ":101E0000F2CF4695F1DF08C0161617061806991F7F\n"
    ":101E1000F1CF86957105610508940895E894BB2774\n"
    ":101E200066277727CB0197F908950BD0C4CFB5DF8C\n"
    ":101E300028F0BADF18F0952309F0A6CFABCF112414\n"
    ":101E4000EECFCADFA0F3959FD1F3950F50E0551F59\n"
    ":101E5000629FF001729FBB27F00DB11D639FAA27FF\n"
    ":101E6000F00DB11DAA1F649F6627B00DA11D661F4E\n"
    ":101E7000829F2227B00DA11D621F739FB00DA11D6F\n"
    ":101E8000621F839FA00D611D221F749F3327A00D29\n"
    ":101E9000611D231F849F600D211D822F762F6A2FC5\n"
    ":101EA00011249F5750408AF0E1F088234AF0EE0F4A\n"
    ":101EB000FF1FBB1F661F771F881F91505040A9F757\n"
    ":101EC0009E3F510570F060CFAACF5F3FECF3983E84\n"
    ":101ED000DCF3869577956795B795F795E7959F5FBE\n"
    ":101EE000C1F7FE2B880F911D9695879597F9089558\n"
    ":101EF000629FD001739FF001829FE00DF11D649FEE\n"
    ":101F0000E00DF11D929FF00D839FF00D749FF00D79\n"
    ":101F1000659FF00D9927729FB00DE11DF91F639F1A\n"
    ":101F2000B00DE11DF91FBD01CF0111240895AA1BB9\n"
    ":101F3000BB1B51E107C0AA1FBB1FA617B70710F0B4\n"
    ":101F4000A61BB70B881F991F5A95A9F780959095E6\n"
    ":101F5000BC01CD01089597FB092E07260AD077FD15\n"
    ":101F600004D0E5DF06D000201AF4709561957F4F0C\n"
    ":101F70000895F6F7909581959F4F0895A1E21A2E46\n"
    ":101F8000AA1BBB1BFD010DC0AA1FBB1FEE1FFF1F1D\n"
    ":101F9000A217B307E407F50720F0A21BB30BE40B6D\n"
    ":101FA000F50B661F771F881F991F1A9469F76095B4\n"
    ":101FB0007095809590959B01AC01BD01CF0108956E\n"
    ":101FC00097FB092E05260ED057FD04D0D7DF0AD087\n"
    ":101FD000001C38F450954095309521953F4F4F4F58\n"
    ":101FE0005F4F0895F6F790958095709561957F4FB6\n"
    ":101FF0008F4F9F4F0895EE0FFF1F0590F491E02D36\n"
    ":062000000994F894FFCFE3\n"
    ":10200600000000000000FF00000000000000FF00CC\n"
    ":10201600000000000000FF000000FF00FF00FF00BE\n"
    ":102026000000CC00FF00FF000000FF00FF0044009E\n"
    ":10203600FF00FF007F00FF00FF00FF00FF005500CC\n"
    ":102046000000FF005500990066000000FF00000038\n"
    ":10205600000000000000000000000000000000007A\n"
    ":10206600040000000000B909AD098D008E09B30116\n"
    ":00000001FF\n"
  }
};

