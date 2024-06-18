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
        Serial.printf("----------latch switch---------------  %d\n", mLatchedOn);
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

//---------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////   MyJoystick   //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MyJoystickBT_::MyJoystickBT_()
{
  this->use8bit(true);
  this->useManualSend(true);

  maButtonValues[0] = 1;
  maButtonValues[1] = 2;
  maButtonValues[2] = 4;
  maButtonValues[3] = 5;
  maButtonValues[4] = 7;
  maButtonValues[5] = 8;
  maButtonValues[6] = 11;
  maButtonValues[7] = 12;
  maButtonValues[8] = 13;
  maButtonValues[9] = 14;
  maButtonValues[10] = 15;
  maButtonValues[11] = 20;

  maJoyValues[0] = JOY_IDLE;
  maJoyValues[1] = JOY_UP;
  maJoyValues[2] = JOY_UP_RIGHT;
  maJoyValues[3] = JOY_RIGHT;
  maJoyValues[4] = JOY_DOWN_RIGHT;
  maJoyValues[5] = JOY_DOWN;
  maJoyValues[6] = JOY_DOWN_LEFT;
  maJoyValues[7] = JOY_LEFT;
  maJoyValues[8] = JOY_UP_LEFT;

  for (int i = 0; i < sizeof(ButtonPins) / sizeof(int); i++)
  {
    pinMode(ButtonPins[i], INPUT_PULLUP);
  }

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);

  maAssignedMacros[0] = &this->mTurboDirectCommand;
  mTurboDirectCommand.mButtonsMask = 0b1111111110111111;
  mTurboDirectCommand.mDelay = 300;

  // test stuff
  mTurboDirectCommand.setIsLatchingButton(false);
  mLatchedCommand.setIsLatchingButton(true);

  maAssignedMacros[1] = &this->mLatchedCommand;
  mLatchedCommand.mButtonsMask = 0b111111101111111;
  mLatchedCommand.mDelay = 300;
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
  if (digitalRead(BUTTON_UP_PIN) == LOW) // read NORTH
  { // read UP and combinations
    mJoyState = maJoyValues[JOY_UP];
    if (digitalRead(BUTTON_RIGHT_PIN) == LOW)
      mJoyState = maJoyValues[JOY_UP_RIGHT];
    else if (digitalRead(BUTTON_LEFT_PIN) == LOW)
      mJoyState = maJoyValues[JOY_UP_LEFT];
  }
  else if (digitalRead(BUTTON_DOWN_PIN) == LOW) //read SOUTH
  { // read DOWN and combinations
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

  this->data.hat = mJoyState;

  if (mJoyTransmit)
    this->send_now();
}

/********************************************************/
/**
 * setHat - sets the dPad value of the gamepad report (bluetooth or usb).
 * The inherited hat() member functino from HID_Joystick wasn't working,
 * so I set the data.hat inherited from HID_Joystick myself here.
 *  @param angle - Value from 1 - 8, clockwise starting from North.
 *
 */
void MyJoystickBT_::setHat(uint8_t angle)
{
  this->data.hat = angle;
}
