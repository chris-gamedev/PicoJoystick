#include "MenuManager.h"

////////////////////////////////////////////////////    CALLBACKS    ///////////////////////////////////////////////////////
// placeholder callback pointers
void myCallback() { Serial.println("inside myCallback"); }
void drawKeyPresses_callback() { AppSwitcher.switchApp(AppletNames::SHOW_BUTTON_PRESSES); }
void remapButtons_callback() { AppSwitcher.switchApp(AppletNames::REMAP_BUTTONS); }
////////////////////////////////////////////////////    CALLBACKS    ///////////////////////////////////////////////////////

uint32_t Menu_::mLastTime = 0;
AnimTextPrompt_ *Menu_::mUpperText = nullptr;
AnimTextPrompt_ *Menu_::mLowerText = nullptr;

MenuManager_::MenuManager_(Compositor_ *comp)
    : mMenuRoot(SubMenu_("Home", nullptr)), Applet_(comp)
    , Animation_(0, 64 - (TEXT_HEIGHT * 2), 128, TEXT_HEIGHT * 4, 2)
    , mUpperTextSpriteStatic(0, 64 - TEXT_HEIGHT, 128, TEXT_HEIGHT * 3, 5)
    , mLowerTextSpriteStatic(0, 0, 128, 128, 1)
    , mpCompositor(comp)

{
    this->buildMenu();
    Menu_::mLastTime = 0;
    Menu_::mUpperText = &mUpperTextSpriteStatic;
    Menu_::mLowerText = &mLowerTextSpriteStatic;
}

void MenuManager_::initApp()
{
    mlife = -1;
    mUpperTextSpriteStatic.mlife = -1;
    mLowerTextSpriteStatic.mlife = -1;
    mpCompositor->registerAnimation(&mUpperTextSpriteStatic, CanvasType::TOP);
    mpCompositor->registerAnimation(this, CanvasType::BG);
}

void MenuManager_::cleanupApp()
{
    mUpperTextSpriteStatic.mlife = 0;
    mLowerTextSpriteStatic.mlife = 0;
    this->mlife = 0;
}

AppletStatus::TAppletStatus MenuManager_::updateApp()
{
    if (currentMenuNode == nullptr)
        currentMenuNode = &mMenuRoot;
    currentMenuNode = currentMenuNode->action();
    if (currentMenuNode == nullptr)
    {
        return AppletStatus::ENDED;
    }
    return AppletStatus::ALIVE;
}

void MenuManager_::draw(JoyDisplay_ *pcanvas)
{
    // pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
    pcanvas->drawRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_FG);
    pcanvas->drawRoundRect(mX +1, mY + 1, mW - 2, mH - 2, 10, MENU_BOX_COLOR_FG);
}

Menu_ *SubMenu_::action()
{
    mUpperText->setText(mChildPrompts, selection);

    if (MyJoystickBT.buttonJustPressed(0))
    {
        mLastTime = 0;
        return mpParent; // back to previous menu
    }

    // accept selection:
    if (MyJoystickBT.buttonJustPressed(4))
    {
        mLastTime = 0;
        // return mChildren[selection]->action(); // into child menu
        return mChildren[selection]; // into child menu
    }

    // move prompts
    if (MyJoystickBT.joyJustPressed(JOY_UP))
    {
        selection = (selection + mChildren.size() - 1) % mChildren.size();
        mLastTime = 0;
    }
    else if (MyJoystickBT.joyJustPressed(JOY_DOWN))
    {
        selection = (selection + 1) % mChildren.size();
        mLastTime = 0;
    }

    return this;
}

