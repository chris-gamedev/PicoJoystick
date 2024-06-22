#include <Arduino.h>
#include <unity.h>
#include "MyJoystick.h"
#include "MyJoystick.cpp"
#include "Display.h"
#include "Compositor.h"
#include "CreateMacro.h"
#include "CreateMacro.cpp"
#include "manifold4pt7b.h"
#include "manifold6pt7b.h"
// #include "ManifoldNumbersOnly6pt7b.h"
// #include "Manifold_ArrowsAt21_12pt7b.h"
#include "Images.h"

// MyJoystickBT_ MyJoystickBT;
JoyDisplay_ display;
Compositor_ compositor(&display);

void setUp(void)
{
    // set stuff up here
    if (!display.begin(0x3D))
    {
        Serial.println("Unable to initialize OLED");
        while (1)
            yield();
    } 
     MyJoystickBT.begin("ChrisJoystick Test 2");
}

void tearDown(void)
{
    // clean stuff up here
}

void test_SmallNumbers()
{

    display.clearDisplay();
    display.setCursor(0, 20);
    display.setFont(&manifold8pt7b);
    display.println("Numbers Only");
    display.setFont(&manifold6pt7b);
    display.setTextWrap(true);
    // display.print("abcdefghijklmnopqrstuvwxyz.!()/\\-*\n");
    // for (int i = 0; i < 100; i++)
    //     display.write(i);
    display.print(" 0123456789");
    display.display();
    delay(2000);
}

void test_Characters()
{
    display.clearDisplay();
    display.setFont(&Manifold_ArrowsAt21_12pt7b);
    display.setCursor(0, 20);
    display.setTextWrap(true);

    display.print("!\"#$%&'()*+");

    display.display();
    delay(3000);
    display.clearDisplay();
    display.setCursor(0, 20);
}

#define LAYOUT_X 28
#define LAYOUT_Y 20
#define BUTTON_SIZE 8

void test_macroButtonLayout()
{
    for (int i = 0; i < 9; i++)
    {

        display.clearDisplay();
        display.setFont(&manifold6pt7b);
        display.setCursor(1, LAYOUT_Y + 10);
        display.drawFastHLine(0, LAYOUT_Y + 6, 6, 0xA);
        display.drawFastHLine(0, LAYOUT_Y + 7, 6, 0xA);
        display.drawFastHLine(0, LAYOUT_Y + 8, 6, 0xA);
        // display.drawRect(0, LAYOUT_Y + 3, 7, 7, 0xA);
        display.setFont(&Manifold_ArrowsAt21_12pt7b);
        display.setCursor(LAYOUT_X - 18, LAYOUT_Y + 15);
        display.write(0X21 + i);
        display.drawBitmap(LAYOUT_X + 0 * BUTTON_SIZE, LAYOUT_Y + 0 * BUTTON_SIZE, bitmap_ButtonLayoutMacroView64x16, 64, 16, 0x8);
        display.drawBitmap(LAYOUT_X + 2 * BUTTON_SIZE, LAYOUT_Y + 0 * BUTTON_SIZE, bitmap_ButtonMacroView8x8, 8, 8, 0xF);
        display.drawBitmap(LAYOUT_X + 3 * BUTTON_SIZE, LAYOUT_Y + 1 * BUTTON_SIZE, bitmap_ButtonMacroView8x8, 8, 8, 0xF);
        display.drawBitmap(LAYOUT_X + 6 * BUTTON_SIZE, LAYOUT_Y + 0.5 * BUTTON_SIZE, bitmap_ButtonMacroView8x8, 8, 8, 0xF);

        display.setFont(&manifold6pt7b);
        display.setCursor(LAYOUT_X + 67, LAYOUT_Y + 11);
        display.print("1234");

        display.display();
        delay(200);
    }
}

void test_PhraseTable()
{
    display.clearDisplay();

    // AnimTextStatic1Line_ title (0, 0);
    // title.setText("title");
    // title.mDrawBox = true;
    // title.drawAnim(&display);
    display.setCursor(46, 10);
    display.setFont(&manifold6pt7b);
    display.drawRoundRect(0, 0, 128, 15, 10, 0xf);
    display.print("Title");
    struct MacroWord word1 = {0x0001, 1, 100};
    struct MacroWord word2 = {0x0002, 2, 200};
    struct MacroWord word3 = {0x0005, 3, 300};
    struct MacroWord word4 = {0x0025, 4, 400};
    struct MacroWord word5 = {0x0115, 5, 500};
    std::vector<MacroWord> vwords = {word1, word2, word3, word4, word5};

    PhraseTable_ PhraseTable(0, 20, vwords);
    while (PhraseTable.update())
    {
        display.clearDisplay();
        MyJoystickBT.pollJoystick();
        PhraseTable.drawAnim(&display);
        display.display();
    }
    vwords = PhraseTable.savePhrase();
    PhraseTable.deleteTable();
    TEST_ASSERT_EQUAL(7, vwords.size());
    TEST_ASSERT_EQUAL(0, PhraseTable.mvpRows.size());
    delay(200);
}

int runUnityTests()
{
    UNITY_BEGIN();

    // RUN_TEST(test_allSizes);
    // RUN_TEST(test_SmallNumbers);
    // delay(1000);
    // RUN_TEST(test_Characters);
    // delay(1000);
    RUN_TEST(test_macroButtonLayout);
    RUN_TEST(test_PhraseTable);

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