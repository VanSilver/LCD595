#include "LCD595.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

LCD595::LCD595(uint8_t clk, uint8_t data, uint8_t latch)
{
    Init(clk, data, latch);
}

LCD595::LCD595(uint8_t latchPin)
{
    InitSPI(latchPin);
}

void LCD595::Init(uint8_t clk, uint8_t data, uint8_t latch)
{
    _data_pins[CLK] = clk;
    _data_pins[DATA] = data;
    _data_pins[LATCH] = latch;

    if (_usingSpi)
    {
        ;
    }
    else
    {
        _usingSpi = false;
    }
}

void LCD595::InitSPI(uint8_t cs)
{
    // initialize SPI:
    _usingSpi = true;
    _latchPin = cs;
    pinMode(_latchPin, OUTPUT); // just in case _latchPin is not 10 or 53 set it to output
                                // otherwise SPI.begin() will set it to output but just in case
    SPI.begin();
}

void LCD595::begin()
{
    sendByte(0x00);
    delayMicroseconds(15000);
    sendByte(0x08);
    command(0x00);
    command(0x02); // dua con tro vao vi tri
    delayMicroseconds(2000);

    command(0x28); // thiet lap giao thuc voi LCD dung 4bit, lcd 2dong, do phan giai 5x8
    command(0x0C); // an con tro
    Clear();       // Xoa lcd
    command(0x06); // ky tu in sau se dung sau ky tu in truoc
    command(0x80); // dua LCD vi tri dong dau tien
}

void LCD595::begin(uint8_t cols, uint8_t rows)
{
    sendByte(0x00);
    delayMicroseconds(15000);
    sendByte(0x08);
    command(0x00);
    command(0x02); // dua con tro vao vi tri
    delayMicroseconds(2000);

    command(0x28); // thiet lap giao thuc voi LCD dung 4bit, lcd 2dong, do phan giai 5x8
    command(0x0C); // an con tro
    Clear();       // Xoa lcd
    command(0x06); // ky tu in sau se dung sau ky tu in truoc
    command(0x80); // dua LCD vi tri dong dau tien
}

void LCD595::Clear(void)
{
    command(0x1);
    delayMicroseconds(2000);
}

void LCD595::setCursor(uint8_t _col, uint8_t _row)
{
    int row = _row - 1;
    char row_offsets[4] = {0x80, 0xC0, 0x94, 0xD4};
    command(row_offsets[row] + ((_col - 1) % 20));
}

void LCD595::home()
{
    command(LCD_RETURNHOME); // set cursor position to zero
    delayMicroseconds(2000); // this command takes a long time!
}

void LCD595::CustomChar(uint8_t location, uint8_t *byteArr)
{
    if (location < 8)
    {
        command(LCD_SETCGRAMADDR + (location * 8));
        for (char i = 0; i < 8; i++)
            write(byteArr[i]);
    }
}

void LCD595::PrintCustomChar(uint8_t m)
{
    if (m < 8)
        write(m);
}

void LCD595::BacklightOn(void)
{
    _backlightVal = LCD_BACKLIGHT;
    sendByte(_backlightVal);
}

void LCD595::BacklightOff(void)
{
    _backlightVal = LCD_NOBACKLIGHT;
    sendByte(_backlightVal);
}

/*********** mid level funtion **********/
inline void LCD595::command(uint8_t value)
{
    hc595_writeCommand(value);
}

inline size_t LCD595::write(uint8_t value)
{
    hc595_writeData(value);
    return 1; // assume sucess
}

/************ low level funtion **********/
void LCD595::hc595_writeData(uint8_t _data)
{
    // ghi 4 bit cao
    sendByte(RS_E_1 | _backlightVal);
    sendByte((_data & 0xF0) | RS_E_1 | _backlightVal);
    sendByte(((_data & 0xF0) | RS_E_1) & 0xF4 | _backlightVal);
    // ghi 4 bit thap
    sendByte(RS_E_1 | _backlightVal);
    sendByte((_data << 4) | RS_E_1 | _backlightVal);
    sendByte(((_data << 4) | RS_E_1) & 0xF4 | _backlightVal);
}

void LCD595::hc595_writeCommand(uint8_t _cmd)
{
    // ghi 4 bit cao
    sendByte(E_1 | _backlightVal);
    sendByte((_cmd & 0xF0) | E_1 | _backlightVal);
    sendByte(((_cmd & 0xF0) | E_1) & 0xF0 | _backlightVal);
    // Son 4 bit thap
    sendByte(E_1 | _backlightVal);
    sendByte((_cmd << 4) | E_1 | _backlightVal);
    sendByte(((_cmd << 4) | E_1) & 0xF0 | _backlightVal);
}

void LCD595::sendByte(uint8_t _byte)
{
    if (!_usingSpi)
        hc595_sendByte(_byte);
    else
        hc595_spiSendByte(_byte);
}
void LCD595::hc595_sendByte(uint8_t _byte)
{
    pinMode(_data_pins[0], OUTPUT); // clk
    pinMode(_data_pins[1], OUTPUT); // data
    pinMode(_data_pins[2], OUTPUT); // latch

    for (char i = 8; i > 0; i--)
    {
        digitalWrite(_data_pins[DATA], LOW);
        if (_byte & 0x80)
            digitalWrite(_data_pins[DATA], HIGH);
        digitalWrite(_data_pins[CLK], HIGH);
        digitalWrite(_data_pins[CLK], LOW);
        _byte *= 2;
    }
    digitalWrite(_data_pins[LATCH], HIGH);
    digitalWrite(_data_pins[LATCH], LOW);
}

void LCD595::hc595_spiSendByte(uint8_t _byte)
{
    digitalWrite(_latchPin, LOW);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_byte);  // Gửi byte dữ liệu
    SPI.endTransaction(); // Kết thúc giao dịch SPI
    digitalWrite(_latchPin, HIGH);
}