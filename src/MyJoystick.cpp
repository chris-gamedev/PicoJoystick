#include "MyJoystick.h"

MyJoystickBT_ MyJoystickBT;
uint16_t Command_::sEnabledButtonsMask = 0xFFFF;
uint8_t Command_::sEnabledJoystickMask = 0xFF;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   Commands & Macros   //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------  Static  --------------------------------------------

void Command_::addButtonToValues(uint8_t b, uint8_t value, uint32_t mask, uint32_t *pValueMap, uint8_t *pJoyState)
{
  // if my button value is allowed
  if (MAKE_BUTTON_BITMASK_16(b) & sEnabledButtonsMask)
    // add my button value to the values sent to the host.
    *pValueMap |= MAKE_BUTTON_VALUE_BITMASK_32(value);
}
//----------------------------------------------------------------------------------------

void DirectButtonCommand_::executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState)
{
  if (!digitalRead(ButtonPins[b]))
  {
    *pStateMap |= MAKE_BUTTON_BITMASK_16(b);
    Command_::addButtonToValues(b, value, mButtonsMask, pValueMap, pJoyState);
  }
}

//---------------------------------------------------------------------------------------

void TurboButtonCommand_::executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState)
{
  bool state = !digitalRead(ButtonPins[b]);
  if (state)
  {
    *pStateMap |= MAKE_BUTTON_BITMASK_16(b);
    if (MyJoystickBT.buttonJustPressed(b))
    {
      mLastTime = 0;
      if (mIsLatchingButton)
      {
        mLatchedOn = !mLatchedOn;
#ifdef DEADBEEF
        Serial.printf("----------latch switch---------------  %d\n", mLatchedOn);
#endif
      }
    }
  }

  if ((mIsLatchingButton && mLatchedOn) || (!mIsLatchingButton && state))
  {
    mActive = true;
    if (mLastTime + mDelay < millis())
    {
      mLastTime = millis();
      Command_::addButtonToValues(b, value, mButtonsMask, pValueMap, pJoyState);
    }
  }
  else
    mActive = false;
}

//----------------------------------------------------------------------------------------

void MacroButtonCommand_ ::executeCommand(uint8_t b, uint8_t value, uint16_t *pStateMap, uint32_t *pValueMap, uint8_t *pJoyState)
{
  // Serial.print("Macro Command: ");
  bool state = !digitalRead(ButtonPins[b]);
  if (state)
  {
    // Serial.print("button pressed, ");
    *pStateMap |= MAKE_BUTTON_BITMASK_16(b);
    // if we're not yet active, we ARE enabled, and we HAVE valid data
    if (!mActive && MAKE_BUTTON_BITMASK_16(b) & sEnabledButtonsMask && mMacro.phrase.size() > 0 && !mForceDisableLock)
    {
      // Serial.print("activating, ");
      mActive = true;
      mLastTime = millis();
      mCurrentWord = 0;
    }
  }

  if (mActive)
  {
    // Serial.print("active, ");
    if (mLastTime + mMacro.phrase[mCurrentWord].mDuration < millis()) // if we're over the duration of the word
    {
      mCurrentWord++;
      mLastTime = millis();
    }
    // Serial.printf("valid count, %d", mCurrentWord);
    if (mCurrentWord >= mMacro.phrase.size()) // if we ran out of words
    {
      mActive = false;
      return;
    }

    // send it.
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) // gather the values of the word buttons and add them to the pointers
    {
      if (mMacro.phrase[mCurrentWord].mButtonStateMap & MAKE_BUTTON_BITMASK_16(i))
      {
        // Serial.printf("button %d active, ", i);
        *pValueMap |= MAKE_BUTTON_VALUE_BITMASK_32(MyJoystickBT.maButtonValues[i]);
      }
    }
    *pJoyState = mMacro.phrase[mCurrentWord].mJoyState;
    // Serial.printf("values: ");
    // Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*pJoyState));
    // Serial.printf("\n");
  }
}

