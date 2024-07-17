#include <Arduino.h>
#include <unity.h>
#include "Configuration.h"
#include "Configuration.cpp"
#include "FSTools.h"
#include "FSTools.cpp"
#include "ParseConfig.h"
#include "ParseConfig.cpp"
#include "PicoJoystick_types.h"

Configurator_ configurator({});
void printMacro(const Macro &m);
Macro macro1, macro2;
// struct MacroWord
// {
//   uint16_t mButtonStateMap = 0;
//   uint8_t mJoyState = 0;
//   uint16_t mDuration = 50;
// };

// struct Macro
// {
//   std::vector<MacroWord> phrase;
//   String name = "-blank-";
//   uint16_t enabledButtonsMap = 0xFFFF;
//   uint8_t enabledJoystickState = 0xFF;
// };

void setUp(void)
{
    Serial.begin();
    // set stuff up here

    macro1.name = "macro1";
    macro1.enabledButtonsMap = 0b1000101011110000;
    macro1.enabledJoystickState = 0xFF;
    macro1.phrase.push_back(MacroWord{0b0000000100000001, 0, 50});
    macro1.phrase.push_back(MacroWord{0b0000000100000010, 1, 51});
    macro1.phrase.push_back(MacroWord{0b0000000100000100, 2, 52});
    macro1.phrase.push_back(MacroWord{0b0000000100001000, 3, 53});
    macro1.phrase.push_back(MacroWord{0b0000000100010000, 4, 54});
    macro1.phrase.push_back(MacroWord{0b0000000100100000, 5, 55});
    macro1.phrase.push_back(MacroWord{0b0000000101000000, 6, 56});
    macro1.phrase.push_back(MacroWord{0b0000000110000000, 7, 57});
    macro1.phrase.push_back(MacroWord{0b0000000101010101, 8, 58});
    macro1.phrase.push_back(MacroWord{0b1111000100000000, 0, 59});
}



void tearDown(void)
{
    // clean stuff up here
}

void test_saveMacro()
{
    printMacro(macro1);
    configurator.saveMacroToFile(macro1);

    fstools::listFilesToSerial();
    fstools::printAllFilesInDirectoryToSerial();
}

void test_readMacroFile()
{
    printMacro(macro2);

    configurator.importMacroFile("macro1", macro2);

    printMacro(macro2);
}

int runUnityTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_saveMacro);
    RUN_TEST(test_readMacroFile);

    return UNITY_END();
}

void setup()
{

    delay(3000); // let unity connect & board connect to serial
    Serial.printf("beginning tests\n");
    runUnityTests();
}

void loop()
{
    delay(10000);
}

void printMacro(const Macro &m)
{
    Serial.printf("\n---------- PRINTED MACRO -----------\n");
    Serial.printf("Macro.name = %s\n", m.name.c_str());
    Serial.printf("EnabledButtonsMap = 0b");
    Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m.enabledButtonsMap >> 8));
    Serial.printf(" ");
    Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(m.enabledButtonsMap & 0x00FF));
    Serial.printf("\n");
    Serial.printf("EnabledJoystickState = %d\n", m.enabledJoystickState);
    Serial.printf("Phrase:");
    for (auto it : m.phrase)
    {
        Serial.printf("\t mButtonStateMap = 0b");
        Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(it.mButtonStateMap >> 8));
        Serial.printf(" ");
        Serial.printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(it.mButtonStateMap & 0x00FF));

        Serial.printf("\t mJoyState = %d", it.mJoyState);
        Serial.printf("\t mDuration = %d\n", it.mDuration);
    }
    Serial.printf("---------- PRINTED MACRO -----------\n\n");
}