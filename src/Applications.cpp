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

void DrawKeyPressesApp_::configure(const Configuration &config)
{
    for (int i = 0; i < 12; i++)
        maMacroMap[i] = config.drawKeypresses_macroMap[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    RemapButtonsApp_     ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RemapButtonsApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    mAppLastTime = millis();

    mPopUpMessage.mlife = APPLET_POPUP_DURATION;
    mPopUpMessage.setDrawBox(true);
    mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::TOP);

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
            mPopUpMessage.mlife = 0;
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
    mTextSpriteBanner.mlife = 0;
    mAnimInputDialog.endDialog();
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     Assign Turbo        ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AssignTurboApp_::initApp()
{
    mpappDrawKeysApp->initApp();

    mAppLastTime = millis();

    mPopUpMessage.mlife = APPLET_POPUP_DURATION;
    mPopUpMessage.setDrawBox(true);
    mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::TOP);

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
    mAnimInputDialogOptions.endDialog();
    mAnimInputDialogDelay.endDialog();
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

    mPopUpMessage.mlife = APPLET_POPUP_DURATION;
    mPopUpMessage.mDrawBox = true;
    mPopUpMessage.setText({"Hold Button", "<- to exit"});
    mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::TOP);

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
                        MyJoystickBT.getMacroName(0)
                      , MyJoystickBT.getMacroName(1)
                      , MyJoystickBT.getMacroName(2)
                      , MyJoystickBT.getMacroName(3)
                      , MyJoystickBT.getMacroName(4)
                      , MyJoystickBT.getMacroName(5)
                      , MyJoystickBT.getMacroName(6)
                      , MyJoystickBT.getMacroName(7)
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

        if (MyJoystickBT.macroIsEmpty(mSelection))
        {
            mPopUpMessage.setText({"Empty Macro", "Try Again"});
            mPopUpMessage.mlife = APPLET_POPUP_DURATION;
            mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
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
    mAnimInputDialogOptions.endDialog();
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
    mPopUpMessage.mlife = APPLET_POPUP_DURATION;
    mPopUpMessage.setDrawBox(true);
    mPopUpMessage.setText({"Enter a", "Filename"});
    mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);

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
            mPopUpMessage.setText({"ERROR:", "Could Not Save"});
            mPopUpMessage.mlife = APPLET_POPUP_DURATION;
            mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::TOP);
            return AppletStatus::RETURN;
        }
    }
    else if (!mShowConfirmation)
    {
        mShowConfirmation = true;
        mPopUpMessage.setText({"Config Saved as", mFilename.c_str()});
        mPopUpMessage.mlife = APPLET_POPUP_DURATION;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::TOP);
        return AppletStatus::RETURN;
    }
    return AppletStatus::ALIVE;
}

void SaveConfigurationApp_::cleanupApp()
{
    manimInputFilenameDialog.endDialog();
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        LoadConfigApp_        //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadConfigApp_::initApp()
{
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);
    mNoFiles = false;

    mvFileList = Configurator.getConfigFileList();
    if (mvFileList.size() == 0)
    {
        mNoFiles = true;
        mPopUpMessage.setText({"No Saved", "Configs."});
        mPopUpMessage.mlife = 50;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
    }
    startFromScratch();
}

void LoadConfigApp_::startFromScratch()
{
    mStartDialog = false;
    mSelectedFile = false;
    mLoadedConfig = false;
}

AppletStatus::TAppletStatus LoadConfigApp_::updateApp()
{

    if (mNoFiles)
        return AppletStatus::RETURN;

    if (!mStartDialog)
    {
        mAnimInputDialogFileList.start("Choose:", &mSelection, {""});
        mAnimInputDialogFileList.manimPromptList.setText(mvFileList, 0);
        mStartDialog = true;
        return AppletStatus::ALIVE;
    }
    else if (!mSelectedFile)
    {
        if (mAnimInputDialogFileList.updateDialog())
            return AppletStatus::ALIVE;

        mSelectedFile = true;
        return AppletStatus::ALIVE;
    }
    else if (!mLoadedConfig)
    {
        if (mAnimInputDialogFileList.mCancel)
            return AppletStatus::RETURN;

        Configuration config;
        char fullFilename[strlen(CONFIG_FILE_PATH) + strlen(mvFileList[mSelection]) + 1];
        strcpy(fullFilename, CONFIG_FILE_PATH);
        strcat(fullFilename, mvFileList[mSelection]);
        if (Configurator.importConfigFile(fullFilename, &config) < 0)
        { // error somewhere
            mPopUpMessage.setText({"Error", "In File"});
            mPopUpMessage.mlife = 50;
            mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
            return AppletStatus::RETURN;
        }
#ifdef DEADBEEF
        Serial.printf("config.button after import ");
        for (int i = 0; i < 12; i++)
            Serial.printf("%d, ", config.joystick_buttonValueMap[i]);
        Serial.println();
#endif
        Configurator.mConfig = config;
#ifdef DEADBEEF
        Serial.printf("configurator.mconfig.button after import ");
        for (int i = 0; i < 12; i++)
            Serial.printf("%d, ", Configurator.mConfig.joystick_buttonValueMap[i]);
        Serial.println();
#endif

        Configurator.configurate();
        mPopUpMessage.setText({"Config", "Updated"});
        mPopUpMessage.mlife = 50;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
        return AppletStatus::RETURN;
    }

    return AppletStatus::ALIVE;
}

