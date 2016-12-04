/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

void setup() {
  Serial.begin(9600);
  pinMode(52,OUTPUT);
  pinMode(A5, INPUT);
}

void loop() {
  double sensorValue = analogRead(A5);
  Serial.println(sensorValue);
  double sensorValue2 = analogRead(A0);
  if(sensorValue2<200) digitalWrite(52, 1);
  else digitalWrite(52, 0);
}
