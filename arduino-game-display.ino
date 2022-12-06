// include the library code:
#include <LiquidCrystal.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int totalScore = 0;
int lasttotalScore = 0;
int easyPin = 8;
int mediumPin = 9;
int hardPin = 10;
int gameStart = 7;

int easyScored = 0;
int mediumScored = 0;
int hardScored = 0;
int gameStarted = 0;

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Clears the LCD screen
  lcd.clear();

  pinMode(easyPin,INPUT);
  pinMode(mediumPin,INPUT);
  pinMode(hardPin,INPUT);
  pinMode(gameStart,INPUT);
}

void loop() 
{ 
  easyScored = digitalRead(easyPin);
  mediumScored = digitalRead(mediumPin);
  hardScored = digitalRead(hardPin);
  gameStarted = digitalRead(gameStart);

  if (gameStarted) {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Current Score:");
     lcd.setCursor(0,1);
     lcd.print(lasttotalScore);
  }
  
  if (gameStarted) {
    delay(40);
     if (easyScored) {
        totalScore += 15;
     }
     else if (mediumScored) {
      totalScore += 63;
     }
     else if (hardScored) {
       totalScore += 255;
     }
     else {
        totalScore += 0;
     }
     lasttotalScore = totalScore;

  }
  else {
     totalScore = 0;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Last Score:");
     lcd.setCursor(0,1);
     lcd.print(lasttotalScore);
     delay(1000);
  }
}