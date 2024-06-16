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
    startFromScratch();
}

void RemapButtonsApp_::startFromScratch()
{
    mFoundTheButton = false;
    mSetTheValue = false;
    mConfirmedTheButton = false;
    mEditButton = -1;
    mNewButtonValue = 0;
    mPromptString1 = "Hold a button.";
    mPromptString2 = "Hold <- to exit";
    mAppDrawPrompt = true;
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
            return AppletStatus::ENDED;

    if (!mFoundTheButton)
    {
        mLocalButtonStateMap = MyJoystickBT.getPackedKeyPresses();
        mAppDrawPrompt = !mLocalButtonStateMap;

        mEditButton = Applet_::findHeldButton(mLocalButtonStateMap, 1500);
        if (mEditButton == 255)
            return AppletStatus::ALIVE;

        mFoundTheButton = true;
        mNewButtonValue = MyJoystickBT.getButtonValue(mEditButton);
        Serial.println("Found a held button!!  " + String(mEditButton));
        mAnimInputDialog.mlife = -1;
        mAnimInputDialog.setPrompt("New Value");
        mAnimInputDialog.setReturnPointer(&mNewButtonValue);
        mAnimInputDialog.setRange(1, 32);
        mpCompositor->registerAnimation(&mAnimInputDialog, CanvasType::TOP);
        return AppletStatus::ALIVE;
    }
    else if (mFoundTheButton && !mSetTheValue)
    {
        mAppDrawPrompt = false;
        
        if (mAnimInputDialog.finished()) {
            mSetTheValue = true;
            MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
            startFromScratch();
        }

        return AppletStatus::ALIVE;
        
        // mPromptString1 = "    New Value:   ";
        // mPromptString2 = "         \x1E";
        // mPromptString2 += (mNewButtonValue < 10) ? "0" : "";
        // mPromptString2 += String(mNewButtonValue) + "\x1F      ";

        // if (MyJoystickBT.buttonJustPressed(0)) // cancel
        // {
        //     startFromScratch();
        //     return AppletStatus::ALIVE;
        // }
        // if (MyJoystickBT.buttonJustPressed(4)) // accept
        // {
        //     mSetTheValue = true;
        //     MyJoystickBT.setButtonValue(mEditButton, mNewButtonValue);
        //     return AppletStatus::ALIVE;
        // }
        // if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
        // {
        //     mNewButtonValue = ((mNewButtonValue + 32- 2) % 32) + 1;
        //     return AppletStatus::ALIVE;
        // }
        // if (MyJoystickBT.joyJustPressed(JOY_UP)) //increment
        // {
        //     mNewButtonValue = (mNewButtonValue % 32) + 1;
        //     return AppletStatus::ALIVE;
        // }
    }
    else if (mSetTheValue && !mConfirmedTheButton) 
    {
        // mPromptString1 = "   Button " + String(mEditButton) + ":";
        // mPromptString2 = "         ";
        // mPromptString2 += (mNewButtonValue < 10) ? "0" : "";
        // mPromptString2 += String(mNewButtonValue);
        // if (MyJoystickBT.buttonJustPressed(4)) {
            mConfirmedTheButton = true;
            startFromScratch();
        // }
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

    if (mAppDrawPrompt)
    {
        pcanvas->setFont(MENU_FONT_FACE);
        // pcanvas->setTextSize(2);
        pcanvas->setTextColor(0xF);
        pcanvas->fillRect(0, 44, 128, 40, 0x6);

        pcanvas->setCursor(0, 59);
        pcanvas->print(mPromptString1);

        pcanvas->setCursor(0, 72);
        pcanvas->print(mPromptString2);
        
    }
}
