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
            buttonCenterColor = 0x5; // disabled
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
        pcanvas->drawBitmap(orgX - 21, orgY - 10, bitmap_arrowLeft, 21, 21, 0xF);
        break;
    }
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
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);
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
        mNewButtonValue = Configurator.mConfig.joystick_buttonValueMap[mEditButton];

        mAnimInputDialog.start("New Value", &mNewButtonValue, 1, 32);
        mAppDrawPrompt = false;
        return AppletStatus::ALIVE;
    }
    else if (mFoundTheButton && !mSetTheValue)
    {

        if (mAnimInputDialog.updateDialog())
            return AppletStatus::ALIVE;

        mSetTheValue = true;
        // MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
        Configurator.mConfig.joystick_buttonValueMap[mEditButton] = mNewButtonValue;
        Configurator.configurate();
        startFromScratch();
    }

    return AppletStatus::ALIVE;
}

void RemapButtonsApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mlife = 0;
    mTextSpriteBanner.mlife = 0;
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
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
////////////////////////////////////////////     Assign Turbo        ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AssignTurboApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    mAppLastTime = millis();

    mTextSpriteStatic.setLife(50);
    mTextSpriteStatic.setDrawBox(true);
    mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);

    mTextSpriteBanner.mlife = -1;
    mpCompositor->registerAnimation(&mTextSpriteBanner, CanvasType::BG);

    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);

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
    }
    else if (MyJoystickBT.joyHeld(JOY_LEFT))
        if (mAppLastTime + mAppDelay < millis())
            return true;

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
    mTextSpriteBanner.mlife = 0;

    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     Assign Macros       ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AssignMacroApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    mAppLastTime = millis();

    mTextSpriteStatic.mlife = 50;
    mTextSpriteStatic.mDrawBox = true;
    mTextSpriteStatic.setText({"Hold Button", "<- to exit"});
    mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);

    mTextSpriteBanner.mlife = -1;
    mpCompositor->registerAnimation(&mTextSpriteBanner, CanvasType::BG);

    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);

    startFromScratch();
}

void AssignMacroApp_::startFromScratch()
{
    mEditButton = -1;
    mFoundTheButton = false;
    mFoundTheButton = false;
    mSetButtonMode = false;
}

bool AssignMacroApp_::exitApp()
{
    if (MyJoystickBT.joyJustPressed(JOY_LEFT))
    {
        startFromScratch();
        mAppLastTime = millis();
    }
    else if (MyJoystickBT.joyHeld(JOY_LEFT))
        if (mAppLastTime + mAppDelay < millis())
            return true;

    return false;
}

AppletStatus::TAppletStatus AssignMacroApp_::updateApp()
{
    mpappDrawKeysApp->updateApp();

    if (exitApp())
        return AppletStatus::RETURN;

    if (!mFoundTheButton)
    {
        mEditButton = Applet_::findHeldButton(MyJoystickBT.getPackedKeyPresses(), 1500);
        if (mEditButton == 255)
            return AppletStatus::ALIVE;

        mFoundTheButton = true;
        // clang-format off
        mAnimInputDialogOptions.start("Macro:", &mSelection, {
                        MyJoystickBT.maMacros[0].mMacro.name
                      , MyJoystickBT.maMacros[1].mMacro.name
                      , MyJoystickBT.maMacros[2].mMacro.name
                      , MyJoystickBT.maMacros[3].mMacro.name
                      , MyJoystickBT.maMacros[4].mMacro.name
                      , MyJoystickBT.maMacros[5].mMacro.name
                      , MyJoystickBT.maMacros[6].mMacro.name
                      , MyJoystickBT.maMacros[7].mMacro.name
                      , "None"});

        // clang-format on
        return AppletStatus::ALIVE;
    }
    else if (!mSetButtonMode)
    {
        if (mAnimInputDialogOptions.updateDialog())
            return AppletStatus::ALIVE;

        if (mAnimInputDialogOptions.mCancel)
        {
            mFoundTheButton = false;
            return AppletStatus::ALIVE;
        }
        if (mSelection == NUMBER_OF_CUSTOM_MACROS)
        {
            MyJoystickBT.setToDefaultMacro(mEditButton);
            mFoundTheButton = false;
            Configurator.mConfig.drawKeypresses_macroMap[mEditButton] = MacroMapType::none;
            Configurator.configurate();
            return AppletStatus::ALIVE;
        }

        if (MyJoystickBT.maMacros[mSelection].mMacro.phrase.size() == 0)
        {
            mTextSpriteStatic.setText({"Empty Macro", "Try Again"});
            mTextSpriteStatic.mlife = 50;
            mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::FG);
            mFoundTheButton = false;
            return AppletStatus::ALIVE;
        }

        MyJoystickBT.setToCustomMacro(mEditButton, mSelection);
        Configurator.mConfig.drawKeypresses_macroMap[mEditButton] = mSelection + 1;
        Configurator.configurate();
        startFromScratch();

        return AppletStatus::ALIVE;
    }

    return AppletStatus::ALIVE;
}

void AssignMacroApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mTextSpriteBanner.mlife = 0;

    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    Save Configuration     /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaveConfigurationApp_::initApp()
{
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);

    startFromScratch();
}

void SaveConfigurationApp_::startFromScratch()
{
    mFilename = "";
    mEnteredFilename = false;
    mSavedFile = false;
    mShowConfirmation = false;
    mTextSpriteStatic.setLife(50);
    mTextSpriteStatic.setDrawBox(true);
    mTextSpriteStatic.setText({"Enter a", "Filename"});
    mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);

    manimInputFilenameDialog.start("Save As:", &mFilename);
}

AppletStatus::TAppletStatus SaveConfigurationApp_::updateApp()
{
    if (!mEnteredFilename)
    {
        if (manimInputFilenameDialog.updateDialog())
            return AppletStatus::ALIVE;

        if (manimInputFilenameDialog.mCancel)
            return AppletStatus::RETURN;

        if (mFilename.length() > 0)
        {
            mEnteredFilename = true;
            return AppletStatus::ALIVE;
        }
        else
            startFromScratch();
    }
    else if (!mSavedFile)
    {
        mSavedFile = Configurator.saveCurrentConfig(mFilename.c_str());
        if (!mSavedFile)
        {
            mTextSpriteStatic.setText({"ERROR:", "Could Not Save"});
            mTextSpriteStatic.setLife(80);
            mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);
            return AppletStatus::RETURN;
        }
    }
    else if (!mShowConfirmation)
    {
        mShowConfirmation = true;
        mTextSpriteStatic.setText({"Config Saved as", mFilename.c_str()});
        mTextSpriteStatic.setLife(100);
        mpCompositor->registerAnimation(&mTextSpriteStatic, CanvasType::TOP);
        return AppletStatus::RETURN;
    }
    return AppletStatus::ALIVE;
}

void SaveConfigurationApp_::cleanupApp()
{
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}
