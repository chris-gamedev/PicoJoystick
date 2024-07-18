#include <Arduino.h>
#include <pico/mutex.h>
#include <pico/util/queue.h>
#include "MyJoystick.h"
#include "Display.h"
#include "MenuManager.h"
#include "Images.h"
#include "Applications.h"
#include "CreateMacro.h"
#include "Configuration.h"
#include "FunThings.h"

Compositor_ Compositor(&JoyDisplay);

// Applets
MenuManager_ MenuManager(&Compositor);
DrawKeyPressesApp_ DrawKeyPresses(&Compositor);
RemapButtonsApp_ RemapButtons(&Compositor, &DrawKeyPresses);
AssignTurboApp_ AssignTurbo(&Compositor, &DrawKeyPresses);
CreateMacroApp_ CreateMacro(&Compositor, &DrawKeyPresses);
AssignMacroApp_ AssignMacro(&Compositor, &DrawKeyPresses);
SaveConfigurationApp_ SaveConfigurationApp(&Compositor);
LoadConfigApp_ LoadConfigApp(&Compositor);
LoadMacroApp_ LoadMacroApp(&Compositor);
FormatFSApp_ FormatFSApp(&Compositor);
AppletSwitcher_ AppSwitcher(&Compositor);

// Fun Things!!
FunThings_ Fun(&Compositor);

// Config
Configurator_ Configurator({&MyJoystickBT, &DrawKeyPresses, &Fun});

// find a good home for these?
uint32_t timeMenuHotkeyDelay = 1000;
uint32_t timeMenuHotkeyLast;

bool menuHotkeyPressed();

void setup()
{
    Serial.begin(115200);
    Serial.println("use BOOTSEL to toggle joystick on / off");

    MyJoystickBT.begin("Chris Joystick");
    if (!JoyDisplay.begin(0x3D))
    {
        Serial.println("Unable to initialize OLED");
        while (1)
            yield();
    }
    JoyDisplay.setTextWrap(false);
    JoyDisplay.clearDisplay();
    JoyDisplay.display();

    Configurator.configurate();

    AppSwitcher.addApp(AppletNames::menu, &MenuManager);
    AppSwitcher.addApp(AppletNames::drawKeyPresses, &DrawKeyPresses);
    AppSwitcher.addApp(AppletNames::remapButtons, &RemapButtons);
    AppSwitcher.addApp(AppletNames::createMacro, &CreateMacro);
    AppSwitcher.addApp(AppletNames::assignTurbo, &AssignTurbo);
    AppSwitcher.addApp(AppletNames::assignMacro, &AssignMacro);
    AppSwitcher.addApp(AppletNames::loadMacro, &LoadMacroApp);
    AppSwitcher.addApp(AppletNames::saveConfiguration, &SaveConfigurationApp);
    AppSwitcher.addApp(AppletNames::loadConfiguration, &LoadConfigApp);
    AppSwitcher.addApp(AppletNames::formatFS, &FormatFSApp);
    AppSwitcher.start();
}

void setup1()
{
    while (!MyJoystickBT.isReadyPoll())
        delay(300);
}

void loop()
{
    // MyJoystickBT.pollJoystick();
    MyJoystickBT.getStateSnapshot();
    if (menuHotkeyPressed())
        AppSwitcher.switchApp(AppletNames::menu);
    AppSwitcher.update();
    Fun.doTheFunThings();
    Compositor.update();
    Compositor.draw();
}

void loop1()
{
    MyJoystickBT.pollJoystick();
}

// todo: rewrite efficiently
bool menuHotkeyPressed()
{
    uint16_t packedKeys = (MyJoystickBT.getPackedKeyPresses());
    if ((packedKeys & BITMASK_MENU_HOTKEY) == BITMASK_MENU_HOTKEY)
    {
        if (timeMenuHotkeyLast == 0)
            timeMenuHotkeyLast = millis();
        else if ((packedKeys & BITMASK_MENU_HOTKEY) == BITMASK_MENU_HOTKEY)
        {
            if (timeMenuHotkeyLast + timeMenuHotkeyDelay <= millis())
            {
                timeMenuHotkeyLast = 0;
                return true;
            }
        }
        else
            timeMenuHotkeyLast = 0;
    }
    else
        timeMenuHotkeyLast = 0;
    return false;
}
