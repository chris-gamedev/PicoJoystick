#ifndef APPLICATIONS_H
#define APPLICATIONS_H
#include <Arduino.h>
#include <stdlib.h>
#include <array>
#include "Compositor.h"
#include "MyJoystick.h"
#include "Applet.h"
#include "Animation.h"

class DrawKeyPressesApp_ : public Applet_, public Animation_
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
    void update();
    void draw(JoyDisplay_ *pcanvas);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////    RemapButtonsApp_     ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class RemapButtonsApp_ : public Applet_, public Animation_
{
public:
    RemapButtonsApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        : Applet_(comp), Animation_(0, 44, 128, 40, 6, -1, -1, 0, 0)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
        , mAnimInputDialog("New Value", &mNewButtonValue, 1, 32, 0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 6)
        , mTextSpriteBanner(0, 128 - TEXT_HEIGHT - 1, 128, TEXT_HEIGHT, 1)
        , mpappDrawKeysApp(drawkeyapp)
    {
        std::vector<String> prompts = {"Hold Button", "<- to exit"};
        mTextSpriteStatic.setText(prompts);
        mTextSpriteBanner.setDrawBox(true);
        mTextSpriteBanner.setText("Map Buttons");
    }
    void startFromScratch();
    // for applet
    void initApp();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();
    // for Animation_
    void update();
    void draw(JoyDisplay_ *pcanvas);

    Applet_ *mpappDrawKeysApp;
    AnimInputDialogInt_<uint8_t> mAnimInputDialog;
    AnimTextPrompt_ mTextSpriteStatic;
    AnimTextStatic1Line_ mTextSpriteBanner;
    bool mExitHotkey = false;
    bool mSettingEditButton = false;
    uint32_t mLocalButtonStateMap;
    uint32_t mLastLocalButtonStateMap;

    uint32_t mAppLastTime;
    uint16_t mAppDelay = 1500;
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


class AssignTurboApp_ : public Applet_, public Animation_
{

public:
    AssignTurboApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        : Applet_(comp)
        , Animation_(0, 44, 128, 40, 6, -1, -1, 0, 0)
        , mTextSpriteStatic(0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 5)
        , mTextSpriteBanner(0, 128 - TEXT_HEIGHT - 1, 128, TEXT_HEIGHT, 1)
        , mAnimInputDialogDelay ("New Value", &mTurboDelayValue, 1, 32, 0, 64 - (2 * TEXT_HEIGHT), 128, TEXT_HEIGHT * 4, 6)
        , mAnimInputDialogOptions("Title", &mSelection)
        , mpappDrawKeysApp(drawkeyapp)

    {
        std::vector<String> prompts = {"Hold Button", "<- to exit"};
        mTextSpriteStatic.setText(prompts);
        mAnimInputDialogOptions.setDrawBox(true);
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
    void update();
    void draw(JoyDisplay_ *pcanvas);

    Applet_ *mpappDrawKeysApp;
    AnimInputDialogInt_<uint16_t> mAnimInputDialogDelay;
    AnimInputDialogList_ mAnimInputDialogOptions;
    AnimTextPrompt_ mTextSpriteStatic;
    AnimTextStatic1Line_ mTextSpriteBanner;
    
    uint16_t mTurboDelayValue;
    uint32_t mAppLastTime;
    uint16_t mAppDelay;
    bool mFoundTheButton = false;
    bool mSetButtonMode = false;
    bool mSetButtonDelay = false;
    bool mSetButtonLatching = false;
    uint8_t mEditButton;
    uint8_t mSelection;

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////      Create Macro       ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CreateMacroApp_ : public Applet_, public Animation_ {
    CreateMacroApp_(Compositor_ *comp, Applet_ *drawkeyapp)
        : Applet_(comp)
        , Animation_(0, 44, 128, 40, 6, -1, -1, 0, 0)
        , mpappDrawKeysApp(drawkeyapp)

    {
        
    }

    // for applet
    void initApp() {}
    void startFromScratch() {}
    AppletStatus::TAppletStatus updateApp() { return AppletStatus::ALIVE;}
    bool exitApp() { return false;}
    void cleanupApp() {}
    // for Animation {}_
    void update() {}
    void draw(JoyDisplay_ *pcanvas) {}

    Applet_ *mpappDrawKeysApp;
    
    uint32_t mAppLastTime;
    uint16_t mAppDelay;
};

#endif