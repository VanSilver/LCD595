#include "LCD595.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

LCD595::LCD595(uint8_t clk, uint8_t data, uint8_t latch)
{
    Init(clk,data,latch);
}

LCD595::LCD595(uint8_t latch)
{
    InitSPI(latch);
}

void LCD595::Init(uint8_t clk, uint8_t data, uint8_t latch)
{
    _data_pins[CLK] = clk;
    _data_pins[DATA] = data;
    _data_pins[LATCH] = latch;
}

void LCD595::InitSPI(uint8_t latch)
{
    _data_pins[LATCH] = latch;
}

void LCD595::begin()
{
    hc595_sendByte(0x00);
    delayMicroseconds(15000);
    hc595_sendByte(0x08);
    hc595_writeCommand(0x00);
    hc595_writeCommand(0x02); // dua con tro vao vi tri
    delayMicroseconds(2000);

    hc595_writeCommand(0x28); // thiet lap giao thuc voi LCD dung 4bit, lcd 2dong, do phan giai 5x8
    hc595_writeCommand(0x0C); // an con tro
    Clear();                  // Xoa lcd
    hc595_writeCommand(0x06); // ky tu in sau se dung sau ky tu in truoc
    hc595_writeCommand(0x80); // dua LCD vi tri dong dau tien
}

void LCD595::begin(uint8_t cols, uint8_t rows)
{
    hc595_sendByte(0x00);
    delayMicroseconds(15000);
    hc595_sendByte(0x08);
    hc595_writeCommand(0x00);
    hc595_writeCommand(0x02); // dua con tro vao vi tri
    delayMicroseconds(2000);

    hc595_writeCommand(0x28); // thiet lap giao thuc voi LCD dung 4bit, lcd 2dong, do phan giai 5x8
    hc595_writeCommand(0x0C); // an con tro
    Clear();                  // Xoa lcd
    hc595_writeCommand(0x06); // ky tu in sau se dung sau ky tu in truoc
    hc595_writeCommand(0x80); // dua LCD vi tri dong dau tien
}

void LCD595::Clear(void)
{
    hc595_writeCommand(0x1);
    delayMicroseconds(2000);
}

void LCD595::setCursor(uint8_t _col, uint8_t _row)
{
    int row = _row - 1;
    char row_offsets[4] = {0x80, 0xC0, 0x94, 0xD4};
    hc595_writeCommand(row_offsets[row] + ((_col - 1) % 20));
}

void LCD595::home()
{
  hc595_writeCommand(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LCD595::CustomChar(uint8_t location, uint8_t *byteArr)
{
    if (location < 8)
    {
        hc595_writeCommand(LCD_SETCGRAMADDR + (location * 8));
        for (char i = 0; i < 8; i++)
            hc595_writeData(byteArr[i]);
    }
}

void LCD595::PrintCustomChar(uint8_t m)
{
    if (m < 8)
        hc595_writeData(m);
}

void LCD595::BacklightOn(void)
{
    _backlightVal = LCD_BACKLIGHT;
    hc595_sendByte(_backlightVal);
}

void LCD595::BacklightOff(void)
{
    _backlightVal = LCD_NOBACKLIGHT;
    hc595_sendByte(_backlightVal);
}

inline size_t LCD595::write(uint8_t value) {
  hc595_writeData(value);
  return 1; // assume sucess
}

void LCD595::hc595_writeData(uint8_t _data)
{
    // ghi 4 bit cao
    hc595_sendByte(RS_E_1 | _backlightVal);
    hc595_sendByte((_data & 0xF0) | RS_E_1 | _backlightVal);
    hc595_sendByte(((_data & 0xF0) | RS_E_1) & 0xF4 | _backlightVal);
    // ghi 4 bit thap
    hc595_sendByte(RS_E_1 | _backlightVal);
    hc595_sendByte((_data << 4) | RS_E_1 | _backlightVal);
    hc595_sendByte(((_data << 4) | RS_E_1) & 0xF4 | _backlightVal);
}

void LCD595::hc595_writeCommand(uint8_t _cmd)
{
    // ghi 4 bit cao
    hc595_sendByte(E_1 | _backlightVal);
    hc595_sendByte((_cmd & 0xF0) | E_1 | _backlightVal);
    hc595_sendByte(((_cmd & 0xF0) | E_1) & 0xF0 | _backlightVal);
    // Son 4 bit thap
    hc595_sendByte(E_1 | _backlightVal);
    hc595_sendByte((_cmd << 4) | E_1 | _backlightVal);
    hc595_sendByte(((_cmd << 4) | E_1) & 0xF0 | _backlightVal);
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
