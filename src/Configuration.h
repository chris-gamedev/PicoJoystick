#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <vector>
#include <array>
#include <map>
#include <FS.h>
#include <LittleFS.h>

#define CONFIG_FILE_PATH "/config/"
#define MACRO_FILE_PATH "/macros/"

namespace MacroMapType
{
    enum TMacroMapType
    {
        none,
        macro1,
        macro2,
        macro3,
        macro4,
        macro5,
        macro6,
        macro7,
        macro8,
        turbo = 9,
        latchedTurbo = 10
    };
}

namespace ConfigTokens
{
    enum TConfigTokens
    {
        global_menuHotKey_ON,
        joystick_joyValueMap,
        joystick_buttonValueMap,
        joystick_transmitToHost,
        drawKeyPresses_macroMap,
        funThings_on

    };
}

typedef struct Configuration
{
    // Global
    bool global_menuHotkey_on = true;
    std::array<uint8_t, 9> joystick_joyValueMap = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::array<uint8_t, 12> joystick_buttonValueMap = {1, 2, 4, 5, 7, 8, 11, 12, 13, 14, 15, 20};
    bool joystick_transmitToHost = true;
    std::array<uint8_t, 12> drawKeypresses_macroMap = {MacroMapType::none};
    bool funThings_on = true;

    inline bool operator==(const Configuration &rhs)
    {
        bool rVal;
        return this->global_menuHotkey_on == rhs.global_menuHotkey_on &&
               this->joystick_joyValueMap == rhs.joystick_joyValueMap &&
               this->joystick_buttonValueMap == rhs.joystick_buttonValueMap &&
               this->joystick_transmitToHost == rhs.joystick_transmitToHost &&
               this->drawKeypresses_macroMap == rhs.drawKeypresses_macroMap &&
               this->funThings_on == rhs.funThings_on;
    }
} Configuration;

class IConfigurable_
{
public:
    virtual void configure(Configuration *pconfig) = 0;
};

class Configurator_
{
public:
    Configurator_(std::initializer_list<IConfigurable_ *> list)
    {
        mvpConfigurables = list;
    }
    // clang-format off
    const std::map<String, ConfigTokens::TConfigTokens> configTokenMap = {
        {String("GLOBAL_MENUHOTKEY_ON"), ConfigTokens::global_menuHotKey_ON}
        , {String("JOYSTICK_JOYVALUEMAP"), ConfigTokens::joystick_joyValueMap}
        , {String("JOYSTICK_BUTTONVALUEMAP"), ConfigTokens::joystick_buttonValueMap}
        , {String("JOYSTICK_TRANSMITTOHOST"), ConfigTokens::joystick_transmitToHost}
        , {String("DRAWKEYPRESSES_MACROMAP"), ConfigTokens::drawKeyPresses_macroMap}
        , {String("FUNTHINGS_ON"), ConfigTokens::funThings_on}
        };
    // clang-format on
    Configuration mConfig;
    std::vector<IConfigurable_ *> mvpConfigurables;
    void registerConfigurable(IConfigurable_ *c) { mvpConfigurables.push_back(c); }
    void configurate()
    {
        for (auto it : mvpConfigurables)
            it->configure(&mConfig);
    }
    // config parsing & files
    File openFileWithMessages(const char *filename, const char *mode);
    bool parsableLine(String line, int lineCounter);
    std::vector<uint16_t> buildTokenVector(String *token, String value, int lineCounter);
    int importConfigFile(const char *filename, Configuration *pconfig);
    bool saveConfigToFile(const char *filename, Configuration *pconfig);
    bool inline saveCurrentConfig(const char *filename) { return saveConfigToFile(filename, &this->mConfig); }
    void saveTokenToConfig(String token, String value, Configuration *pconfig)
    {
        auto t = configTokenMap.find(token);
        if (t == configTokenMap.end())
        {
            Serial.printf("--TOKEN %s NOT FOUND IN CONFIGURATION--\n", token.c_str());
            return;
        }
    }
    void saveTokenToConfig(String token, std::vector<uint16_t> values, Configuration *pconfig)
    {
        auto t = configTokenMap.find(token);
        if (t == configTokenMap.end())
        {
            Serial.printf("--TOKEN %s NOT FOUND IN CONFIGURATION--\n", token.c_str());
            return;
        }
        int i = 0;
        switch (t->second)
        {
        case ConfigTokens::joystick_joyValueMap:
            assignTokenValuesToArray(token, pconfig->joystick_joyValueMap, &values);
            break;
        case ConfigTokens::joystick_buttonValueMap:
            assignTokenValuesToArray(token, pconfig->joystick_buttonValueMap, &values);
            break;
        case ConfigTokens::drawKeyPresses_macroMap:
            assignTokenValuesToArray(token, pconfig->drawKeypresses_macroMap, &values);
            break;
        case ConfigTokens::global_menuHotKey_ON:
            pconfig->global_menuHotkey_on = static_cast<bool>(values[0]);
            break;
        case ConfigTokens::joystick_transmitToHost:
            pconfig->joystick_transmitToHost = static_cast<bool>(values[0]);
            break;
        case ConfigTokens::funThings_on:
            pconfig->funThings_on = static_cast<bool>(values[0]);
            break;
        }
        Serial.printf("%s, size:%d -- seemed to be successful\n", token.c_str(), values.size());
    }
    template <std::size_t N>
    void assignTokenValuesToArray(String token, std::array<uint8_t, N> &arr, std::vector<uint16_t> *values)
    {
        if (values->size() != arr.size())
        {
            Serial.printf("%s : --INVALID NUMBER OF VALUES--\n", token.c_str());
            return;
        }
        int i = 0;
        for (auto it : *values)
            arr[i++] = static_cast<uint8_t>(it);
    }
    template <std::size_t N>
    void tokenizeArrayToFile(String name, File f, const std::array<uint8_t, N> &arr);

    // Printing to Serial
private:
    void listFilesToSerialRcrsv(String dirname = "/");
    void printAllFilesInDirectoryToSerialRcrsv(String dirname = "/");
    void printFileTreeToSerialRcrsv(String path = "/", String dirName = "/", String treeString = "");

public:
    void printFileSystemInfoToSerial();
    void printFileToSerial(const char *name);
    void printFileToSerial(File f);
    void listFilesToSerial(String dirname = "/")
    {
        if (!LittleFS.begin())
        {
            Serial.printf("Failed to mount file system\n");
            return;
        }
        listFilesToSerialRcrsv(dirname);
        LittleFS.end();
    }
    void printAllFilesInDirectoryToSerial(String dirname = "/")
    {
        if (!LittleFS.begin())
        {
            Serial.printf("Failed to mount file system\n");
            return;
        }
        printAllFilesInDirectoryToSerialRcrsv(dirname);
        LittleFS.end();
    }
    void printFileTreeToSerial(String path = "/", String dirName = "/", String treeString = "")
    {
        if (!LittleFS.begin())
        {
            Serial.printf("Failed to mount file system\n");
            return;
        }

        printFileTreeToSerialRcrsv(path, dirName, treeString);
        LittleFS.end();
    };
    std::vector<String> getDirListOnlyFiles(String path);
    // file manipulation
    std::vector<String> inline getConfigFileList() {return getDirListOnlyFiles(CONFIG_FILE_PATH);}
    std::vector<String> inline getMacroFileList() {return getDirListOnlyFiles(MACRO_FILE_PATH);}
    

};

#endif // CONFIGURATION_H