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
 *   				 						  	       	    -------------------------------------------------
 *                                                           |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                                           |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                              VSS----------+  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *                                              VCC-------------+  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *   MCU                74HC595              	VEE----------------+  |  |  |  |  |  |  |  |  |  |  |  |  |
 *  ----------		    ----------              RS -------------------+  |  |  |  |  |  |  |  |  |  |  |  |
 *  |DATApin |-	DS	 K -|Q1   VCC|-VCC          RW ----------------------+  |  |  |  |  |  |  |  |  |  |  |
 *  |CLKpin  |-	SH	 RS-|Q2    Q0|-             E  -------------------------+  |  |  |  |  |  |  |  |  |  |
 *  |LATCHpin|-	ST	 E -|Q3    DS|- DATApin     D0 ----------------------------+  |  |  |  |  |  |  |  |  |
 *  ----------		 D4-|Q4    OE|- GND         D1 -------------------------------+  |  |  |  |  |  |  |  |
 *                   D5-|Q5    ST|- LATCHpin    D2 ----------------------------------+  |  |  |  |  |  |  |
 *                   D6-|Q6    SH|- CLKpin      D3 -------------------------------------+  |  |  |  |  |  |
 *                   D7-|Q7    MR|- VCC         D4 ----------------------------------------+  |  |  |  |  |
 *	 				GND-|GND  Q7'|-             D5 -------------------------------------------+  |  |  |  |
 *   				    ----------              D6 ----------------------------------------------+  |  |  |
 *                                              D7 -------------------------------------------------+  |  |
 *                                              A  ----------------------------------------------------+  |
 *                                              K  -------------------------------------------------------+                        

 * 
 *  You can connection any MCU to Module using Arduino Framework   
 *  DATApin, CLKpin, LATCHpin this is pin for USER DEFINE for MCU
 ***********************************************************************************************************/
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
    void clear(void);
    void setCursor(uint8_t _col, uint8_t _row);

    void scrollDisplayLeft(const char *s1, const char *s2);  // dich trai
    void scrollDisplayRight(const char *s1, const char *s2); // dich phai
    void customChar(unsigned char vt, unsigned char *mgs);
    void printCustomChar(uint8_t m);

    void backlightOn(void);
    void backlightOff(void);
    virtual size_t write(uint8_t);
    void command(uint8_t);

    // void LCD_2line(const char *s1, const char *s2);             // in 2 hang
    // void LCD_Int2str(int i, char *sring);                       // Convert Interger to String Max =99999
private:
    void sendByte(uint8_t);
    void hc595_sendByte(uint8_t);
    void hc595_spiSendByte(uint8_t);
    void hc595_writeData(uint8_t);
    void hc595_writeCommand(uint8_t);

    uint8_t _backlightVal;
    uint8_t _enable_pin;
    uint8_t _data_pins[8];
    uint8_t _latchPin;
    bool _usingSpi;
};

#endif
