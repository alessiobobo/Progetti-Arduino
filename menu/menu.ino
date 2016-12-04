#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  lcd.begin(16, 2);
}

const int buttonPin1 = 44;
const int buttonPin2 = 45;
int vocemenu = 0;
int vocesottomenu = 0;
int buttonState1 = 0;
int buttonState2 = 0;

void loop(){

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  lcd.clear();
  
  if (buttonState1 == HIGH) {     
    
    if (buttonState1 == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("pulsante 1");
    delay(1000);                  // waits for a second    
  
  }
  
  
  if (vocemenu == 0){
    lcd.setCursor(0,0);
    lcd.print("MENU0");
    if (vocesottomenu == 0){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");
    if (vocesottomenu == 1){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");  
 
  if (vocemenu == 0){
    lcd.setCursor(0,0);
    lcd.print("MENU0");
    if (vocesottomenu == 0){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");
    if (vocesottomenu == 1){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");  
  
  if (vocemenu == 0){
        lcd.setCursor(0,0);
    lcd.print("MENU0");
    if (vocesottomenu == 0){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");
    if (vocesottomenu == 1){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");  
 
  if (vocemenu == 0){
        lcd.setCursor(0,0);
    lcd.print("MENU0");
    if (vocesottomenu == 0){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");
    if (vocesottomenu == 1){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");  
  
  if (vocemenu == 0){
        lcd.setCursor(0,0);
    lcd.print("MENU0");
    if (vocesottomenu == 0){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");
    if (vocesottomenu == 1){
      lcd.setCursor(0,1);
      lcd.print("sottoMENU0");  
