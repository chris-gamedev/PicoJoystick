#include <Arduino.h>
#include <unity.h>
#include "MyJoystick.h"
#include "MyJoystick.cpp"
#include "Display.h"
#include "Compositor.h"
#include "Compositor.cpp"
#include "Animation.h"
#include "Animation.cpp"
// #include "manifold8pt7b.h"

// MyJoystickBT_ MyJoystickBT;
JoyDisplay_ JoyDisplay;
Compositor_ compositor(&JoyDisplay);

void setUp(void)
{
    // set stuff up here
        if (!JoyDisplay.begin(0x3D))
    {
        Serial.println("Unable to initialize OLED");
        while (1)
            yield();
    }
    
    JoyDisplay.setFont(&manifold8pt7b);
    JoyDisplay.setTextWrap(false);
    JoyDisplay.clearDisplay();
    JoyDisplay.display();

    MyJoystickBT.begin("Chris Joystick testing");
}

void tearDown(void)
{
    // clean stuff up here
}
void startTheDialog() {

}
void test_integerInput()
{

    JoyDisplay.clearDisplay();
    JoyDisplay.print("testing");
    JoyDisplay.display();
    delay(300);

    uint32_t testInput = 5;
    AnimInputDialogInt_<uint32_t> inputdialog("This Font!", &testInput, 5, 10000);
    compositor.registerAnimation(&inputdialog, CanvasType::FG);
    while (!inputdialog.finished())
    {
        MyJoystickBT.pollJoystick();
        compositor.update();
        compositor.draw();
    }

    TEST_ASSERT(testInput == 5);
}

int runUnityTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_integerInput);

    return UNITY_END();
}

void setup()
{
    delay(3000); // let unity connect & board connect to serial

    runUnityTests();
}

void loop()
{
}