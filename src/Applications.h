#ifndef APPLICATIONS_H
#define APPLICATIONS_H
#include <Arduino.h>
#include <vector>
#include <array>
#include "Compositor.h"
#include "Dialog.h"
#include "MyJoystick.h"
#include "Applet.h"
#include "Animation.h"
#include "Configuration.h"

class DrawKeyPressesApp_ : public Applet_, public Animation_, public IConfigurable_
{
public:
    DrawKeyPressesApp_(Compositor_ *comp)
        : Applet_(comp), Animation_(0, 0, 128, 128, 5, -1, -1, 0, 0)
    {
    }
    void initApp();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_
    void updateAnim();
    void drawAnim(JoyDisplay_ *pcanvas);
    void configure(const Configuration &config);

    uint8_t maMacroMap[12] = {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    RemapButtonsApp_     ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemapButtonsApp_ : public Applet_, public Animation_
{
public:
    RemapButtonsApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        // clang-format off
        : Applet_(comp), Animation_(0, 44, 128, 40, 6, -1, -1, 0, 0)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
        , mAnimInputDialog(comp, &mNewButtonValue, 1, 32, "New Value", 0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 6)
        , mTextSpriteBanner(0, 128 - TEXT_HEIGHT - 1, 128, TEXT_HEIGHT, 1)
        , mpappDrawKeysApp(drawkeyapp)
    // clang-format on
    {
        mTextSpriteStatic.setText({"Hold Button", "<- to exit"});
        mTextSpriteBanner.setDrawBox(true);
        mTextSpriteBanner.setText("Map Buttons");
    }
    void startFromScratch();
    // for applet
    void initApp();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_
    void updateAnim();
    void drawAnim(JoyDisplay_ *pcanvas);

    Applet_ *mpappDrawKeysApp;
    AnimInputDialogInt_<uint8_t> mAnimInputDialog;
    AnimTextPrompt_ mTextSpriteStatic;
    AnimTextStatic1Line_ mTextSpriteBanner;
    uint32_t mLocalButtonStateMap;
    uint32_t mLastLocalButtonStateMap;

    uint32_t mAppLastTime;
    uint16_t mAppDelay = 300;
    uint8_t mEditButton = -1;
    uint8_t mNewButtonValue;
    bool mAppDrawPrompt = true;
    bool mFoundTheButton = false;
    bool mSetTheValue = false;
    bool mConfirmedTheButton = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     AssignTurboApp      ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AssignTurboApp_ : public Applet_
{
    // clang-format off
public:
    AssignTurboApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        : Applet_(comp)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
        , mTextSpriteBanner(0, 128 - TEXT_HEIGHT - 1, 128, TEXT_HEIGHT, 1)
        , mAnimInputDialogDelay(comp, &mTurboDelayValue, 10, 4000, "New Value", 0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 6)
        , mAnimInputDialogOptions(comp, &mSelection, "Title")
        , mpappDrawKeysApp(drawkeyapp)
    // clang-format on
    {
        mTextSpriteStatic.setText({"Hold Button", "<- to exit"});
        mTextSpriteBanner.setText("Edit Turbo");
        mTextSpriteBanner.setDrawBox(true);
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    bool exitApp();
    void cleanupApp();
    // for Animation_

    Applet_ *const mpappDrawKeysApp;
    AnimInputDialogInt_<uint16_t> mAnimInputDialogDelay;
    AnimInputDialogList_ mAnimInputDialogOptions;
    AnimTextPrompt_ mTextSpriteStatic;
    AnimTextStatic1Line_ mTextSpriteBanner;

    uint16_t mTurboDelayValue;
    uint32_t mAppLastTime;
    uint16_t mAppDelay = 300;
    bool mFoundTheButton = false;
    bool mSetButtonMode = false;
    bool mSetButtonDelay = false;
    bool mSetButtonLatching = false;
    uint8_t mEditButton;
    uint8_t mSelection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////     AssignMacroApp      ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AssignMacroApp_ : public Applet_
{
    // clang-format off
public:
    AssignMacroApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        : Applet_(comp)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
        , mTextSpriteBanner(0, 128 - TEXT_HEIGHT - 1, 128, TEXT_HEIGHT, 1)
        , mAnimInputDialogOptions(comp, &mSelection, "Title")
        , mpappDrawKeysApp(drawkeyapp)
    // clang-format on
    {
        mTextSpriteStatic.setText({"Hold Button", "<- to exit"});
        mTextSpriteBanner.setText("Assign Macro");
        mTextSpriteBanner.setDrawBox(true);
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    bool exitApp();
    void cleanupApp();
    // for Animation_

    Applet_ *const mpappDrawKeysApp;
    AnimInputDialogList_ mAnimInputDialogOptions;
    AnimTextPrompt_ mTextSpriteStatic;
    AnimTextStatic1Line_ mTextSpriteBanner;

    uint32_t mAppLastTime;
    uint16_t mAppDelay = 300;
    bool mFoundTheButton = false;
    bool mSetButtonMode = false;
    uint8_t mEditButton;
    uint8_t mSelection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    Save Configuration     /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SaveConfigurationApp_ : public Applet_
{
public:
    SaveConfigurationApp_(Compositor_ *comp)
        : Applet_(comp)
        , manimInputFilenameDialog(comp, &mFilename)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
    {
        mTextSpriteStatic.setText({"Line 1", "Line 2"});
        mTextSpriteStatic.mDrawBox = true;
    }
    void initApp();
    
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    void startFromScratch();

    String mFilename = "";
    AnimInputDialogString_ manimInputFilenameDialog;
    AnimTextPrompt_ mTextSpriteStatic;
    bool mEnteredFilename = false;
    bool mSavedFile = false;
    bool mShowConfirmation = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        LoadConfigApp_        //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LoadConfigApp_ : public Applet_
{
    // clang-format off
public:
    LoadConfigApp_(Compositor_ *comp)
        : Applet_(comp)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 99)
        , mAnimInputDialogFileList(comp, &mSelection)
    // clang-format on
    {
        mAnimInputDialogFileList.setTitle("Choose:");
        mTextSpriteStatic.setText({"Configuration", "Successful"});
        mTextSpriteStatic.mDrawBox = true;
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_

    AnimInputDialogList_ mAnimInputDialogFileList;
    AnimTextPrompt_ mTextSpriteStatic;
    std::vector<String> mvFileList;
    uint32_t mAppLastTime;
    uint16_t const mAppDelay = 300;
    uint8_t mSelection;
    bool mNoFiles = false;
    bool mStartDialog = false;
    bool mSelectedFile = false;
    bool mLoadedConfig = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////        LoadMacroApp_        //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LoadMacroApp_ : public Applet_
{
    // clang-format off
public:
    LoadMacroApp_(Compositor_ *comp)
        : Applet_(comp)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 99)
        , mAnimInputDialogFileList(comp, &mSelection)
    // clang-format on
    {
        mAnimInputDialogFileList.setTitle("Choose:");
        mTextSpriteStatic.setText({"Configuration", "Successful"});
        mTextSpriteStatic.mDrawBox = true;
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_

    AnimInputDialogList_ mAnimInputDialogFileList;
    AnimTextPrompt_ mTextSpriteStatic;
    std::vector<String> mvFileList;
    Macro mMacro;
    uint32_t mAppLastTime;
    uint16_t const mAppDelay = 300;
    uint8_t mSelection;
    bool mNoFiles = false;
    bool mStartDialog = false;
    bool mSelectedFile = false;
    bool mLoadedMacro = false;
    bool mSelectedSlot = false;
    bool mSentToJoystick = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////          FormatFSApp_         /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FormatFSApp_ : public Applet_
{
    // clang-format off
public:
    FormatFSApp_(Compositor_ *comp)
        : Applet_(comp)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 99)
        , mAnimInputDialogConfirm(comp, &mSelection)
    // clang-format on
    {
        mAnimInputDialogConfirm.setTitle("Format?");
        mTextSpriteStatic.setText({"Configuration", "Successful"});
        mTextSpriteStatic.mDrawBox = true;
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_

    AnimInputDialogList_ mAnimInputDialogConfirm;
    AnimTextPrompt_ mTextSpriteStatic;
    uint32_t mAppLastTime;
    uint16_t const mAppDelay = 300;
    uint8_t mSelection;
    bool mStartDialog = false;
    bool mConfirming1 = false;
    bool mConfirming2 = false;
    bool mDidIt = false;
};




#endif