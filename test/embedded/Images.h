#ifndef IMAGES_H
#define IMAGES_H
#include <Arduino.h>

// clang-format off

// map of button centers in bitmap_SmallButtonLayout. 12 is Joystick.
const uint8_t map_bitmap_SmallButtonLayout_count = 13;
const uint8_t map_bitmap_SmallButtonLayout[13][2] = {
      {54,73}   //0 
    , {74,64}   //1
    , {96,64}   //2
    , {118,64}  //3
    , {49,96}   //4
    , {69,88}   //5
    , {91,88}   //6 
    , {113,89}  //7
    , {15,32}   //8
    , {37,32}   //9
    , {93,33}   //10
    , {114,33}  //11
    // Joystick
    , {21,73}   //12

};
// 'SmallButtonLayout', 128x128px
const unsigned char bitmap_SmallButtonLayout [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1f, 0xe0, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 
	0x00, 0x3f, 0xf8, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x03, 0xfe, 0x00, 
	0x00, 0x7f, 0xfc, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x07, 0xff, 0x00, 
	0x00, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0x80, 
	0x01, 0xff, 0xfe, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x1f, 0xff, 0xc0, 
	0x01, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 
	0x03, 0xff, 0xff, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 
	0x03, 0xff, 0xff, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 
	0x03, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0xff, 0xf0, 
	0x03, 0xff, 0xff, 0x8f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0xff, 0xf0, 
	0x03, 0xff, 0xff, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0xff, 0xf0, 
	0x01, 0xff, 0xff, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 
	0x01, 0xff, 0xff, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x3f, 0xff, 0xe0, 
	0x01, 0xff, 0xfe, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x3f, 0xff, 0xe0, 
	0x00, 0xff, 0xfc, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x1f, 0xff, 0xc0, 
	0x00, 0x7f, 0xfc, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x1f, 0xff, 0x80, 
	0x00, 0x1f, 0xf0, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x0f, 0xff, 0x00, 
	0x00, 0x07, 0xc0, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x03, 0xfe, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0xf8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x07, 0xc0, 0x00, 0x1f, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x1f, 0xf8, 0x00, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x3f, 0xfc, 0x01, 0xff, 0xf0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x7f, 0xfe, 0x03, 0xff, 0xf8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0xff, 0xfe, 0x03, 0xff, 0xf8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x87, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x8f, 0xff, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x8f, 0xff, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x8f, 0xff, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x8f, 0xff, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x87, 0xff, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x3f, 0xff, 0xc0, 0xff, 0xff, 0x07, 0xff, 0xf8, 
	0x00, 0x00, 0x1f, 0x80, 0x00, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0x80, 0xff, 0xfe, 0x03, 0xff, 0xf8, 
	0x00, 0x00, 0x3f, 0xc0, 0x00, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0x80, 0x7f, 0xfc, 0x01, 0xff, 0xf0, 
	0x00, 0x00, 0x3f, 0xc0, 0x00, 0x07, 0xff, 0xfe, 0x0f, 0xfe, 0x00, 0x3f, 0xf8, 0x00, 0xff, 0xe0, 
	0x00, 0x00, 0x3f, 0xc0, 0x00, 0x07, 0xff, 0xfe, 0x03, 0xfc, 0x00, 0x0f, 0xf0, 0x00, 0x3f, 0x80, 
	0x00, 0x00, 0x3f, 0xc0, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1f, 0x80, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x1f, 0x80, 0x00, 0xfe, 0x00, 0x03, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x7f, 0xe0, 0x03, 0xff, 0x80, 0x0f, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x01, 0xff, 0xf0, 0x07, 0xff, 0xc0, 0x1f, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 0x3f, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x1f, 0xff, 0xe0, 0x7f, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x7f, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x07, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x07, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x87, 0xff, 0xfe, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x87, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x7f, 0xff, 0xc0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc3, 0xff, 0xfc, 0x1f, 0xff, 0xe0, 0x7f, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc3, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 0x3f, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe1, 0xff, 0xf0, 0x07, 0xff, 0xc0, 0x1f, 0xff, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0xff, 0xe0, 0x03, 0xff, 0x80, 0x0f, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x1f, 0x80, 0x00, 0xfc, 0x00, 0x03, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/
// 'arrowUp', 21x21px
const unsigned char bitmap_arrowUp [] PROGMEM = {
	0x00, 0x70, 0x00, 0x00, 0xf8, 0x00, 0x01, 0xfc, 0x00, 0x03, 0xfe, 0x00, 0x07, 0xff, 0x00, 0x0f, 
	0xff, 0x80, 0x1f, 0xff, 0xc0, 0x3e, 0xfb, 0xe0, 0x7c, 0xf9, 0xf0, 0xf8, 0xf8, 0xf8, 0x70, 0xf8, 
	0x70, 0x20, 0xf8, 0x20, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 
	0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x70, 0x00
};
// 'arrowRight', 21x21px
const unsigned char bitmap_arrowRight [] PROGMEM = {
	0x00, 0x10, 0x00, 0x00, 0x38, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x1f, 0x00, 0x00, 
	0x0f, 0x80, 0x00, 0x07, 0xc0, 0x00, 0x03, 0xe0, 0x7f, 0xff, 0xf0, 0xff, 0xff, 0xf8, 0xff, 0xff, 
	0xf8, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x80, 
	0x00, 0x1f, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x38, 0x00, 0x00, 0x10, 0x00
};
// 'arrowDown', 21x21px
const unsigned char bitmap_arrowDown [] PROGMEM = {
	0x00, 0x70, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 
	0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf8, 0x00, 0x20, 0xf8, 0x20, 0x70, 0xf8, 
	0x70, 0xf8, 0xf8, 0xf8, 0x7c, 0xf9, 0xf0, 0x3e, 0xfb, 0xe0, 0x1f, 0xff, 0xc0, 0x0f, 0xff, 0x80, 
	0x07, 0xff, 0x00, 0x03, 0xfe, 0x00, 0x01, 0xfc, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x70, 0x00
};
// 'arrowLeft', 21x21px
const unsigned char bitmap_arrowLeft [] PROGMEM = {
	0x00, 0x40, 0x00, 0x00, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xc0, 0x00, 0x0f, 
	0x80, 0x00, 0x1f, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7f, 0xff, 0xf0, 0xff, 0xff, 0xf8, 0xff, 0xff, 
	0xf8, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xf0, 0x3e, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x0f, 0x80, 0x00, 
	0x07, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00
};

// // Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 352)
// const int bitmap_arrows_allArray_LEN = 4;
// const unsigned char* bitmap_arrows_allArray[4] = {
// 	bitmap_arrowDown,
// 	bitmap_arrowLeft,
// 	bitmap_arrowRight,
// 	bitmap_arrowUp
// };


/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/

// 'menuBorderBottom', 120x5px
const unsigned char bitmap_menuBorderBottom [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 
	0xe0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
// 'menuBorderTop', 120x5px
const unsigned char bitmap_menuBorderTop [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xe0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 0x03, 0xe0, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'menuBorderL', 4x64px
const unsigned char bitmap_menuBorderL [] PROGMEM = {
	0xf0, 0xf0, 0xe0, 0xd0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xf0, 
	0xf0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xd0, 0xe0, 0xf0, 0xf0
};
// 'menuBorderR', 4x64px
const unsigned char bitmap_menuBorderR [] PROGMEM = {
	0xf0, 0xf0, 0x70, 0xb0, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x30, 0x30, 0x30, 0x30, 0x70, 0xf0, 
	0xf0, 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xb0, 0x70, 0xf0, 0xf0
};

#endif // MYJOYSTICK_H