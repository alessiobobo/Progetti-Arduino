//int k=1;

void setup()
{
	pinMode(7,OUTPUT);	//NOT ENABLE shift register
	pinMode(2,OUTPUT);	//SERIAL CLOCK shift register	
	pinMode(4,OUTPUT);  //SERIAL IN shift register

	pinMode(3,OUTPUT);	//PWM spie rosse
	pinMode(5,OUTPUT);	//PWM luce lancette
	pinMode(6,OUTPUT);	//PWM luce strumentazione
	pinMode(9,OUTPUT);	//PWM spie verdi
	pinMode(10,OUTPUT);	//PWM spie blu
	pinMode(11,OUTPUT); //PWM spie gialle

	digitalWrite(3,1);
	digitalWrite(9,1);
	digitalWrite(10,1);
	digitalWrite(11,1);
}

void loop()
{
	for(int k=1; k<25; k++)
	{
		digitalWrite(7,0);
		for(int i=1; i<25; i++)
		{
			if(i==k) digitalWrite(4,1);
			else digitalWrite(4,0);
			digitalWrite(2,1);
			delay(10);
			digitalWrite(2,0);
			delay(10);
		}
		digitalWrite(7,1);
		delay(100);
	}
}