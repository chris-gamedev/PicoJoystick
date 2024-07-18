#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#ifndef FSTOOLS_LITTLEFS
#define FSTOOLS_LITTLEFS
#endif
#include <vector>
#include <array>
#include <map>
#include "PicoJoystick_types.h"
#include "ParseConfig.h"
#include "FSTools.h"



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
        global_menuHotKey_ON
        , joystick_joyValueMap
        , joystick_buttonValueMap
        , drawKeyPresses_macroMap
        , funThings_on
        // macro stuff
        

    };
}

typedef struct Configuration
{
    // Global
    bool global_menuHotkey_on = true;
    std::array<uint8_t, 9> joystick_joyValueMap = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::array<uint8_t, 12> joystick_buttonValueMap = {1, 2, 4, 5, 7, 8, 11, 12, 13, 14, 15, 20};
    std::array<uint8_t, 12> drawKeypresses_macroMap = {MacroMapType::none};
    bool funThings_on = true;

    inline bool operator==(const Configuration &rhs)
    {
        bool rVal;
        return this->global_menuHotkey_on == rhs.global_menuHotkey_on &&
               this->joystick_joyValueMap == rhs.joystick_joyValueMap &&
               this->joystick_buttonValueMap == rhs.joystick_buttonValueMap &&
               this->drawKeypresses_macroMap == rhs.drawKeypresses_macroMap &&
               this->funThings_on == rhs.funThings_on;
    }
} Configuration;

class IConfigurable_
{
public:
    virtual void configure(const Configuration &config) = 0;
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
        , {String("DRAWKEYPRESSES_MACROMAP"), ConfigTokens::drawKeyPresses_macroMap}
        , {String("FUNTHINGS_ON"), ConfigTokens::funThings_on}
        // macro stuff
        
        };
    // clang-format on
    void registerConfigurable(IConfigurable_ *c) { mvpConfigurables.push_back(c); }
    void inline configurate()
    {
        for (auto it : mvpConfigurables)
            it->configure(mConfig);
    }
    // config parsing & files
    bool prepParsableLine(String &line, String &token, String &value, int lineCounter);
    std::vector<uint16_t> buildTokenVector(String &token, String &value, int lineCounter);
    int importConfigFile(const char *filename, Configuration *pconfig);
    bool saveConfigToFile(const char *filename, const Configuration &pconfig);
    bool inline saveCurrentConfig(const char *filename) { return saveConfigToFile(filename, this->mConfig); }
    void saveTokenToConfig(const String &token, const String &value, Configuration *pconfig);
    void saveTokenToConfig(const String &token, const std::vector<uint16_t>  &values, Configuration *pconfig);
    template <std::size_t N>
    void assignTokenValuesToArray(const String &token, std::array<uint8_t, N> &arr, const std::vector<uint16_t> &values);
    template <std::size_t N>
    void tokenizeContainerToFileInt(const char * name, File f, const std::array<uint8_t, N> &arr);

    // macro parsing & files
    bool importMacroFile(const char *filename, Macro &macro);
    bool saveMacroToFile(const Macro &macro);
    
    

    // Printing to Serial
public:
    void inline printFileSystemInfoToSerial() { fstools::printFileSystemInfoToSerial(); }
    void inline printFileToSerial(const char *name) { fstools::printFileToSerial(name); }
    void inline printFileToSerial(File f) { fstools::printFileToSerial(f); }
    void inline listFilesToSerial(String dirname = "/") { fstools::listFilesToSerial(dirname.c_str()); }
    void inline printAllFilesInDirectoryToSerial(String dirname = "/") { fstools::printAllFilesInDirectoryToSerial(dirname.c_str()); }
    void inline printFileTreeToSerial(String path = "/", String dirName = "/", String treeString = "") { fstools::printFileTreeToSerial(path, dirName, treeString); }

    // file manipulation
    std::vector<const char *> inline getConfigFileList() { return fstools::getDirListOnlyFiles(CONFIG_FILE_PATH); }
    std::vector<const char *> inline getMacroFileList() { return fstools::getDirListOnlyFiles(MACRO_FILE_PATH); }
    bool formatFS();

    Configuration mConfig;
    std::vector<IConfigurable_ *> mvpConfigurables;
};

#endif // CONFIGURATION_H