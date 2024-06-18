#ifndef MYJOYSTICK_H
#define MYJOYSTICK_H
#include <Arduino.h>
#include <JoystickBT.h>
#include <HID_Joystick.h>
#include <forward_list>

// clang-format off
// physical pinout of buttons and Joystick
#define BUTTON_00_PIN     0
#define BUTTON_01_PIN     1
#define BUTTON_02_PIN     2
#define BUTTON_03_PIN     3
#define BUTTON_04_PIN    10
#define BUTTON_05_PIN    11
#define BUTTON_06_PIN    12
#define BUTTON_07_PIN    13
#define BUTTON_08_PIN    14
#define BUTTON_09_PIN    15
#define BUTTON_10_PIN    21
#define BUTTON_11_PIN    22
#define BUTTON_LEFT_PIN   6
#define BUTTON_RIGHT_PIN  7
#define BUTTON_DOWN_PIN   8 
#define BUTTON_UP_PIN     9



// button bitmasks
#define BITMASK_BUTTON_0     (1UL << 0 )
#define BITMASK_BUTTON_1     (1UL << 1 )   
#define BITMASK_BUTTON_2     (1UL << 2 )   
#define BITMASK_BUTTON_3     (1UL << 3 )   
#define BITMASK_BUTTON_4     (1UL << 4 )   
#define BITMASK_BUTTON_5     (1UL << 5 )   
#define BITMASK_BUTTON_6     (1UL << 6 )   
#define BITMASK_BUTTON_7     (1UL << 7 )   
#define BITMASK_BUTTON_8     (1UL << 8 )   
#define BITMASK_BUTTON_9     (1UL << 9 )   
#define BITMASK_BUTTON_10    (1UL << 10)   
#define BITMASK_BUTTON_11    (1UL << 11)   

#define BITMASK_MENU_HOTKEY (BITMASK_BUTTON_11)

//single button check macro
#define BUTTON_IS_PRESSED(n, packed) (1U << n & packed)
#define MAKE_BUTTON_VALUE_BITMASK_32(n) (1UL << (n - 1))
#define MAKE_BUTTON_BITMASK_16(n) (1U << (n))

// Joystick Direction values 
#define JOY_IDLE          0
#define JOY_UP            1    
#define JOY_UP_RIGHT      2     
#define JOY_RIGHT         3     
#define JOY_DOWN_RIGHT    4      
#define JOY_DOWN          5      
#define JOY_DOWN_LEFT     6      
#define JOY_LEFT          7 
#define JOY_UP_LEFT       8 

// array of physical button pin numbers on pico

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
// clang-format on

const int ButtonPins[] =
    {
        BUTTON_00_PIN,
        BUTTON_01_PIN,
        BUTTON_02_PIN,
        BUTTON_03_PIN,
        BUTTON_04_PIN,
        BUTTON_05_PIN,
        BUTTON_06_PIN,
        BUTTON_07_PIN,
        BUTTON_08_PIN,
        BUTTON_09_PIN,
        BUTTON_10_PIN,
        BUTTON_11_PIN};

namespace JoystickPins
{
  enum TJoystickPins
  {
    LEFT = 6,
    RIGHT = 7,
    DOWN = 8,
    UP = 9
  };
}

class Debouncer_
{
public:
  uint16_t mHistory[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t mMask = 0b11000000;
  Debouncer_() {}
  u_int16_t debounce(uint16_t b)
  {

    mHistory[b] = (mHistory[b] << 1) | digitalRead(b);
    Serial.printf("History of button %d is :", mHistory[b]);
    Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mHistory[b]));
    Serial.printf("\n");
    if (mHistory[b] == mMask)
      mHistory[b] = 0;

    return !mHistory[b];
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////   COMMANDS AND MACROS   ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class Command_
{
public:
  /** @brief Executed when button is pressed.
   * @param pPackedJoyState - bits 1-32 are button received by host. Joystick is integer 1 - 8 clockwise starting from North, and 0
   *                          is at rest.
   */

  virtual void executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState) = 0;
  virtual void updateMasks()
  {
    if (mActive)
    {
      sEnabledButtonsMask &= mButtonsMask;
      sEnabledJoystickMask &= mJoyMask;
    }
  }
  void inline setButtonMask(uint32_t buttonMask) { mButtonsMask = buttonMask; }
  void inline setJoystickMask(uint8_t joyMask) { sEnabledJoystickMask = joyMask; }
  static void inline addButtonToValues(uint8_t b, uint8_t value, uint32_t mask, uint32_t *pValueMap, uint8_t *pJoyState);
  static void inline resetGlobalMasks()
  {
    sEnabledButtonsMask = 0xFFFF;
    sEnabledJoystickMask = 0xFF;
  }

  static uint16_t sEnabledButtonsMask;
  static uint8_t sEnabledJoystickMask;
  uint32_t mAssembledOutput;

