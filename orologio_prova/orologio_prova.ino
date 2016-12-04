#define MAX_MILLIS_VALUE 34359738
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long current_millis_value = 0;
unsigned long previous_millis_value = 0;
unsigned long m = 0;
unsigned int seconds = 0;
unsigned int minutes = 46;
unsigned int hours = 1;

void setup (){
}

void loop() {
  
  cli();  // disable interrupts
  current_millis_value = millis();
  sei();  // enable interrupts
  
  // overflow millis()
  if (current_millis_value < previous_millis_value)
     m += MAX_MILLIS_VALUE - previous_millis_value + current_millis_value;
  else m += current_millis_value - previous_millis_value;
  
  seconds += m / 1000;
  m = m % 1000;
  minutes += seconds / 60;
  seconds = seconds % 60;
  hours += minutes / 60;
  minutes = minutes % 60;
  hours = hours % 24;
  previous_millis_value = current_millis_value;
  lcd.setCursor(0, 0);
  if (hours<10)
    lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes<10)
    lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds<10)
    lcd.print("0");
  lcd.print(seconds);
  
  delay(150);
  lcd.clear();
} 
