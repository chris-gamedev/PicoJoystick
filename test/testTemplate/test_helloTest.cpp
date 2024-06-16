#include <Arduino.h>
#include <unity.h>  

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_integerInput() {
    Serial.println("Hello to serial");

}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_integerInput);

    return UNITY_END();

}

void setup() {

    delay(3000);  // let unity connect & board connect to serial

    runUnityTests();

}

void loop() {

}