#include "Applications.h"
#include "manifold6pt7b.h"
#include "Images.h"

extern Configurator_ Configurator;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     DrawKeyPresses      ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawKeyPressesApp_::initApp()
{
    this->mlife = -1;
    mpCompositor->registerAnimation(this, CanvasType::BG);
}

AppletStatus::TAppletStatus DrawKeyPressesApp_::updateApp()
{
    return AppletStatus::ALIVE;
}

void DrawKeyPressesApp_::cleanupApp()
{
    this->mlife = 0;
}
// for animation class
void DrawKeyPressesApp_::updateAnim()
{
}

void DrawKeyPressesApp_::drawAnim(JoyDisplay_ *pcanvas)
{
    // uint16_t packedKeyPresses, uint8_t buttonValues[], uint8_t joyState
    pcanvas->setFont(&manifold6pt7b);
    pcanvas->setTextSize(1);
    pcanvas->setTextColor(0xF);
    uint32_t packedKeyPresses = MyJoystickBT.getPackedKeyPresses();
    uint8_t *buttonValues = MyJoystickBT.getButtonValues();
    uint8_t joyState = MyJoystickBT.getJoystickState();
    uint8_t buttonCenterColor = 0x9;
    uint8_t pressColor = 0x3;
    for (int i = 0; i < map_bitmap_SmallButtonLayout_count - 1; i++)
    {
        // set Disabled Button center color:
        if (packedKeyPresses & MAKE_BUTTON_BITMASK_16(i))
            buttonCenterColor = 0x2;
        else if (!(Command_::sEnabledButtonsMask & MAKE_BUTTON_BITMASK_16(i)))
            buttonCenterColor = 0x6; // disabled
        else
            buttonCenterColor = 0x8; // enabled
        
        // draw button center
        pcanvas->drawBitmap(map_bitmap_SmallButtonLayout[i][0] - 9, map_bitmap_SmallButtonLayout[i][1] - 9, bitmap_ButtonCenter, 20, 20, buttonCenterColor);
        // draw button ring
        pcanvas->drawBitmap(map_bitmap_SmallButtonLayout[i][0] - 9, map_bitmap_SmallButtonLayout[i][1] - 9, bitmap_buttonRingAllArray[maMacroMap[i]], 20, 20, 0xD);

        if (packedKeyPresses & MAKE_BUTTON_BITMASK_16(i))
        {
            pcanvas->setCursor(map_bitmap_SmallButtonLayout[i][0] - 3 - (buttonValues[i] > 9) * 3, map_bitmap_SmallButtonLayout[i][1] + 4);
            pcanvas->setTextColor(0xB);
            pcanvas->print(buttonValues[i]);
        }
    }

    int8_t orgX = map_bitmap_SmallButtonLayout[12][0];
    int8_t orgY = map_bitmap_SmallButtonLayout[12][1];

    // draw arrow bitmaps. 21x21
    // Draw N / S on Joystick
    switch (joyState)
    {
    case JOY_UP_LEFT:
    case JOY_UP:
    case JOY_UP_RIGHT:
        // NORTH
        pcanvas->drawBitmap(orgX - 10, orgY - 21, bitmap_arrowUp, 21, 21, 0xF);
        break;
    case JOY_DOWN_RIGHT:
    case JOY_DOWN:
    case JOY_DOWN_LEFT:
        // SOUTH
        pcanvas->drawBitmap(orgX - 10, orgY, bitmap_arrowDown, 21, 21, 0xF);
        break;
    }
    // draw W / E on Joystick
    switch (joyState)
    {
    case JOY_UP_RIGHT:
    case JOY_RIGHT:
    case JOY_DOWN_RIGHT:
        // EAST
        pcanvas->drawBitmap(orgX, orgY - 10, bitmap_arrowRight, 21, 21, 0xF);
        break;
    case JOY_DOWN_LEFT:
    case JOY_LEFT:
    case JOY_UP_LEFT:
        // WEST
        if (joyState == 8)
        pcanvas->drawBitmap(orgX - 21, orgY - 10, bitmap_arrowLeft, 21, 21, 0xF);
        break;
    }

    pcanvas->drawPixel(map_bitmap_SmallButtonLayout[12][0], map_bitmap_SmallButtonLayout[12][1], 0xA);
}

void DrawKeyPressesApp_::configure(Configuration *pconfig)
{
        for (int i = 0; i < 12; i++) 
            maMacroMap[i] = pconfig->drawKeypresses_macroMap[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    RemapButtonsApp_     ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RemapButtonsApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    this->mlife = -1;
    mpCompositor->registerAnimation(this, CanvasType::BOTTOM);
    mAppLastTime = millis();

    mTextSpriteStatic.setLife(50);
    mTextSpriteStatic.setDrawBox(true);
    mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);

    mTextSpriteBanner.mlife = -1;
    mpCompositor->registerAnimation(&mTextSpriteBanner, CanvasType::BG);
    mAppDrawPrompt = true;
    startFromScratch();
}

void RemapButtonsApp_::startFromScratch()
{
    mFoundTheButton = false;
    mSetTheValue = false;
    mConfirmedTheButton = false;
    mEditButton = -1;
    mNewButtonValue = 0;
}

