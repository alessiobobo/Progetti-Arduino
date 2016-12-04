#include "EEPROM24.h"
#include "SoftI2C.h"
#include "I2CMaster.h"

  SoftI2C i2c(A2, A3);
  EEPROM24 eeprom(i2c, EEPROM_24LC02);

void setup() {
  Serial.begin(9600);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A4,OUTPUT);
}
void loop(){
analogWrite(A0, 255);
analogWrite(A1, 0);
analogWrite(A4, 0);
size_t ciuao = eeprom.read(8);
Serial.println(ciuao);
delay(100);
}
\
