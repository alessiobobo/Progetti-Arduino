#include <ESP_SoftwareSerial.h>

SoftwareSerial swSer(D8, D7, false, 256); //RX, TX

void setup() {
  Serial.begin(9600);
  swSer.begin(9600);
}

void loop() {

  swSer.println("Ciao");
  Serial.println("Test Running");
  delay(1000);

}