  bool mActive = false;
  bool mLastButtonState = false;
  uint32_t mLastTime = 0;
  int16_t mDelay = 100;
  uint32_t mButtonsMask = 0xFFFFFFF;
  uint8_t mJoyMask = 0xFF;

protected:
};

class DirectButtonCommand_ : public Command_
{
public:
  void executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState);
};

class TurboButtonCommand_ : public Command_
{
public:
  void executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState);
  void inline setIsLatchingButton(bool latching) { mIsLatchingButton = latching; }
  bool mLatchedOn = false;
  bool mIsLatchingButton;
};

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  MyJoystick  /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class MyJoystickBT_ : public JoystickBT_, public Debouncer_
{
public:
  MyJoystickBT_();
  void pollJoystick();
  void setHat(uint8_t angle);
  uint32_t inline getPackedKeyPresses() { return mPackedButtonStates; }
  uint8_t inline getJoystickState() { return mJoyState; }
  uint8_t inline *getButtonValues() { return maButtonValues; }
  bool inline buttonJustPressed(uint8_t b) { return ((MAKE_BUTTON_BITMASK_16(b) & mPackedButtonStates) && !(MAKE_BUTTON_BITMASK_16(b) & mLastPackedButtonStates)); }
  bool inline buttonHeldDown(uint8_t b) { return ((MAKE_BUTTON_BITMASK_16(b) & mPackedButtonStates) && (MAKE_BUTTON_BITMASK_16(b) & mLastPackedButtonStates)); }
  bool inline buttonJustReleased(uint8_t b) { return (!(MAKE_BUTTON_BITMASK_16(b) & mPackedButtonStates) && (MAKE_BUTTON_BITMASK_16(b) & mLastPackedButtonStates)); }
  bool inline buttonIsFree(uint8_t b) { return (!(MAKE_BUTTON_BITMASK_16(b) & mPackedButtonStates) && !(MAKE_BUTTON_BITMASK_16(b) & mLastPackedButtonStates)); }
  bool inline allButtonsFree() { return (!mPackedButtonStates && !mLastPackedButtonStates); }
  bool inline joyJustPressed(uint8_t jstate) { return mJoyState == jstate && mLastJoyState != jstate; }
  bool inline joyHeld(uint8_t jstate) { return mJoyState == jstate && mLastJoyState == jstate; }
  bool inline joyJustReleased(uint8_t jstate) { return mJoyState != jstate && mLastJoyState == jstate; }
  bool inline joyisFree(uint8_t jstate) { return mJoyState != jstate && mLastJoyState != jstate; }
  // transmit stuff
  void inline setJoyTransmit(bool transmit) { mJoyTransmit = transmit;}
  void inline setPauseButton(uint8_t bValue) { mPauseButtonValue = bValue; }
  void inline sendPauseButtonPress()
  {
    this->data.buttons &= MAKE_BUTTON_VALUE_BITMASK_32(mPauseButtonValue);
    this->send_now();
  }
  // Macro Stuff
  inline void setToDefaultMacro(uint8_t button) { maAssignedMacros[button] = &mDefaultDirectCommand;} 
  inline void setToTurboMacro(uint8_t button) { maAssignedMacros[button] = &maTurboMacros[button];}
  inline void setTurboMacroDelay(uint8_t button, uint16_t delay) {maTurboMacros[button].mDelay = delay;}
  inline void setTurboMacroLatching(uint8_t button, bool latch) { maTurboMacros[button].mIsLatchingButton = latch;}
  inline uint16_t getTurboMacroDelay(uint8_t button) {return maTurboMacros[button].mDelay;}
  
  void inline setButtonValue(uint8_t b, uint8_t v)
  {
    if ((b >= 0) && (b < 12) && (v > 0) && (v < 33))
      maButtonValues[b] = v;
  }
  uint8_t inline getButtonValue(uint8_t b) { return ((b >= 0) && (b < 12)) ? maButtonValues[b] : -1; }

  DirectButtonCommand_ mDefaultDirectCommand;
  TurboButtonCommand_ mTurboDirectCommand;
  TurboButtonCommand_ mLatchedCommand;
  std::forward_list<Command_ *, std::allocator<Command_ *>> mlActiveMacros;
  Command_ *maAssignedMacros[12] = {&mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand, &mDefaultDirectCommand};
  TurboButtonCommand_ maTurboMacros[12];
  uint8_t mButtons[12];
  uint8_t maButtonValues[12];
  uint8_t maJoyValues[8];
  bool mJoyTransmit = true;
  uint16_t mPackedButtonStates;
  uint32_t mPackedButtonValues;
  uint16_t mLastPackedButtonStates;
  uint8_t mJoyState = 0;
  uint8_t mLastJoyState = 0;
  Debouncer_ mDebouncer;
  uint8_t mPauseButtonValue = 255;
};

extern MyJoystickBT_ MyJoystickBT;

#endif // MYJOYSTICK_H