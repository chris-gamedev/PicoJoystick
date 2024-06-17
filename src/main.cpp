#include <Arduino.h>
#include <stdlib.h>
#include "MyJoystick.h"
#include "Display.h"
#include "MenuManager.h"
#include "Images.h"
#include "Applications.h"
#include "Pacman.h"

Compositor_ Compositor(&JoyDisplay);
MenuManager_ MenuManager(&Compositor);

// Applets
DrawKeyPressesApp_ DrawKeyPresses(&Compositor);
RemapButtonsApp_ RemapButtons(&Compositor, &DrawKeyPresses);
AppletSwitcher_ AppSwitcher(&Compositor);

// temp. add this state to the joystick
bool joyON = true;

// find a good home for these?
uint32_t timeMenuHotkeyDelay = 1000;
uint32_t timeMenuHotkeyLast;

bool menuHotkeyPressed();

// temp testing fun things

AnimSprite8_ aSprite8_1(0, 20, 16, 16, 5, 200, 1, 3, 0);
AnimSprite8_ aSprite8_2(0, 45, 16, 16, 4, 200, 4, 2, 0);
AnimSprite8_ aSprite8_blinky(0, 55, 16, 16, 5, 80, 4, 3, 0);
AnimSprite8_ aSprite8_pinky(0, 60, 16, 16, 4, 200, 4, 3, 0);
AnimSprite8_ aScardey8_1(0, 115, 16, 16, 5, 125, 4, 1, 0);
AnimSprite8_ aScardey8_2(0, 60, 16, 16, 4, 150, 4, 1, 0);

Animation_ *funNotUsed[6] = {&aScardey8_1, &aScardey8_2, &aSprite8_1, &aSprite8_2, &aSprite8_blinky, &aSprite8_pinky};
Animation_ *funInUse[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

uint32_t funDelay = 10;
uint32_t funLastTime = 0;


void doFunThings();

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

    AppSwitcher.addApp(AppletNames::MENU, &MenuManager);
    AppSwitcher.addApp(AppletNames::SHOW_BUTTON_PRESSES, &DrawKeyPresses);
    AppSwitcher.addApp(AppletNames::REMAP_BUTTONS, &RemapButtons);
    AppSwitcher.start();

    // temp testing fun things
    aSprite8_1.setBitmapArray(msPacMan_spritesheetR, msPacMan_spritesheet_size);
    aSprite8_2.setBitmapArray(msPacMan_spritesheetR, msPacMan_spritesheet_size);
    aSprite8_blinky.setBitmapArray(blinky_spritesheet, blinky_spritesheet_size);
    aSprite8_pinky.setBitmapArray(pinky_spritesheet, pinky_spritesheetR_size);
    aScardey8_1.setBitmapArray(scaredy_spritesheet, scaredy_spritesheet_size);
    aScardey8_2.setBitmapArray(scaredy_spritesheet, scaredy_spritesheet_size);
}

void loop()
{
    MyJoystickBT.pollJoystick();
    if (menuHotkeyPressed())
        AppSwitcher.switchApp(AppletNames::MENU);
    AppSwitcher.update();
    doFunThings();
    Compositor.update();
    Compositor.draw();


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

void doFunThings()
{
    // delay for starting a random animation
    if ((funLastTime + funDelay) > millis())
        return;

    funLastTime += funDelay;
    Animation_ *useMe = nullptr;
    Animation_ *temp = nullptr;
    // sort out the dead ones
    for (int i = 0; i < 6; i++)
    {
        if (funInUse[i] == nullptr)
            continue;
        if (funInUse[i]->mlife == 0)
        {
            temp = funInUse[i];
            funInUse[i] = nullptr;
            funNotUsed[i] = temp;
        }
    }
    // pick a free one
    for (int i = 0; i < 6; i++)
    {
        if (funNotUsed[i] != nullptr)
        {
            useMe = funNotUsed[i];
            funNotUsed[i] = nullptr;
            funInUse[i] = useMe;
            break;
        }
    }

    // set up and release our new friend
    if (useMe != nullptr)
    {
        int deltax = random(5) + 1;
        int y = random(100) + 10;
        useMe->mX = -20;
        useMe->mY = y;
        useMe->mdrawOrder = deltax;
        useMe->mlife = 800 * (1 / deltax) + random(200) + 100;
        useMe->mdelay = -1;
        useMe->mdelaycounter = useMe->mdelay;
        useMe->mdeltaX = deltax;
        CanvasType::TCanvasType layer = (random(6) + 1 > 4) ? CanvasType::FG : CanvasType::BG;

        Compositor.registerAnimation(useMe, layer);
    }
}

class DoFunThings
{

public:
    DoFunThings()
    {
    }
};
