/*------------------------------------------------------------------------------
  Program:      GT-511C3_test

  Description:  Sketch that tests basic GT-511C3 communications by sending an
                "open" command and displaying the returned data in the Serial
                Monitor window of the Arduino IDE
  
  Hardware:     - Arduino Uno
                - GT-511C3 Optical Fingerprint Scanner Module
                
  Software:     - Developed using Arduino 1.0.6 software
                - Should be compatible with Arduino 1.0 +
  
  References:   GT-511C3 datasheet v1.1, www.adh-tech.com.tw
  
  Date:         8 January 2015
 
  Author:       W.A. Smith, http://startingelectronics.org
------------------------------------------------------------------------------*/
#include <SoftwareSerial.h>

// need a serial port to communicate with the GT-511C3
SoftwareSerial gtSerial(D8, D7); // Arduino RX (GT TX), Arduino TX (GT RX)
// the Arduino TX pin needs a voltage divider, see wiring diagram at:
// http://startingelectronics.org/articles/GT-511C3-fingerprint-scanner-hardware/

// command packet that sends the "Open" command to the GT-511C3 module
byte tx_cmd[12] = { 0x55, 0xAA,             // header
                    0x01, 0x00,             // device ID
                    0x01, 0x00, 0x00, 0x00, // input parameter - get extra info
                    0x01, 0x00,             // command code - Open
                    0x02, 0x01              // checksum
                  };
                  
void setup() {
  Serial.begin(115200);  // for displaying received data from the GT-511C3
  gtSerial.begin(9600);  // for communicating with the GT-511C3
  Serial.println("Online");
}

byte rx_byte = 0;        // stores received byte

void loop() {
  // send any character from the Serial Monitor window to send packet
  if (Serial.available()) {
    // flush the buffer so we only send packet once per received character
    rx_byte = Serial.read();
    gtSerial.write(tx_cmd, 12); // send the command packet to the GT-511C3
  }
  // check for a packet from the GT-511C3
  if (gtSerial.available()) {
    rx_byte = gtSerial.read();     // get the received byte
    Serial.print("0x");
    Serial.println(rx_byte, HEX);  // echo the byte to the Serial Monitor window
  }
}