void LoadConfigApp_::cleanupApp()
{
    mAnimInputDialogFileList.endDialog();
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        LoadMacroApp_         //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadMacroApp_::initApp()
{
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);
    mNoFiles = false;

    mvFileList = Configurator.getMacroFileList();
    if (mvFileList.size() == 0)
    {
        mNoFiles = true;
        mPopUpMessage.setText({"No Saved", "Macros"});
        mPopUpMessage.mlife = 50;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
    }
    startFromScratch();
}

void LoadMacroApp_::startFromScratch()
{
    mStartDialog = false;
    mSelectedFile = false;
    mLoadedMacro = false;
    mSelectedSlot = false;
    mSentToJoystick = false;
}

AppletStatus::TAppletStatus LoadMacroApp_::updateApp()
{

    if (mNoFiles)
        return AppletStatus::RETURN;

    if (!mStartDialog)
    {
        mAnimInputDialogFileList.start("Choose:", &mSelection, {""});
        mAnimInputDialogFileList.manimPromptList.setText(mvFileList, 0);
        mStartDialog = true;
        return AppletStatus::ALIVE;
    }
    else if (!mSelectedFile)
    {
        if (mAnimInputDialogFileList.updateDialog())
            return AppletStatus::ALIVE;

        mSelectedFile = true;
        return AppletStatus::ALIVE;
    }
    else if (!mLoadedMacro)
    {
        if (mAnimInputDialogFileList.mCancel)
            return AppletStatus::RETURN;

        if (!Configurator.importMacroFile(mvFileList[mSelection], mMacro))
        { // error somewhere
            mPopUpMessage.setText({"Error", "In File"});
            mPopUpMessage.mlife = 50;
            mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
            return AppletStatus::RETURN;
        }
        mLoadedMacro = true;
        // clang-format off
        mAnimInputDialogFileList.start("Slot:", &mSelection, {
                MyJoystickBT.getMacroName(0)
                , MyJoystickBT.getMacroName(1)
                , MyJoystickBT.getMacroName(2)
                , MyJoystickBT.getMacroName(3)
                , MyJoystickBT.getMacroName(4)
                , MyJoystickBT.getMacroName(5)
                , MyJoystickBT.getMacroName(6)
                , MyJoystickBT.getMacroName(7)
                });
        // clang-format on
        return AppletStatus::ALIVE;
    }
    else if (!mSelectedSlot)
    {
        if (mAnimInputDialogFileList.updateDialog())
            return AppletStatus::ALIVE;

        if (mAnimInputDialogFileList.mCancel)
            return AppletStatus::RETURN;

        Serial.printf("APP:  Macro phrase contents:\n");
              Serial.printf("name = %s, enabledBut = %d, enabJoy = %d\n", mMacro.name, mMacro.enabledButtonsMap, mMacro.enabledJoystickState);

        for (auto it : mMacro.phrase) {
            Serial.printf("\tb=%d, j=%d, dur=%d\n", it.mButtonStateMap, it.mJoyState, it.mDuration);
        }
        Serial.printf("\n\n");
        MyJoystickBT.importMacro(mMacro, mSelection);
        mPopUpMessage.setText({"Macro", "Imported"});
        mPopUpMessage.mlife = 50;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
        startFromScratch();
        return AppletStatus::ALIVE;
    }

    return AppletStatus::ALIVE;
}

void LoadMacroApp_::cleanupApp()
{
    mAnimInputDialogFileList.endDialog();
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        FormatFSApp_        ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FormatFSApp_::initApp()
{
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);

    startFromScratch();
}

void FormatFSApp_::startFromScratch()
{
    mStartDialog = false;
    mConfirming1 = false;
    mConfirming2 = false;
    mDidIt = false;
    mSelection = 0;
}

AppletStatus::TAppletStatus FormatFSApp_::updateApp()
{

    if (!mStartDialog)
    {
        mAnimInputDialogConfirm.start("Format?", &mSelection, {"NO!!", "Yes"});
        mStartDialog = true;
        return AppletStatus::ALIVE;
    }
    else if (!mConfirming1)
    {
        if (mAnimInputDialogConfirm.updateDialog())
            return AppletStatus::ALIVE;

        if (mSelection == 0 || mAnimInputDialogConfirm.mCancel)
            return AppletStatus::RETURN;

        mSelection = 0;
        mConfirming1 = true;
        mAnimInputDialogConfirm.start("R U Sure?", &mSelection, {"NOO! OOPS!", "Yes"});
        return AppletStatus::ALIVE;
    }
    else if (!mConfirming2)
    {
        if (mAnimInputDialogConfirm.updateDialog())
            return AppletStatus::ALIVE;

        if (mSelection == 0 || mAnimInputDialogConfirm.mCancel)
            return AppletStatus::RETURN;

        mConfirming2 = true;
        return AppletStatus::ALIVE;
    }
    else if (!mDidIt)
    {
        bool success = Configurator.formatFS();
        if (success)
            mPopUpMessage.setText({"Format", "Complete"});
        else
            mPopUpMessage.setText({"Format", "Failed"});
        mPopUpMessage.mlife = 50;
        mpCompositor->registerAnimation(&mPopUpMessage, CanvasType::FG);
        return AppletStatus::RETURN;
    }

    return AppletStatus::ALIVE;
}

void FormatFSApp_::cleanupApp()
{
    mAnimInputDialogConfirm.endDialog();
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}
