// diamo i nomi dei segmenti ai contatti di arduino
int a = 2;
int b = 3;
int c = A3;
int d = A4;
int e = A5;
int f = 1;
int g = 0;
int p = A2;

// mettiamo il tempo che vogliamo aspettare tra un numero e l'altro
// è in millesimi di secondo, quindi 1000=1s
int attesa=1000;

void setup() {         
  
// diciamo che quei contatti sono degli OUTPUT,
// ovvero dati che escono da arduino  
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);     
  pinMode(c, OUTPUT);     
  pinMode(d, OUTPUT);     
  pinMode(e, OUTPUT);     
  pinMode(f, OUTPUT);     
  pinMode(g, OUTPUT);     
  pinMode(p, OUTPUT);    

// diciamo che all'accensione vogliamo
// che sia tutto spento
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(p,HIGH);
}

void loop() {
  
  //---NOVE-----------------------------------------------------------------------------------
  // scriviamo il numero 9
  // per fare questo bisogna accendere i segmenti
  // g,f,a,b,c,d
  digitalWrite(g, LOW);  
  digitalWrite(f, LOW);  
  digitalWrite(a, LOW);  
  digitalWrite(b, LOW);  
  digitalWrite(c, LOW);  
  digitalWrite(d, LOW);  
 
  // bisogna anche spegnere il segmento e
  digitalWrite(e, HIGH);
  digitalWrite(p,HIGH);
  
  // aspettiamo un po' prima di cambiare numero
  delay(attesa);
    
  //---OTTO-----------------------------------------------------------------------------------
  // per scrivere il numero 8 accendiamo tutti i segmenti
  digitalWrite(a, LOW);   
  digitalWrite(b, LOW);   
  digitalWrite(c, LOW);   
  digitalWrite(d, LOW);   
  digitalWrite(e, LOW);   
  digitalWrite(f, LOW);   
  digitalWrite(g, LOW);   

  // aspettiamo ancora
  delay(1000);

  //---e così via con gli altri numeri!-------------------------------------------------------

}
