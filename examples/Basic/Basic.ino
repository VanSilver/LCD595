/***********************************************************************************************************
 *   Designed by VanSilver
 *   Contact:
 *   https://www.youtube.com/@vansilver1711
 *   https://facebook.com/VanSilver.ytb
 ***********************************************************************************************************/
 /***********************************************************************************************************
 *                                          -CONNECTION-
 * **********************************************************************************************************
 *                                                          -------------------------------------------------
 *                                                          |                    LCD16x2                    |
 *                                                          |                                               |
 *                                                          -------------------------------------------------
 *                                                           |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                                           |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                              VSS----------+  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                              VCC-------------+  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *   MCU                74HC595              	  VEE----------------+  |  |  |  |  |  |  |  |  |  |  |  |  |
 *  ----------          ----------              RS -------------------+  |  |  |  |  |  |  |  |  |  |  |  |
 *  |DATApin |-	DS   K -|Q1   VCC|-VCC          RW ----------------------+  |  |  |  |  |  |  |  |  |  |  |
 *  |CLKpin  |-	SH   RS-|Q2    Q0|-             E  -------------------------+  |  |  |  |  |  |  |  |  |  |
 *  |LATCHpin|-	ST   E -|Q3    DS|- DATApin     D0 ----------------------------+  |  |  |  |  |  |  |  |  |
 *  ----------       D4-|Q4    OE|- GND         D1 -------------------------------+  |  |  |  |  |  |  |  |
 *                   D5-|Q5    ST|- LATCHpin    D2 ----------------------------------+  |  |  |  |  |  |  |
 *                   D6-|Q6    SH|- CLKpin      D3 -------------------------------------+  |  |  |  |  |  |
 *                   D7-|Q7    MR|- VCC         D4 ----------------------------------------+  |  |  |  |  |
 *                  GND-|GND  Q7'|-             D5 -------------------------------------------+  |  |  |  |
 *                      ----------              D6 ----------------------------------------------+  |  |  |
 *                                              D7 -------------------------------------------------+  |  |
 *                                              A  ----------------------------------------------------+  |
 *                                              K  -------------------------------------------------------+                        
 * 
 *  You can connection any MCU to Module using Arduino Framework   
 *  DATApin, CLKpin, LATCHpin this is pin for USER DEFINE for MCU
 ***********************************************************************************************************/
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
  lcd.customChar(0, Add_sub);
}

void loop()
{
  lcd.setCursor(1, 1);
  lcd.print("LCD595-VANSILVER");
  lcd.setCursor(1, 2);
  lcd.printCustomChar(0);
  lcd.print(123);
}
