#include <Arduino.h>
#include <unity.h>
#include <FS.h>
#include <LittleFS.h>
#include <vector>
#include "Configuration.h"
#include "Configuration.cpp"


Configurator_ configurator({nullptr});

void setUp(void)
{
    // set stuff up here
    LittleFSConfig cfg;
    cfg.setAutoFormat(false);
    
    LittleFS.setConfig(cfg);

    Serial.begin(115200);
    delay(1000);
    Serial.flush();
    Serial.println("beginning test");
    delay(1000);

    // TEST_ASSERT_MESSAGE(LittleFS.begin(), "filesystem opened\n");
}

void test_readTestFile()
{
    
    const char *filename = "/testconfig.txt";

    int rval = configurator.importConfigFile(filename, &configurator.mConfig);
    // TEST_ASSERT_EQUAL(0, rval);

    Configuration testConfig = {
      .global_menuHotkey_on = true
    , .joystick_joyValueMap = {0,1,2,3,4,5,6,7,8}
    , .joystick_buttonValueMap = {0,1,2,3,4,5,6,7,8,9,10,11}
    , .joystick_transmitToHost=false
    , .drawKeypresses_macroMap = {0,1,2,3,4,5,6,7,8,9,10,11}
    , .funThings_on = false
    };
    TEST_ASSERT(configurator.mConfig == testConfig);

    Serial.printf("\nresults in configurator.mConfig:\n");
    Serial.printf(" configurator.mConfig.global_menuHotkey_on %d\n", configurator.mConfig.global_menuHotkey_on);
    Serial.printf(" configurator.mConfig.joystick_joyValueMap:\n");
    for (int i = 0; i < 9; i++)
        Serial.printf("    [%d]=%d\n", i, configurator.mConfig.joystick_joyValueMap[i]);

    Serial.printf(" configurator.mConfig.joystick_buttonValueMap:\n");
    for (int i = 0; i < 12; i++)
        Serial.printf("    [%d]=%d\n", i, configurator.mConfig.joystick_buttonValueMap[i]);
    Serial.printf(" configurator.mConfig.joystick_transmitToHost %d\n", configurator.mConfig.joystick_transmitToHost);
    Serial.printf(" configurator.mConfig.drawKeypresses_macroMap:\n");
    for (int i = 0; i < 12; i++)
        Serial.printf("    [%d]=%d\n", i, configurator.mConfig.drawKeypresses_macroMap[i]);
    Serial.printf("    configurator.mConfig.funThings_on  %d\n", configurator.mConfig.funThings_on );
    
    LittleFS.end();

}


void tearDown(void)
{
    // clean stuff up here
    // LittleFS.end();
}
template <std::size_t N>
void tokenizeArrayToFile(String name, File f, const std::array<uint8_t, N> & arr) {
    int size = arr.size();
    f.printf("<%s:%d=", name.c_str(), size);
    for (int i = 0; i < size; i++) {
        f.printf("%d", arr[i]);
        if (i != size - 1)
            f.printf(",");
        else    
            f.printf(">\n");
    }

}



void test_createConfigFile()
{
    const char *filename = "/testconfig.txt";

    Configuration defaultConfig;

    Configuration testConfig = {
      .global_menuHotkey_on = false
    , .joystick_joyValueMap = {2,4,6,3,4,5,6,7,8}
    , .joystick_buttonValueMap = {2,4,6,3,4,5,6,7,8,9,10,11}
    , .joystick_transmitToHost=true
    , .drawKeypresses_macroMap = {2,4,6,3,4,5,6,7,8,9,10,11}
    , .funThings_on = true
    };
    Serial.printf("resetting configurator.mConfig to defaults\n");
    configurator.mConfig = defaultConfig;
    Serial.printf("saving test configuration to %s\n", filename);
    configurator.saveConfigToFile(filename, &testConfig);

    configurator.printFileToSerial(filename);

    Serial.printf("Importing configuration into configurator...\n");
    configurator.importConfigFile(filename, &configurator.mConfig);
    delay(1000);

    TEST_ASSERT(configurator.mConfig == testConfig);

    LittleFS.end();
}

void test_openConfigFile()
{
}

void test_writeConfigFile()
{
}

void test_readConfigFile()
{
}

int runUnityTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_createConfigFile);
    // RUN_TEST(test_readTestFile);
    // RUN_TEST(test_openConfigFile);
    // RUN_TEST(test_writeConfigFile);
    // RUN_TEST(test_readConfigFile);

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

