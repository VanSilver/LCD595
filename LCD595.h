/*
*   Designed by VanSilver
*   Contact:
*   https://www.youtube.com/@vansilver1711
*   https://facebook.com/VanSilver.ytb
*/

#ifndef Arduino_LCD595_h
#define Arduino_LCD595_h

#include <inttypes.h>
#include "Print.h"

#include <SPI.h>

#define CLK 0
#define DATA 1
#define LATCH 2

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define nop() asm("nop")
#define LCD_BACKLIGHT 0x00
#define LCD_NOBACKLIGHT 0x02
#define E_1 0x08
#define RS_E_1 0x0C

class LCD595 : public Print
{
public:
    LCD595(uint8_t clk, uint8_t data, uint8_t latch);
    LCD595(uint8_t latch); // SPI protocol

    void Init(uint8_t clk, uint8_t data, uint8_t latch);
    void InitSPI(uint8_t latch);

    void begin();
    void begin(uint8_t cols, uint8_t rows);

    void home();
    void Clear(void);
    void setCursor(uint8_t _col, uint8_t _row);

    void scrollDisplayLeft(const char *s1, const char *s2);  // dich trai
    void scrollDisplayRight(const char *s1, const char *s2); // dich phai
    void CustomChar(unsigned char vt, unsigned char *mgs);
    void PrintCustomChar(uint8_t m);

    void BacklightOn(void);
    void BacklightOff(void);
    virtual size_t write(uint8_t);

    // void In_so(unsigned long val, char dec, unsigned char neg); // In_so(so,vi tri dau phay, 0= "+" or 1= "-"
    // void In_chuoi(const char *);
    // void Doi_float(char *string, int a, int b);                 // Bien doi so thuc float sang chuoi den in len LCD
    // void LCD_2line(const char *s1, const char *s2);             // in 2 hang
    // void LCD_Int2str(int i, char *sring);                       // Convert Interger to String Max =99999
private:
    void hc595_sendByte(uint8_t);
    void hc595_spiSendByte();
    void hc595_writeData(uint8_t);
    void hc595_writeCommand(uint8_t);

    uint8_t _backlightVal;
    uint8_t _rs_pin;     // LOW: command.  HIGH: character.
    uint8_t _rw_pin;     // LOW: write to LCD.  HIGH: read from LCD.
    uint8_t _enable_pin; // activated by a HIGH pulse.
    uint8_t _data_pins[8];
};

#endif