AppletStatus::TAppletStatus RemapButtonsApp_::updateApp()
{

    mpappDrawKeysApp->updateApp();

    // Hold LEFT to exit
    if (MyJoystickBT.joyJustPressed(JOY_LEFT))
    {
        startFromScratch();
        mAppLastTime = millis();
        return AppletStatus::ALIVE;
    }
    else if (MyJoystickBT.joyHeld(JOY_LEFT))
        if (mAppLastTime + mAppDelay > millis())
            return AppletStatus::ALIVE;
        else
        {
            mTextSpriteStatic.mlife = 0;
            return AppletStatus::RETURN;
        }

    if (MyJoystickBT.buttonJustPressed(4))
        mAppDrawPrompt = false;

    if (!mFoundTheButton)
    {
        mLocalButtonStateMap = MyJoystickBT.getPackedKeyPresses();

        mEditButton = Applet_::findHeldButton(mLocalButtonStateMap, 1500);
        if (mEditButton == 255)
            return AppletStatus::ALIVE;

        mFoundTheButton = true;
        mNewButtonValue = MyJoystickBT.getButtonValue(mEditButton);
        Serial.println("Found a held button!!  " + String(mEditButton));

        mAnimInputDialog.start("New Value", &mNewButtonValue, 1, 32);
        mAppDrawPrompt = false;
        return AppletStatus::ALIVE;
    }
    else if (mFoundTheButton && !mSetTheValue)
    {

        if (mAnimInputDialog.updateDialog())
            return AppletStatus::ALIVE;

        mSetTheValue = true;
        MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
        startFromScratch();
    }

    return AppletStatus::ALIVE;
}

void RemapButtonsApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mlife = 0;
    mTextSpriteBanner.mlife = 0;
}

void RemapButtonsApp_::updateAnim()
{
}

void RemapButtonsApp_::drawAnim(JoyDisplay_ *pcanvas)
{

    if (!mAppDrawPrompt)
    {
        mTextSpriteStatic.mlife = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     Assign Macros       ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AssignTurboApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    this->mlife = -1;
    mpCompositor->registerAnimation(this, CanvasType::TOP);
    mAppLastTime = millis();

    mTextSpriteStatic.setLife(50);
    mTextSpriteStatic.setDrawBox(true);
    mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);

    mTextSpriteBanner.mlife = -1;
    mpCompositor->registerAnimation(&mTextSpriteBanner, CanvasType::BG);
    startFromScratch();
}

void AssignTurboApp_::startFromScratch()
{
    mEditButton = -1;
    mFoundTheButton = false;
    mFoundTheButton = false;
    mSetButtonMode = false;
    mSetButtonDelay = false;
    mSetButtonLatching = false;
}

bool AssignTurboApp_::exitApp()
{

    if (MyJoystickBT.joyJustPressed(JOY_LEFT))
    {
        startFromScratch();
        mAppLastTime = millis();
        return false;
    }
    else if (MyJoystickBT.joyHeld(JOY_LEFT))
    {
        if (mAppLastTime + mAppDelay > millis())
            return false;
        else
        {
            return true;
        }
    }
    return false;
}

AppletStatus::TAppletStatus AssignTurboApp_::updateApp()
{
    mpappDrawKeysApp->updateApp();

    if (exitApp())
    {
        return AppletStatus::RETURN;
    }

    if (!mFoundTheButton)
    {

        mEditButton = Applet_::findHeldButton(MyJoystickBT.getPackedKeyPresses(), 1500);
        if (mEditButton == 255)
            return AppletStatus::ALIVE;

        mFoundTheButton = true;
        Serial.println("Found a held button!!  " + String(mEditButton));
        mAnimInputDialogOptions.start("Turbo:", &mSelection, {"On", "Off"});
        return AppletStatus::ALIVE;
    }
    else if (!mSetButtonMode)
    {
        if (mAnimInputDialogOptions.updateDialog())
            return AppletStatus::ALIVE;

        mSetButtonMode = true;
        switch (mSelection)
        {
        case 1:
            MyJoystickBT.setToDefaultMacro(mEditButton);
            Configurator.mConfig.drawKeypresses_macroMap[mEditButton] = MacroMapType::none;
            Configurator.configurate();
            startFromScratch();
            break;
        case 0:
            MyJoystickBT.setToTurboMacro(mEditButton);
            mTurboDelayValue = MyJoystickBT.getTurboMacroDelay(mEditButton);
            mAnimInputDialogDelay.start("Delay (m/s)", &mTurboDelayValue, 10, 4000);
            break;
        }
        return AppletStatus::ALIVE;
    }
    else if (!mSetButtonDelay)
    {
        if (mAnimInputDialogDelay.updateDialog())
        {
            return AppletStatus::ALIVE;
        }
        mSetButtonDelay = true;
        MyJoystickBT.setTurboMacroDelay(mEditButton, mTurboDelayValue);
        mAnimInputDialogOptions.start("Latching", &mSelection, {"Off", "On"});
        return AppletStatus::ALIVE;
    }
    else if (!mSetButtonLatching)
    {
        if (mAnimInputDialogOptions.updateDialog())
            return AppletStatus::ALIVE;

        MyJoystickBT.setTurboMacroLatching(mEditButton, (bool)mSelection);
        Configurator.mConfig.drawKeypresses_macroMap[mEditButton] = (mSelection) ? MacroMapType::latchedTurbo : MacroMapType::turbo;
        Configurator.configurate();
        startFromScratch();
    }
    return AppletStatus::ALIVE;
}

void AssignTurboApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mlife = 0;
    mTextSpriteBanner.mlife = 0;
}

void AssignTurboApp_::updateAnim()
{
}

void AssignTurboApp_::drawAnim(JoyDisplay_ *pcanvas)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     Create Macro        ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
