#ifndef PICOJOYSTICK_TYPES_H
#define PICOJOYSTICK_TYPES_H
#include <Arduino.h>

// clang-format off
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')


  // Button definitions for applets / menu / etc
#define APPLET_CANCEL_BUTTON    0
#define APPLET_CONFIRM_BUTTON   4
#define APPLET_EXIT_JOYSTATE    7

#define NUMBER_OF_JOY_VALUES 9
#define NUMBER_OF_BUTTONS 12
#define NUMBER_OF_CUSTOM_MACROS 8


struct MacroWord
{
  uint16_t mButtonStateMap = 0;
  uint8_t mJoyState = 0;
  uint16_t mDuration = 50;
};

struct Macro
{
  std::vector<MacroWord> phrase;
  String name = "-blank-";
  uint16_t enabledButtonsMap = 0xFFFF;
  uint8_t enabledJoystickState = 0xFF;
  
};


  
//clang-format on
#endif