//---------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////   MyJoystick   //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MyJoystickBT_::MyJoystickBT_()
{
  this->use8bit(true);
  this->useManualSend(true);

  for (int i = 0; i < sizeof(ButtonPins) / sizeof(int); i++)
  {
    pinMode(ButtonPins[i], INPUT_PULLUP);
  }

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);

  // testing
  maMacros[0].mMacro.name = "testMacro";
  maMacros[0].mMacro.phrase = {
      {0b0000000000000001, 1, 20},
      {0b0000000000000010, 2, 20},
      {0b0000000000000100, 3, 20},
      {0b0000000000001000, 4, 20},
      {0b0000000000010000, 5, 20},
      {0b0000000000100000, 6, 20},
      {0b0000000001000000, 7, 20},
      {0b0000000010000000, 8, 20}}; 
  
  maMacros[1].mMacro.name = "Fireball R";
  maMacros[1].mButtonsMask = 0x0000;
  maMacros[1].mJoyMask = 0;
  maMacros[1].mMacro.phrase = {
      {0b0000000000000000, 5, 50},
      {0b0000000000000000, 4, 50},
      {0b0000000000100000, 3, 50}}; 

  
  maMacros[2].mMacro.name = "Fireball L";
  maMacros[2].mButtonsMask = 0x0000;
  maMacros[2].mJoyMask = 0;
  maMacros[2].mMacro.phrase = {
      {0b0000000000000000, 5, 50},
      {0b0000000000000000, 6, 50},
      {0b0000000000100000, 7, 50}};



      // maMacros[0].mMacro.enabledJoystickState = 0;
      // maMacros[0].mMacro.enabledButtonsMap = 0b1111111111111011;

      maMacros[0].mJoyMask = 0;
      maMacros[0].mButtonsMask = 0b1111111111111011;
}

/***************************************************************/
/**
 * Read values from all buttons and joystick directions.  After
 * Polling, manually send.
 */
void MyJoystickBT_::pollJoystick()
{
  // BOOTSEL toggles flag to transmit input to host
  if (BOOTSEL)
  {
    delay(10);
    if (BOOTSEL)
    {
      mJoyTransmit = !mJoyTransmit;
      Serial.println("BOOTSEL Pressed, Joystick is " + String(mJoyTransmit));
    }
  }

  mLastPackedButtonStates = mPackedButtonStates;
  mLastJoyState = mJoyState;
  mPackedButtonStates = 0;
  mPackedButtonValues = 0;
  Command_::resetGlobalMasks();

  // assemble the button state & value masks
  for (int i = 0; i < sizeof(ButtonPins) / sizeof(ButtonPins[0]); i++)
    maAssignedMacros[i]->updateMasks();
  for (int i = 0; i < sizeof(ButtonPins) / sizeof(ButtonPins[0]); i++)
    maAssignedMacros[i]->executeCommand(i, maButtonValues[i], &mPackedButtonStates, &mPackedButtonValues, &mJoyState);

  this->data.buttons = mPackedButtonValues;

  // MyJoystick.hat() takes joy position in degrees.  Internally,
  // it maps clockwise from North to 1-8 value.  0 is at rest.
  if (Command_::sEnabledJoystickMask)
  {
    if (digitalRead(BUTTON_UP_PIN) == LOW) // read NORTH
    {                                      // read UP and combinations
      mJoyState = maJoyValues[JOY_UP];
      if (digitalRead(BUTTON_RIGHT_PIN) == LOW)
        mJoyState = maJoyValues[JOY_UP_RIGHT];
      else if (digitalRead(BUTTON_LEFT_PIN) == LOW)
        mJoyState = maJoyValues[JOY_UP_LEFT];
    }
    else if (digitalRead(BUTTON_DOWN_PIN) == LOW) // read SOUTH
    {                                             // read DOWN and combinations
      mJoyState = maJoyValues[JOY_DOWN];
      if (digitalRead(BUTTON_RIGHT_PIN) == LOW)
        mJoyState = maJoyValues[JOY_DOWN_RIGHT];
      else if (digitalRead(BUTTON_LEFT_PIN) == LOW)
        mJoyState = maJoyValues[JOY_DOWN_LEFT];
    }
    else if (digitalRead(BUTTON_RIGHT_PIN) == LOW) // read solo RIGHT
      mJoyState = maJoyValues[JOY_RIGHT];
    else if (digitalRead(BUTTON_LEFT_PIN) == LOW) // read solo LEFT
      mJoyState = maJoyValues[JOY_LEFT];
    else
      mJoyState = maJoyValues[JOY_IDLE];
  }
  this->data.hat = mJoyState;

  if (mJoyTransmit)
    this->send_now();
}
