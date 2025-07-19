#include <LiquidCrystal_74HC595.h>

#define LCD_DATA  A1
#define LCD_CLOCK A2
#define LCD_LATCH A3

LiquidCrystal_74HC595 lcd(LCD_DATA, LCD_CLOCK, LCD_LATCH, 1, 3, 4, 5, 6, 7);

void setup() {
  delay(100); // Allow LCD to stabilise
  lcd.begin(16, 2);
  lcd.print("It works!");
}

void loop() {}
