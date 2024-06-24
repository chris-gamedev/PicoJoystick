#ifndef APPLET_H
#define APPLET_H

namespace AppletNames
{
    enum TAppletNames
    {
        SHOW_BUTTON_PRESSES,
        MENU,
        REMAP_BUTTONS,
        ASSIGN_TURBO,
        CREATE_MACRO,
        ASSIGN_MACRO,
        BLANK,
        MAX_VALUE
    };
}

namespace AppletStatus
{
    enum TAppletStatus
    {
        ALIVE,
        RETURN,
        ENDED
    };
}

class Applet_
{
public:
    Applet_(Compositor_ *comp) : mpCompositor(comp) {}
    virtual void initApp() {};
    virtual AppletStatus::TAppletStatus updateApp() { return AppletStatus::ALIVE; }
    virtual void cleanupApp() {}
    Compositor_ *const mpCompositor;

    static uint8_t
    findHeldButton(uint32_t statemap, uint16_t delay)
    {
        static uint32_t sLastStateMap = 0x80000000;
        static bool sFreshData = false;
        static bool sStartedTheCount = false;
        static uint32_t sLastTime;
        uint8_t foundButton = 255;

        // Gotta start with all buttons released.
        if (!sFreshData && !statemap && !sLastStateMap)
            return foundButton;

        sFreshData = true;

        // mLocalButtonStateMap = MyJoystickBT.getPackedKeyPresses();
        if (!statemap || !sLastStateMap)
        {
            sLastStateMap = statemap;
            sStartedTheCount = false;
            return foundButton;
        }
        if (statemap != sLastStateMap)
        {
            sLastStateMap = statemap;
            sStartedTheCount = false;
            return foundButton;
        }
        sLastStateMap = statemap;
        // we finally have 2 consecutive, identical, non 0 states!
        if (!sStartedTheCount)
        {
            sLastTime = millis();
            sStartedTheCount = true;
        }

        if (delay + sLastTime > millis())
            return foundButton;

        for (foundButton = 0; foundButton < 12; foundButton++)
            if (MAKE_BUTTON_BITMASK_16(foundButton) & statemap)
                break;

        sStartedTheCount = false;
        sFreshData = false;
        sLastStateMap = 0x80000000;
        return foundButton;
    }
};

class AppletSwitcher_
{
public:
    AppletSwitcher_(Compositor_ *comp) : mpCompositor(comp), mBlankApplet(comp)

    {
        // WHEN ADDING:  REMEMBER TO EXPAND THE ARRAY IN THE DAMN HEADER!!!

        mApplets[AppletNames::BLANK] = &mBlankApplet;
        mCurrentApp = AppletNames::ASSIGN_MACRO;
    }
    void inline addApp(AppletNames::TAppletNames name, Applet_ *app) { mApplets[name] = app; }
    void inline setDefaultApp(AppletNames::TAppletNames app) { mDefaultApp = app; }
    void inline switchApp(AppletNames::TAppletNames app)
    {
        mApplets[mCurrentApp]->cleanupApp();
        mLastApp = mCurrentApp;
        mCurrentApp = app;
        mApplets[mCurrentApp]->initApp();
    }

    void inline start() { mApplets[mCurrentApp]->initApp(); }
    void inline update()
    {
        AppletStatus::TAppletStatus returnCode;
        if (mCurrentApp == AppletNames::BLANK)
            return;
        returnCode = mApplets[mCurrentApp]->updateApp();
        switch (returnCode)
        {
        case AppletStatus::ENDED:
            switchApp(mDefaultApp);
            break;
        case AppletStatus::RETURN:
            switchApp(mLastApp);
            break;
        }
    }

    Compositor_ *const mpCompositor;
    Applet_ *mApplets[AppletNames::MAX_VALUE];
    AppletNames::TAppletNames mDefaultApp = AppletNames::BLANK;
    AppletNames::TAppletNames mCurrentApp = AppletNames::BLANK;
    AppletNames::TAppletNames mLastApp = mDefaultApp;
    Applet_ mBlankApplet;
};

extern AppletSwitcher_ AppSwitcher;

#endif