void MenuManager_::buildMenu()
{
    // clang-format off
    Menu_ *pRoot = &mMenuRoot;
    pRoot = &mMenuRoot;
    pRoot->addChild(new SubMenu_("Buttons", pRoot));
        Menu_* pButtons = pRoot->mChildren[0];
        pButtons->addChild(new SubMenu_("Mapping", pButtons));
            Menu_* pMapping = pButtons->mChildren[0];
            pMapping->addChild(new Leaf_("Remap", remapButtons_callback, pMapping));
            pMapping->addChild(new Leaf_("Swap", myCallback, pMapping));
            pMapping->addChild(new Leaf_("Show", myCallback, pMapping));
            pMapping->addChild(new Leaf_("Set Pause", myCallback, pMapping));
        pButtons->addChild(new SubMenu_("Auto-Fire", pButtons));
            Menu_* pAutoFire = pButtons->mChildren[1];
            pAutoFire->addChild(new Leaf_("Manual", myCallback, pAutoFire));
            pAutoFire->addChild(new Leaf_("Wizard", myCallback, pAutoFire));
        pButtons->addChild(new SubMenu_("Macros", mMenuRoot.mChildren[0]));
            Menu_* pMacros = pButtons->mChildren[2];
            pMacros->addChild(new Leaf_("Assign", myCallback, pMacros));
            pMacros->addChild(new Leaf_("Edit", myCallback, pMacros));
            pMacros->addChild(new Leaf_("Wizard", myCallback, pMacros));
            pMacros->addChild(new Leaf_("Manual", myCallback, pMacros));
    pRoot->addChild(new SubMenu_("Joystick", &mMenuRoot));
        Menu_* pJoystick = pRoot->mChildren[1];
        pJoystick->addChild(new Leaf_("Orient", myCallback, pJoystick));
        pJoystick->addChild(new Leaf_("Invert X", myCallback, pJoystick));
        pJoystick->addChild(new Leaf_("Invert Y", myCallback, pJoystick));
    pRoot->addChild(new SubMenu_("Display", &mMenuRoot));
        Menu_* pDisplay = pRoot->mChildren[2];
        pDisplay->addChild(new Leaf_("On - Off", myCallback, pDisplay));
        pDisplay->addChild(new SubMenu_("Theme", pDisplay));
            Menu_* pTheme = pDisplay->mChildren[1];
            pTheme->addChild(new Leaf_("Joypad", drawKeyPresses_callback, pTheme));
            pTheme->addChild(new Leaf_("Theme", myCallback, pTheme));
        pDisplay->addChild(new SubMenu_("Brightness", pDisplay));
            Menu_* pBrightness = pDisplay->mChildren[2];
            pBrightness->addChild(new Leaf_("??", myCallback, pBrightness));
        pDisplay->addChild(new Leaf_("Battery", myCallback, pDisplay));
    pRoot->addChild(new SubMenu_("System", &mMenuRoot));
        Menu_* pSystem = pRoot->mChildren[3];
        pSystem->addChild(new SubMenu_("Bluetooth", pSystem));
            Menu_* pBluetooth = pSystem->mChildren[0];
            pBluetooth->addChild(new Leaf_("On / Off", myCallback,pBluetooth));
            pBluetooth->addChild(new Leaf_("Pair", myCallback,pBluetooth));
            pBluetooth->addChild(new Leaf_("Unpair", myCallback,pBluetooth));
            pBluetooth->addChild(new Leaf_("Name", myCallback,pBluetooth));
        pSystem->addChild(new SubMenu_("USB", pSystem));
            Menu_* mUSBMode = pSystem->mChildren[1];
            mUSBMode->addChild(new Leaf_("Use USB", myCallback, mUSBMode));
            mUSBMode->addChild(new Leaf_("Power Only", myCallback, mUSBMode));
            mUSBMode->addChild(new Leaf_("Charging", myCallback, mUSBMode));
        pSystem->addChild(new SubMenu_("Config", pSystem));
            Menu_* pSaveConfig = pSystem->mChildren[2];
            pSaveConfig->addChild(new Leaf_("?", myCallback, pSaveConfig));
        pSystem->addChild(new Leaf_("Defaults", myCallback, pSystem));
    // clang-format on
}