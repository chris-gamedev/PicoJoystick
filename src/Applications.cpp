#include "Applications.h"

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
void DrawKeyPressesApp_::update()
{
}

void DrawKeyPressesApp_::draw(JoyDisplay_ *pcanvas)
{
    // uint16_t packedKeyPresses, uint8_t buttonValues[], uint8_t joyState
    pcanvas->setTextSize(1);
    pcanvas->setFont();
    uint32_t packedKeyPresses = MyJoystickBT.getPackedKeyPresses();
    uint8_t *buttonValues = MyJoystickBT.getButtonValues();
    uint8_t joyState = MyJoystickBT.getJoystickState();

    // draw background
    pcanvas->drawBitmap(0, 0, bitmap_SmallButtonLayout, 128, 128, 0xf);
    pcanvas->setFont();
    pcanvas->setTextColor(0xF);
    uint8_t pressColor = 0x0;
    for (int i = 0; i < map_bitmap_SmallButtonLayout_count - 1; i++)
    {
        // draw disabled buttons:
        if (!(Command_::sEnabledButtonsMask & MAKE_BUTTON_BITMASK_16(i)))
            pcanvas->fillCircle(map_bitmap_SmallButtonLayout[i][0], map_bitmap_SmallButtonLayout[i][1], 8, 0x8);
        if ((1 << (i)) & packedKeyPresses)
        {
            // Add color here: Black when pressed, gray when pressed but disabled by a macro.
            // probably implement by checking the enabled button mask bitfield?
            pcanvas->fillCircle(map_bitmap_SmallButtonLayout[i][0], map_bitmap_SmallButtonLayout[i][1], 8, 0x0);
            pcanvas->setCursor(map_bitmap_SmallButtonLayout[i][0] - 3 - (buttonValues[i] > 9) * 3, map_bitmap_SmallButtonLayout[i][1] - 4);
            pcanvas->print(buttonValues[i]);
        }
    }

    int8_t orgX = map_bitmap_SmallButtonLayout[12][0];
    int8_t orgY = map_bitmap_SmallButtonLayout[12][1];

    // draw arrow bitmaps. 21x21
    // Draw N / S on Joystick
    switch (joyState)
    {
    case 8:
    case 1:
    case 2:
        // NORTH
        pcanvas->drawBitmap(orgX - 10, orgY - 21, bitmap_arrowUp, 21, 21, 0xF);
        break;
    case 4:
    case 5:
    case 6:
        // SOUTH
        pcanvas->drawBitmap(orgX - 10, orgY, bitmap_arrowDown, 21, 21, 0xF);
        break;
    }
    // draw W / E on Joystick
    switch (joyState)
    {
    case 2:
    case 3:
    case 4:
        // EAST
        pcanvas->drawBitmap(orgX, orgY - 10, bitmap_arrowRight, 21, 21, 0xF);
        break;
    case 6:
    case 7:
    case 8:
        // WEST
        pcanvas->drawBitmap(orgX - 21, orgY - 10, bitmap_arrowLeft, 21, 21, 0xF);
        break;
    }

    pcanvas->drawPixel(map_bitmap_SmallButtonLayout[12][0], map_bitmap_SmallButtonLayout[12][1], 0xA);
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
        mpCompositor->registerAnimation(&mAnimInputDialog, CanvasType::TOP);
        mAppDrawPrompt = false;
        return AppletStatus::ALIVE;
    }
    else if (mFoundTheButton && !mSetTheValue)
    {

        if (mAnimInputDialog.finished())
        {
            mSetTheValue = true;
            MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
            startFromScratch();
        }
    }

    return AppletStatus::ALIVE;
}

void RemapButtonsApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mlife = 0;
}

void RemapButtonsApp_::update()
{
}

void RemapButtonsApp_::draw(JoyDisplay_ *pcanvas)
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
    startFromScratch();
}

void AssignTurboApp_::startFromScratch()
{
    mFoundTheButton = false;
    mSetTheValue = false;
    mEditButton = -1;
}

bool AssignTurboApp_::exitApp()
{

    if (MyJoystickBT.joyJustPressed(JOY_LEFT))
    {
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
        return AppletStatus::RETURN;

    if (!mFoundTheButton)
    {

        mEditButton = Applet_::findHeldButton(MyJoystickBT.getPackedKeyPresses(), 1500);
        if (mEditButton == 255)
            return AppletStatus::ALIVE;

        mFoundTheButton = true;
        // mNewButtonValue = MyJoystickBT.getButtonValue(mEditButton);
        Serial.println("Found a held button!!  " + String(mEditButton));

        // mAnimInputDialog.start("New Value", &mNewButtonValue, 1, 32);
        mpCompositor->registerAnimation(&mAnimInputDialog, CanvasType::TOP);
        return AppletStatus::ALIVE;
    }
    else if (mFoundTheButton && !mSetTheValue)
    {

    //     if (mAnimInputDialog.finished())
    //     {
    //         mSetTheValue = true;
    //         MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
    //         startFromScratch();
    //     }
    }

    return AppletStatus::TAppletStatus::ALIVE;
}

void AssignTurboApp_::cleanupApp()
{
    mpappDrawKeysApp->cleanupApp();
    mlife = 0;
}

void AssignTurboApp_::update()
{
}

void AssignTurboApp_::draw(JoyDisplay_ *pcanvas)
{
}
