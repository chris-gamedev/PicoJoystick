#include <Arduino.h>
#include <unity.h>
#include <vector>
#include "ParseConfig.h"
#include "ParseConfig.cpp"

using namespace parseTools;

template <typename T>
void printIntVector(const String &name, std::vector<T> &vec)
{
    Serial.printf("line:%s    =    ", name.c_str());
    for (auto it : vec)
        Serial.printf("%d  ", it);
    Serial.printf("\n");
}

void setUp(void)
{
    // set stuff up here
    Serial.begin(115200);
    delay(5000);
    Serial.flush();
    Serial.println("beginning test");
}

void tearDown(void)
{
    // clean stuff up here
}

void test_ValidateLineFormat()
{

    const char *s = "<fine:5=1,2,3,4,5>";
    TEST_ASSERT(validateLineFormat(s));
    s = "fine:5=1,2,3,4,5>";
    TEST_ASSERT(!validateLineFormat(s));
    s = "<fine:5=1,2,3,4,5";
    TEST_ASSERT(!validateLineFormat(s));
    s = "<fine:51,2,3,4,5>";
    TEST_ASSERT(!validateLineFormat(s));
}


void test_binStringToInt()
{

    uint8_t testInt8 = 0;
    const char *testbinary;

    testbinary = "0b00000011"; // ok
    TEST_ASSERT(binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(3, testInt8);
    testbinary = " 0 b 0 0 0 0 0 0 1 1 "; // ok
    TEST_ASSERT(binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(3, testInt8);

    uint16_t testInt16 = 0;
    testbinary = "0b0000000000000011"; // ok
    TEST_ASSERT(binStringToInt<uint16_t>(testbinary, testInt16));
    Serial.printf("test int string %d\n", testInt16);
    TEST_ASSERT_EQUAL(3, testInt16);
    uint32_t testInt32 = 0;
    testbinary = "0b00000000000000000000000000000011"; // ok
    TEST_ASSERT(binStringToInt<uint32_t>(testbinary, testInt32));
    Serial.printf("test int string %d\n", testInt32);
    TEST_ASSERT_EQUAL(3, testInt32);
    uint64_t testInt64 = 0;
    testbinary = "0b0000000000000000000000000000000000000000000000000000000000000001"; // ok
    TEST_ASSERT(binStringToInt<uint64_t>(testbinary, testInt64));
    Serial.printf("test int string %d\n", testInt64);
    TEST_ASSERT_EQUAL(1, testInt64);

    testInt32 = 0;
    testbinary = "0b10000101"; // too short for type
    TEST_ASSERT(!binStringToInt<uint32_t>(testbinary, testInt32));
    Serial.printf("test int string %d\n", testInt32);

    testInt8 = 0;
    testbinary = "0b10110001"; // ok
    TEST_ASSERT(binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(177, testInt8);

    testInt8 = 0;
    testbinary = "0Z10110001"; // malformed prefix
    TEST_ASSERT(!binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(0, testInt8);

    testInt8 = 0;
    testbinary = "0b0110001"; // too short
    TEST_ASSERT(!binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(0, testInt8);

    testInt8 = 0;
    testbinary = "10110001"; // no prefix
    TEST_ASSERT(!binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);
    TEST_ASSERT_EQUAL(0, testInt8);

    testInt8 = 0;
    testbinary = "0b000000011"; // too long for type
    TEST_ASSERT(!binStringToInt<uint8_t>(testbinary, testInt8));
    Serial.printf("test int string %d\n", testInt8);

    testInt16 = 0;
    testbinary = "0b00000000000000011"; // too long for type
    TEST_ASSERT(!binStringToInt<uint16_t>(testbinary, testInt16));
    Serial.printf("test int string %d\n", testInt16);

    testInt32 = 0;
    testbinary = "0b000000000000000010000000000000011"; // too long for type
    TEST_ASSERT(!binStringToInt<uint32_t>(testbinary, testInt32));
    Serial.printf("test int string %d\n", testInt32);

    testInt64 = 0;
    testbinary = "0b00000000000000000000000000000000000000000000000000000000000000011"; // too long for type
    TEST_ASSERT(!binStringToInt<uint64_t>(testbinary, testInt64));
    Serial.printf("test int string %d\n", testInt64);
}

void test_prepParsableLine()
{

    char *token;
    char *value;
    int valueCount;
    char line[100];
    bool pass;

    const char *s = "<THISISOK:4=1,2,3,4>";
    strcpy(line, s);
    TEST_ASSERT(prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    TEST_ASSERT_EQUAL_STRING(token, "THISISOK");
    TEST_ASSERT_EQUAL(valueCount, 4);
    TEST_ASSERT_EQUAL_STRING(value, "1,2,3,4");
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISnotOK:41,2,3,4=>";
    strcpy(line, s);
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    strcpy(line, "<=THISISnotOK:1,2,3,4>");
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISnotOK:4=>";
    strcpy(line, s);
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISOK:0=1,2.3>";
    strcpy(line, s);
    TEST_ASSERT(prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    TEST_ASSERT_EQUAL_STRING(token, "THISISOK");
    TEST_ASSERT_EQUAL(valueCount, 0);
    TEST_ASSERT_EQUAL_STRING(value, "1,2.3");
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISnotOK:a=1,2.3>";
    strcpy(line, s);
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISnotOK:0t=1,2.3>";
    strcpy(line, s);
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISnotOk:=1,2.3>";
    strcpy(line, s);
    TEST_ASSERT(!prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);

    s = "<THISISOK=1>";
    strcpy(line, s);
    TEST_ASSERT(prepParsableLine(line, token, valueCount, value, "<", "=", ">"));
    Serial.printf("%s : %d  =   %s   \n", token, valueCount, value);
    TEST_ASSERT_EQUAL_STRING(token, "THISISOK");
    TEST_ASSERT_EQUAL(valueCount, 1);
    TEST_ASSERT_EQUAL_STRING(value, "1");

    Serial.printf("\n\n");
}
void test_buildIntTokenVector()
{
    std::vector<uint16_t> v;
    v.reserve(14);
    const char *s;
    
    // good with defaults
    s = "1,2,3,4";
    TEST_ASSERT(buildIntTokenVector(s, v));
    TEST_ASSERT_EQUAL(v.size(), 4);
    printIntVector(s, v);

    // good with expected size & delimeter
    s = "1,2,3,4";
    TEST_ASSERT(buildIntTokenVector(s, v, ",", 4));
    TEST_ASSERT_EQUAL(v.size(), 4);
    printIntVector(s, v);

    // wrong expected size
    s = "1,2";
    TEST_ASSERT(!buildIntTokenVector(s, v, ",", 3));
    TEST_ASSERT_EQUAL(v.size(), 0);
    printIntVector(s, v);

    // good with whitespace
    s = " 1, 2 , 3,4,   5,6    ,7";
    TEST_ASSERT(buildIntTokenVector(s, v));
    TEST_ASSERT_EQUAL(v.size(), 7);
    printIntVector(s, v);

    // bad list
    s = "1,23,4,a";
    TEST_ASSERT(!buildIntTokenVector(s, v));
    TEST_ASSERT_EQUAL(v.size(), 0);
    printIntVector(s, v);

    // bad list
    s = "1, ,3,4,5";
    TEST_ASSERT(!buildIntTokenVector(s, v));
    TEST_ASSERT_EQUAL(v.size(), 0);
    printIntVector(s, v);
}


void test_removeWhitespace()
{
    const char *sample_strings[] =
        {
            "nothing to trim",
            "    trim the front",
            "trim the back     ",
            "    trim front and back     ",
            " trim one char front and back ",
            " trim one char front",
            "trim one char back ",
            "                   ",
            " ",
            "a",
            " a ",
            "",
            NULL};
                const char *result_remove_strings[] =
        {
            "nothingtotrim",
            "trimthefront",
            "trimtheback",
            "trimfrontandback",
            "trimonecharfrontandback",
            "trimonecharfront",
            "trimonecharback",
            "",
            "",
            "a",
            "a",
            "",
            NULL};

    char buff[100];
    Serial.printf("\n--- parseTools::RemoveWhitespace() ---\n");
    const char *pad = " ";
    for (int i = 0; sample_strings[i] != NULL; i++)
    {
        strcpy(buff, sample_strings[i]);
        char *s = parseTools::removeWhitespace(buff); /* code */
        Serial.printf("sample_strings[%d] = \"%s\" %*s returned string = \"%s\"\n", i, sample_strings[i], 35 - strlen(sample_strings[i]), pad, s);
        TEST_ASSERT_EQUAL_STRING(result_remove_strings[i], s);
    }
}

void test_trim()
{
    const char *sample_strings[] =
        {
            "nothing to trim",
            "    trim the front",
            "trim the back     ",
            "    trim front and back     ",
            " trim one char front and back ",
            " trim one char front",
            "trim one char back ",
            "                   ",
            " ",
            "a",
            " a ",
            "",
            NULL};
    const char *result_trim_strings[] =
        {
            "nothing to trim",
            "trim the front",
            "trim the back",
            "trim front and back",
            "trim one char front and back",
            "trim one char front",
            "trim one char back",
            "",
            "",
            "a",
            "a",
            "",
            NULL};
    
    char buff[100];
    Serial.printf("\n--- parseTools::trim() ---\n");
    const char* pad = " ";
    for (int i = 0; sample_strings[i] != NULL; i++)
    {
        strcpy(buff, sample_strings[i]);
        char *s = parseTools::trim(buff); /* code */
        Serial.printf("sample_strings[%d] = \"%s\" %*s returned string = \"%s\"\n", i, sample_strings[i], 35 - strlen(sample_strings[i]), pad, s);
        TEST_ASSERT_EQUAL_STRING(result_trim_strings[i], s);
    }
}


int runUnityTests()
{
    UNITY_BEGIN();

    RUN_TEST(test_ValidateLineFormat);
    RUN_TEST(test_binStringToInt);
    RUN_TEST(test_prepParsableLine);
    RUN_TEST(test_buildIntTokenVector);
    RUN_TEST(test_trim);
    RUN_TEST(test_removeWhitespace);

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