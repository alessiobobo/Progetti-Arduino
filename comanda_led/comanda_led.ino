
void setup() {                
  pinMode(A8, OUTPUT);     
}
  int val=0;
  
void loop() {

  analogWrite(8, val);   
  analogWrite(9, val);  
  analogWrite(10, val); 
  delay(1000);
  val=val+20;
  if (val>250)val=0;
}
