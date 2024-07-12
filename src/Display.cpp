#include "Display.h"

JoyDisplay_ JoyDisplay;

/// @brief Draw prompts from a given vector of strings to the menu buffer
/// @param vP vector of strings
/// @param currentPrompt current prompt position in the prompt vector

void JoyDisplay_::drawBitmap4Bit(int16_t x, int16_t y, const uint8_t *bitmap,
                                 int16_t w, int16_t h)
{
    uint8_t b = 0;
    int16_t byteWidth = (w + 1) / 2; // Bitmap scanline pad = whole byte

    startWrite();
    for (int16_t j = 0; j < h; j++, y++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            // when do we get another byte
            if (i & 1)
                b <<= 4;
            else
                b = pgm_read_byte(&bitmap[j * byteWidth + i / 2]);
            // if the leftmost 4 bits are not black
            if (b & 0b11110000)
                // write leftmost 4 bits
                writePixel(x + i, y, (b >> 4));
        }
    }
    endWrite();
}

void JoyDisplay_::drawBitmap4Bit(int16_t x, int16_t y, const uint8_t *bitmap,
                                 int16_t w, int16_t h, bool mirrorY)
{
    int8_t incy = 1;
    if (mirrorY)
    {
        y += h - 1;
        incy = -1;
    }
    uint8_t b = 0;
    int16_t byteWidth = (w + 1) / 2; // Bitmap scanline pad = whole byte

    startWrite();
    for (int16_t j = 0; j < h; j++, y += incy)
    {
        for (int16_t i = 0; i < w; i++)
        {
            // when do we get another byte
            if (i & 1)
                b <<= 4;
            else
                b = pgm_read_byte(&bitmap[j * byteWidth + i / 2]);
            // if the leftmost 4 bits are not black
            if (b & 0b11110000)
                // write leftmost 4 bits
                writePixel(x + i, y, (b >> 4));
        }
    }
    endWrite();
}

void JoyDisplay_::drawBitmap4BitRotate90(int16_t x, int16_t y, const uint8_t *bitmap,
                                         int16_t w, int16_t h, bool mirrorY)
{
    int8_t incx = 1;
    if (!mirrorY)
    {
        incx = -1;
        x += w - 1;
    }

    uint8_t b = 0;
    int16_t byteWidth = (w + 1) / 2; // Bitmap scanline pad = whole byte

    startWrite();
    for (int16_t j = 0; j < h; j++, x += incx)
    {
        for (int16_t i = 0; i < w; i++)
        {
            // when do we get another byte
            if (i & 1)
                b <<= 4;
            else
                b = pgm_read_byte(&bitmap[j * byteWidth + i / 2]);
            // if the leftmost 4 bits are not black
            if (b & 0b11110000)
                // write leftmost 4 bits
                writePixel(x, y + i, (b >> 4));
        }
    }
    endWrite();
}

void JoyDisplay_::drawBitmap4BitRotate180(int16_t x, int16_t y, const uint8_t *bitmap,
                                          int16_t w, int16_t h, bool mirrorY)
{
    x += w - 1;

    int8_t incy = 1;
    if (!mirrorY)
    {
        incy = -1;
        y += h - 1;
    }

    uint8_t b = 0;
    int16_t byteWidth = (w + 1) / 2; // Bitmap scanline pad = whole byte

    startWrite();
    for (int16_t j = 0; j < h; j++, y += incy)
    {
        for (int16_t i = 0; i < w; i++)
        {
            // when do we get another byte
            if (i & 1)
                b <<= 4;
            else
                b = pgm_read_byte(&bitmap[j * byteWidth + i / 2]);
            // if the leftmost 4 bits are not black
            if (b & 0b11110000)
                // write leftmost 4 bits
                writePixel(x - i, y, (b >> 4));
        }
    }
    endWrite();
}

void JoyDisplay_::drawBitmap4BitRotate270(int16_t x, int16_t y, const uint8_t *bitmap,
                                          int16_t w, int16_t h, bool mirrorY)
{
    y += h - 1;
    int8_t incx = 1;
    if (mirrorY)
    {
        incx = -1;
        x += w - 1;
    }

    uint8_t b = 0;
    int16_t byteWidth = (w + 1) / 2; // Bitmap scanline pad = whole byte

    startWrite();
    for (int16_t j = 0; j < h; j++, x += incx)
    {
        for (int16_t i = 0; i < w; i++)
        {
            // when do we get another byte
            if (i & 1)
                b <<= 4;
            else
                b = pgm_read_byte(&bitmap[j * byteWidth + i / 2]);
            // if the leftmost 4 bits are not black
            if (b & 0b11110000)
                // write leftmost 4 bits
                writePixel(x, y - i, (b >> 4));
        }
    }
    endWrite();
}
