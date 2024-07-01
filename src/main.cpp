#include <Arduino.h>
#include <stdlib.h>
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

    MyJoystickBT.begin("ChrisJoystick Test 2");

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
    
    AppSwitcher.addApp(AppletNames::MENU, &MenuManager);
    AppSwitcher.addApp(AppletNames::SHOW_BUTTON_PRESSES, &DrawKeyPresses);
    AppSwitcher.addApp(AppletNames::REMAP_BUTTONS, &RemapButtons);
    AppSwitcher.addApp(AppletNames::CREATE_MACRO, &CreateMacro);
    AppSwitcher.addApp(AppletNames::ASSIGN_TURBO, &AssignTurbo);
    AppSwitcher.addApp(AppletNames::ASSIGN_MACRO, &AssignMacro);
    AppSwitcher.addApp(AppletNames::SAVE_CONFIGURATION, &SaveConfigurationApp);
    AppSwitcher.addApp(AppletNames::LOAD_CONFIGURATION, &LoadConfigApp);
    AppSwitcher.start();

}

void loop()
{
    MyJoystickBT.pollJoystick();
    if (menuHotkeyPressed())
        AppSwitcher.switchApp(AppletNames::MENU);
    AppSwitcher.update();
    Fun.doTheFunThings();
    Compositor.update();
    Compositor.draw();

    

#ifdef DEADBEEF
    if (MyJoystickBT.buttonJustPressed(0))
        Serial.println("------------- discrete press-0--------------");
    if (MyJoystickBT.buttonJustPressed(1))
        Serial.println("------------- discrete press-1--------------");
    if (MyJoystickBT.buttonJustPressed(2))
        Serial.println("------------- discrete press-2--------------");
    if (MyJoystickBT.buttonJustPressed(3))
        Serial.println("------------- discrete press-3--------------");
    if (MyJoystickBT.buttonJustPressed(4))
        Serial.println("------------- discrete press-4--------------");

    if (MyJoystickBT.joyJustPressed(JOY_UP))
        Serial.println("------------- discrete press-JOY-UP--------------");
    if (MyJoystickBT.joyJustPressed(JOY_DOWN))
        Serial.println("------------- discrete press-JOY-DOWN--------------");
    if (MyJoystickBT.joyJustPressed(JOY_LEFT))
        Serial.println("------------- discrete press-JOY-LEFT--------------");
    if (MyJoystickBT.joyJustPressed(JOY_RIGHT))
        Serial.println("------------- discrete press-JOY-RIGHT--------------");
    if (MyJoystickBT.joyJustPressed(JOY_UP_RIGHT))
        Serial.println("------------- discrete press-JOY-UP-RIGHT--------------");
#endif        
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
