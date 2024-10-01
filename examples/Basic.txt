// coppy all and paste to main.cpp for test
#include "Arduino.h"
#include "LCD595.h"

// address default sympol
char Degree_symbol[8] = {0xDF};
char Ohm_symbol[8] = {0xF4};
char Alpha_symbol[8] = {0xE0};
char Beta_symbol[8] = {0xE2};
char Pi_symbol[8] = {0xF7};
char Sq_symbol[8] = {0xE8};
char Inf_symbol[8] = {0xF3};

// create symbol
unsigned char Add_sub[8] = {0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x1F, 0x00};

const uint8_t clk = 14;
const uint8_t data = 26;
const uint8_t latch = 27;

LCD595 lcd(clk, data, latch);

void setup()
{
  lcd.begin(16, 2);
  lcd.CustomChar(0, Add_sub);
}

void loop()
{
  lcd.setCursor(1, 1);
  lcd.print("LCD595-VANSILVER");
  lcd.setCursor(1, 2);
  lcd.PrintCustomChar(0);
  lcd.print(123);
}
