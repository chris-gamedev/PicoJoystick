#include <Arduino.h>
#include <unity.h> 
#include "Display.h"
#include "Compositor.h"
#include "Compositor.cpp"
#include "MyJoystick.h"
#include "MyJoystick.cpp"
#include "Animation.h"
#include "Animation.cpp"
#include "Dialog.h"
#include "Dialog.cpp"

// MyJoystickBT_ MyJoystickBT;
JoyDisplay_ display;
Compositor_ compositor(&display);



void setUp(void) {
    // set stuff up here
    Serial.begin(115200);
    Serial.println("starting test");

    MyJoystickBT.begin("ChrisJoystick Test 2");

    if (!display.begin(0x3D))
    {
        Serial.println("Unable to initialize OLED");
        while (1)
            yield();
    }
    display.setTextWrap(false);
    display.clearDisplay();
    display.display();

    display.setCursor(50, 50);
    display.print("begin test");
    display.display();

    delay(2000);
}

void tearDown(void) {
    // clean stuff up here
}

void test_dialog()
{
    String testString = "123";
    AnimInputDialogString_ inputD(&compositor, &testString, "Title");

    inputD.start("Title", &testString);
    while(inputD.updateDialog()){
        MyJoystickBT.pollJoystick();
        compositor.update();
        compositor.draw();
    }
    display.clearDisplay();
    display.setCursor(0, 20);
    display.setTextWrap(true);
    display.println("ended");
    display.println("output:");
    display.printf("\"%s\"", testString.c_str());
    display.display();
    delay(3000);
    display.clearDisplay();
    display.display();
    // TEST_ASSERT(testString == "test");
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_dialog);

    return UNITY_END();

}

void setup() {

    delay(3000);  // let unity connect & board connect to serial

    runUnityTests();

}

void loop() {

}