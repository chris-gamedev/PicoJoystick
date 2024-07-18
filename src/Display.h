#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Fonts/FreeSans9pt7b.h>
#include "manifold8pt7b.h"
#include "Images.h"

// Used for software SPI
#define OLED_CLK 18
#define OLED_MOSI 19

// Used for software or hardware SPI
#define OLED_CS 17
#define OLED_DC 20

// Used for I2C or SPI
#define OLED_RESET 16

// Used for text characteristics
#define MENU_FONT_FACE &manifold8pt7b
#define TEXT_HEIGHT 14
#define TEXT_WIDTH 10
#define FONT_HANG 0

// Decoration Sizes
#define MENU_TRIM_WIDTH 5
#define MENU_BOX_COLOR_BG 0X4
#define MENU_BOX_COLOR_FG 0X9
#define MENU_TEXT_COLOR_FG 0xF

/// @brief Interface to be used with Animator Class
class AdaFourBit : Adafruit_GFX
{
public:
};

/// @brief Wrapper class
class MyGFXcanvas8_ : public GFXcanvas8
{
public:
    MyGFXcanvas8_(uint16_t w, uint16_t h) : GFXcanvas8(w, h) {}
};
/// @brief Wrapper class
class MyGFXcanvas1_ : public GFXcanvas1
{
public:
    MyGFXcanvas1_(uint16_t w, uint16_t h) : GFXcanvas1(w, h) {}
};

class JoyDisplay_ : public Adafruit_SSD1327
{
public:
    JoyDisplay_()
        : Adafruit_SSD1327{128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS}
    {
    }
    void drawBitmap4Bit(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);
    void drawBitmap4Bit(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool mirrorY);
    void drawBitmap4BitRotate90(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool mirrorY);
    void drawBitmap4BitRotate180(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool mirrorY);
    void drawBitmap4BitRotate270(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, bool mirrorY);
};

extern JoyDisplay_ JoyDisplay;

#endif // DISPLAY_H